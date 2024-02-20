#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP_SOLID)
RGB_MATRIX_EFFECT(TYPING_HEATMAP_SOLID)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

HSV anim_heatmap_solid(uint8_t *buffer_val, bool decrease_heatmap_values, effect_params_t* params) {
    uint8_t val = *buffer_val;
    uint8_t v_min = rgb_matrix_config.speed;

    HSV hsv = {rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, qadd8(v_min, scale8(val, qsub8(rgb_matrix_config.hsv.v, v_min)))};

    if (decrease_heatmap_values) {
        *buffer_val = qsub8(val, 1);
    }
    return hsv;
}

bool TYPING_HEATMAP_SOLID(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    if (params->init) {
        HSV hsv = {rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_matrix_config.speed};
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color_all(rgb.r, rgb.g, rgb.b);
    }

    return effect_runner_heatmap(params, anim_heatmap_solid);
}

#    endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif      // defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP_SOLID)
