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

#include "process_tap_repeat.h"
#include "tap_repeat.h"
// This is based off of process_key_lock

#define IS_STANDARD_KEYCODE(code) ((code) <= QK_BASIC_MAX)

// Translate any OSM keycodes back to their unmasked versions.
static inline uint16_t translate_keycode(uint16_t keycode) {
    if (keycode > QK_ONE_SHOT_MOD && keycode <= QK_ONE_SHOT_MOD_MAX) {
        return keycode ^ QK_ONE_SHOT_MOD;
    } else {
        return keycode;
    }
}

static bool process_translated_keycode(uint16_t translated_keycode, keyrecord_t *record) {
    // We start by categorizing the keypress event. In the event of a down
    // event, there are several possibilities (processed in order):
    // 1. The key is BN_TRTG. In this case, toggle the watching state.
    // 2. We are not watching and the key is and not BN_TRTG. In this case,
    //    ignore it (processed normally).
    // 2. The key is NOT a standard key (keycode > 0xFF) and not BN_TRTG. In
    //    this case, ignore it (processed normally).
    // 3. We are watching and the key is a standard key (keycode <= 0xFF) that is
    //    not currently tapping. In this case, add it to the tapping keys.
    // 4. We are watching and the key is a standard key (keycode <= 0xFF) that is
    //    currently tapping. In this case, remove it from the tapping keys
    //
    // This does nothing for release events.
    if (record->event.pressed) {
        // Toggle watching on BN_TRTG press.
        if (translated_keycode == BN_TRTG) {
            if (!is_tap_repeat_watching()) {
                clear_tap_repeat_keys();
            };
            toggle_tap_repeat_watching();
            return false;
        } else if (!is_tap_repeat_watching()) {
            // Not watching and the key is and not BN_TRTG. In this case,
            // it gets processed normally.
            return true;
        } else if (!(IS_STANDARD_KEYCODE(translated_keycode))) {
            // Non-standard keycode (ant not BN_TRTG) while watching, it gets processed normally.
            return true;
        } else {
            // Watching and it is a standard keycode, toggle its tapping state.
            toggle_key_tap_repeat(translated_keycode, record);

            // Suppress the normal events
            return false;
        }
        // Either the key isn't a standard key, or we need to send the down event. Continue standard
        // processing
        return true;
    } else {
        // Nothing needed for key up
        return true;
    }
    return true;
}

/**
 * Tap repeat allows for setting keys to auto repeat.
 * To set a key, press TR_LOCK (fn+CapsLock) then press the key you want to have repeat.
 * To disable that repeating key, repeat the process.
 */
bool process_tap_repeat(uint16_t *keycode, keyrecord_t *record) {
    // We translate any OSM keycodes back to their original keycodes, so that if the key being
    // one-shot modded is a standard keycode, we can handle it. This is the only set of special
    // keys that we handle
    uint16_t translated_keycode = translate_keycode(*keycode);
    if (process_translated_keycode(translated_keycode, record)) {
        // We need to set the keycode passed in to be the translated keycode, in case we
        // translated a OSM back to the original keycode.
        *keycode = translated_keycode;
        return true;
    }
    return false;
}
