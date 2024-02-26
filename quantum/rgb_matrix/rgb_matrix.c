/* Copyright 2017 Jason Williams
 * Copyright 2017 Jack Humbert
 * Copyright 2018 Yiancar
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

#include "rgb_matrix.h"
#include "progmem.h"
#include "eeprom.h"
#include "eeconfig.h"
#include "keyboard.h"
#include "sync_timer.h"
#include "debug.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include <lib/lib8tion/lib8tion.h>

#ifndef RGB_MATRIX_CENTER
const led_point_t k_rgb_matrix_center = {112, 32};
#else
const led_point_t k_rgb_matrix_center = RGB_MATRIX_CENTER;
#endif

__attribute__((weak)) RGB rgb_matrix_hsv_to_rgb(HSV hsv) {
    return hsv_to_rgb(hsv);
}

// Generic effect runners
#include "rgb_matrix_runners.inc"

// ------------------------------------------
// -----Begin rgb effect includes macros-----
#define RGB_MATRIX_EFFECT(name)
#define RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#ifdef RGB_MATRIX_PROCESS_HEATMAP
// if any heatmaps are enabled, this is used to process them
#    include "process_rgb_matrix_typing_heatmap.h"
#endif

#include "rgb_matrix_effects.inc"
#ifdef RGB_MATRIX_CUSTOM_KB
#    include "rgb_matrix_kb.inc"
#endif
#ifdef RGB_MATRIX_CUSTOM_USER
#    include "rgb_matrix_user.inc"
#endif

#undef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#undef RGB_MATRIX_EFFECT
// -----End rgb effect includes macros-------
// ------------------------------------------

// globals
rgb_config_t rgb_matrix_config; // TODO: would like to prefix this with g_ for global consistancy, do this in another pr
uint32_t     g_rgb_timer;
#ifdef RGB_MATRIX_FRAMEBUFFER_EFFECTS
uint8_t g_rgb_frame_buffer[RGB_MATRIX_LED_COUNT] = {0};

#endif // RGB_MATRIX_FRAMEBUFFER_EFFECTS
#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
last_hit_t g_last_hit_tracker;
#endif // RGB_MATRIX_KEYREACTIVE_ENABLED

// internals
static bool            suspend_state      = false;
static uint8_t         rgb_last_enable    = UINT8_MAX;
static uint8_t         rgb_last_effect    = UINT8_MAX;
static effect_params_t rgb_effect_params  = {0, LED_FLAG_ALL, false};
static bool            rendering_complete = false;
#ifdef DUAL_RGB_MATRIX_ENABLE
static bool            primary_rendering_complete   = false;
static bool            secondary_rendering_complete = false;
static effect_params_t rgb_secondary_effect_params  = {0, LED_FLAG_NONE, false};
static uint8_t         rgb_secondary_last_enable    = UINT8_MAX;
static uint8_t         rgb_secondary_last_effect    = UINT8_MAX;
#endif
static rgb_task_states rgb_task_state = SYNCING;
#if RGB_MATRIX_TIMEOUT > 0
static uint32_t rgb_anykey_timer;
#endif // RGB_MATRIX_TIMEOUT > 0

// double buffers
static uint32_t rgb_timer_buffer;
#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
static last_hit_t last_hit_buffer;
#endif // RGB_MATRIX_KEYREACTIVE_ENABLED

// split rgb matrix
#if defined(RGB_MATRIX_ENABLE) && defined(RGB_MATRIX_SPLIT)
const uint8_t k_rgb_matrix_split[2] = RGB_MATRIX_SPLIT;
#endif

EECONFIG_DEBOUNCE_HELPER(rgb_matrix, EECONFIG_RGB_MATRIX, rgb_matrix_config);

void eeconfig_update_rgb_matrix(void) {
    eeconfig_flush_rgb_matrix(true);
}

void eeconfig_update_rgb_matrix_default(void) {
    dprintf("eeconfig_update_rgb_matrix_default\n");
    rgb_matrix_config.enable = RGB_MATRIX_DEFAULT_ON;
    rgb_matrix_config.mode   = RGB_MATRIX_DEFAULT_MODE;
    rgb_matrix_config.hsv    = (HSV){RGB_MATRIX_DEFAULT_HUE, RGB_MATRIX_DEFAULT_SAT, RGB_MATRIX_DEFAULT_VAL};
    rgb_matrix_config.speed  = RGB_MATRIX_DEFAULT_SPD;
    rgb_matrix_config.flags  = LED_FLAG_ALL;
    eeconfig_flush_rgb_matrix(true);
}

void eeconfig_debug_rgb_matrix(void) {
    dprintf("rgb_matrix_config EEPROM\n");
    dprintf("rgb_matrix_config.enable = %d\n", rgb_matrix_config.enable);
    dprintf("rgb_matrix_config.mode = %d\n", rgb_matrix_config.mode);
    dprintf("rgb_matrix_config.hsv.h = %d\n", rgb_matrix_config.hsv.h);
    dprintf("rgb_matrix_config.hsv.s = %d\n", rgb_matrix_config.hsv.s);
    dprintf("rgb_matrix_config.hsv.v = %d\n", rgb_matrix_config.hsv.v);
    dprintf("rgb_matrix_config.speed = %d\n", rgb_matrix_config.speed);
    dprintf("rgb_matrix_config.flags = %d\n", rgb_matrix_config.flags);
}

void rgb_matrix_reload_from_eeprom(void) {
    rgb_primary_matrix_disable_noeeprom();
    /* Reset back to what we have in eeprom */
    eeconfig_init_rgb_matrix();
    eeconfig_debug_rgb_matrix(); // display current eeprom values
    if (rgb_matrix_config.enable) {
        rgb_matrix_mode_noeeprom(rgb_matrix_config.mode);
    }
}

