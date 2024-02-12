/*
QMK Firmware Massdrop Alt M-AS Keymap
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
#include QMK_KEYBOARD_H
#include <lib/lib8tion/lib8tion.h>

#ifdef TAP_REPEAT_RGB
#    include "tap_repeat.h"
#endif

#ifdef KEY_LOCK_TOGGLE_RGB
#    include "key_lock_toggle.h"
#endif

// HID has not yet been implemented for this keyboard
// #include "raw_hid.h"

#define MILLISECONDS_IN_SECOND 1000

// These are just to make it neater to use builtin HSV values in the keymap
// clang-format off
#define WHITE {HSV_WHITE}
#define RED {HSV_RED}
#define CORAL {HSV_CORAL}
#define ORANGE {HSV_ORANGE}
#define GOLDEN {HSV_GOLDENROD}
#define GOLD {HSV_GOLD}
#define YELLOW {HSV_YELLOW}
#define CHART {HSV_CHARTREUSE}
#define GREEN {HSV_GREEN}
#define SPRING {HSV_SPRINGGREEN}
#define TURQ {HSV_TURQUOISE}
#define TEAL {HSV_TEAL}
#define CYAN {HSV_CYAN}
#define AZURE {HSV_AZURE}
#define BLUE {HSV_BLUE}
#define PURPLE {HSV_PURPLE}
#define MAGENT {HSV_MAGENTA}
#define PINK {HSV_PINK}
#define HSV_CREAM 24, 240, 255
#define CREAM {HSV_CREAM}
#define HSV_9B59B5 208, 192, 255
#define M9B59B5 {HSV_9B59B5}
// clang-format on

#define _LAYER_ KC_TRNS

#if !defined(RGB_MATRIX_MAXIMUM_BRIGHTNESS) || RGB_MATRIX_MAXIMUM_BRIGHTNESS > UINT8_MAX
#    undef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#    define RGB_MATRIX_MAXIMUM_BRIGHTNESS UINT8_MAX
#endif

#if !defined(RGB_MATRIX_HUE_STEP)
#    define RGB_MATRIX_HUE_STEP 8
#endif

#if !defined(RGB_MATRIX_SAT_STEP)
#    define RGB_MATRIX_SAT_STEP 16
#endif

#if !defined(RGB_MATRIX_VAL_STEP)
#    define RGB_MATRIX_VAL_STEP 16
#endif

#if !defined(RGB_MATRIX_SPD_STEP)
#    define RGB_MATRIX_SPD_STEP 16
#endif

//========================================================== CONFIGURABLE DEFAULTS ==========================================================
extern rgb_config_t rgb_matrix_config;
bool                disable_layer_color;

bool rgb_enabled_flag;  // Current LED state flag. If false then LED is off.

enum layout_names {
    _KL = 0,  // Keys Layout: The main keyboard layout that has all the characters
    _FL,      // Function Layout: The function key activated layout with default functions and some added ones
};

enum alt_keycodes {
    MAS_CRM = SAFE_RANGE,
    MAS_PRP,
    MAS_RED,
    MAS_GRN,
    MAS_BLU,
    MAS_CYN,
    MAS_MGT,
    MAS_YEL,
    MAS_KEY,
    MAS_WHT,
};

// clang-format off

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_KL] = LAYOUT_65_ansi_blocker(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   KC_PGDN,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(_FL), KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [_FL] = LAYOUT_65_ansi_blocker(
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, KC_MUTE,
        _______, RGB_RMOD,RGB_VAI, RGB_MOD, RGB_HUI, RGB_SAI, _______, MAS_MGT, MAS_BLU, MAS_WHT, KC_PSCR, KC_SCRL, KC_PAUS, _______, KC_END,
        BN_TRTG, RGB_SPD, RGB_VAD, RGB_SPI, RGB_HUD, RGB_SAD, _______, MAS_RED, MAS_CYN, MAS_PRP, _______, _______,          BN_KLTG, KC_VOLU,
        _______, RGB_TOG, _______, _______, _______, QK_BOOT, NK_TOGG, MAS_YEL, MAS_GRN, MAS_CRM, _______, _______,          KC_MPLY, KC_VOLD,
        _______, _______, _______,                            EE_CLR,                             KC_APP,  _______, KC_MPRV, KC_MSTP, KC_MNXT
    ),
    /*
    [DEFAULT] = LAYOUT(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   KC_PGDN,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(1),   KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [X] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______
    ),
    [RGB] = LAYOUT(
        ESC: 0,    1: 1,      2: 2,      3: 3,    4: 4,    5: 5,    6: 6,    7: 7,    8: 8,      9: 9,      0: 10,      MINS: 11,   EQL: 12,   BSPC: 13,  DEL: 14,
        TAB: 15,   Q: 16,     W: 17,     E: 18,   R: 19,   T: 20,   Y: 21,   U: 22,   I: 23,     O: 24,     P: 25,      LBRC: 26,   RBRC: 27,  BSLS: 28,  HOME: 29,
        CAPS: 30,  A: 31,     S: 32,     D: 33,   F: 34,   G: 35,   H: 36,   J: 37,   K: 38,     L: 39,     SCLN: 40,   QUOT: 41,              ENT: 42,   PGUP: 43,
        LSFT: 44,  Z: 45,     X: 46,     C: 47,   V: 48,   B: 49,   N: 50,   M: 51,   COMM: 52,  DOT: 53,   SLSH: 54,   RSFT: 55,              UP: 56,    PGDN: 57,
        LCTL: 58,  LGUI: 59,  LALT: 60,                    SPC: 61,                              RALT: 62,  FN: 63,                  LEFT: 64, DOWN: 65,  RGHT: 66
        // UnderGlow
        :67, :68, :69, :70, :71, :72, :73, :74, :75, :76, :77, :78, :79, :80, :81,
        :82, :83, :84, :85,
        :86, :87, :88, :89,
        :90, :91, :92, :93, :94, :95, :96, :97, :98, :99, :100, :101, :102, :103, :104, :105,
    ),
    [MATRIX] = LAYOUT(
        0,       1,       2,       3,       4,       5,       6,       7,       8,       9,       10,      11,      12       13       14,
        15,      16,      17,      18,      19,      20,      21,      22,      23,      24,      25,      26,      27,      28,      29,
        30,      31,      32,      33,      34,      35,      36,      37,      38,      39,      40,      41,               42,      43,
        44,      45,      46,      47,      48,      49,      50,      51,      52,      53,      54,      55,               56,      57,
        58,      59,      60,                                 61,                                 62,      63,      64,      65,      66
        // UnderGlow
        90,     91,     92,     93,     94,     95,     96,     97,     98,     99,     100,    101,    102,    103,        104,        105,
        88,                                                                                                                             89,
        86,                                                                                                                             87,
        84,                                                                                                                             85,
        67,     68,     69,     70,     71,     72,     73,     74,     75,     76,     77,     78,     79,     80,     81,     82,     83
    ),
    */
};

