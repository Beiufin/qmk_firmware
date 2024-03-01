/*
QMK Firmware Drop Alt v2 Beiufin Keymap
Copyright (C) 2020 Beiufin

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

/* RGB Matrix Settings*/
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_TYPING_HEATMAP_LEDON // Sets the default mode, if none has been set
#define RGB_MATRIX_DEFAULT_SPD 127
#define RGB_MATRIX_SPD_STEP 8
#define RGB_MATRIX_DEFAULT_HUE 112

/* Additional Animations (these will be added to the ones in info.json) */
#define ENABLE_RGB_MATRIX_TYPING_HEATMAP_SOLID // Typing heatmap where the leds remain a solid color, brighten
#define ENABLE_RGB_MATRIX_TYPING_HEATMAP_LEDON // Typing heatmap where the leds remain on and just shift color.

#define LOCK_INDICATOR_INVERT_COLOR_ENABLE // Comment out to disable custom indicator functionality for *Lock keys

#define FORCE_NKRO // NKRO by default requires to be turned on, this forces it on during keyboard DEFAULT regardless of EEPROM setting. NKRO can still be turned off but will be turned on again if the keyboard reboots.

/**
 * Tap repeat allows for setting keys to auto repeat.
 * To set a key, press TR_LOCK (fn+CapsLock) then press the key you want to have repeat.
 * To disable that repeating key, repeat the process.
 */
// #define ENABLE_TAP_REPEAT        // Enable Tap Repeat.
// #define TAP_REPEAT_INTERVAL 200  // Frequency of taps. Default 200
// #define TAP_REPEAT_MAX 16        // Max keys to allow tap repeating at once. Default 16. More requires more memory and processing time.
