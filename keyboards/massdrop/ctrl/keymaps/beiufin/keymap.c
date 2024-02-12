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

#include QMK_KEYBOARD_H
#include <lib/lib8tion/lib8tion.h>

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


#if defined(TAP_REPEAT_RGB) && defined(TAP_REPEAT_ENABLE)
#    include "tap_repeat.h"
#endif

#if defined(KEY_LOCK_TOGGLE_RGB) && defined(KEY_LOCK_TOGGLE_ENABLE)
#    include "key_lock_toggle.h"
#endif

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
extern bool         g_suspend_state;
extern rgb_config_t rgb_matrix_config;
bool                disable_layer_color;

bool rgb_enabled_flag;  // Current LED state flag. If false then LED is off.

enum layout_names {
    _KL = 0,  // Keys Layout: The main keyboard layout that has all the characters
    _FL,      // Function Layout: The function key activated layout with default functions and some added ones
};

enum ctrl_keycodes {
    U_T_AUTO = SAFE_RANGE,  // USB Extra Port Toggle Auto Detect / Always Active
    U_T_AGCR,               // USB Toggle Automatic GCR control
    DBG_TOG,                // DEBUG Toggle On / Off
    DBG_MTRX,               // DEBUG Toggle Matrix Prints
    DBG_KBD,                // DEBUG Toggle Keyboard Prints
    DBG_MOU,                // DEBUG Toggle Mouse Prints
    MAS_CRM,
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
        _______, _______, _______, _______, _______, _______, _______, _______, _______, DBG_TOG, DBG_KBD, EE_CLR,  _______,            KC_MUTE, KC_MSEL, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   KC_MSTP, KC_MPLY, KC_VOLU,
        _______, RGB_MOD, RGB_SPI, RGB_HUI, RGB_SAI, RGB_VAI, _______, _______, MAS_MGT, MAS_BLU, MAS_WHT, RGB_RMOD,RGB_MOD, _______,   KC_MPRV, KC_MNXT, KC_VOLD,
        BN_TRTG, RGB_RMOD,RGB_SPD, RGB_HUD, RGB_SAD, RGB_VAD, _______, MAS_RED, MAS_KEY, MAS_CYN, MAS_PRP, _______, _______,
        _______, RGB_TOG, _______, _______, _______, QK_BOOT, NK_TOGG, MAS_YEL, MAS_GRN, MAS_CRM, _______, _______,                              RGB_VAI,
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
#   define CUSTHSV _______
#endif

#ifdef TAP_REPEAT_ENABLE
#   define TRHSV RED
#else
#   define TRHSV _______
#endif

const uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3] = {
    [_FL] = {
        _______, _______, _______, _______, _______, _______, _______, _______, _______, RED,     RED,     RED,     _______,            RED,     GREEN,   _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   RED,     GREEN,   AZURE,
        _______, GOLD,    CUSTHSV, CUSTHSV, CUSTHSV, CUSTHSV, _______, _______, MAGENT,  BLUE,    WHITE,   GOLD,    GOLD,    _______,   ORANGE,  ORANGE,  AZURE,
        TRHSV,   GOLD,    CUSTHSV, CUSTHSV, CUSTHSV, CUSTHSV, _______, RED,     {1,0,0}, CYAN,    M9B59B5, _______, _______,
        _______, RED,     _______, _______, _______, RED,     PINK,    YELLOW,  GREEN,   CREAM,   _______, _______,                              CUSTHSV,
        _______, _______, _______,                   _______,                            _______, WHITE,   _______, _______,            CUSTHSV, CUSTHSV, CUSTHSV
    },
    /*
    [_FL] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, DBG_TOG, DBG_KBD, EE_CLR,  _______,            KC_MUTE, KC_MSEL, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   KC_MSTP, KC_MPLY, KC_VOLU,
        _______, RGB_MOD, RGB_SPI, RGB_HUI, RGB_SAI, RGB_VAI, _______, _______, MAS_MGT, MAS_BLU, MAS_WHT, RGB_RMOD,RGB_MOD, _______,   KC_MPRV, KC_MNXT, KC_VOLD,
        BN_TRTG, RGB_RMOD,RGB_SPD, RGB_HUD, RGB_SAD, RGB_VAD, _______, MAS_RED, MAS_KEY, MAS_CYN, MAS_PRP, _______, _______,
        _______, RGB_TOG, _______, _______, _______, QK_BOOT, NK_TOGG, MAS_YEL, MAS_GRN, MAS_CRM, _______, _______,                              RGB_VAI,
        _______, _______, _______,                   _______,                            _______, _LAYER_, _______, _______,            RGB_SPD, RGB_VAD, RGB_SPI
    ),
    */
};
#undef _______
#define _______ KC_TRNS

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
    rgb_enabled_flag = true;  // Initially, keyboard RGB is enabled. Change to false config.h initializes RGB disabled.
};

