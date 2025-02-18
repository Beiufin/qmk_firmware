#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP)
RGB_MATRIX_EFFECT(TYPING_HEATMAP)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

HSV anim_heatmap(uint8_t* buffer_val, bool decrease_heatmap_values, effect_params_t* params, rgb_config_t* config) {
    uint8_t val = *buffer_val;

    HSV hsv = {170 - qsub8(val, 85), config->hsv.s, scale8((qadd8(170, val) - 170) * 3, config->hsv.v)};

    if (decrease_heatmap_values) {
        *buffer_val = qsub8(val, 1);
    }
    return hsv;
}

bool TYPING_HEATMAP(effect_params_t* params, rgb_config_t* config) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    if (params->init) {
        rgb_matrix_set_color_all(0, 0, 0);
    }

    return effect_runner_heatmap(params, config, anim_heatmap, true);
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP)
