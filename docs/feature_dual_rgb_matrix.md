# Dual RGB Matrix :id=dual-rgb-matrix

Dual RGB Matrix adds a secondary rgb matrix config. It allows for the a different rgb settings/effects for a set of leds (based on [flags](feature_rgb_matrix#flags-idflags)). For example,
there can one effect run on keys with `LED_FLAG_KEYLIGHT` and another on `LED_FLAG_UNDERGLOW | LED_FLAG_MODIFIER`.

## Usage :id=usage

Add the following to your `rules.mk`:

```make
DUAL_RGB_ENABLE = yes
```

## Additional `config.h` Options :id=additional-configh-options

A set of the [Additional RGB Matrix Options](feature_rgb_matrix#additional-configh-options-idadditional-configh-options) are also available for the secondary matrix.

```c
// Sets the default mode, if none has been set
#define RGB_SECONDARY_MATRIX_DEFAULT_MODE RGB_MATRIX_CYCLE_LEFT_RIGHT
// Sets the default hue value, if none has been set
#define RGB_SECONDARY_MATRIX_DEFAULT_HUE 0
// Sets the default saturation value, if none has been set
#define RGB_SECONDARY_MATRIX_DEFAULT_SAT 255
// Sets the default enabled state, if none has been set
#define RGB_SECONDARY_MATRIX_DEFAULT_ON true
// Sets the default brightness value, if none has been set
#define RGB_SECONDARY_MATRIX_DEFAULT_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS
// Sets the default animation speed, if none has been set
#define RGB_SECONDARY_MATRIX_DEFAULT_SPD 127
```

## Functions :id=functions

Similarly, all functions from [RGB Matrix Functions](feature_rgb_matrix#functions-idfunctions) (apart from the Direct Operations) have a `rgb_secondary_matrix_*` equivalent.

For example, to set the color of the secondary rgb matrix, call `rgb_secondary_matrix_sethsv(h, s, v);`.

Enabling this feature also affects the behavior of a few existing functions to help avoid breaking exisiting code. These affects are detailed in the docs of [RGB Matrix Functions](feature_rgb_matrix#functions-idfunctions).

## Flags :id=flags

The flags for the dual configs are mutually exclusive. When a flag is set in either it is unset in the other.

For example, if the primary config has the flags set to `LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_UNDERGLOW` and the secondary flags are set to `LED_FLAG_MODIFIER` (e.g. `rgb_secondary_matrix_set_flags(LED_FLAG_MODIFIER);` is called), then the `LED_FLAG_MODIFIER` will be unset from the primary config. Resulting in the primary flags now being `LED_FLAG_KEYLIGHT | LED_FLAG_UNDERGLOW`

## Keycodes :id=keycodes

Holding Alt while pressing any existing [RGB Matrix Keycodes](feature_rgb_matrix#keycodes-idkeycodes) will make them affect the secondary rgb matrix config.

The `RGB_CYCLE_*` keycodes can be used to cycle an LED flag between the configs.
