#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP_LEDON_APM) && defined(APM_ENABLE)
RGB_MATRIX_EFFECT(TYPING_HEATMAP_LEDON_APM)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#include "apm.h"

static uint8_t relative_apm;

static inline HSV anim_apm_underglow(uint8_t apm_val, effect_params_t* params) {
    HSV hsv = {rgb_matrix_config.hsv.h - qsub8(apm_val, 85), rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v};
    return hsv;
}

static inline HSV anim_heatmap_ledon_apm(uint8_t *buffer_val, bool decrease_heatmap_values, effect_params_t* params) {
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
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    if (params->init) {
        HSV hsv = {rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v};
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color_all(rgb.r, rgb.g, rgb.b);
        memset(g_rgb_frame_buffer, 0, sizeof g_rgb_frame_buffer);
    }
    if (params->iter == 0){
        relative_apm = get_relative_apm();
        decrease_heatmap_values = timer_elapsed(heatmap_decrease_timer) >= RGB_MATRIX_TYPING_HEATMAP_DECREASE_DELAY_MS;

        // Restart the timer if we are going to decrease the heatmap this frame.
        if (decrease_heatmap_values) {
            heatmap_decrease_timer = timer_read();
        }
    }

    // Render heatmap & decrease
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
            // underglow led
            RGB rgb = rgb_matrix_hsv_to_rgb(anim_apm_underglow(relative_apm, params));
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        } else {
            // everything else
            RGB rgb = rgb_matrix_hsv_to_rgb(anim_heatmap_ledon_apm(&g_rgb_frame_buffer[i], decrease_heatmap_values, params));
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#    endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif      // defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP_LEDON_APM) && defined(APM_ENABLE)
