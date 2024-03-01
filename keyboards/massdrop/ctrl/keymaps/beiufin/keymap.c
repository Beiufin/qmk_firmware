/*
QMK Firmware Massdrop CTRL Beiufin Keymap
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

#define _LAYER_ KC_TRNS

//========================================================== CONFIGURABLE DEFAULTS ==========================================================
extern bool g_suspend_state;

bool rgb_enabled_flag; // Current LED state flag. If false then LED is off.

enum ctrl_keycodes {
    U_T_AUTO = NEW_SAFE_RANGE, // USB Extra Port Toggle Auto Detect / Always Active
    U_T_AGCR,                  // USB Toggle Automatic GCR control
    DBG_TOG,                   // DEBUG Toggle On / Off
    DBG_MTRX,                  // DEBUG Toggle Matrix Prints
    DBG_KBD,                   // DEBUG Toggle Keyboard Prints
    DBG_MOU,                   // DEBUG Toggle Mouse Prints
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_KL] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,             KC_PSCR, KL_TOGG, KC_PAUS,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,   KC_INS,  KC_HOME, KC_PGUP,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,   KC_DEL,  KC_END,  KC_PGDN,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,                              KC_UP,
        KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                             KC_RALT, MO(_FL), KC_APP,  KC_RCTL,            KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [_FL] = LAYOUT(
        RGB_TOG, RGB_C_M, RGB_C_U, RGB_C_K, _______, _______, _______, _______, _______, DBG_TOG, DBG_KBD, EE_CLR,  _______,            KC_MUTE, KC_MSEL, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   KC_MSTP, KC_MPLY, KC_VOLU,
        _______, RGB_MOD, RGB_SPI, RGB_HUI, RGB_SAI, RGB_VAI, _______, _______, BEI_MGT, BEI_BLU, BEI_WHT, RGB_RMOD,RGB_MOD, _______,   KC_MPRV, KC_MNXT, KC_VOLD,
        BN_TRTG, RGB_RMOD,RGB_SPD, RGB_HUD, RGB_SAD, RGB_VAD, _______, BEI_RED, BEI_KEY, BEI_CYN, BEI_PRP, _______, _______,
        _______, _______, _______, _______, _______, QK_BOOT, NK_TOGG, BEI_YEL, BEI_GRN, BEI_CRM, _______, _______,                              RGB_VAI,
        _______, _______, _______,                   DBG_TOG,                            _______, _______, _______, _______,            RGB_SPD, RGB_VAD, RGB_SPI
    ),
    /*
    [DEFAULT] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,           KC_PSCR, KL_TOGG, KC_PAUS,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_INS,  KC_HOME, KC_PGUP,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL,  KC_END,  KC_PGDN,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,                            KC_UP,
        KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                             KC_RALT, MO(1),   KC_APP,  KC_RCTL,          KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [X] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, TG_NKRO, _______, _______, _______, _______, _______,                            _______,
        _______, _______, _______,                   _______,                            _______, _______, _______, _______,          _______, _______, _______
    ),
    [RGB] = LAYOUT(
        ESC: 0,   F1: 1,    F2: 2,    F3: 3,    F4: 4,    F5: 5,    F6: 6,    F7: 7,    F8: 8,    F9: 9,    F10: 10,  F11: 11,  F12: 12,            PSCR: 13, SLCK: 14, PAUS: 15,
        GRV: 16,  1: 17,    2: 18,    3: 19,    4: 20,    5: 21,    6: 22,    7: 23,    8: 24,    9: 25,    0: 26,    MINS: 27, EQL: 28,  BSPC: 29, INS: 30,  HOME: 31, PGUP: 32,
        TAB: 33,  Q: 34,    W: 35,    E: 36,    R: 37,    T: 38,    Y: 39,    U: 40,    I: 41,    O: 42,    P: 43,    LBRC: 44, RBRC: 45, BSLS: 46, DEL: 47,  END: 48,  PGDN: 49,
        CAPS: 50, A: 51,    S: 52,    D: 53,    F: 54,    G: 55,    H: 56,    J: 57,    K: 58,    L: 59,    SCLN: 60, QUOT: 61, ENT: 62,
        LSFT: 63, Z: 64,    X: 65,    C: 66,    V: 67,    B: 68,    N: 69,    M: 70,    COMM: 71, DOT: 72,  SLSH: 73, RSFT: 74,                               UP: 75,
        LCTL: 76, LGUI: 77, LALT: 78,                   SPC: 79,                                  RALT: 80, Fn: 81,   APP: 82,  RCTL: 83,           LEFT: 84, DOWN: 85, RGHT: 86
    ),
    [MATRIX] = LAYOUT(
        0,       1,       2,       3,       4,       5,       6,       7,       8,       9,       10,      11,      12,                 13,      14,      15,
        16,      17,      18,      19,      20,      21,      22,      23,      24,      25,      26,      27,      28,      29,        30,      31,      32,
        33,      34,      35,      36,      37,      38,      39,      40,      41,      42,      43,      44,      45,      46,        47,      48,      49,
        50,      51,      52,      53,      54,      55,      56,      57,      58,      59,      60,      61,      62,
        63,      64,      65,      66,      67,      68,      69,      70,      71,      72,      73,      74,                                   75,
        76,      77,      78,                        79,                                 80,      81,      82,      83,                 84,      85,      86
        //UnderGlow
        103,    104,    105,    106,    107,    108,    109,    110,    111,    112,    113,    114,    115,
        102,                                                                                            116,
        101,                                                                                            117,
        100,                                                                                            118,
        99,     98,     97,     96,     95,     94,     93,     92,     91,     90,     89,     88,     87,
    ),
    */
};

