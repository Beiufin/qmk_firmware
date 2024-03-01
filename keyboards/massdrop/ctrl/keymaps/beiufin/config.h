/*
QMK Firmware Massdrop CTRL M-AS Keymap
Copyright (C) 2020 matthewrobo

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

/* Key Lock Settings */
#define KL_TOGG KC_SCRL // Allows for using any key as the Key lock toggle. If not set, defaults to BN_KLTG

/* RGB Settings */
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS // enables more advances RBG effects
#define RGB_MATRIX_KEYPRESSES          // reacts to keypresses
// #define RGB_MATRIX_KEYRELEASES         // reacts to keyreleases (instead of keypresses)
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_TYPING_HEATMAP_LEDON // Sets the default mode, if none has been set
// #define RGB_MATRIX_HUE_STEP 12         // Units to step when in/decreasing hue
// #define RGB_MATRIX_SAT_STEP 25         // Units to step when in/decreasing saturation
// #define RGB_MATRIX_VAL_STEP 12         // Units to step when in/decreasing value (brightness)
#define RGB_MATRIX_SPD_STEP 8 // Units to step when in/decreasing speed
// #define RGB_DISABLE_AFTER_TIMEOUT 0    // number of ticks to wait until disabling effects
// #define RGB_DISABLE_WHEN_USB_SUSPENDED // turn off effects when suspended
// #define RGB_MATRIX_LED_PROCESS_LIMIT (RGB_MATRIX_LED_COUNT + 4) / 5 // limits the number of LEDs to process in an animation per task run (increases keyboard responsiveness)
// #define RGB_MATRIX_LED_FLUSH_LIMIT 16  // limits in milliseconds how frequently an animation will update the LEDs. 16 (16ms) is equivalent to limiting to 60fps (increases keyboard responsiveness)
#define RGB_MATRIX_DEFAULT_HUE 85  // Sets the default hue value, if none has been set
#define RGB_MATRIX_DEFAULT_SAT 255 // Sets the default saturation value, if none has been set
// #define RGB_MATRIX_DEFAULT_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS // Sets the default brightness value, if none has been set
// #define RGB_MATRIX_DEFAULT_SPD 127 // Sets the default animation speed, if none has been set

/* RGB Matrix Animation modes. Explicitly enabled */
// For full list of effects, see:
// https://docs.qmk.fm/#/feature_rgb_matrix?id=rgb-matrix-effects
// This list in in the correct mode order. Next mode is the following line, previous mode is previous line. Loops around.
#define ENABLE_RGB_MATRIX_SOLID_COLOR            // Static single hue, no speed support
#define ENABLE_RGB_MATRIX_ALPHAS_MODS            // Static dual hue, speed is hue for secondary hue
#define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN       // Static gradient top to bottom, speed controls how much gradient changes
#define ENABLE_RGB_MATRIX_BREATHING              // Single hue brightness cycling animation
#define ENABLE_RGB_MATRIX_BAND_SAT               // Single hue band fading saturation scrolling left to right
#define ENABLE_RGB_MATRIX_BAND_VAL               // Single hue band fading brightness scrolling left to right
#define ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT      // Single hue 3 blade spinning pinwheel fades saturation
#define ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL      // Single hue 3 blade spinning pinwheel fades brightness
#define ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT        // Single hue spinning spiral fades saturation
#define ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL        // Single hue spinning spiral fades brightness
#define ENABLE_RGB_MATRIX_CYCLE_ALL              // Full keyboard solid hue cycling through full gradient
#define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT       // Full gradient scrolling left to right
#define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN          // Full gradient scrolling top to bottom
#define ENABLE_RGB_MATRIX_CYCLE_OUT_IN           // Full gradient scrolling out to in
#define ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL      // Full dual gradients scrolling out to in
#define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON // Full gradent Chevron shapped scrolling left to right
#define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL         // Full gradient spinning pinwheel around center of keyboard
#define ENABLE_RGB_MATRIX_CYCLE_SPIRAL           // Full gradient spinning spiral around center of keyboard
#define ENABLE_RGB_MATRIX_DUAL_BEACON            // Full gradient spinning around center of keyboard
#define ENABLE_RGB_MATRIX_RAINBOW_BEACON         // Full tighter gradient spinning around center of keyboard
#define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS      // Full dual gradients spinning two halfs of keyboard
#define ENABLE_RGB_MATRIX_RAINDROPS              // Randomly changes a single key's hue
#define ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS    // Randomly changes a single key's hue and saturation
// #define ENABLE_RGB_MATRIX_APM                       // Leds with change color based on your Actions Per Minute
//  =================================================== Requires RGB_MATRIX_FRAMEBUFFER_EFFECTS =============================================================
#define ENABLE_RGB_MATRIX_TYPING_HEATMAP       // How hot is your WPM!
#define ENABLE_RGB_MATRIX_TYPING_HEATMAP_SOLID // Typing heatmap where the leds remain a solid color, brighten
#define ENABLE_RGB_MATRIX_TYPING_HEATMAP_LEDON // Typing heatmap where the leds remain on and just shift color.
// #define ENABLE_RGB_MATRIX_TYPING_HEATMAP_LEDON_APM  // Same as above, but the "extra" leds will use ENABLE_RGB_MATRIX_APM.
#define ENABLE_RGB_MATRIX_DIGITAL_RAIN // That famous computer simulation
// =================================================== RGB_MATRIX_KEYPRESSES OR RGB_MATRIX_KEYRELEASES =====================================================
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE            // Static single hue, pulses keys hit to shifted hue then fades to current hue
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE     // Pulses keys hit to hue & value then fades value out
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE       // Hue & value pulse near a single key hit then fades value out
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE  // Hue & value pulse near multiple key hits then fades value out
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS      // Hue & value pulse the same column and row of a single key hit then fades value out
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS // Hue & value pulse the same column and row of multiple key hits then fades value out
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS      // Hue & value pulse away on the same column and row of a single key hit then fades value out
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS // Hue & value pulse away on the same column and row of multiple key hits then fades value out
#define ENABLE_RGB_MATRIX_SPLASH                    // Full gradient & value pulse away from a single key hit then fades value out
#define ENABLE_RGB_MATRIX_MULTISPLASH               // Full gradient & value pulse away from multiple key hits then fades value out
#define ENABLE_RGB_MATRIX_SOLID_SPLASH              // Hue & value pulse away from a single key hit then fades value out
#define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH         // Hue & value pulse away from multiple key hits then fades value out