__attribute__((weak)) uint8_t rgb_matrix_map_row_column_to_led_kb(uint8_t row, uint8_t column, uint8_t *led_i) {
    return 0;
}

uint8_t rgb_matrix_map_row_column_to_led(uint8_t row, uint8_t column, uint8_t *led_i) {
    uint8_t led_count = rgb_matrix_map_row_column_to_led_kb(row, column, led_i);
    uint8_t led_index = g_led_config.matrix_co[row][column];
    if (led_index != NO_LED) {
        led_i[led_count] = led_index;
        led_count++;
    }
    return led_count;
}

void rgb_matrix_update_pwm_buffers(void) {
    rgb_matrix_driver.flush();
}

void rgb_matrix_set_color(int index, uint8_t red, uint8_t green, uint8_t blue) {
    rgb_matrix_driver.set_color(index, red, green, blue);
}

void rgb_matrix_set_color_all(uint8_t red, uint8_t green, uint8_t blue) {
#if defined(RGB_MATRIX_ENABLE) && defined(RGB_MATRIX_SPLIT)
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++)
        rgb_matrix_set_color(i, red, green, blue);
#else
    rgb_matrix_driver.set_color_all(red, green, blue);
#endif
}

void process_rgb_matrix(uint8_t row, uint8_t col, bool pressed) {
#ifndef RGB_MATRIX_SPLIT
    if (!is_keyboard_master()) return;
#endif
#if RGB_MATRIX_TIMEOUT > 0
    rgb_anykey_timer = 0;
#endif // RGB_MATRIX_TIMEOUT > 0

#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
    uint8_t led[LED_HITS_TO_REMEMBER];
    uint8_t led_count = 0;

#    if defined(RGB_MATRIX_KEYRELEASES)
    if (!pressed)
#    elif defined(RGB_MATRIX_KEYPRESSES)
    if (pressed)
#    endif // defined(RGB_MATRIX_KEYRELEASES)
    {
        led_count = rgb_matrix_map_row_column_to_led(row, col, led);
    }

    if (last_hit_buffer.count + led_count > LED_HITS_TO_REMEMBER) {
        memcpy(&last_hit_buffer.x[0], &last_hit_buffer.x[led_count], LED_HITS_TO_REMEMBER - led_count);
        memcpy(&last_hit_buffer.y[0], &last_hit_buffer.y[led_count], LED_HITS_TO_REMEMBER - led_count);
        memcpy(&last_hit_buffer.tick[0], &last_hit_buffer.tick[led_count], (LED_HITS_TO_REMEMBER - led_count) * 2); // 16 bit
        memcpy(&last_hit_buffer.index[0], &last_hit_buffer.index[led_count], LED_HITS_TO_REMEMBER - led_count);
        last_hit_buffer.count = LED_HITS_TO_REMEMBER - led_count;
    }

    for (uint8_t i = 0; i < led_count; i++) {
        uint8_t index                = last_hit_buffer.count;
        last_hit_buffer.x[index]     = g_led_config.point[led[i]].x;
        last_hit_buffer.y[index]     = g_led_config.point[led[i]].y;
        last_hit_buffer.index[index] = led[i];
        last_hit_buffer.tick[index]  = 0;
        last_hit_buffer.count++;
    }
#endif // RGB_MATRIX_KEYREACTIVE_ENABLED

#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(RGB_MATRIX_PROCESS_HEATMAP)
#    if defined(RGB_MATRIX_KEYRELEASES)
    if (!pressed)
#    else
    if (pressed)
#    endif // defined(RGB_MATRIX_KEYRELEASES)
    {
        if (
#    ifdef ENABLE_RGB_MATRIX_TYPING_HEATMAP
            rgb_matrix_config.mode == RGB_MATRIX_TYPING_HEATMAP ||
#    endif
#    ifdef ENABLE_RGB_MATRIX_TYPING_HEATMAP_LEDON
            rgb_matrix_config.mode == RGB_MATRIX_TYPING_HEATMAP_LEDON ||
#    endif
#    ifdef ENABLE_RGB_MATRIX_TYPING_HEATMAP_SOLID
            rgb_matrix_config.mode == RGB_MATRIX_TYPING_HEATMAP_SOLID ||
#    endif
#    ifdef ENABLE_RGB_MATRIX_TYPING_HEATMAP_LEDON_APM
            rgb_matrix_config.mode == RGB_MATRIX_TYPING_HEATMAP_LEDON_APM ||
#    endif
            false) {
            process_rgb_matrix_typing_heatmap(row, col);
        }
    }
#endif // defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(RGB_MATRIX_PROCESS_HEATMAP)
}

