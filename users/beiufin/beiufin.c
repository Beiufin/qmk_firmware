#include "beiufin.h"
#include <lib/lib8tion/lib8tion.h>

extern uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3];

bool disable_layer_color;

#if defined(TAP_REPEAT_RGB) && defined(TAP_REPEAT_ENABLE)
#    include "tap_repeat.h"
#endif

#if defined(KEY_LOCK_TOGGLE_RGB) && defined(KEY_LOCK_TOGGLE_ENABLE)
#    include "key_lock_toggle.h"
#endif

#define MODS_SHIFT (get_mods() & MOD_MASK_SHIFT)
#define MODS_CTRL (get_mods() & MOD_MASK_CTRL)
#define MODS_ALT (get_mods() & MOD_MASK_ALT)

#ifdef DUAL_RGB_MATRIX_ENABLE
#    define DUAL_AWARE_RGB_FUNC(name, ...)            \
        if (!MODS_ALT) {                              \
            rgb_matrix_##name(__VA_ARGS__);           \
        } else {                                      \
            rgb_secondary_matrix_##name(__VA_ARGS__); \
        }
#else
#    define DUAL_AWARE_RGB_FUNC(name, ...) rgb_matrix_##name(__VA_ARGS__);
#endif

__attribute__((weak)) bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_keymap(keycode, record)) {
        return false;
    }
    // The only custom processing is for non-standard keys. So avoid any extra processing
    // for codes in the standard/basic range.
    if (record->event.pressed && keycode > QK_BASIC_MAX) {
        switch (keycode) {
            // ======================================================== CUSTOM KEYCOADS BELOW ========================================================
            case BEI_CRM:
                DUAL_AWARE_RGB_FUNC(sethsv, HSV_CREAM)
                return false;
            case BEI_PRP:
                DUAL_AWARE_RGB_FUNC(sethsv, HSV_9B59B5)
                return false;
            case BEI_RED:
                DUAL_AWARE_RGB_FUNC(sethsv, HSV_RED)
                return false;
            case BEI_GRN:
                DUAL_AWARE_RGB_FUNC(sethsv, HSV_GREEN)
                return false;
            case BEI_BLU:
                DUAL_AWARE_RGB_FUNC(sethsv, HSV_BLUE)
                return false;
            case BEI_CYN:
                DUAL_AWARE_RGB_FUNC(sethsv, HSV_CYAN)
                return false;
            case BEI_MGT:
                DUAL_AWARE_RGB_FUNC(sethsv, HSV_MAGENTA)
                return false;
            case BEI_YEL:
                DUAL_AWARE_RGB_FUNC(sethsv, HSV_YELLOW)
                return false;
            case BEI_KEY:
                DUAL_AWARE_RGB_FUNC(sethsv, 0, 0, 0)
                return false;
            case BEI_WHT:
                DUAL_AWARE_RGB_FUNC(sethsv, 128, 0, 255)
                return false;
            default:
                return true; // Process all other keycodes normally
        }
    }
    return true;
}
#undef DUAL_AWARE_RGB_FUNC

#define RGB_KEYCODE_DYANMIC_COLOR(RGB_VAR, REVERESED_RGB_VAR)                                   \
    if (!MODS_SHIFT) {                                                                          \
        rgb_matrix_set_color(i, RGB_VAR.r, RGB_VAR.g, RGB_VAR.b);                               \
    } else {                                                                                    \
        rgb_matrix_set_color(i, REVERESED_RGB_VAR.r, REVERESED_RGB_VAR.g, REVERESED_RGB_VAR.b); \
    }

