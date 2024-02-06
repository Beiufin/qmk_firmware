# Massdrop CTRL - M-AS

Based on the ["matthewrobo" keymap](https://github.com/qmk/qmk_firmware/tree/master/keyboards/massdrop/ctrl/keymaps/matthewrobo).

## Layers

### Base Layer

![Base Layer](https://i.imgur.com/qMXpYUb.png)

### Function Layer

![Function Layer](https://i.imgur.com/dp7zs0I.png)

## RGB Matrix Effects :id=rgb-matrix-effects

All effects have been configured to support current configuration values (Hue, Saturation, Value, & Speed) unless otherwise noted below. These are the effects that are currently available:

```c
enum rgb_matrix_effects {
    RGB_MATRIX_NONE = 0,
    RGB_MATRIX_SOLID_COLOR = 1,     // Static single hue, no speed support
    RGB_MATRIX_ALPHAS_MODS,         // Static dual hue, speed is hue for secondary hue
    RGB_MATRIX_GRADIENT_UP_DOWN,    // Static gradient top to bottom, speed controls how much gradient changes
    RGB_MATRIX_GRADIENT_LEFT_RIGHT,    // Static gradient left to right, speed controls how much gradient changes
    RGB_MATRIX_BREATHING,           // Single hue brightness cycling animation
    RGB_MATRIX_BAND_SAT,        // Single hue band fading saturation scrolling left to right
    RGB_MATRIX_BAND_VAL,        // Single hue band fading brightness scrolling left to right
    RGB_MATRIX_BAND_PINWHEEL_SAT,   // Single hue 3 blade spinning pinwheel fades saturation
    RGB_MATRIX_BAND_PINWHEEL_VAL,   // Single hue 3 blade spinning pinwheel fades brightness
    RGB_MATRIX_BAND_SPIRAL_SAT,     // Single hue spinning spiral fades saturation
    RGB_MATRIX_BAND_SPIRAL_VAL,     // Single hue spinning spiral fades brightness
    RGB_MATRIX_CYCLE_ALL,           // Full keyboard solid hue cycling through full gradient
    RGB_MATRIX_CYCLE_LEFT_RIGHT,    // Full gradient scrolling left to right
    RGB_MATRIX_CYCLE_UP_DOWN,       // Full gradient scrolling top to bottom
    RGB_MATRIX_CYCLE_OUT_IN,        // Full gradient scrolling out to in
    RGB_MATRIX_CYCLE_OUT_IN_DUAL,   // Full dual gradients scrolling out to in
    RGB_MATRIX_RAINBOW_MOVING_CHEVRON,  // Full gradent Chevron shapped scrolling left to right
    RGB_MATRIX_CYCLE_PINWHEEL,      // Full gradient spinning pinwheel around center of keyboard
    RGB_MATRIX_CYCLE_SPIRAL,        // Full gradient spinning spiral around center of keyboard
    RGB_MATRIX_DUAL_BEACON,         // Full gradient spinning around center of keyboard
    RGB_MATRIX_RAINBOW_BEACON,      // Full tighter gradient spinning around center of keyboard
    RGB_MATRIX_RAINBOW_PINWHEELS,   // Full dual gradients spinning two halfs of keyboard
    RGB_MATRIX_RAINDROPS,           // Randomly changes a single key's hue
    RGB_MATRIX_JELLYBEAN_RAINDROPS, // Randomly changes a single key's hue and saturation
    RGB_MATRIX_HUE_BREATHING,       // Hue shifts up a slight ammount at the same time, then shifts back
    RGB_MATRIX_HUE_PENDULUM,        // Hue shifts up a slight ammount in a wave to the right, then back to the left
    RGB_MATRIX_HUE_WAVE,            // Hue shifts up a slight ammount and then back down in a wave to the right
#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS)
    RGB_MATRIX_TYPING_HEATMAP,      // How hot is your WPM!
    RGB_MATRIX_TYPING_HEATMAP_SOLID      // Custom heatmap that only increases brightness. Speed will adjust base brightness.
    RGB_MATRIX_TYPING_HEATMAP_LEDON      // Custom heatmap that allows for a base brightness and adjusting starting hue. Speed controls starting brightness, value controls max brightness.
    RGB_MATRIX_DIGITAL_RAIN,        // That famous computer simulation
#endif
#if defined(RGB_MATRIX_KEYPRESSES) || defined(RGB_MATRIX_KEYRELEASES)
    RGB_MATRIX_SOLID_REACTIVE_SIMPLE,   // Pulses keys hit to hue & value then fades value out
    RGB_MATRIX_SOLID_REACTIVE,      // Static single hue, pulses keys hit to shifted hue then fades to current hue
    RGB_MATRIX_SOLID_REACTIVE_WIDE       // Hue & value pulse near a single key hit then fades value out
    RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE  // Hue & value pulse near multiple key hits then fades value out
    RGB_MATRIX_SOLID_REACTIVE_CROSS      // Hue & value pulse the same column and row of a single key hit then fades value out
    RGB_MATRIX_SOLID_REACTIVE_MULTICROSS // Hue & value pulse the same column and row of multiple key hits then fades value out
    RGB_MATRIX_SOLID_REACTIVE_NEXUS      // Hue & value pulse away on the same column and row of a single key hit then fades value out
    RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS // Hue & value pulse away on the same column and row of multiple key hits then fades value out
    RGB_MATRIX_SPLASH,              // Full gradient & value pulse away from a single key hit then fades value out
    RGB_MATRIX_MULTISPLASH,         // Full gradient & value pulse away from multiple key hits then fades value out
    RGB_MATRIX_SOLID_SPLASH,        // Hue & value pulse away from a single key hit then fades value out
    RGB_MATRIX_SOLID_MULTISPLASH,   // Hue & value pulse away from multiple key hits then fades value out
#endif
#if defined(RGB_MATRIX_CUSTOM_USER)
    RGB_MATRIX_CUSTOM_SOLID_REACTIVE_WIDE2      // Custom verion of RGB_MATRIX_SOLID_REACTIVE_WIDE
    RGB_MATRIX_CUSTOM_SOLID_REACTIVE_MULTIWIDE2     // Custom verion of RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
    RGB_MATRIX_CUSTOM_SOLID_REACTIVE_NEXUS2     // Custom verion of RGB_MATRIX_SOLID_REACTIVE_NEXUS
    RGB_MATRIX_CUSTOM_SOLID_REACTIVE_MULTINEXUS     // Custom verion of RGB_MATRIX_REACTIVE_MULTINEXUS
    RGB_MATRIX_SOLID_REACTIVE_FADE       // Custom inverted Reactive simple that fades
#endif
    RGB_MATRIX_EFFECT_MAX

};
```

You can disable a single effect by undefining `ENABLE_[EFFECT_NAME]` in your `config.h`:

| Define                                               | Description                                     |
| ---------------------------------------------------- | ----------------------------------------------- |
| `#undef ENABLE_RGB_MATRIX_ALPHAS_MODS`               | Disables `RGB_MATRIX_ALPHAS_MODS`               |
| `#undef ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN`          | Disables `RGB_MATRIX_GRADIENT_UP_DOWN`          |
| `#undef ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT`       | Disables `MATRIX_GRADIENT_LEFT_RIGHT`           |
| `#undef ENABLE_RGB_MATRIX_BREATHING`                 | Disables `RGB_MATRIX_BREATHING`                 |
| `#undef ENABLE_RGB_MATRIX_BAND_SAT`                  | Disables `RGB_MATRIX_BAND_SAT`                  |
| `#undef ENABLE_RGB_MATRIX_BAND_VAL`                  | Disables `RGB_MATRIX_BAND_VAL`                  |
| `#undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT`         | Disables `RGB_MATRIX_BAND_PINWHEEL_SAT`         |
| `#undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL`         | Disables `RGB_MATRIX_BAND_PINWHEEL_VAL`         |
| `#undef ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT`           | Disables `RGB_MATRIX_BAND_SPIRAL_SAT`           |
| `#undef ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL`           | Disables `RGB_MATRIX_BAND_SPIRAL_VAL`           |
| `#undef ENABLE_RGB_MATRIX_CYCLE_ALL`                 | Disables `RGB_MATRIX_CYCLE_ALL`                 |
| `#undef ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT`          | Disables `RGB_MATRIX_CYCLE_LEFT_RIGHT`          |
| `#undef ENABLE_RGB_MATRIX_CYCLE_UP_DOWN`             | Disables `RGB_MATRIX_CYCLE_UP_DOWN`             |
| `#undef ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON`    | Disables `RGB_MATRIX_RAINBOW_MOVING_CHEVRON`    |
| `#undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN`              | Disables `RGB_MATRIX_CYCLE_OUT_IN`              |
| `#undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL`         | Disables `RGB_MATRIX_CYCLE_OUT_IN_DUAL`         |
| `#undef ENABLE_RGB_MATRIX_CYCLE_PINWHEEL`            | Disables `RGB_MATRIX_CYCLE_PINWHEEL`            |
| `#undef ENABLE_RGB_MATRIX_CYCLE_SPIRAL`              | Disables `RGB_MATRIX_CYCLE_SPIRAL`              |
| `#undef ENABLE_RGB_MATRIX_DUAL_BEACON`               | Disables `RGB_MATRIX_DUAL_BEACON`               |
| `#undef ENABLE_RGB_MATRIX_RAINBOW_BEACON`            | Disables `RGB_MATRIX_RAINBOW_BEACON`            |
| `#undef ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS`         | Disables `RGB_MATRIX_RAINBOW_PINWHEELS`         |
| `#undef ENABLE_RGB_MATRIX_RAINDROPS`                 | Disables `RGB_MATRIX_RAINDROPS`                 |
| `#undef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS`       | Disables `RGB_MATRIX_JELLYBEAN_RAINDROPS`       |
| `#undef ENABLE_RGB_MATRIX_HUE_BREATHING`             | Disables `RGB_MATRIX_HUE_BREATHING`             |
| `#undef ENABLE_RGB_MATRIX_HUE_PENDULUM`              | Disables `RGB_MATRIX_HUE_PENDULUM`              |
| `#undef ENABLE_RGB_MATRIX_HUE_WAVE `                 | Disables `RGB_MATRIX_HUE_WAVE `                 |
| `#undef ENABLE_RGB_MATRIX_TYPING_HEATMAP`            | Disables `RGB_MATRIX_TYPING_HEATMAP`            |
| `#undef ENABLE_RGB_MATRIX_DIGITAL_RAIN`              | Disables `RGB_MATRIX_DIGITAL_RAIN`              |
| `#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE`     | Disables `RGB_MATRIX_SOLID_REACTIVE_SIMPLE`     |
| `#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE`            | Disables `RGB_MATRIX_SOLID_REACTIVE`            |
| `#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE`       | Disables `RGB_MATRIX_SOLID_REACTIVE_WIDE`       |
| `#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE`  | Disables `RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE`  |
| `#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS`      | Disables `RGB_MATRIX_SOLID_REACTIVE_CROSS`      |
| `#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS` | Disables `RGB_MATRIX_SOLID_REACTIVE_MULTICROSS` |
| `#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS`      | Disables `RGB_MATRIX_SOLID_REACTIVE_NEXUS`      |
| `#undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS` | Disables `RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS` |
| `#undef ENABLE_RGB_MATRIX_SPLASH`                    | Disables `RGB_MATRIX_SPLASH`                    |
| `#undef ENABLE_RGB_MATRIX_MULTISPLASH`               | Disables `RGB_MATRIX_MULTISPLASH`               |
| `#undef ENABLE_RGB_MATRIX_SOLID_SPLASH`              | Disables `RGB_MATRIX_SOLID_SPLASH`              |
| `#undef ENABLE_RGB_MATRIX_SOLID_MULTISPLASH`         | Disables `RGB_MATRIX_SOLID_MULTISPLASH`         |

### RGB Matrix Effect Typing Heatmap :id=rgb-matrix-effect-typing-heatmap

This effect will color the RGB matrix according to a heatmap of recently pressed
keys. Whenever a key is pressed its "temperature" increases as well as that of
its neighboring keys. The temperature of each key is then decreased
automatically every 25 milliseconds by default.

In order to change the delay of temperature decrease define
`RGB_MATRIX_TYPING_HEATMAP_DECREASE_DELAY_MS`:

```c
#define RGB_MATRIX_TYPING_HEATMAP_DECREASE_DELAY_MS 50
```