void rgb_matrix_test(void) {
    // Mask out bits 4 and 5
    // Mask out bits 4 and 5
    // Increase the factor to make the test animation slower (and reduce to make it faster)
    uint8_t factor = 10;
    switch ((g_rgb_timer & (0b11 << factor)) >> factor) {
        case 0: {
            rgb_matrix_set_color_all(20, 0, 0);
            break;
        }
        case 1: {
            rgb_matrix_set_color_all(0, 20, 0);
            break;
        }
        case 2: {
            rgb_matrix_set_color_all(0, 0, 20);
            break;
        }
        case 3: {
            rgb_matrix_set_color_all(20, 20, 20);
            break;
        }
    }
}

static bool rgb_matrix_none(effect_params_t *params) {
    if (!params->init) {
        return false;
    }

    rgb_matrix_set_color_all(0, 0, 0);
    return false;
}
#ifdef DUAL_RGB_MATRIX_ENABLE
static bool rgb_matrix_partial_none(effect_params_t *params) {
    // just a subset of leds means we need to iterate over each, thus use limits.
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        rgb_matrix_set_color(i, 0, 0, 0);
    }

    return rgb_matrix_check_finished_leds(led_max);
}
#endif

static void rgb_task_timers(void) {
#if defined(RGB_MATRIX_KEYREACTIVE_ENABLED) || RGB_MATRIX_TIMEOUT > 0
    uint32_t deltaTime = sync_timer_elapsed32(rgb_timer_buffer);
#endif // defined(RGB_MATRIX_KEYREACTIVE_ENABLED) || RGB_MATRIX_TIMEOUT > 0
    rgb_timer_buffer = sync_timer_read32();

    // Update double buffer timers
#if RGB_MATRIX_TIMEOUT > 0
    if (rgb_anykey_timer + deltaTime <= UINT32_MAX) {
        rgb_anykey_timer += deltaTime;
    }
#endif // RGB_MATRIX_TIMEOUT > 0

    // Update double buffer last hit timers
#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
    uint8_t count = last_hit_buffer.count;
    for (uint8_t i = 0; i < count; ++i) {
        if (UINT16_MAX - deltaTime < last_hit_buffer.tick[i]) {
            last_hit_buffer.count--;
            continue;
        }
        last_hit_buffer.tick[i] += deltaTime;
    }
#endif // RGB_MATRIX_KEYREACTIVE_ENABLED
}

void restart_rgb_task(void) {
    rgb_task_state = STARTING;
}

static void rgb_task_sync(void) {
    eeconfig_flush_rgb_matrix(false);
    // next task
    if (sync_timer_elapsed32(g_rgb_timer) >= RGB_MATRIX_LED_FLUSH_LIMIT) restart_rgb_task();
}

static void rgb_task_start(void) {
    // reset iter
    rgb_effect_params.iter = 0;
    rendering_complete     = false;
#ifdef DUAL_RGB_MATRIX_ENABLE
    rgb_secondary_effect_params.iter = 0;
    secondary_rendering_complete     = false;
    primary_rendering_complete       = false;
#endif

    // update double buffers
    g_rgb_timer = rgb_timer_buffer;
#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
    g_last_hit_tracker = last_hit_buffer;
#endif // RGB_MATRIX_KEYREACTIVE_ENABLED

    // next task
    rgb_task_state = RENDERING;
}

