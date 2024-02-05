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

#include "tap_repeat.h"
#include "rgb_matrix.h"

#define IS_STANDARD_KEYCODE(code) ((code) <= 0xFF)

static bool    watching     = false;
// This is optimized for time over memory. Keeping an array takes up much more space than
static uint8_t repeating_keys[TAP_REPEAT_MAX] = {};
static uint8_t count = 0;
static uint32_t tap_timer;

inline bool is_tap_repeat_watching(void) {
    return watching;
}

inline bool toggle_tap_repeat_watching(void) {
    watching = !watching;
    return watching;
}

static inline bool tap_current_keys(void) {
    if (count == 0) {
        return false;
    }
    if (!watching) {
        uint8_t i = 0;
        while (i < count) tap_code(repeating_keys[i++]);
    }
    return true;
}


bool add_repeat_key(uint16_t translated_keycode) {
    if (count >= TAP_REPEAT_MAX) {
        return false;
    }
    repeating_keys[count++] = translated_keycode;
    return true;
}

int16_t find_repeat_key_index(uint16_t translated_keycode) {
    uint8_t i = 0;
    while ( i < count && repeating_keys[i] != translated_keycode ) ++i;

    return ( i == count ? -1 : i );
}

bool remove_repeat_key_by_index(int8_t index) {
    if (index < 0 || index >= count) {
        return false;
    }
    repeating_keys[index] = repeating_keys[--count];
    return true;
}


#ifdef TAP_REPEAT_RGB
static bool tr_leds[RGB_MATRIX_LED_COUNT] = {};
#endif
static void set_led_state(keyrecord_t *record, bool state) {
#ifdef TAP_REPEAT_RGB
    uint8_t row = record->event.key.row;
    uint8_t col = record->event.key.col;
    uint8_t idx = g_led_config.matrix_co[row][col];
    tr_leds[idx] = state;
#endif //TAP_REPEAT_RGB
}

void clear_tap_repeat_keys(void) {
    count = 0;
#ifdef TAP_REPEAT_RGB
    memset(tr_leds, 0, sizeof tr_leds);
#endif // TAP_REPEAT_RGB
}

void toggle_key_tap_repeat(uint16_t translated_keycode, keyrecord_t *record) {
    int8_t i = find_repeat_key_index(translated_keycode);
    if (i == -1) {
        if (add_repeat_key(translated_keycode)) {
            set_led_state(record, true);
        };
    } else {
        if (remove_repeat_key_by_index(i)) {
            set_led_state(record, false);
        }
    }
}

#ifdef TAP_REPEAT_RGB
static uint32_t led_timer;
static bool tap_indicator;
static inline RGB hsv_to_adjusted_rgb(uint8_t hue, uint8_t sat, uint8_t val){
    HSV hsv = {hue, sat, val};
    if (hsv.v > rgb_matrix_get_val()) {
        hsv.v = rgb_matrix_get_val();
    }
    return hsv_to_rgb(hsv);
}

bool rgb_matrix_indicators_tap_repeat(int tr_togg_idx) {
    if (timer_elapsed32(led_timer) > TAP_REPEAT_INTERVAL) {
        tap_indicator = !tap_indicator;
        led_timer = timer_read32();
    }
    if (watching) {
        for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
            RGB rgb = {0,0,0};
            if (i == tr_togg_idx) {
               continue;
            } else if (tr_leds[i]) {
                rgb = hsv_to_adjusted_rgb(TAP_REPEAT_HSV);
            }
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
        return false;
    } else {
        // leave when tap_indicator is true, just leave the leds alone
        if (count == 0 || tap_indicator) {
            return true;
        }
        for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
            if (tr_leds[i]) {
                rgb_matrix_set_color(i, 0, 0, 0);
            }
        }
        return true;
    }
    return true;
}
#endif //TAP_REPEAT_RGB

// This is for a manual timer
void tap_repeat_task(void) {
    if (count == 0) {
        return;
    }
    if (timer_elapsed32(tap_timer) > TAP_REPEAT_INTERVAL) {
        tap_current_keys();
        tap_timer = timer_read32();
    }
}
