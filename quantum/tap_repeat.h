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

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "action.h"

#ifndef TAP_REPEAT_INTERVAL
#   define TAP_REPEAT_INTERVAL 100
#endif

#ifndef TAP_REPEAT_MAX
#   define TAP_REPEAT_MAX 16
#endif

// define TAP_REPEAT_RGB if you want the leds indicators when the TR_TOGG key is pressed
#ifdef TAP_REPEAT_RGB
#   ifndef TAP_REPEAT_HSV
#       define TAP_REPEAT_HSV HSV_RED
#   endif //TAP_REPEAT_HSV

bool rgb_matrix_indicators_tap_repeat(int tr_togg_idx);
#endif //TAP_REPEAT_RGB

bool is_tap_repeat_watching(void);

void toggle_key_tap_repeat(uint16_t translated_keycode, keyrecord_t *record);

bool toggle_tap_repeat_watching(void);

void tap_repeat_task(void);

void clear_tap_repeat_keys(void);