static bool rgb_task_render_effect(uint8_t effect, effect_params_t *effect_params, rgb_config_t *rgb_config) {
    bool rendering = false;

    // each effect can opt to do calculations
    // and/or request PWM buffer updates.
    switch (effect) {
        case RGB_MATRIX_NONE:
            rendering = rgb_matrix_none(effect_params);
            break;
#ifdef DUAL_RGB_MATRIX_ENABLE
        case RGB_MATRIX_PARTIAL_NONE:
            rendering = rgb_matrix_partial_none(effect_params);
            break;
#endif

// ---------------------------------------------
// -----Begin rgb effect switch case macros-----
#define RGB_MATRIX_EFFECT(name, ...)                 \
    case RGB_MATRIX_##name:                          \
        rendering = name(effect_params, rgb_config); \
        break;
#include "rgb_matrix_effects.inc"
#undef RGB_MATRIX_EFFECT

#if defined(RGB_MATRIX_CUSTOM_KB) || defined(RGB_MATRIX_CUSTOM_USER)
#    define RGB_MATRIX_EFFECT(name, ...)                 \
        case RGB_MATRIX_CUSTOM_##name:                   \
            rendering = name(effect_params, rgb_config); \
            break;
#    ifdef RGB_MATRIX_CUSTOM_KB
#        include "rgb_matrix_kb.inc"
#    endif
#    ifdef RGB_MATRIX_CUSTOM_USER
#        include "rgb_matrix_user.inc"
#    endif
#    undef RGB_MATRIX_EFFECT
#endif
            // -----End rgb effect switch case macros-------
            // ---------------------------------------------

        // Factory default magic value
        case UINT8_MAX: {
            rgb_matrix_test();
            rendering = false;
        }
    }

    effect_params->iter++;
    return rendering;
}

