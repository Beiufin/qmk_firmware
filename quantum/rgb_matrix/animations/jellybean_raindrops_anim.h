#ifdef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
RGB_MATRIX_EFFECT(JELLYBEAN_RAINDROPS)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static void jellybean_raindrops_set_color(int i, effect_params_t* params, rgb_config_t* config) {
    if (!HAS_ANY_FLAGS(g_led_config.flags[i], params->flags)) return;
    HSV hsv = {random8(), random8_min_max(127, 255), config->hsv.v};
    RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
}

bool JELLYBEAN_RAINDROPS(effect_params_t* params, rgb_config_t* config) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    if (!params->init) {
        // Change one LED every tick, make sure speed is not 0
        if (scale16by8(g_rgb_timer, qadd8(config->speed, 16)) % 5 == 0) {
            jellybean_raindrops_set_color(random8_max(RGB_MATRIX_LED_COUNT), params, config);
        }
    } else {
        for (int i = led_min; i < led_max; i++) {
            jellybean_raindrops_set_color(i, params, config);
        }
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
