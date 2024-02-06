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

#include "key_lock_toggle.h"
#include "rgb_matrix.h"

#define KLT_BV_64(shift) (((uint64_t)1) << (shift))
#define KLT_GET_KEY_ARRAY(code) (((code) < 0x40) ? KLT_KEY_STATE[0] : ((code) < 0x80) ? KLT_KEY_STATE[1] : ((code) < 0xC0) ? KLT_KEY_STATE[2] : KLT_KEY_STATE[3])
#define KLT_GET_CODE_INDEX(code) (((code) < 0x40) ? (code) : ((code) < 0x80) ? (code)-0x40 : ((code) < 0xC0) ? (code)-0x80 : (code)-0xC0)
#define KLT_KEY_STATE(code) (KLT_GET_KEY_ARRAY(code) & KLT_BV_64(KLT_GET_CODE_INDEX(code))) == KLT_BV_64(KLT_GET_CODE_INDEX(code))
#define KLT_SET_KEY_ARRAY_STATE(code, val) \
    do {                               \
        switch (code) {                \
            case 0x00 ... 0x3F:        \
                KLT_KEY_STATE[0] = (val);  \
                break;                 \
            case 0x40 ... 0x7F:        \
                KLT_KEY_STATE[1] = (val);  \
                break;                 \
            case 0x80 ... 0xBF:        \
                KLT_KEY_STATE[2] = (val);  \
                break;                 \
            case 0xC0 ... 0xFF:        \
                KLT_KEY_STATE[3] = (val);  \
                break;                 \
        }                              \
    } while (0)
#define KLT_SET_KEY_STATE(code) KLT_SET_KEY_ARRAY_STATE(code, (KLT_GET_KEY_ARRAY(code) | KLT_BV_64(KLT_GET_CODE_INDEX(code))))
#define KLT_UNSET_KEY_STATE(code) KLT_SET_KEY_ARRAY_STATE(code, (KLT_GET_KEY_ARRAY(code)) & ~(KLT_BV_64(KLT_GET_CODE_INDEX(code))))

// This is an array of 256 bits, one for each of the standard keys supported qmk.
uint64_t KLT_KEY_STATE[4] = {0x0, 0x0, 0x0, 0x0};
#ifdef KEY_LOCK_TOGGLE_RGB
static bool klt_leds[RGB_MATRIX_LED_COUNT] = {};
#endif // KEY_LOCK_TOGGLE_RGB

static bool watching = false;
static uint8_t locked_count = 0;

inline bool is_klt_watching(void) {
    return watching;
}

inline bool is_key_locked(uint16_t translated_keycode){
    return KLT_KEY_STATE(translated_keycode);
}

inline void set_klt_watching(bool new_watching) {
    watching = new_watching;
}

static void set_led_state(keyrecord_t *record, bool state) {
#ifdef KEY_LOCK_TOGGLE_RGB
    uint8_t row = record->event.key.row;
    uint8_t col = record->event.key.col;
    uint8_t idx = g_led_config.matrix_co[row][col];
    klt_leds[idx] = state;
#endif //KEY_LOCK_TOGGLE_RGB
}

inline void lock_key(uint16_t translated_keycode, keyrecord_t *record) {
    if (!(KLT_KEY_STATE(translated_keycode))) {
        locked_count++;
    }
    KLT_SET_KEY_STATE(translated_keycode);
    set_led_state(record, true);
}

inline void unlock_key(uint16_t translated_keycode, keyrecord_t *record) {
    if (!(KLT_KEY_STATE(translated_keycode))) {
        locked_count--;
    }
    KLT_UNSET_KEY_STATE(translated_keycode);
    set_led_state(record, false);
}

#ifdef KEY_LOCK_TOGGLE_RGB
static inline RGB hsv_to_adjusted_rgb(uint8_t hue, uint8_t sat, uint8_t val){
    HSV hsv = {hue, sat, val};
    if (hsv.v > rgb_matrix_get_val()) {
        hsv.v = rgb_matrix_get_val();
    }
    return hsv_to_rgb(hsv);
}

static RGB get_inverted_color(void) {
    HSV hsv = rgb_matrix_get_hsv();
    RGB rgb = hsv_to_rgb(hsv);
    rgb.r = 255 - rgb.r;
    rgb.g = 255 - rgb.g;
    rgb.b = 255 - rgb.b;
    return rgb;
}

bool rgb_matrix_indicators_key_lock_toggle(int kl_togg_idx) {
    if (watching) {
        for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
            RGB rgb = {0,0,0};
            if (i == kl_togg_idx) {
                continue;
            } else if (klt_leds[i]) {
                rgb = hsv_to_adjusted_rgb(KEY_LOCK_TOGGLE_HSV);
            }
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
        return false;
    } else {
        if (locked_count > 0) {
            RGB rgb = get_inverted_color();
            rgb_matrix_set_color(kl_togg_idx, rgb.r, rgb.g, rgb.b);
            for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
                if (klt_leds[i]) {
                    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
                }
            }
        }
        return true;
    }
    return true;
}
#endif //KEY_LOCK_TOGGLE_RGB