static void rgb_task_render(uint8_t primary_effect, uint8_t secondary_effect) {
    // this trusts that rgb_matrix_config will restart the rgb_task (rgb_task_state = STARTING)
    // if either enable or mode is changed.
    rgb_effect_params.init = (primary_effect != rgb_last_effect) || (rgb_matrix_config.enable != rgb_last_enable);
#ifdef DUAL_RGB_MATRIX_ENABLE
    rgb_secondary_effect_params.init = (secondary_effect != rgb_secondary_last_effect) || (rgb_secondary_matrix_config.enable != rgb_secondary_last_enable);
    if (rgb_effect_params.flags != rgb_matrix_config.flags || rgb_secondary_effect_params.flags != rgb_secondary_matrix_config.flags) {
        rgb_effect_params.flags           = rgb_matrix_config.flags;
        rgb_secondary_effect_params.flags = rgb_secondary_matrix_config.flags;
        // clear all leds
        rgb_matrix_set_color_all(0, 0, 0);
    }
#else
    if (rgb_effect_params.flags != rgb_matrix_config.flags) {
        rgb_effect_params.flags = rgb_matrix_config.flags;
        rgb_matrix_set_color_all(0, 0, 0);
    }
#endif

#ifdef DUAL_RGB_MATRIX_ENABLE
    if (!primary_rendering_complete) {
        primary_rendering_complete = rgb_task_render_effect(primary_effect, &rgb_effect_params, &rgb_matrix_config);
    }
    if (!secondary_rendering_complete) {
        secondary_rendering_complete = rgb_task_render_effect(secondary_effect, &rgb_secondary_effect_params, &rgb_secondary_matrix_config);
    }
    rendering_complete = primary_rendering_complete && secondary_rendering_complete;
#else
    rendering_complete = rgb_task_render_effect(primary_effect, &rgb_effect_params, &rgb_matrix_config);
#endif

    // next task
    if (!rendering_complete) {
        rgb_task_state = FLUSHING;
#ifdef DUAL_RGB_MATRIX_ENABLE
        if (!rgb_secondary_effect_params.init && !rgb_effect_params.init && primary_effect == RGB_MATRIX_NONE && secondary_effect == RGB_MATRIX_NONE) {
#else
        if (!rgb_effect_params.init && primary_effect == RGB_MATRIX_NONE) {
#endif
            // We only need to flush once if we are RGB_MATRIX_NONE
            rgb_task_state = SYNCING;
        }
    }
}

static void rgb_task_flush(uint8_t primary_effect, uint8_t secondary_effect) {
    // update last trackers after the first full render so we can init over several frames
    rgb_last_effect = primary_effect;
    rgb_last_enable = rgb_matrix_config.enable;
#ifdef DUAL_RGB_MATRIX_ENABLE
    rgb_secondary_last_effect = secondary_effect;
    rgb_secondary_last_enable = rgb_secondary_matrix_config.enable;
#endif

    // update pwm buffers
    rgb_matrix_update_pwm_buffers();

    // next task
    rgb_task_state = SYNCING;
}

void rgb_matrix_task(void) {
    rgb_task_timers();

    // Ideally we would also stop sending zeros to the LED driver PWM buffers
    // while suspended and just do a software shutdown. This is a cheap hack for now.
    bool suspend_backlight = suspend_state ||
#if RGB_MATRIX_TIMEOUT > 0
                             (rgb_anykey_timer > (uint32_t)RGB_MATRIX_TIMEOUT) ||
#endif // RGB_MATRIX_TIMEOUT > 0
                             false;

    uint8_t primary_effect = suspend_backlight || !rgb_matrix_config.enable ? RGB_MATRIX_NONE : rgb_matrix_config.mode;
#ifdef DUAL_RGB_MATRIX_ENABLE
    uint8_t secondary_effect = suspend_backlight || !rgb_secondary_matrix_config.enable ? RGB_MATRIX_NONE : rgb_secondary_matrix_config.mode;
    if (!secondary_effect && primary_effect) {
        // if there is a primary effect and not a secondary effect, use RGB_MATRIX_PARTIAL_NONE for the secondary
        // so that it only clears leds for the secondary flags.
        secondary_effect = RGB_MATRIX_PARTIAL_NONE;
    }
#else
    uint8_t secondary_effect = 0;
#endif

    switch (rgb_task_state) {
        case STARTING:
            rgb_task_start();
            break;
        case RENDERING:
            rgb_task_render(primary_effect, secondary_effect);
            // if there is a secondary rendering, we will call these later.
            if (primary_effect || secondary_effect) {
                if (rgb_task_state == FLUSHING) { // ensure we only draw basic indicators once rendering is finished
                    rgb_matrix_indicators();
                }
                rgb_matrix_indicators_advanced(&rgb_effect_params);
            }
            break;
        case FLUSHING:
            rgb_task_flush(primary_effect, secondary_effect);
            break;
        case SYNCING:
            rgb_task_sync();
            break;
    }
}

void rgb_matrix_indicators(void) {
    rgb_matrix_indicators_kb();
}

__attribute__((weak)) bool rgb_matrix_indicators_kb(void) {
    return rgb_matrix_indicators_user();
}

__attribute__((weak)) bool rgb_matrix_indicators_user(void) {
    return true;
}

struct rgb_matrix_limits_t rgb_matrix_get_limits(uint8_t iter) {
    struct rgb_matrix_limits_t limits = {0};
#if defined(RGB_MATRIX_LED_PROCESS_LIMIT) && RGB_MATRIX_LED_PROCESS_LIMIT > 0 && RGB_MATRIX_LED_PROCESS_LIMIT < RGB_MATRIX_LED_COUNT
    limits.led_min_index = RGB_MATRIX_LED_PROCESS_LIMIT * (iter);
    limits.led_max_index = limits.led_min_index + RGB_MATRIX_LED_PROCESS_LIMIT;
    if (limits.led_max_index > RGB_MATRIX_LED_COUNT) limits.led_max_index = RGB_MATRIX_LED_COUNT;
#else
    limits.led_min_index = 0;
    limits.led_max_index = RGB_MATRIX_LED_COUNT;
#endif
#if defined(RGB_MATRIX_SPLIT)
    uint8_t k_rgb_matrix_split[2] = RGB_MATRIX_SPLIT;
    if (is_keyboard_left() && (limits.led_max_index > k_rgb_matrix_split[0])) limits.led_max_index = k_rgb_matrix_split[0];
    if (!(is_keyboard_left()) && (limits.led_min_index < k_rgb_matrix_split[0])) limits.led_min_index = k_rgb_matrix_split[0];
#endif
    return limits;
}

void rgb_matrix_indicators_advanced(effect_params_t *params) {
    /* special handling is needed for "params->iter", since it's already been incremented.
     * Could move the invocations to rgb_task_render, but then it's missing a few checks
     * and not sure which would be better. Otherwise, this should be called from
     * rgb_task_render, right before the iter++ line.
     */
    RGB_MATRIX_USE_LIMITS_ITER(min, max, params->iter - 1);
    rgb_matrix_indicators_advanced_kb(min, max);
}

__attribute__((weak)) bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    return rgb_matrix_indicators_advanced_user(led_min, led_max);
}

__attribute__((weak)) bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    return true;
}

