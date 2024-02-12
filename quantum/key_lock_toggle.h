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

// Allow for choosing any key to use as the toggle key.
#ifndef KL_TOGG
#    define KL_TOGG BN_KLTG
#endif

// define KEY_LOCK_TOGGLE_RGB if you want the leds indicators when the BN_KLTG key is pressed
#ifdef KEY_LOCK_TOGGLE_RGB
#   ifndef KEY_LOCK_TOGGLE_HSV
#       define KEY_LOCK_TOGGLE_HSV HSV_GREEN
#   endif //KEY_LOCK_TOGGLE_HSV

bool rgb_matrix_indicators_key_lock_toggle(int kl_togg_idx);
#endif //KEY_LOCK_TOGGLE_RGB

bool is_klt_watching(void);

bool is_key_locked(uint16_t translated_keycode);

void lock_key(uint16_t translated_keycode, keyrecord_t *record);

void unlock_key(uint16_t translated_keycode, keyrecord_t *record);

void set_klt_watching(bool new_watching);