#undef _______
#define _______ {0,0,0}


// This is to denote that the led value is set programatically
#ifndef CUSTHSV
#    define CUSTHSV _______
#endif

#ifdef TAP_REPEAT_ENABLE
#    define TRHSV RED
#else
#    define TRHSV _______
#endif

const uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3] = {
    [_FL] = {
        RED,     RED,     RED,     RED,     _______, _______, _______, _______, _______, RED,     RED,     RED,     _______,            RED,     GREEN,   _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   RED,     GREEN,   AZURE,
        _______, GOLD,    CUSTHSV, CUSTHSV, CUSTHSV, CUSTHSV, _______, _______, MAGENT,  BLUE,    WHITE,   GOLD,    GOLD,    _______,   ORANGE,  ORANGE,  AZURE,
        TRHSV,   GOLD,    CUSTHSV, CUSTHSV, CUSTHSV, CUSTHSV, _______, RED,     {1,0,0}, CYAN,    M9B59B5, _______, _______,
        _______, _______, _______, _______, _______, RED,     PINK,    YELLOW,  GREEN,   CREAM,   _______, _______,                              CUSTHSV,
        _______, _______, _______,                   _______,                            _______, WHITE,   _______, _______,            CUSTHSV, CUSTHSV, CUSTHSV
    },
    /*
    [_FL] = LAYOUT(
        RGB_TOG, RGB_C_M, RGB_C_U, RGB_C_K, _______, _______, _______, _______, _______, DBG_TOG, DBG_KBD, EE_CLR,  _______,            KC_MUTE, KC_MSEL, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   KC_MSTP, KC_MPLY, KC_VOLU,
        _______, RGB_MOD, RGB_SPI, RGB_HUI, RGB_SAI, RGB_VAI, _______, _______, BEI_MGT, BEI_BLU, BEI_WHT, RGB_RMOD,RGB_MOD, _______,   KC_MPRV, KC_MNXT, KC_VOLD,
        BN_TRTG, RGB_RMOD,RGB_SPD, RGB_HUD, RGB_SAD, RGB_VAD, _______, BEI_RED, BEI_KEY, BEI_CYN, BEI_PRP, _______, _______,
        _______, _______, _______, _______, _______, QK_BOOT, NK_TOGG, BEI_YEL, BEI_GRN, BEI_CRM, _______, _______,                              RGB_VAI,
        _______, _______, _______,                   _______,                            _______, _LAYER_, _______, _______,            RGB_SPD, RGB_VAD, RGB_SPI
    ),
    */
};
#undef _______
#define _______ KC_TRNS
// clang-format on

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
    rgb_enabled_flag = true; // Initially, keyboard RGB is enabled. Change to false config.h initializes RGB disabled.
};

void keyboard_post_init_user(void) {
    rgb_matrix_enable();
}

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    // The only custom processing is for non-standard keys. So avoid any extra processing
    // for codes in the standard/basic range.
    if (record->event.pressed && keycode > QK_BASIC_MAX) {
        switch (keycode) {
            case U_T_AUTO:
                if (MODS_SHIFT && MODS_CTRL) {
                    TOGGLE_FLAG_AND_PRINT(usb_extra_manual, "USB extra port manual mode");
                }
                return false;
            case U_T_AGCR:
                if (MODS_SHIFT && MODS_CTRL) {
                    TOGGLE_FLAG_AND_PRINT(usb_gcr_auto, "USB GCR auto mode");
                }
                return false;
            case DBG_TOG:
                TOGGLE_FLAG_AND_PRINT(debug_enable, "Debug mode");
                return false;
            case DBG_MTRX:
                TOGGLE_FLAG_AND_PRINT(debug_matrix, "Debug matrix");
                return false;
            case DBG_KBD:
                TOGGLE_FLAG_AND_PRINT(debug_keyboard, "Debug keyboard");
                return false;
            case DBG_MOU:
                TOGGLE_FLAG_AND_PRINT(debug_mouse, "Debug mouse");
                return false;
            default:
                return true; // Process all other keycodes normally
        }
    }
    return true;
}