void rgb_matrix_init(void) {
    rgb_matrix_driver.init();

#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
    g_last_hit_tracker.count = 0;
    for (uint8_t i = 0; i < LED_HITS_TO_REMEMBER; ++i) {
        g_last_hit_tracker.tick[i] = UINT16_MAX;
    }

    last_hit_buffer.count = 0;
    for (uint8_t i = 0; i < LED_HITS_TO_REMEMBER; ++i) {
        last_hit_buffer.tick[i] = UINT16_MAX;
    }
#endif // RGB_MATRIX_KEYREACTIVE_ENABLED

    if (!eeconfig_is_enabled()) {
        dprintf("rgb_matrix_init_drivers eeconfig is not enabled.\n");
        eeconfig_init();
        eeconfig_update_rgb_matrix_default();
    }

    eeconfig_init_rgb_matrix();
    if (!rgb_matrix_config.mode) {
        dprintf("rgb_matrix_init_drivers rgb_matrix_config.mode = 0. Write default values to EEPROM.\n");
        eeconfig_update_rgb_matrix_default();
    }
    eeconfig_debug_rgb_matrix(); // display current eeprom values
#ifdef DUAL_RGB_MATRIX_ENABLE
    // Secondary is not yet in eeconfig
    rgb_secondary_matrix_init();
#endif
}

void rgb_matrix_set_suspend_state(bool state) {
#ifdef RGB_DISABLE_WHEN_USB_SUSPENDED
    if (state && !suspend_state) { // only run if turning off, and only once
        rgb_task_render(0, 0);     // turn off all LEDs when suspending
        rgb_task_flush(0, 0);      // and actually flush led state to LEDs
    }
    suspend_state = state;
#endif
}

bool rgb_matrix_get_suspend_state(void) {
    return suspend_state;
}

void rgb_primary_matrix_toggle_eeprom_helper(bool write_to_eeprom) {
    rgb_matrix_config.enable ^= 1;
    restart_rgb_task();
    eeconfig_flag_rgb_matrix(write_to_eeprom);
    dprintf("rgb matrix toggle [%s]: rgb_matrix_config.enable = %u\n", (write_to_eeprom) ? "EEPROM" : "NOEEPROM", rgb_matrix_config.enable);
}
void rgb_primary_matrix_toggle_noeeprom(void) {
    rgb_primary_matrix_toggle_eeprom_helper(false);
}
void rgb_primary_matrix_toggle(void) {
    rgb_primary_matrix_toggle_eeprom_helper(true);
}

void rgb_primary_matrix_set_state_eeprom_helper(bool enable, bool write_to_eeprom) {
    if (enable != !!rgb_matrix_config.enable) restart_rgb_task();
    rgb_matrix_config.enable = enable ? 1 : 0;
    eeconfig_flag_rgb_matrix(write_to_eeprom);
    dprintf("rgb matrix set state [%s]: rgb_matrix_config.enable = %u\n", (write_to_eeprom) ? "EEPROM" : "NOEEPROM", rgb_matrix_config.enable);
}

void rgb_primary_matrix_enable(void) {
    rgb_primary_matrix_set_state_eeprom_helper(true, true);
}

void rgb_primary_matrix_enable_noeeprom(void) {
    rgb_primary_matrix_set_state_eeprom_helper(true, false);
}

void rgb_primary_matrix_disable(void) {
    rgb_primary_matrix_set_state_eeprom_helper(false, true);
}

void rgb_primary_matrix_disable_noeeprom(void) {
    rgb_primary_matrix_set_state_eeprom_helper(false, false);
}

uint8_t rgb_primary_matrix_is_enabled(void) {
    return rgb_matrix_config.enable;
}

void rgb_primary_matrix_mode_eeprom_helper(uint8_t mode, bool write_to_eeprom) {
    if (!rgb_matrix_config.enable) {
        return;
    }
    if (mode < 1) {
        rgb_matrix_config.mode = 1;
    } else if (mode >= RGB_MATRIX_EFFECT_MAX) {
        rgb_matrix_config.mode = RGB_MATRIX_EFFECT_MAX - 1;
    } else {
        rgb_matrix_config.mode = mode;
    }
    restart_rgb_task();
    eeconfig_flag_rgb_matrix(write_to_eeprom);
    dprintf("rgb matrix mode [%s]: %u\n", (write_to_eeprom) ? "EEPROM" : "NOEEPROM", rgb_matrix_config.mode);
}
void rgb_matrix_mode_noeeprom(uint8_t mode) {
    rgb_primary_matrix_mode_eeprom_helper(mode, false);
}
void rgb_matrix_mode(uint8_t mode) {
    rgb_primary_matrix_mode_eeprom_helper(mode, true);
}

uint8_t rgb_matrix_get_mode(void) {
    return rgb_matrix_config.mode;
}

void rgb_primary_matrix_step_helper(bool write_to_eeprom) {
    uint8_t mode = rgb_matrix_config.mode + 1;
    rgb_primary_matrix_mode_eeprom_helper((mode < RGB_MATRIX_EFFECT_MAX) ? mode : 1, write_to_eeprom);
}
void rgb_matrix_step_noeeprom(void) {
    rgb_primary_matrix_step_helper(false);
}
void rgb_matrix_step(void) {
    rgb_primary_matrix_step_helper(true);
}

