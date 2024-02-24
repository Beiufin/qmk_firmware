#if defined(ENABLE_RGB_MATRIX_APM) && defined(APM_ENABLE)
RGB_MATRIX_EFFECT(APM)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#        include "apm.h"

static uint8_t relative_apm;

static inline HSV anim_apm(uint8_t apm_val, effect_params_t* params, rgb_config_t* config) {
    HSV hsv = {config->hsv.h - qsub8(apm_val, 85), config->hsv.s, config->hsv.v};
    return hsv;
}

// changes led based on APM
bool APM(effect_params_t* params, rgb_config_t* config) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    if (params->iter == 0) {
        relative_apm = get_relative_apm();
    }

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        RGB rgb = rgb_matrix_hsv_to_rgb(anim_apm(relative_apm, params, config));
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // ENABLE_RGB_MATRIX_ALPHAS_MODS
