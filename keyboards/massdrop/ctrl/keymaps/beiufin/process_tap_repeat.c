#ifdef ENABLE_TAP_REPEAT
/* Copyright 2017 Fredric Silberberg
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

#define IS_STANDARD_KEYCODE(code) ((code) <= 0xFF)

uint32_t tap_timer;

bool     watching     = false;
uint16_t repeating_keys[TAP_REPEAT_MAX] = {};
uint8_t count = 0;

// Translate any OSM keycodes back to their unmasked versions.
static inline uint16_t translate_keycode(uint16_t keycode) {
    if (keycode > QK_ONE_SHOT_MOD && keycode <= QK_ONE_SHOT_MOD_MAX) {
        return keycode ^ QK_ONE_SHOT_MOD;
    } else {
        return keycode;
    }
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

void remove_repeat_key_by_index(int8_t index) {
    if (index < 0 || index >= count) {
        return;
    }
    repeating_keys[index] = repeating_keys[--count];
}

/**
 * Tap repeat allows for setting keys to auto repeat.
 * To set a key, press TR_LOCK (fn+CapsLock) then press the key you want to have repeat.
 * To disable that repeating key, repeat the process.
 */
bool process_tap_repeat(uint16_t *keycode, keyrecord_t *record, bool *trToggled) {
    // We start by categorizing the keypress event. In the event of a down
    // event, there are several possibilities:
    // 1. The key is not being locked, and we are not watching for new keys.
    //    In this case, we bail immediately. This is the common case for down events.
    // 2. The key was locked, and we need to unlock it. In this case, we will
    //    reset the state in our map and return false. When the user releases the
    //    key, the up event will no longer be masked and the OS will observe the
    //    released key.
    // 3. TR_LOCK was just pressed. In this case, we set up the state machine
    //    to watch for the next key down event, and finish processing
    // 4. The keycode is below 0xFF, and we are watching for new keys. In this case,
    //    we will send the key down event to the os, and set the key_state for that
    //    key to mask the up event.
    // 5. The keycode is above 0xFF, and we're wathing for new keys. In this case,
    //    the user pressed a key that we cannot "lock", as it's a series of keys,
    //    or a macro invocation, or a layer transition, or a custom-defined key, or
    //    or some other arbitrary code. In this case, we bail immediately, reset
    //    our watch state, and return true.
    //
    // In the event of an up event, there are these possibilities:
    // 1. The key is not being locked. In this case, we return true and bail
    //    immediately. This is the common case.
    // 2. The key is being locked. In this case, we will mask the up event
    //    by returning false, so the OS never sees that the key was released
    //    until the user pressed the key again.

    // We translate any OSM keycodes back to their original keycodes, so that if the key being
    // one-shot modded is a standard keycode, we can handle it. This is the only set of special
    // keys that we handle
    uint16_t translated_keycode = translate_keycode(*keycode);
    int8_t i = find_repeat_key_index(translated_keycode);

    if (record->event.pressed) {
        // Non-standard keycode, reset and return
        if (!(IS_STANDARD_KEYCODE(translated_keycode) || translated_keycode == TR_LOCK)) {
            watching = false;
            return true;
        }

        // If we're already watching, turn off the watch.
        if (translated_keycode == TR_LOCK) {
            watching = !watching;
            return false;
        }

        if (IS_STANDARD_KEYCODE(translated_keycode)) {
            // We check watching first. This is so that in the following scenario, we continue to
            // hold the key: TR_LOCK, KC_F, TR_LOCK, KC_F
            // If we checked in reverse order, we'd end up holding the key pressed after the second
            // KC_F press is registered, when the user likely meant to hold F
            if (watching) {
                watching = false;
                if (i == -1) {
                  *trToggled = add_repeat_key(translated_keycode);
                }

                // We need to set the keycode passed in to be the translated keycode, in case we
                // translated a OSM back to the original keycode.
                *keycode = translated_keycode;

                // Suppress the normal events
                return false;
            }

            if (i != -1) {
                remove_repeat_key_by_index(i);
                *trToggled = true;
                // The key is already held, stop this process. Return true so this is treated like a normal keypress as key up will also be sent.
                return true;
            }
        }

        // Either the key isn't a standard key, or we need to send the down event. Continue standard
        // processing
        return true;
    } else {
        // Stop processing if it's a standard key and we're masking up.
        return !(IS_STANDARD_KEYCODE(translated_keycode) && i != -1);
    }
}


void process_tap_repeat_step(void) {
    if (count == 0) {
        return;
    }
    if (timer_elapsed32(tap_timer) > TAP_REPEAT_INTERVAL) {
        uint8_t i = 0;
        while (i < count) tap_code(repeating_keys[i++]);
        tap_timer = timer_read32();
    }
}
#endif
