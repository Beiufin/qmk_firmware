#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP_LEDON_APM) && defined(APM_ENABLE)
RGB_MATRIX_EFFECT(TYPING_HEATMAP_LEDON_APM)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#include "apm.h"

static uint8_t relative_apm;

static inline HSV anim_apm_extra(uint8_t apm_val, effect_params_t* params) {
    HSV hsv = {rgb_matrix_config.hsv.h - qsub8(apm_val, 85), rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v};
    return hsv;
}

HSV anim_heatmap_ledon_apm(uint8_t *buffer_val, bool decrease_heatmap_values, effect_params_t* params) {
    uint8_t val = *buffer_val;

    // Orig: {170 to 0, s, 0 + val*3}
    // HSV hsv = {170 - qsub8(val, 85), rgb_matrix_config.hsv.s, scale8((qadd8(170, val) - 170) * 3, rgb_matrix_config.hsv.v)};
    // {h to h-170, s, v}
    HSV hsv = {rgb_matrix_config.hsv.h - qsub8(val, 85), rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v};

    if (decrease_heatmap_values) {
        *buffer_val = qsub8(val, 1);
    }
    return hsv;
}

bool TYPING_HEATMAP_LEDON_APM(effect_params_t* params) {
    if (params->init) {
        HSV hsv = {rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v};
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color_all(rgb.r, rgb.g, rgb.b);
    }
    if (params->iter == 0){
        relative_apm = get_relative_apm();
    }

    effect_runner_heatmap(params, anim_heatmap_ledon_apm, false);

#if RGB_MATRIX_EXTRA_LED_COUNT > 0
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    // This assumes extra leds always go from RGB_MATRIX_EXTRA_LED_START -> RGB_MATRIX_EXTRA_LED_COUNT
    if (led_max > RGB_MATRIX_EXTRA_LED_START) {
        uint8_t relative_apm = get_relative_apm();
        // starting led, also used to keep a current led_idx
        uint8_t led_i = led_min < RGB_MATRIX_EXTRA_LED_START ? RGB_MATRIX_EXTRA_LED_START : led_min;
        for (uint8_t buf_i = led_i - RGB_MATRIX_EXTRA_LED_START; led_i < led_max; buf_i++, led_i++) {
            if (!HAS_ANY_FLAGS(led_i, params->flags)) continue;
            RGB rgb = rgb_matrix_hsv_to_rgb(anim_apm(relative_apm, params));
            rgb_matrix_set_color(led_i, rgb.r, rgb.g, rgb.b);
        }
    }
#endif // RGB_MATRIX_EXTRA_LED_COUNT > 0
    return rgb_matrix_check_finished_leds(led_max);
}

#    endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif      // defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP)
