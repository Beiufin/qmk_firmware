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

#include "rgb_matrix.h"
#include <stdint.h>
#include <stdbool.h>
#include "rgb_matrix_types.h"
#include "color.h"
#include "keyboard.h"

#ifndef RGB_SECONDARY_MATRIX_DEFAULT_ON
#    define RGB_SECONDARY_MATRIX_DEFAULT_ON true
#endif

#ifndef RGB_SECONDARY_MATRIX_DEFAULT_MODE
#    define RGB_SECONDARY_MATRIX_DEFAULT_MODE RGB_MATRIX_DEFAULT_MODE
#endif

#ifndef RGB_SECONDARY_MATRIX_DEFAULT_HUE
#    define RGB_SECONDARY_MATRIX_DEFAULT_HUE RGB_MATRIX_DEFAULT_HUE
#endif

#ifndef RGB_SECONDARY_MATRIX_DEFAULT_SAT
#    define RGB_SECONDARY_MATRIX_DEFAULT_SAT RGB_MATRIX_DEFAULT_SAT
#endif

#ifndef RGB_SECONDARY_MATRIX_DEFAULT_VAL
#    define RGB_SECONDARY_MATRIX_DEFAULT_VAL RGB_MATRIX_DEFAULT_VAL
#endif

#ifndef RGB_SECONDARY_MATRIX_DEFAULT_SPD
#    define RGB_SECONDARY_MATRIX_DEFAULT_SPD RGB_MATRIX_DEFAULT_SPD
#endif

void rgb_secondary_matrix_init(void);

// stuff that affects both primary and secondary
uint8_t     dual_rgb_matrix_is_enabled(void);
void        dual_rgb_matrix_toggle(void);
void        dual_rgb_matrix_toggle_noeeprom(void);
void        dual_rgb_matrix_enable(void);
void        dual_rgb_matrix_enable_noeeprom(void);
void        dual_rgb_matrix_disable(void);
void        dual_rgb_matrix_disable_noeeprom(void);
void        dual_rgb_matrix_cycle_flags(led_flags_t flags);
void        dual_rgb_matrix_cycle_flags_noeeprom(led_flags_t flags);
led_flags_t dual_rgb_matrix_get_flags(void);
// secondary only
void        rgb_secondary_matrix_toggle(void);
void        rgb_secondary_matrix_toggle_noeeprom(void);
void        rgb_secondary_matrix_enable(void);
void        rgb_secondary_matrix_enable_noeeprom(void);
void        rgb_secondary_matrix_disable(void);
void        rgb_secondary_matrix_disable_noeeprom(void);
uint8_t     rgb_secondary_matrix_is_enabled(void);
void        rgb_secondary_matrix_mode(uint8_t mode);
void        rgb_secondary_matrix_mode_noeeprom(uint8_t mode);
uint8_t     rgb_secondary_matrix_get_mode(void);
void        rgb_secondary_matrix_step(void);
void        rgb_secondary_matrix_step_noeeprom(void);
void        rgb_secondary_matrix_step_reverse(void);
void        rgb_secondary_matrix_step_reverse_noeeprom(void);
void        rgb_secondary_matrix_sethsv(uint16_t hue, uint8_t sat, uint8_t val);
void        rgb_secondary_matrix_sethsv_noeeprom(uint16_t hue, uint8_t sat, uint8_t val);
HSV         rgb_secondary_matrix_get_hsv(void);
uint8_t     rgb_secondary_matrix_get_hue(void);
uint8_t     rgb_secondary_matrix_get_sat(void);
uint8_t     rgb_secondary_matrix_get_val(void);
void        rgb_secondary_matrix_increase_hue(void);
void        rgb_secondary_matrix_increase_hue_noeeprom(void);
void        rgb_secondary_matrix_decrease_hue(void);
void        rgb_secondary_matrix_decrease_hue_noeeprom(void);
void        rgb_secondary_matrix_increase_sat(void);
void        rgb_secondary_matrix_increase_sat_noeeprom(void);
void        rgb_secondary_matrix_decrease_sat(void);
void        rgb_secondary_matrix_decrease_sat_noeeprom(void);
void        rgb_secondary_matrix_increase_val(void);
void        rgb_secondary_matrix_increase_val_noeeprom(void);
void        rgb_secondary_matrix_decrease_val(void);
void        rgb_secondary_matrix_decrease_val_noeeprom(void);
void        rgb_secondary_matrix_set_speed(uint8_t speed);
void        rgb_secondary_matrix_set_speed_noeeprom(uint8_t speed);
uint8_t     rgb_secondary_matrix_get_speed(void);
void        rgb_secondary_matrix_increase_speed(void);
void        rgb_secondary_matrix_increase_speed_noeeprom(void);
void        rgb_secondary_matrix_decrease_speed(void);
void        rgb_secondary_matrix_decrease_speed_noeeprom(void);
led_flags_t rgb_secondary_matrix_get_flags(void);
void        rgb_secondary_matrix_set_flags(led_flags_t flags);
void        rgb_secondary_matrix_set_flags_noeeprom(led_flags_t flags);
void        rgb_secondary_matrix_toggle_flags(led_flags_t flags);
void        rgb_secondary_matrix_toggle_flags_noeeprom(led_flags_t flags);

extern rgb_config_t rgb_secondary_matrix_config;