void keyboard_post_init_user(void) {
    rgb_matrix_enable();
}

#define MODS_SHIFT  (get_mods() & MOD_MASK_SHIFT)
#define MODS_CTRL   (get_mods() & MOD_MASK_CTRL)
#define MODS_ALT    (get_mods() & MOD_MASK_ALT)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
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
            case RGB_TOG:
                switch (rgb_matrix_get_flags()) {
                    case LED_FLAG_ALL: {
                        rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER);
                        rgb_matrix_set_color_all(0, 0, 0);
                    } break;
                    case LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER: {
                        rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
                        rgb_matrix_set_color_all(0, 0, 0);
                    } break;
                    case LED_FLAG_UNDERGLOW: {
                        // This line is for LED idle timer. It disables the toggle so you can turn off LED completely if you like
                        rgb_matrix_set_flags(LED_FLAG_NONE);
                        rgb_matrix_disable_noeeprom();
                    } break;
                    default: {
                        rgb_matrix_set_flags(LED_FLAG_ALL);
                        rgb_matrix_enable_noeeprom();
                    } break;
                }
                return false;
            // ======================================================== CUSTOM KEYCOADS BELOW ========================================================
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
                case 36:
                    rgb_matrix_set_color(i, rgbHUI.r, rgbHUI.g, rgbHUI.b);
                    break;
                case 53:
                    rgb_matrix_set_color(i, rgbHUD.r, rgbHUD.g, rgbHUD.b);
                    break;
                case 54:
                    rgb_matrix_set_color(i, rgbSAI.r, rgbSAI.g, rgbSAI.b);
                    break;
                case 37:
                    rgb_matrix_set_color(i, rgbSAD.r, rgbSAD.g, rgbSAD.b);
                    break;
                case 75:
                case 38:
                    rgb_matrix_set_color(i, rgbVAI.r, rgbVAI.g, rgbVAI.b);
                    break;
                case 85:
                case 55:
                    rgb_matrix_set_color(i, rgbVAD.r, rgbVAD.g, rgbVAD.b);
                    break;
                case 84:
                case 52:
                    rgb_matrix_set_color(i, rgbSPD.r, rgbSPD.g, rgbSPD.b);
                    break;
                case 86:
                case 35:
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

bool rgb_matrix_indicators_user(void) {
    if (disable_layer_color ||
        rgb_matrix_get_flags() == LED_FLAG_NONE ||
        rgb_matrix_get_flags() == LED_FLAG_UNDERGLOW
    ) {
        return false;
    }
    if (!(
#if defined(KEY_LOCK_TOGGLE_RGB) && defined(KEY_LOCK_TOGGLE_ENABLE)
        rgb_matrix_indicators_key_lock_toggle(14) &&
#endif
#if defined(TAP_REPEAT_RGB) && defined(TAP_REPEAT_ENABLE)
        rgb_matrix_indicators_tap_repeat(50) &&
#endif
        set_layer_color(get_highest_layer(layer_state)))
    ) {
        return false;
    }
    return true;
}
