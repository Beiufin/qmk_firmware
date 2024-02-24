/* Copyright 2024 Ben Cortina
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "dual_rgb_matrix.h"
#include "progmem.h"
// TODO: eeconfig for dual matrix.
// #include "eeprom.h"
// #include "eeconfig.h"
#include "debug.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <lib/lib8tion/lib8tion.h>

// TODO: eeconfig for dual matrix
// EECONFIG_DEBOUNCE_HELPER(rgb_matrix, EECONFIG_RGB_SECONDARY_MATRIX, rgb_secondary_matrix_config);
void eeconfig_flag_rgb_secondary_matrix(bool write) {
    // no-op
    return;
}

// globals
rgb_config_t rgb_secondary_matrix_config;

void rgb_secondary_matrix_init(void) {
    // Secondary is not yet in eeconfig
    rgb_secondary_matrix_config.enable = RGB_SECONDARY_MATRIX_DEFAULT_ON;
    rgb_secondary_matrix_config.mode   = RGB_SECONDARY_MATRIX_DEFAULT_MODE;
    rgb_secondary_matrix_config.hsv    = (HSV){RGB_SECONDARY_MATRIX_DEFAULT_HUE, RGB_SECONDARY_MATRIX_DEFAULT_SAT, RGB_SECONDARY_MATRIX_DEFAULT_VAL};
    rgb_secondary_matrix_config.speed  = RGB_SECONDARY_MATRIX_DEFAULT_SPD;
    rgb_secondary_matrix_config.flags  = LED_FLAG_NONE;
}

uint8_t dual_rgb_matrix_is_enabled(void) {
    return rgb_matrix_config.enable || rgb_secondary_matrix_config.enable;
}

void rgb_secondary_matrix_set_flags_eeprom_helper(led_flags_t flags, bool write_to_eeprom) {
    rgb_secondary_matrix_config.flags = flags;
    eeconfig_flag_rgb_secondary_matrix(write_to_eeprom);
    dprintf("rgb secondary matrix set flags [%s]: %u\n", (write_to_eeprom) ? "EEPROM" : "NOEEPROM", rgb_secondary_matrix_config.flags);

    if (HAS_ANY_FLAGS(rgb_matrix_config.flags, flags)) {
        // to avoid collisions remove the flags from the other config
        if (write_to_eeprom) {
            rgb_matrix_set_flags(rgb_matrix_config.flags & ~(flags));
        } else {
            rgb_matrix_set_flags_noeeprom(rgb_matrix_config.flags & ~(flags));
        }
    }
}

void rgb_secondary_matrix_toggle_flags_helper(led_flags_t flags, bool write_to_eeprom) {
    rgb_secondary_matrix_set_flags_eeprom_helper(rgb_secondary_matrix_config.flags ^ flags, write_to_eeprom);
}

void dual_rgb_matrix_cycle_flags_helper(led_flags_t flags, bool write_to_eeprom) {
    // three possibilities
    if (HAS_ANY_FLAGS(rgb_secondary_matrix_config.flags, flags) || HAS_ANY_FLAGS(rgb_matrix_config.flags, flags)) {
        // 1. secondary has the flags. This should switch it off (these flags will be off for both)
        // 2. primary has the flags. This should move the flags to secondary.
        // using toggle_flags for the secondary accomplishes both
        rgb_secondary_matrix_toggle_flags_helper(flags, write_to_eeprom);
    } else {
        // 3. nothing has the flags. Turn them on for the primary
        if (write_to_eeprom) {
            rgb_matrix_toggle_flags(flags);
        } else {
            rgb_matrix_toggle_flags_noeeprom(flags);
        }
    }
}

void dual_rgb_matrix_toggle_eeprom_helper(bool write_to_eeprom) {
    // if either is enabled, disable both.
    if (rgb_matrix_config.enable || rgb_secondary_matrix_config.enable) {
        if (write_to_eeprom) {
            dual_rgb_matrix_disable();
        } else {
            dual_rgb_matrix_disable_noeeprom();
        }
    } else {
        // both are off, enable them
        if (write_to_eeprom) {
            dual_rgb_matrix_enable();
        } else {
            dual_rgb_matrix_enable_noeeprom();
        }
    }
}

void rgb_secondary_matrix_toggle_eeprom_helper(bool write_to_eeprom) {
    rgb_secondary_matrix_config.enable ^= 1;
    restart_rgb_task();
    eeconfig_flag_rgb_secondary_matrix(write_to_eeprom);
    dprintf("rgb secondary matrix toggle [%s]: rgb_secondary_matrix_config.enable = %u\n", (write_to_eeprom) ? "EEPROM" : "NOEEPROM", rgb_secondary_matrix_config.enable);
}

void rgb_secondary_matrix_set_state_eeprom_helper(bool enable, bool write_to_eeprom) {
    if (enable != !!rgb_secondary_matrix_config.enable) restart_rgb_task();
    rgb_secondary_matrix_config.enable = enable ? 1 : 0;
    eeconfig_flag_rgb_secondary_matrix(write_to_eeprom);
    dprintf("rgb secondary matrix set state [%s]: rgb_secondary_matrix_config.enable = %u\n", (write_to_eeprom) ? "EEPROM" : "NOEEPROM", rgb_secondary_matrix_config.enable);
}

void rgb_matrix_mode_eeprom_helper(uint8_t mode, bool write_to_eeprom) {
    if (!rgb_secondary_matrix_config.enable) {
        return;
    }
    if (mode < 1) {
        rgb_secondary_matrix_config.mode = 1;
    } else if (mode >= RGB_MATRIX_EFFECT_MAX) {
        rgb_secondary_matrix_config.mode = RGB_MATRIX_EFFECT_MAX - 1;
    } else {
        rgb_secondary_matrix_config.mode = mode;
    }
    restart_rgb_task();
    eeconfig_flag_rgb_secondary_matrix(write_to_eeprom);
    dprintf("rgb secondary matrix mode [%s]: %u\n", (write_to_eeprom) ? "EEPROM" : "NOEEPROM", rgb_secondary_matrix_config.mode);
}

void rgb_matrix_step_helper(bool write_to_eeprom) {
    uint8_t mode = rgb_secondary_matrix_config.mode + 1;
    rgb_matrix_mode_eeprom_helper((mode < RGB_MATRIX_EFFECT_MAX) ? mode : 1, write_to_eeprom);
}

void rgb_matrix_step_reverse_helper(bool write_to_eeprom) {
    uint8_t mode = rgb_secondary_matrix_config.mode - 1;
    rgb_matrix_mode_eeprom_helper((mode < 1) ? RGB_MATRIX_EFFECT_MAX - 1 : mode, write_to_eeprom);
}

void rgb_matrix_sethsv_eeprom_helper(uint16_t hue, uint8_t sat, uint8_t val, bool write_to_eeprom) {
    if (!rgb_secondary_matrix_config.enable) {
        return;
    }
    rgb_secondary_matrix_config.hsv.h = hue;
    rgb_secondary_matrix_config.hsv.s = sat;
    rgb_secondary_matrix_config.hsv.v = (val > RGB_MATRIX_MAXIMUM_BRIGHTNESS) ? RGB_MATRIX_MAXIMUM_BRIGHTNESS : val;
    eeconfig_flag_rgb_secondary_matrix(write_to_eeprom);
    dprintf("rgb secondary matrix set hsv [%s]: %u,%u,%u\n", (write_to_eeprom) ? "EEPROM" : "NOEEPROM", rgb_secondary_matrix_config.hsv.h, rgb_secondary_matrix_config.hsv.s, rgb_secondary_matrix_config.hsv.v);
}

void rgb_matrix_increase_hue_helper(bool write_to_eeprom) {
    rgb_matrix_sethsv_eeprom_helper(rgb_secondary_matrix_config.hsv.h + RGB_MATRIX_HUE_STEP, rgb_secondary_matrix_config.hsv.s, rgb_secondary_matrix_config.hsv.v, write_to_eeprom);
}

void rgb_matrix_decrease_hue_helper(bool write_to_eeprom) {
    rgb_matrix_sethsv_eeprom_helper(rgb_secondary_matrix_config.hsv.h - RGB_MATRIX_HUE_STEP, rgb_secondary_matrix_config.hsv.s, rgb_secondary_matrix_config.hsv.v, write_to_eeprom);
}

void rgb_secondary_matrix_increase_sat_helper(bool write_to_eeprom) {
    rgb_matrix_sethsv_eeprom_helper(rgb_secondary_matrix_config.hsv.h, qadd8(rgb_secondary_matrix_config.hsv.s, RGB_MATRIX_SAT_STEP), rgb_secondary_matrix_config.hsv.v, write_to_eeprom);
}

void rgb_secondary_matrix_decrease_sat_helper(bool write_to_eeprom) {
    rgb_matrix_sethsv_eeprom_helper(rgb_secondary_matrix_config.hsv.h, qsub8(rgb_secondary_matrix_config.hsv.s, RGB_MATRIX_SAT_STEP), rgb_secondary_matrix_config.hsv.v, write_to_eeprom);
}

void rgb_secondary_matrix_increase_val_helper(bool write_to_eeprom) {
    rgb_matrix_sethsv_eeprom_helper(rgb_secondary_matrix_config.hsv.h, rgb_secondary_matrix_config.hsv.s, qadd8(rgb_secondary_matrix_config.hsv.v, RGB_MATRIX_VAL_STEP), write_to_eeprom);
}

void rgb_secondary_matrix_decrease_val_helper(bool write_to_eeprom) {
    rgb_matrix_sethsv_eeprom_helper(rgb_secondary_matrix_config.hsv.h, rgb_secondary_matrix_config.hsv.s, qsub8(rgb_secondary_matrix_config.hsv.v, RGB_MATRIX_VAL_STEP), write_to_eeprom);
}

void rgb_secondary_matrix_set_speed_eeprom_helper(uint8_t speed, bool write_to_eeprom) {
    rgb_secondary_matrix_config.speed = speed;
    eeconfig_flag_rgb_secondary_matrix(write_to_eeprom);
    dprintf("rgb secondary matrix set speed [%s]: %u\n", (write_to_eeprom) ? "EEPROM" : "NOEEPROM", rgb_secondary_matrix_config.speed);
}

void rgb_secondary_matrix_increase_speed_helper(bool write_to_eeprom) {
    rgb_secondary_matrix_set_speed_eeprom_helper(qadd8(rgb_secondary_matrix_config.speed, RGB_MATRIX_SPD_STEP), write_to_eeprom);
}

void rgb_secondary_matrix_decrease_speed_helper(bool write_to_eeprom) {
    rgb_secondary_matrix_set_speed_eeprom_helper(qsub8(rgb_secondary_matrix_config.speed, RGB_MATRIX_SPD_STEP), write_to_eeprom);
}

// ================= Methods for affecting both ===============
void dual_rgb_matrix_toggle_noeeprom(void) {
    dual_rgb_matrix_toggle_eeprom_helper(false);
}
void dual_rgb_matrix_toggle(void) {
    dual_rgb_matrix_toggle_eeprom_helper(true);
}

void dual_rgb_matrix_enable(void) {
    rgb_primary_matrix_enable();
    rgb_secondary_matrix_enable();
}
void dual_rgb_matrix_enable_noeeprom(void) {
    rgb_primary_matrix_enable_noeeprom();
    rgb_secondary_matrix_enable_noeeprom();
}

void dual_rgb_matrix_disable(void) {
    rgb_primary_matrix_disable();
    rgb_secondary_matrix_disable();
}
void dual_rgb_matrix_disable_noeeprom(void) {
    rgb_primary_matrix_disable_noeeprom();
    rgb_secondary_matrix_disable_noeeprom();
}

void dual_rgb_matrix_cycle_flags(led_flags_t flags) {
    dual_rgb_matrix_cycle_flags_helper(flags, true);
}
void dual_rgb_matrix_cycle_flags_noeeprom(led_flags_t flags) {
    dual_rgb_matrix_cycle_flags_helper(flags, false);
}

led_flags_t dual_rgb_matrix_get_flags(void) {
    return rgb_primary_matrix_get_flags() | rgb_secondary_matrix_get_flags();
}

// ========================== Methods for affecting secondary config ==============================
void rgb_secondary_matrix_toggle_noeeprom(void) {
    rgb_secondary_matrix_toggle_eeprom_helper(false);
}
void rgb_secondary_matrix_toggle(void) {
    rgb_secondary_matrix_toggle_eeprom_helper(true);
}

void rgb_secondary_matrix_enable(void) {
    rgb_secondary_matrix_set_state_eeprom_helper(true, true);
}

void rgb_secondary_matrix_enable_noeeprom(void) {
    rgb_secondary_matrix_set_state_eeprom_helper(true, false);
}

void rgb_secondary_matrix_disable(void) {
    rgb_secondary_matrix_set_state_eeprom_helper(false, true);
}

void rgb_secondary_matrix_disable_noeeprom(void) {
    rgb_secondary_matrix_set_state_eeprom_helper(false, false);
}

uint8_t rgb_secondary_matrix_is_enabled(void) {
    return rgb_secondary_matrix_config.enable;
}

void rgb_secondary_matrix_mode_noeeprom(uint8_t mode) {
    rgb_matrix_mode_eeprom_helper(mode, false);
}
void rgb_secondary_matrix_mode(uint8_t mode) {
    rgb_matrix_mode_eeprom_helper(mode, true);
}

uint8_t rgb_secondary_matrix_get_mode(void) {
    return rgb_secondary_matrix_config.mode;
}

void rgb_secondary_matrix_step_noeeprom(void) {
    rgb_matrix_step_helper(false);
}
void rgb_secondary_matrix_step(void) {
    rgb_matrix_step_helper(true);
}

void rgb_secondary_matrix_step_reverse_noeeprom(void) {
    rgb_matrix_step_reverse_helper(false);
}
void rgb_secondary_matrix_step_reverse(void) {
    rgb_matrix_step_reverse_helper(true);
}

void rgb_secondary_matrix_sethsv_noeeprom(uint16_t hue, uint8_t sat, uint8_t val) {
    rgb_matrix_sethsv_eeprom_helper(hue, sat, val, false);
}
void rgb_secondary_matrix_sethsv(uint16_t hue, uint8_t sat, uint8_t val) {
    rgb_matrix_sethsv_eeprom_helper(hue, sat, val, true);
}

HSV rgb_secondary_matrix_get_hsv(void) {
    return rgb_secondary_matrix_config.hsv;
}
uint8_t rgb_secondary_matrix_get_hue(void) {
    return rgb_secondary_matrix_config.hsv.h;
}
uint8_t rgb_secondary_matrix_get_sat(void) {
    return rgb_secondary_matrix_config.hsv.s;
}
uint8_t rgb_secondary_matrix_get_val(void) {
    return rgb_secondary_matrix_config.hsv.v;
}

void rgb_secondary_matrix_increase_hue_noeeprom(void) {
    rgb_matrix_increase_hue_helper(false);
}
void rgb_secondary_matrix_increase_hue(void) {
    rgb_matrix_increase_hue_helper(true);
}

void rgb_secondary_matrix_decrease_hue_noeeprom(void) {
    rgb_matrix_decrease_hue_helper(false);
}
void rgb_secondary_matrix_decrease_hue(void) {
    rgb_matrix_decrease_hue_helper(true);
}

void rgb_secondary_matrix_increase_sat_noeeprom(void) {
    rgb_secondary_matrix_increase_sat_helper(false);
}
void rgb_secondary_matrix_increase_sat(void) {
    rgb_secondary_matrix_increase_sat_helper(true);
}

void rgb_secondary_matrix_decrease_sat_noeeprom(void) {
    rgb_secondary_matrix_decrease_sat_helper(false);
}
void rgb_secondary_matrix_decrease_sat(void) {
    rgb_secondary_matrix_decrease_sat_helper(true);
}

void rgb_secondary_matrix_increase_val_noeeprom(void) {
    rgb_secondary_matrix_increase_val_helper(false);
}
void rgb_secondary_matrix_increase_val(void) {
    rgb_secondary_matrix_increase_val_helper(true);
}

void rgb_secondary_matrix_decrease_val_noeeprom(void) {
    rgb_secondary_matrix_decrease_val_helper(false);
}
void rgb_secondary_matrix_decrease_val(void) {
    rgb_secondary_matrix_decrease_val_helper(true);
}

void rgb_secondary_matrix_set_speed_noeeprom(uint8_t speed) {
    rgb_secondary_matrix_set_speed_eeprom_helper(speed, false);
}
void rgb_secondary_matrix_set_speed(uint8_t speed) {
    rgb_secondary_matrix_set_speed_eeprom_helper(speed, true);
}

uint8_t rgb_secondary_matrix_get_speed(void) {
    return rgb_secondary_matrix_config.speed;
}

void rgb_secondary_matrix_increase_speed_noeeprom(void) {
    rgb_secondary_matrix_increase_speed_helper(false);
}
void rgb_secondary_matrix_increase_speed(void) {
    rgb_secondary_matrix_increase_speed_helper(true);
}

void rgb_secondary_matrix_decrease_speed_noeeprom(void) {
    rgb_secondary_matrix_decrease_speed_helper(false);
}
void rgb_secondary_matrix_decrease_speed(void) {
    rgb_secondary_matrix_decrease_speed_helper(true);
}

led_flags_t rgb_secondary_matrix_get_flags(void) {
    return rgb_secondary_matrix_config.flags;
}

void rgb_secondary_matrix_set_flags(led_flags_t flags) {
    rgb_secondary_matrix_set_flags_eeprom_helper(flags, true);
}

void rgb_secondary_matrix_set_flags_noeeprom(led_flags_t flags) {
    rgb_secondary_matrix_set_flags_eeprom_helper(flags, false);
}

void rgb_secondary_matrix_toggle_flags(led_flags_t flags) {
    rgb_secondary_matrix_toggle_flags_helper(flags, true);
}
void rgb_secondary_matrix_toggle_flags_noeeprom(led_flags_t flags) {
    rgb_secondary_matrix_toggle_flags_helper(flags, false);
}