void rgb_primary_matrix_step_reverse_helper(bool write_to_eeprom) {
    uint8_t mode = rgb_matrix_config.mode - 1;
    rgb_primary_matrix_mode_eeprom_helper((mode < 1) ? RGB_MATRIX_EFFECT_MAX - 1 : mode, write_to_eeprom);
}
void rgb_matrix_step_reverse_noeeprom(void) {
    rgb_primary_matrix_step_reverse_helper(false);
}
void rgb_matrix_step_reverse(void) {
    rgb_primary_matrix_step_reverse_helper(true);
}

void rgb_primary_matrix_sethsv_eeprom_helper(uint16_t hue, uint8_t sat, uint8_t val, bool write_to_eeprom) {
    if (!rgb_matrix_config.enable) {
        return;
    }
    rgb_matrix_config.hsv.h = hue;
    rgb_matrix_config.hsv.s = sat;
    rgb_matrix_config.hsv.v = (val > RGB_MATRIX_MAXIMUM_BRIGHTNESS) ? RGB_MATRIX_MAXIMUM_BRIGHTNESS : val;
    eeconfig_flag_rgb_matrix(write_to_eeprom);
    dprintf("rgb matrix set hsv [%s]: %u,%u,%u\n", (write_to_eeprom) ? "EEPROM" : "NOEEPROM", rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v);
}
void rgb_matrix_sethsv_noeeprom(uint16_t hue, uint8_t sat, uint8_t val) {
    rgb_primary_matrix_sethsv_eeprom_helper(hue, sat, val, false);
}
void rgb_matrix_sethsv(uint16_t hue, uint8_t sat, uint8_t val) {
    rgb_primary_matrix_sethsv_eeprom_helper(hue, sat, val, true);
}

HSV rgb_matrix_get_hsv(void) {
    return rgb_matrix_config.hsv;
}
uint8_t rgb_matrix_get_hue(void) {
    return rgb_matrix_config.hsv.h;
}
uint8_t rgb_matrix_get_sat(void) {
    return rgb_matrix_config.hsv.s;
}
uint8_t rgb_matrix_get_val(void) {
    return rgb_matrix_config.hsv.v;
}

void rgb_primary_matrix_increase_hue_helper(bool write_to_eeprom) {
    rgb_primary_matrix_sethsv_eeprom_helper(rgb_matrix_config.hsv.h + RGB_MATRIX_HUE_STEP, rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v, write_to_eeprom);
}
void rgb_matrix_increase_hue_noeeprom(void) {
    rgb_primary_matrix_increase_hue_helper(false);
}
void rgb_matrix_increase_hue(void) {
    rgb_primary_matrix_increase_hue_helper(true);
}

void rgb_primary_matrix_decrease_hue_helper(bool write_to_eeprom) {
    rgb_primary_matrix_sethsv_eeprom_helper(rgb_matrix_config.hsv.h - RGB_MATRIX_HUE_STEP, rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v, write_to_eeprom);
}
void rgb_matrix_decrease_hue_noeeprom(void) {
    rgb_primary_matrix_decrease_hue_helper(false);
}
void rgb_matrix_decrease_hue(void) {
    rgb_primary_matrix_decrease_hue_helper(true);
}

void rgb_primary_matrix_increase_sat_helper(bool write_to_eeprom) {
    rgb_primary_matrix_sethsv_eeprom_helper(rgb_matrix_config.hsv.h, qadd8(rgb_matrix_config.hsv.s, RGB_MATRIX_SAT_STEP), rgb_matrix_config.hsv.v, write_to_eeprom);
}
void rgb_matrix_increase_sat_noeeprom(void) {
    rgb_primary_matrix_increase_sat_helper(false);
}
void rgb_matrix_increase_sat(void) {
    rgb_primary_matrix_increase_sat_helper(true);
}

void rgb_primary_matrix_decrease_sat_helper(bool write_to_eeprom) {
    rgb_primary_matrix_sethsv_eeprom_helper(rgb_matrix_config.hsv.h, qsub8(rgb_matrix_config.hsv.s, RGB_MATRIX_SAT_STEP), rgb_matrix_config.hsv.v, write_to_eeprom);
}
void rgb_matrix_decrease_sat_noeeprom(void) {
    rgb_primary_matrix_decrease_sat_helper(false);
}
void rgb_matrix_decrease_sat(void) {
    rgb_primary_matrix_decrease_sat_helper(true);
}

