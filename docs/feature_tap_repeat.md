# Tap Repeat Toggle

Sometimes you may find yourself needing to key tapping a certain key for a long period of time. Tap Repeat allows you to set a set of keys to be tapped repeatedly for as long as you need.

Let's say you need to tap F1 rapidly. Tap Repeat makes this no issue. Press `TR_TOGG`, and then tap F1, then press `TR_TOGG` again. Now, F1 will be tapped repeatedly.

The easiest way to think of this is: Pressing `TR_TOGG` toggles a Tap Repeat Configuration state. When `TR_TOGG` is pressed.  
For example, starting not in Configuration state:

1. Press `TR_TOGG` - Clears all currently tapping keys and enters Configuration state.
1. Press 'A', 'D', and '9' - Any key pressed will be registered to start tapping once the keyboard leaves the Tap Repeat Configuration state. So this has registered 'A', 'D', and '9' to be tapped.
1. Press `TR_TOGG` - Exits Configuration state and and registered keys (in this case: 'A', 'D', and '9') are repeatedly tapped.
1. Press `TR_TOGG` - Clears all currently tapping keys (in this case: 'A', 'D', and '9') and enters Configuration state.
1. Press `TR_TOGG` - Exits Configuration state. No keys are registered anymore, so no keys are tapping.

## Usage

First, enable Tap Repeat by setting `TAP_REPEAT_ENABLE = yes` in your `rules.mk`. Then pick a key in your keymap and assign it the keycode `TR_TOGG`.

### Customization

The Tap Repeat feature has some extra optional customization.

#### Repeat Interval

Tap repeat defaults to sending the keys every 200ms. This can be customized by defining `TAP_REPEAT_INTERVAL` to a different value in your config.h.

For example, to set it to repeat every 500ms, simple add
`#define TAP_REPEAT_INTERVAL 500`

#### Max Tapping Keys

To limit memory and cpu usage, Tap Repeat defaults to only allowing 16 keys to be registered for tapping at a time. To adjust this, simply define `TAP_REPEAT_MAX` with the desired number in your config.h.

For example, to allow 25 keys to be tapping at the same time, simple add
`#define TAP_REPEAT_MAX 25`

#### RGB

Tap Repeat also has a feature to utilize the key leds to indicate which keys are being held.  
To enable this feature:

1. Add `#define TAP_REPEAT_RGB` to your config.h
1. Add `#include "tap_repeat.h"`
    1. It is recommended to wrap this in `#if defined(TAP_REPEAT_RGB) && defined(TAP_REPEAT_ENABLE)` as it will only be needed in that case
1. Then add `rgb_matrix_indicators_tap_repeat(<TR_TOGG's led_idx>)` to your `rgb_matrix_indicators_*`. Provide the led index of your TR_TOGG key as the parameter to `rgb_matrix_indicators_tap_repeat`.
    1. `rgb_matrix_indicators_tap_repeat` returns a bool following the same pattern of `rgb_matrix_indicators_*` functions.
    1. It is recommended to wrap this in `#if defined(TAP_REPEAT_RGB) && defined(TAP_REPEAT_ENABLE)` as it will only be needed in that case.

An example `rgb_matrix_indicators_user` utilizing this

```
bool rgb_matrix_indicators_user(void) {
    if (!(
#if defined(TAP_REPEAT_RGB) && defined(TAP_REPEAT_ENABLE)
        rgb_matrix_indicators_tap_repeat(50) &&
#endif
        set_layer_color(get_highest_layer(layer_state)))
    ) {
        return false;
    }
    return true;
}
```

Additionally, the HSV color used for the TR_TOGG key's led can be customized by defining `TAP_REPEAT_HSV` with the HSV color in your config.h.

For example, to have the TR_TOGG led be green when in Tap Repeat Configuration mode, add `#define TAP_REPEAT_HSV 85, 255, 255` or `#define TAP_REPEAT_HSV HSV_GREEN`

## Keycodes

| Keycode   | Description                                                                                                                                      |
| --------- | ------------------------------------------------------------------------------------------------------------------------------------------------ |
| `TR_TOGG` | Press this to toggle the Tap Repeat Configuration mode. Then press any keys you wish to repeat. Press this again to exit the Configuration mode. |

## Caveats

Tap Repeat is only able to tap standard action keys and [One Shot modifier](one_shot_keys.md) keys (for example, if you have your Shift defined as `OSM(MOD_LSFT)`).
This does not include any of the QMK special functions (except One Shot modifiers), or shifted versions of keys such as `KC_LPRN`. If it's in the [Basic Keycodes](keycodes_basic.md) list, it can be held.