#ifdef TAP_REPEAT_ENABLE
#   define TRHSV RED
#else
#   define TRHSV _______
#endif

#ifdef KEY_LOCK_TOGGLE_ENABLE
#   define KLHSV RED
#else
#   define KLHSV _______
#endif

# undef _______
# define _______ {0,0,0}

// This is to denote that the led value is set programatically
# ifndef CUSTHSV
#  define CUSTHSV _______
# endif

const uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3] = {
    [_FL] = {
        WHITE,   WHITE,   WHITE,   WHITE,   WHITE,   WHITE,   WHITE,   WHITE,   WHITE,   WHITE,   WHITE,   WHITE,   WHITE,   _______, AZURE,
        _______, GOLD,    CUSTHSV, GOLD,    CUSTHSV, CUSTHSV, _______, MAGENT,  BLUE,    WHITE,   GREEN,   BLUE,    ORANGE,  _______, ORANGE,
        TRHSV,   CUSTHSV, CUSTHSV, CUSTHSV, CUSTHSV, CUSTHSV, _______, RED,     CYAN,    PURPLE,  _______, _______,          KLHSV,   AZURE,
        _______, TEAL,    _______, _______, _______, RED,     _______, YELLOW,  GREEN,   CREAM,   _______, _______,          GREEN,   AZURE,
        _______, _______, _______,                            _______,                            _______, _______, YELLOW,  RED,     YELLOW,
        // UnderGlow
        CHART,   CHART,   CHART,   CHART,   CHART,   CHART,   CHART,   CHART,   CHART,   CHART,   CHART,   CHART,   CHART,   CHART,   CHART,
        CHART,                                                                                                                        CHART,
        CHART,                                                                                                                        CHART,
        CHART,                                                                                                                        CHART,
        CHART,                                                                                                                        CHART,
        CHART,   CHART,   CHART,   CHART,   CHART,   CHART,   CHART,   CHART,   CHART,   CHART,   CHART,   CHART,   CHART,   CHART,   CHART
    },
    /*
    [_FL] = LAYOUT(
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, KC_MUTE,
        _______, RGB_RMOD,RGB_VAI, RGB_MOD, RGB_HUI, RGB_SAI, _______, MAS_MGT, MAS_BLU, MAS_WHT, KC_PSCR, KC_SCRL, KC_PAUS, _______, KC_END,
        BN_TRTG, RGB_SPD, RGB_VAD, RGB_SPI, RGB_HUD, RGB_SAD, _______, MAS_RED, MAS_CYN, MAS_PRP, _______, _______,          BN_KLTG, KC_VOLU,
        _______, RGB_TOG, _______, _______, _______, MD_BOOT, _______, MAS_YEL, MAS_GRN, MAS_CRM, _______, _______,          KC_MPLY, KC_VOLD,
        _______, _______, _______,                            _______,                            _______, _______, KC_MPRV, KC_MSTP, KC_MNXT
    ),
    */
};