void rgb_primary_matrix_increase_val_helper(bool write_to_eeprom) {
    rgb_primary_matrix_sethsv_eeprom_helper(rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, qadd8(rgb_matrix_config.hsv.v, RGB_MATRIX_VAL_STEP), write_to_eeprom);
}
void rgb_matrix_increase_val_noeeprom(void) {
    rgb_primary_matrix_increase_val_helper(false);
}
void rgb_matrix_increase_val(void) {
    rgb_primary_matrix_increase_val_helper(true);
}

void rgb_primary_matrix_decrease_val_helper(bool write_to_eeprom) {
    rgb_primary_matrix_sethsv_eeprom_helper(rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, qsub8(rgb_matrix_config.hsv.v, RGB_MATRIX_VAL_STEP), write_to_eeprom);
}
void rgb_matrix_decrease_val_noeeprom(void) {
    rgb_primary_matrix_decrease_val_helper(false);
}
void rgb_matrix_decrease_val(void) {
    rgb_primary_matrix_decrease_val_helper(true);
}

void rgb_primary_matrix_set_speed_eeprom_helper(uint8_t speed, bool write_to_eeprom) {
    rgb_matrix_config.speed = speed;
    eeconfig_flag_rgb_matrix(write_to_eeprom);
    dprintf("rgb matrix set speed [%s]: %u\n", (write_to_eeprom) ? "EEPROM" : "NOEEPROM", rgb_matrix_config.speed);
}
void rgb_matrix_set_speed_noeeprom(uint8_t speed) {
    rgb_primary_matrix_set_speed_eeprom_helper(speed, false);
}
void rgb_matrix_set_speed(uint8_t speed) {
    rgb_primary_matrix_set_speed_eeprom_helper(speed, true);
}

uint8_t rgb_matrix_get_speed(void) {
    return rgb_matrix_config.speed;
}

void rgb_primary_matrix_increase_speed_helper(bool write_to_eeprom) {
    rgb_primary_matrix_set_speed_eeprom_helper(qadd8(rgb_matrix_config.speed, RGB_MATRIX_SPD_STEP), write_to_eeprom);
}
void rgb_matrix_increase_speed_noeeprom(void) {
    rgb_primary_matrix_increase_speed_helper(false);
}
void rgb_matrix_increase_speed(void) {
    rgb_primary_matrix_increase_speed_helper(true);
}

void rgb_primary_matrix_decrease_speed_helper(bool write_to_eeprom) {
    rgb_primary_matrix_set_speed_eeprom_helper(qsub8(rgb_matrix_config.speed, RGB_MATRIX_SPD_STEP), write_to_eeprom);
}
void rgb_matrix_decrease_speed_noeeprom(void) {
    rgb_primary_matrix_decrease_speed_helper(false);
}
void rgb_matrix_decrease_speed(void) {
    rgb_primary_matrix_decrease_speed_helper(true);
}

void rgb_primary_matrix_set_flags_eeprom_helper(led_flags_t flags, bool write_to_eeprom) {
    rgb_matrix_config.flags = flags;
    eeconfig_flag_rgb_matrix(write_to_eeprom);
    dprintf("rgb matrix set flags [%s]: %u\n", (write_to_eeprom) ? "EEPROM" : "NOEEPROM", rgb_matrix_config.flags);
#ifdef DUAL_RGB_MATRIX_ENABLE
    if (HAS_ANY_FLAGS(rgb_secondary_matrix_config.flags, flags)) {
        // to avoid collisions remove the flags from the other config
        if (write_to_eeprom) {
            rgb_secondary_matrix_set_flags(rgb_secondary_matrix_config.flags & ~(flags));
        } else {
            rgb_secondary_matrix_set_flags_noeeprom(rgb_secondary_matrix_config.flags & ~(flags));
        }
    }
#endif
}

led_flags_t rgb_primary_matrix_get_flags(void) {
    return rgb_matrix_config.flags;
}
void rgb_primary_matrix_toggle_flags_helper(led_flags_t flags, bool write_to_eeprom) {
    rgb_primary_matrix_set_flags_eeprom_helper(rgb_matrix_config.flags ^ flags, write_to_eeprom);
}

void rgb_matrix_set_flags(led_flags_t flags) {
    rgb_primary_matrix_set_flags_eeprom_helper(flags, true);
}

void rgb_matrix_set_flags_noeeprom(led_flags_t flags) {
    rgb_primary_matrix_set_flags_eeprom_helper(flags, false);
}

void rgb_matrix_toggle_flags(led_flags_t flags) {
    rgb_primary_matrix_toggle_flags_helper(flags, true);
}
void rgb_matrix_toggle_flags_noeeprom(led_flags_t flags) {
    rgb_primary_matrix_toggle_flags_helper(flags, false);
}
