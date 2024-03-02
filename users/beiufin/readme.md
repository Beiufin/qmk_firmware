Copyright 2024 Beiufin beiufin@gmail.com @Beiufin

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Usage

When creating keymaps in the beiufin userspace, there are some unique behaviors.

## LED map

To have leds be set per layer based on an led map, initialize an ledmap in the keymap as follows `const uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3]`. The first dimension is the layer, is the led index and the third is the HSV for the led.

Index 0 of this is never used as it is the base layer.  
**TODO**: adjust this so there isn't an unused RGB_MATRIX_LED_COUNT*3 bytes being taken up by index 0.

## User functions

When using the beiufin userspace, the `*_user` functions in the keymaps should be replaced with `*_keymap` function.

## Custom keycodes

The beiufin userpace adds some custom keycodes starting at the `SAFE_RANGE`. Custom keycodes in the keymaps should start with `NEW_SAFE_RANGE` instead.

## Custom Layers

The beiufin userpace adds some specific layers. If the keymap adds aditional layers, they should start from `SAFE_LAYER_RANGE`.

## Keycode definitions

The beiufin userpace provides custom lighting for some keycodes. To do this, it needs to know the LED location for the associated key.

Add a `beiufin_led_codes.h` file to your keymap folder. In this file, use the `BEIUFIN_LED_*(LED_IDX)` preprocessor macros to define the led index for each of the appropriate leds.

The available macros are:
* `BEIUFIN_LED_RGB_HUI`: For keycode `RGB_HUI`
* `BEIUFIN_LED_RGB_HUD`: For keycode `RGB_HUD`
* `BEIUFIN_LED_RGB_SAI`: For keycode `RGB_SAI`
* `BEIUFIN_LED_RGB_SAD`: For keycode `RGB_SAD`
* `BEIUFIN_LED_RGB_VAI`: For keycode `RGB_VAI`
* `BEIUFIN_LED_RGB_VAD`: For keycode `RGB_VAD`
* `BEIUFIN_LED_RGB_SPI`: For keycode `RGB_SPI`
* `BEIUFIN_LED_RGB_SPD`: For keycode `RGB_SPD`

An example `beiufin_led_codes.h` where there are multiple keys are assigned to some of these keycodes. Note the lack of semi colons.
```
BEIUFIN_LED_RGB_HUI(36)
BEIUFIN_LED_RGB_HUD(53)
BEIUFIN_LED_RGB_SAI(54)
BEIUFIN_LED_RGB_SAD(37)
BEIUFIN_LED_RGB_VAI(75)
BEIUFIN_LED_RGB_VAI(38)
BEIUFIN_LED_RGB_VAD(85)
BEIUFIN_LED_RGB_VAD(55)
BEIUFIN_LED_RGB_SPI(86)
BEIUFIN_LED_RGB_SPI(35)
BEIUFIN_LED_RGB_SPD(84)
BEIUFIN_LED_RGB_SPD(52)
```