# undef _______
# define _______ KC_TRNS
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // The only custom processing is for non-standard keys. So avoid any extra processing
    // for codes in the standard/basic range.
    if (record->event.pressed && keycode > QK_BASIC_MAX) {
        switch (keycode) {
            // ======================================================== CUSTOM KEYCODES BELOW ========================================================
            case MAS_CRM:
                rgb_matrix_sethsv(HSV_CREAM);
                return false;
            case MAS_PRP:
                rgb_matrix_sethsv(HSV_9B59B5);
                return false;
            case MAS_RED:
                rgb_matrix_sethsv(HSV_RED);
                return false;
            case MAS_GRN:
                rgb_matrix_sethsv(HSV_GREEN);
                return false;
            case MAS_BLU:
                rgb_matrix_sethsv(HSV_BLUE);
                return false;
            case MAS_CYN:
                rgb_matrix_sethsv(HSV_CYAN);
                return false;
            case MAS_MGT:
                rgb_matrix_sethsv(HSV_MAGENTA);
                return false;
            case MAS_YEL:
                rgb_matrix_sethsv(HSV_YELLOW);
                return false;
            case MAS_KEY:
                rgb_matrix_sethsv(0, 0, 0);
                return false;
            case MAS_WHT:
                rgb_matrix_sethsv(128, 0, 255);
                return false;
            default:
                return true;  // Process all other keycodes normally
        }
    }
    return true;
}

bool set_layer_color(int layer) {
    if (layer == _KL) {
        return true;
    }
    // Non default layers
    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        HSV hsv = {
            .h = pgm_read_byte(&ledmap[layer][i][0]),
            .s = pgm_read_byte(&ledmap[layer][i][1]),
            .v = pgm_read_byte(&ledmap[layer][i][2]),
        };
        if (hsv.h || hsv.s || hsv.v) {
            RGB   rgb = hsv_to_rgb(hsv);
            float f   = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
            rgb_matrix_set_color(i, f * rgb.r, f * rgb.g, f * rgb.b);
        } else if (layer == _FL) {
            // For the dynamic keys on _FL layer
            // This is not efficient, but it also doesnt need to be because it is the function layer
            HSV hsv2   = {rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, RGB_MATRIX_MAXIMUM_BRIGHTNESS};
            HSV hui    = hsv2;
            HSV hud    = hsv2;
            HSV sai    = hsv2;
            HSV sad    = hsv2;
            HSV vai    = hsv2;
            HSV vad    = hsv2;
            HSV spd    = hsv2;
            HSV spi    = hsv2;
            hui.h      = hsv2.h + RGB_MATRIX_HUE_STEP;
            hud.h      = hsv2.h - RGB_MATRIX_HUE_STEP;
            sai.s      = qadd8(hsv2.s, RGB_MATRIX_SAT_STEP);
            sad.s      = qsub8(hsv2.s, RGB_MATRIX_SAT_STEP);
            vai.v      = hsv2.v + RGB_MATRIX_VAL_STEP > RGB_MATRIX_MAXIMUM_BRIGHTNESS ? RGB_MATRIX_MAXIMUM_BRIGHTNESS : rgb_matrix_config.hsv.v + RGB_MATRIX_VAL_STEP;
            vad.v      = qsub8(hsv2.v, RGB_MATRIX_VAL_STEP);
            spd.h      = scale8(255 - qsub8(rgb_matrix_config.speed, RGB_MATRIX_SPD_STEP), 92);
            spi.h      = scale8(255 - qadd8(rgb_matrix_config.speed, RGB_MATRIX_SPD_STEP), 92);
            RGB rgbHUI = hsv_to_rgb(hui);
            RGB rgbHUD = hsv_to_rgb(hud);
            RGB rgbSAI = hsv_to_rgb(sai);
            RGB rgbSAD = hsv_to_rgb(sad);
            RGB rgbVAI = hsv_to_rgb(vai);
            RGB rgbVAD = hsv_to_rgb(vad);
            RGB rgbSPD = hsv_to_rgb(spd);
            RGB rgbSPI = hsv_to_rgb(spi);

            switch (i) {
                case 19: // R
                    rgb_matrix_set_color(i, rgbHUI.r, rgbHUI.g, rgbHUI.b);
                    break;
                case 34: // F
                    rgb_matrix_set_color(i, rgbHUD.r, rgbHUD.g, rgbHUD.b);
                    break;
                case 20: // T
                    rgb_matrix_set_color(i, rgbSAI.r, rgbSAI.g, rgbSAI.b);
                    break;
                case 35: // G
                    rgb_matrix_set_color(i, rgbSAD.r, rgbSAD.g, rgbSAD.b);
                    break;
                case 17: // W
                    rgb_matrix_set_color(i, rgbVAI.r, rgbVAI.g, rgbVAI.b);
                    break;
                case 32: // S
                    rgb_matrix_set_color(i, rgbVAD.r, rgbVAD.g, rgbVAD.b);
                    break;
                case 31: // A
                    rgb_matrix_set_color(i, rgbSPD.r, rgbSPD.g, rgbSPD.b);
                    break;
                case 33: // D
                    rgb_matrix_set_color(i, rgbSPI.r, rgbSPI.g, rgbSPI.b);
                    break;
                default:
                    rgb_matrix_set_color(i, 0, 0, 0);
                    break;
            }
        }
    }
    return false;
}

