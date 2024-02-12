# Key Lock Toggle

**NOTE**: This is an improved version of the Key Lock feature

Sometimes you may find yourself needing to hold down a specific key for a long period of time. Key Lock Toggle holds can hold down any number of keys for you. Press it again, and it will be released.

Let's say you are playing a game that needs an auto run but doesn't have one. Key Lock Toggle makes this no issue. Hold `KL_TOGG`, and then tap Shift and W, then release `KL_TOGG`. Now, Shift+W will be considered held until you tap either of them again.

## Usage

First, enable Key Lock Toggle by setting `KEY_LOCK_TOGGLE_ENABLE = yes` in your `rules.mk`. Then pick a key in your keymap and assign it the keycode `KL_TOGG`.

### Customization

The Key Lock Toggle feature has some extra optional customization.

#### Toggle key

By default, an custom keycode is used for `KL_TOGG` that has no side effects.
If you want to utilize an exising key to that you can keep the normal key functionality while also having it function as the key lock toggle key, then simple define the `KL_TOGG` as the key you want to use.

For example, to use ScrLk as they Key Lock Toggle key:  
Add `#define KL_TOGG KC_SCRL` to your config.h and use the keycode KL_TOGG for the key you want to work as both `KC_SCRL` and the key lock toggle key.

#### RGB

Key Lock Toggle also has a feature to utilize the key leds to indicate which keys are being held.  
To enable this feature:

1. Add `#define KEY_LOCK_TOGGLE_RGB` to your config.h
1. Add `#include "key_lock_toggle.h"`
    1. It is recommended to wrap this in `#if defined(KEY_LOCK_TOGGLE_RGB) && defined(KEY_LOCK_TOGGLE_ENABLE)` as it will only be needed in that case
1. Then add `rgb_matrix_indicators_key_lock_toggle(<KL_TOGG's led_idx>)` to your `rgb_matrix_indicators_*`. Provide the led index of your KL_TOGG key as the parameter to `rgb_matrix_indicators_key_lock_toggle`.
    1. `rgb_matrix_indicators_key_lock_toggle` returns a bool following the same pattern of `rgb_matrix_indicators_*` functions.
    1. It is recommended to wrap this in `#if defined(KEY_LOCK_TOGGLE_RGB) && defined(KEY_LOCK_TOGGLE_ENABLE)` as it will only be needed in that case.

An example `rgb_matrix_indicators_user` utilizing this

```
bool rgb_matrix_indicators_user(void) {
    if (!(
#if defined(KEY_LOCK_TOGGLE_RGB) && defined(KEY_LOCK_TOGGLE_ENABLE)
        rgb_matrix_indicators_key_lock_toggle(14) &&
#endif
        set_layer_color(get_highest_layer(layer_state)))
    ) {
        return false;
    }
    return true;
}
```

Additionally, the led color of the keys held by Key Lock Toggle can be customized by defining `KEY_LOCK_TOGGLE_HSV` with an HSV color in your config.h. By default it uses `HSV_GREEN` (which is 85, 255, 255)

For example, to have the held keys be red, add `#define KEY_LOCK_TOGGLE_HSV 0, 255, 255` or `#define KEY_LOCK_TOGGLE_HSV HSV_RED`

## Keycodes

| Keycode   | Description                                                                          |
| --------- | ------------------------------------------------------------------------------------ |
| `KL_TOGG` | Any key pressed while this is held will remain pressed, until they are pressed again |

## Caveats

Key Lock Toggle is only able to hold standard action keys and [One Shot modifier](one_shot_keys.md) keys (for example, if you have your Shift defined as `OSM(MOD_LSFT)`).
This does not include any of the QMK special functions (except One Shot modifiers), or shifted versions of keys such as `KC_LPRN`. If it's in the [Basic Keycodes](keycodes_basic.md) list, it can be held.
