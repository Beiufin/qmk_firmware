#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP_LEDON)
RGB_MATRIX_EFFECT(TYPING_HEATMAP_LEDON)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

HSV anim_heatmap_ledon(uint8_t *buffer_val, uint8_t led_idx, effect_params_t* params) {
    uint8_t val = *buffer_val;
    if (!HAS_ANY_FLAGS(g_led_config.flags[led_idx], params->flags)) return;

    // Orig: {170 to 0, s, 0 + val*3}
    // HSV hsv = {170 - qsub8(val, 85), rgb_matrix_config.hsv.s, scale8((qadd8(170, val) - 170) * 3, rgb_matrix_config.hsv.v)};
    // {h to h-170, s, v}
    HSV hsv = {rgb_matrix_config.hsv.h - qsub8(val, 85), rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v};

    if (decrease_heatmap_values) {
        *buffer_val = qsub8(val, 1);
    }
    return hsv;
}

bool TYPING_HEATMAP_LEDON(effect_params_t* params) {
    if (params->init) {
        HSV hsv = {rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v};
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color_all(rgb.r, rgb.g, rgb.b);
    }

    return effect_runner_heatmap(params, anim_heatmap_ledon);
}

#    endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif      // defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP)
