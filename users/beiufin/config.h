#pragma once

/*
 * Feature disable options
 *  These options are also useful to reduce firmware size.
 */
// #define NO_ACTION_LAYER
#define NO_ACTION_TAPPING
#define NO_ACTION_ONESHOT

/* Tap Repeat Settings */
// #define TAP_REPEAT_INTERVAL 200          // Frequency of taps. Default 200
// #define TAP_REPEAT_MAX 16                // Max keys to allow tap repeating at once. Default 16. More requires more memory and processing time.
#define TAP_REPEAT_RGB // Enables RGB indicators for tap repeat
// #define TAP_REPEAT_HSV HSV_RED           // Color for the repeating keys

/* Key Lock Settings */
#define KEY_LOCK_TOGGLE_RGB // Enables RGB indicators for key lock

/* QMK features, see docs\config_options.md for full list */
#define FORCE_NKRO       // NKRO by default requires to be turned on, this forces it on during keyboard DEFAULT regardless of EEPROM setting. NKRO can still be turned off but will be turned on again if the keyboard reboots.
#define TAPPING_TERM 200 // How long before a tap becomes a hold, if set above 500, a key tapped during the tapping term will turn it into a hold too
#define PERMISSIVE_HOLD  // Makes tap and hold keys trigger the hold if another key is pressed before releasing, even if it hasn't hit the TAPPING_TERM. See Permissive Hold for details
