/* Copyright 2019
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
#include "process_rgb.h"
#include "action_util.h"

#ifdef RGB_MATRIX_ENABLE
#    include "rgb_matrix.h"
#endif

typedef void (*rgb_func_pointer)(void);

/**
 * Wrapper for inc/dec rgb keycode
 *
 * noinline to optimise for firmware size not speed (not in hot path)
 */
#if (defined(RGB_MATRIX_ENABLE) && !defined(RGB_MATRIX_DISABLE_KEYCODES))
static void __attribute__((noinline)) handleKeycodeDualRGB(const uint8_t is_shifted, const rgb_func_pointer inc_func, const rgb_func_pointer dec_func) {
    if (is_shifted) {
        dec_func();
    } else {
        inc_func();
    }
}
#endif

/**
 * Handle keycodes for secondary rgbmatrix
 */
bool process_dual_rgb_matrix(const uint16_t keycode, const keyrecord_t *record) {
#if (defined(RGB_MATRIX_ENABLE) && !defined(RGB_MATRIX_DISABLE_KEYCODES))
    // the RGB keycodes only affect secondary if alt is held. early exit if not alt.
    uint8_t alt = get_mods() & MOD_MASK_ALT;
    if (!alt) {
        return true;
    }

    // need to trigger on key-up for edge-case issue
#    ifndef RGB_TRIGGER_ON_KEYDOWN
    if (!record->event.pressed) {
#    else
    if (record->event.pressed) {
#    endif
        uint8_t shifted = get_mods() & MOD_MASK_SHIFT;

        switch (keycode) {
            case RGB_TOG:
                rgb_secondary_matrix_toggle();
                return false;
            case RGB_MODE_FORWARD:
                handleKeycodeDualRGB(shifted, rgb_secondary_matrix_step, rgb_secondary_matrix_step_reverse);
                return false;
            case RGB_MODE_REVERSE:
                handleKeycodeDualRGB(shifted, rgb_secondary_matrix_step_reverse, rgb_secondary_matrix_step);
                return false;
            case RGB_HUI:
                handleKeycodeDualRGB(shifted, rgb_secondary_matrix_increase_hue, rgb_secondary_matrix_decrease_hue);
                return false;
            case RGB_HUD:
                handleKeycodeDualRGB(shifted, rgb_secondary_matrix_decrease_hue, rgb_secondary_matrix_increase_hue);
                return false;
            case RGB_SAI:
                handleKeycodeDualRGB(shifted, rgb_secondary_matrix_increase_sat, rgb_secondary_matrix_decrease_sat);
                return false;
            case RGB_SAD:
                handleKeycodeDualRGB(shifted, rgb_secondary_matrix_decrease_sat, rgb_secondary_matrix_increase_sat);
                return false;
            case RGB_VAI:
                handleKeycodeDualRGB(shifted, rgb_secondary_matrix_increase_val, rgb_secondary_matrix_decrease_val);
                return false;
            case RGB_VAD:
                handleKeycodeDualRGB(shifted, rgb_secondary_matrix_decrease_val, rgb_secondary_matrix_increase_val);
                return false;
            case RGB_SPI:
                handleKeycodeDualRGB(shifted, rgb_secondary_matrix_increase_speed, rgb_secondary_matrix_decrease_speed);
                return false;
            case RGB_SPD:
                handleKeycodeDualRGB(shifted, rgb_secondary_matrix_decrease_speed, rgb_secondary_matrix_increase_speed);
                return false;
            case RGB_MODE_PLAIN:
                rgb_secondary_matrix_mode(RGB_MATRIX_SOLID_COLOR);
                return false;
#    if defined(ENABLE_RGB_MATRIX_BREATHING)
            case RGB_MODE_BREATHE:
                rgb_secondary_matrix_mode(RGB_MATRIX_BREATHING);
                return false;
#    endif
#    if defined(ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT)
            case RGB_MODE_RAINBOW:
                rgb_secondary_matrix_mode(RGB_MATRIX_CYCLE_LEFT_RIGHT);
                return false;
#    endif
#    if defined(ENABLE_RGB_MATRIX_CYCLE_PINWHEEL)
            case RGB_MODE_SWIRL:
                rgb_secondary_matrix_mode(RGB_MATRIX_CYCLE_PINWHEEL);
                return false;
#    endif
        }
    }
#endif // defined(RGB_MATRIX_ENABLE) && !defined(RGB_MATRIX_DISABLE_KEYCODES)

    return true;
}
