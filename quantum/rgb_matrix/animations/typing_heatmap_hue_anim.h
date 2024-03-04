#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP_HUE)
RGB_MATRIX_EFFECT(TYPING_HEATMAP_HUE)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

HSV anim_heatmap_hue(uint8_t* buffer_val, bool decrease_heatmap_values, effect_params_t* params, rgb_config_t* config) {
    uint8_t val = *buffer_val;

    // Orig: {170 to 0, s, 0 + val*3}
    // HSV hsv = {170 - qsub8(val, 85), config->hsv.s, scale8((qadd8(170, val) - 170) * 3, config->hsv.v)};
    // {h to h-170, s, v}
    HSV hsv = {config->hsv.h - qsub8(val, 85), config->hsv.s, config->hsv.v};

    if (decrease_heatmap_values) {
        *buffer_val = qsub8(val, 1);
    }
    return hsv;
}

bool TYPING_HEATMAP_HUE(effect_params_t* params, rgb_config_t* config) {
    if (params->init) {
        HSV hsv = {config->hsv.h, config->hsv.s, config->hsv.v};
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color_all(rgb.r, rgb.g, rgb.b);
    }

    return effect_runner_heatmap(params, config, anim_heatmap_hue, true);
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP_HUE)