#ifdef LOCK_INDICATOR_INVERT_COLOR_ENABLE
// grants access to LED PWM buffers from drivers\led\issi\is31fl3733.c
extern uint8_t g_pwm_buffer[IS31FL3733_DRIVER_COUNT][192];

RGB get_led_rgb_one(uint8_t i) {
    RGB led_color;

    is31fl3733_led_t led;
    if (i >= 0 && i < IS31FL3733_LED_COUNT) {
        memcpy_P(&led, (&g_is31fl3733_leds[i]), sizeof(led));
        led_color.r = g_pwm_buffer[led.driver][led.r];
        led_color.g = g_pwm_buffer[led.driver][led.g];
        led_color.b = g_pwm_buffer[led.driver][led.b];
    }

    return led_color;
}

void invert_led_rgb_one(uint8_t i) {
    RGB led_color = get_led_rgb_one(i);
    led_color.r = 255 - led_color.r;
    led_color.g = 255 - led_color.g;
    led_color.b = 255 - led_color.b;
    rgb_matrix_set_color(i, led_color.r, led_color.g, led_color.b);
}
#endif // INDICATOR_INVERT_COLOR_ENABLE

bool rgb_matrix_indicators_user(void) {
    if (disable_layer_color ||
        rgb_matrix_get_flags() == LED_FLAG_NONE ||
        rgb_matrix_get_flags() == LED_FLAG_UNDERGLOW
    ) {
        // Let rgb_matrix_indicators_kb do whatever it wants
        return false;
    }

    if (!(
#if defined(KEY_LOCK_TOGGLE_RGB) && defined(KEY_LOCK_TOGGLE_ENABLE)
        rgb_matrix_indicators_key_lock_toggle(-1) &&
#endif
#if defined(TAP_REPEAT_RGB) && defined(TAP_REPEAT_ENABLE)
        rgb_matrix_indicators_tap_repeat(30) &&
#endif
        set_layer_color(get_highest_layer(layer_state)))
    ) {
        return false;
    }

#ifdef LOCK_INDICATOR_INVERT_COLOR_ENABLE
# if RGB_MATRIX_CAPS_LOCK_INDEX
    if (host_keyboard_led_state().caps_lock && (rgb_matrix_get_flags() & LED_FLAG_INDICATOR)) {
        invert_led_rgb_one(RGB_MATRIX_CAPS_LOCK_INDEX);
    }
# endif // RGB_MATRIX_CAPS_LOCK_INDEX
    return false;
#else
    return true;
#endif //LOCK_INDICATOR_INVERT_COLOR_ENABLE
}
