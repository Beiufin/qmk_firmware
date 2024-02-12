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

#include <inttypes.h>
#include <stdint.h>
#include "process_key_lock_toggle.h"
#include "key_lock_toggle.h"

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
    // 1. The key is BN_KLTG. In this case, turn on the watching state
    // 2. We are not watching and the key is not locked and not BN_KLTG. In this case,
    //    ignore it (processed normally).
    // 3. The key is locked and not BN_KLTG. In this case, unlock it.
    // 4. The key is NOT a standard key (keycode > 0xFF) and not BN_KLTG. In
    //    this case, ignore it (processed normally).
    // 5. We are watching and the key is a standard key (keycode <= 0xFF) that is
    //    not locked. In this case, lock the key.
    // 6. We are watching and the key is a standard key (keycode <= 0xFF) that is
    //    locked. In this case, unlock the key
    //
    // In the event of an up event, there are these possibilities:
    // 1. The key is BN_KLTG. In this case, turn off the watching state
    // 2. The key is not being locked. In this case, we return true and bail
    //    immediately. This is the common case.
    // 3. The key is being locked. In this case, we will mask the up event
    //    by returning false, so the OS never sees that the key was released
    //    until the user pressed the key again.

    if (record->event.pressed) {
        bool watching = is_klt_watching();

        // If we're already watching, turn off the watch.
        if (translated_keycode == KL_TOGG) {
            set_klt_watching(true);
            // return true as long as it is not BN_KLTG so that a custom KL_TOGG can still be handled normally
            return translated_keycode != BN_KLTG;
        } else if (!(IS_STANDARD_KEYCODE(translated_keycode))) {
            // Non-standard keycode and not KL_TOGG, ignore it
            return true;
        } else {
            // standard keycode
            if (is_key_locked(translated_keycode)) {
                // watching or not, we want to unlock the key
                unlock_key(translated_keycode, record);
                // The key is already held, stop this process. The up event will be sent when the user
                // releases the key.
                return false;
            } else {
                if (watching) {
                    // only lock if watching
                    lock_key(translated_keycode, record);
                }
                // As long as it is not a locked key, this should return true so the down event is handled normally.
                return true;
            }
        }
        return true;
    } else {
        if (translated_keycode == KL_TOGG) {
            set_klt_watching(false);
            // return true as long as it is not BN_KLTG so that a custom KL_TOGG can still be handled normally
            return translated_keycode != BN_KLTG;
        }
        // Stop processing if it's a standard key and we're masking up.
        return !(IS_STANDARD_KEYCODE(translated_keycode) && is_key_locked(translated_keycode));
    }
}

// Any standard key press while KL_TOGG is down will lock the key as on.
// Pressing the key again will unlock it.
bool process_key_lock_toggle(uint16_t *keycode, keyrecord_t *record) {
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
