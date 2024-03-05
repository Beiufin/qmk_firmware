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
#include QMK_KEYBOARD_H
#include "beiufin.h"

// HID has not yet been implemented for this keyboard
// #include "raw_hid.h"

#define _LAYER_ KC_TRNS

//========================================================== CONFIGURABLE DEFAULTS ==========================================================
extern rgb_config_t rgb_matrix_config;
bool                disable_layer_color;
bool                rgb_enabled_flag; // Current LED state flag. If false then LED is off.

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
        _______, RGB_RMOD,RGB_VAI, RGB_MOD, RGB_HUI, RGB_SAI, _______, BEI_MGT, BEI_BLU, BEI_WHT, KC_PSCR, KC_SCRL, KC_PAUS, _______, KC_END,
        BN_TRTG, RGB_SPD, RGB_VAD, RGB_SPI, RGB_HUD, RGB_SAD, _______, BEI_RED, BEI_CYN, BEI_PRP, _______, _______,          BN_KLTG, KC_VOLU,
        _______, RGB_TOG, RGB_C_M, RGB_C_U, RGB_C_K, QK_BOOT, NK_TOGG, BEI_YEL, BEI_GRN, BEI_CRM, _______, _______,          KC_MPLY, KC_VOLD,
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
        _______, TEAL,    TEAL,    TEAL,    TEAL,    RED,     _______, YELLOW,  GREEN,   CREAM,   _______, _______,          GREEN,   AZURE,
        _______, _______, _______,                            _______,                            WHITE,   _______, YELLOW,  RED,     YELLOW,
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
        _______, RGB_RMOD,RGB_VAI, RGB_MOD, RGB_HUI, RGB_SAI, _______, BEI_MGT, BEI_BLU, BEI_WHT, KC_PSCR, KC_SCRL, KC_PAUS, _______, KC_END,
        BN_TRTG, RGB_SPD, RGB_VAD, RGB_SPI, RGB_HUD, RGB_SAD, _______, BEI_RED, BEI_CYN, BEI_PRP, _______, _______,          BN_KLTG, KC_VOLU,
        _______, RGB_TOG, _______, _______, _______, MD_BOOT, _______, BEI_YEL, BEI_GRN, BEI_CRM, _______, _______,          KC_MPLY, KC_VOLD,
        _______, _______, _______,                            _______,                            _______, _______, KC_MPRV, KC_MSTP, KC_MNXT
    ),
    */
};

# undef _______
# define _______ KC_TRNS
// clang-format on

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
    led_color.r   = 255 - led_color.r;
    led_color.g   = 255 - led_color.g;
    led_color.b   = 255 - led_color.b;
    rgb_matrix_set_color(i, led_color.r, led_color.g, led_color.b);
}
#endif // INDICATOR_INVERT_COLOR_ENABLE

bool rgb_matrix_indicators_keymap(void) {
#ifdef LOCK_INDICATOR_INVERT_COLOR_ENABLE
#    if RGB_MATRIX_CAPS_LOCK_INDEX
    if (host_keyboard_led_state().caps_lock && (rgb_matrix_get_flags() & LED_FLAG_INDICATOR)) {
        invert_led_rgb_one(RGB_MATRIX_CAPS_LOCK_INDEX);
    }
#    endif // RGB_MATRIX_CAPS_LOCK_INDEX
    return false;
#else
    return true;
#endif // LOCK_INDICATOR_INVERT_COLOR_ENABLE
}