bool set_layer_color(int layer) {
    if (layer == _KL) {
        return true;
    }
    rgb_config_t *config = &rgb_matrix_config;
#ifdef DUAL_RGB_MATRIX_ENABLE
    if (MODS_ALT) {
        config = &rgb_secondary_matrix_config;
    }
#endif
    HSV hsv2                           = {config->hsv.h, config->hsv.s, RGB_MATRIX_MAXIMUM_BRIGHTNESS};
    HSV hui                            = hsv2;
    HSV hud                            = hsv2;
    HSV sai                            = hsv2;
    HSV sad                            = hsv2;
    HSV vai                            = hsv2;
    HSV vad                            = hsv2;
    HSV spd                            = hsv2;
    HSV spi                            = hsv2;
    hui.h                              = hsv2.h + RGB_MATRIX_HUE_STEP;
    hud.h                              = hsv2.h - RGB_MATRIX_HUE_STEP;
    sai.s                              = qadd8(hsv2.s, RGB_MATRIX_SAT_STEP);
    sad.s                              = qsub8(hsv2.s, RGB_MATRIX_SAT_STEP);
    vai.v                              = hsv2.v + RGB_MATRIX_VAL_STEP > RGB_MATRIX_MAXIMUM_BRIGHTNESS ? RGB_MATRIX_MAXIMUM_BRIGHTNESS : config->hsv.v + RGB_MATRIX_VAL_STEP;
    vad.v                              = qsub8(hsv2.v, RGB_MATRIX_VAL_STEP);
    spd.h                              = scale8(255 - qsub8(config->speed, RGB_MATRIX_SPD_STEP), 92);
    spi.h                              = scale8(255 - qadd8(config->speed, RGB_MATRIX_SPD_STEP), 92);
    RGB __attribute__((unused)) rgbHUI = hsv_to_rgb(hui);
    RGB __attribute__((unused)) rgbHUD = hsv_to_rgb(hud);
    RGB __attribute__((unused)) rgbSAI = hsv_to_rgb(sai);
    RGB __attribute__((unused)) rgbSAD = hsv_to_rgb(sad);
    RGB __attribute__((unused)) rgbVAI = hsv_to_rgb(vai);
    RGB __attribute__((unused)) rgbVAD = hsv_to_rgb(vad);
    RGB __attribute__((unused)) rgbSPD = hsv_to_rgb(spd);
    RGB __attribute__((unused)) rgbSPI = hsv_to_rgb(spi);

    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        HSV hsv = {
            .h = pgm_read_byte(&ledmap[layer][i][0]),
            .s = pgm_read_byte(&ledmap[layer][i][1]),
            .v = pgm_read_byte(&ledmap[layer][i][2]),
        };
        if (hsv.h || hsv.s || hsv.v) {
            RGB   rgb = hsv_to_rgb(hsv);
            float f   = (float)config->hsv.v / UINT8_MAX;
            rgb_matrix_set_color(i, f * rgb.r, f * rgb.g, f * rgb.b);
        } else if (layer == _FL) {
            switch (i) {
#define BEIUFIN_LED_RGB_HUI(LED_IDX)              \
    case LED_IDX:                                 \
        RGB_KEYCODE_DYANMIC_COLOR(rgbHUI, rgbHUD) \
        break;
#define BEIUFIN_LED_RGB_HUD(LED_IDX)              \
    case LED_IDX:                                 \
        RGB_KEYCODE_DYANMIC_COLOR(rgbHUD, rgbHUI) \
        break;
#define BEIUFIN_LED_RGB_SAI(LED_IDX)              \
    case LED_IDX:                                 \
        RGB_KEYCODE_DYANMIC_COLOR(rgbSAI, rgbSAD) \
        break;
#define BEIUFIN_LED_RGB_SAD(LED_IDX)              \
    case LED_IDX:                                 \
        RGB_KEYCODE_DYANMIC_COLOR(rgbSAD, rgbSAI) \
        break;
#define BEIUFIN_LED_RGB_VAI(LED_IDX)              \
    case LED_IDX:                                 \
        RGB_KEYCODE_DYANMIC_COLOR(rgbVAI, rgbVAD) \
        break;
#define BEIUFIN_LED_RGB_VAD(LED_IDX)              \
    case LED_IDX:                                 \
        RGB_KEYCODE_DYANMIC_COLOR(rgbVAD, rgbVAI) \
        break;
#define BEIUFIN_LED_RGB_SPI(LED_IDX)              \
    case LED_IDX:                                 \
        RGB_KEYCODE_DYANMIC_COLOR(rgbSPI, rgbSPD) \
        break;
#define BEIUFIN_LED_RGB_SPD(LED_IDX)              \
    case LED_IDX:                                 \
        RGB_KEYCODE_DYANMIC_COLOR(rgbSPD, rgbSPI) \
        break;
#include "beiufin_led_codes.h"
#undef BEIUFIN_LED_RGB_HUI
#undef BEIUFIN_LED_RGB_HUD
#undef BEIUFIN_LED_RGB_SAI
#undef BEIUFIN_LED_RGB_SAD
#undef BEIUFIN_LED_RGB_VAI
#undef BEIUFIN_LED_RGB_VAD
#undef BEIUFIN_LED_RGB_SPI
#undef BEIUFIN_LED_RGB_SPD
                default:
                    rgb_matrix_set_color(i, 0, 0, 0);
                    break;
            }
        }
    }
    return false;
}
#undef RGB_KEYCODE_DYANMIC_COLOR

__attribute__((weak)) layer_state_t layer_state_set_keymap(layer_state_t state) {
    return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    if (get_highest_layer(state) == _KL) rgb_matrix_set_color_all(0, 0, 0);
    return layer_state_set_keymap(state);
}

__attribute__((weak)) bool rgb_matrix_indicators_keymap(void) {
    return true;
}

bool rgb_matrix_indicators_user(void) {
    if (disable_layer_color || !rgb_matrix_is_enabled() || rgb_matrix_get_flags() == LED_FLAG_NONE || rgb_matrix_get_flags() == LED_FLAG_UNDERGLOW) {
        return false;
    }
    if (!(
#if defined(KEY_LOCK_TOGGLE_RGB) && defined(KEY_LOCK_TOGGLE_ENABLE)
            rgb_matrix_indicators_key_lock_toggle(14) &&
#endif
#if defined(TAP_REPEAT_RGB) && defined(TAP_REPEAT_ENABLE)
            rgb_matrix_indicators_tap_repeat(50) &&
#endif
            set_layer_color(get_highest_layer(layer_state)))) {
        return false;
    }
    return rgb_matrix_indicators_keymap();
}

__attribute__((weak)) bool rgb_matrix_indicators_advanced_keymap(uint8_t led_min, uint8_t led_max) {
    return true;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    return rgb_matrix_indicators_advanced_keymap(led_min, led_max);
}
