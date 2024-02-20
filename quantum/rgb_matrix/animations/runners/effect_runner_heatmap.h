#pragma once

typedef HSV (*heatmap_f)(uint8_t *buffer_val, uint8_t led_idx, bool decrease_heatmap_values, effect_params_t* params);

// A timer to track the last time we decremented all heatmap values.
static uint16_t heatmap_decrease_timer;
// Whether we should decrement the heatmap values during the next update.
static bool decrease_heatmap_values;

bool effect_runner_heatmap(effect_params_t* params, heatmap_f effect_func) {
    return effect_runner_heatmap_extra(params, effect_func, effect_func);
}

bool effect_runner_heatmap_extra(effect_params_t* params, heatmap_f effect_func, heatmap_f extra_effect_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    if (params->init) {
        memset(g_rgb_frame_buffer, 0, sizeof g_rgb_frame_buffer);
#if RGB_MATRIX_EXTRA_LED_COUNT > 0
        memset(g_rgb_frame_buffer_extra, 0, sizeof g_rgb_frame_buffer_extra);
#endif
    }

    // The heatmap animation might run in several iterations depending on
    // `RGB_MATRIX_LED_PROCESS_LIMIT`, therefore we only want to update the
    // timer when the animation starts.
    if (params->iter == 0) {
        decrease_heatmap_values = timer_elapsed(heatmap_decrease_timer) >= RGB_MATRIX_TYPING_HEATMAP_DECREASE_DELAY_MS;

        // Restart the timer if we are going to decrease the heatmap this frame.
        if (decrease_heatmap_values) {
            heatmap_decrease_timer = timer_read();
        }
    }

    // Render heatmap & decrease
    uint8_t count = 0;
    for (uint8_t row = 0; row < MATRIX_ROWS && count < RGB_MATRIX_LED_PROCESS_LIMIT; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS && count < RGB_MATRIX_LED_PROCESS_LIMIT; col++) {
            if (g_led_config.matrix_co[row][col] >= led_min && g_led_config.matrix_co[row][col] < led_max) {
                count++;
                RGB rgb = rgb_matrix_hsv_to_rgb(effect_func(&g_rgb_frame_buffer[row][col], g_led_config.matrix_co[row][col], decrease_heatmap_values, params));
                rgb_matrix_set_color(g_led_config.matrix_co[row][col], rgb.r, rgb.g, rgb.b);
            }
        }
    }
#if RGB_MATRIX_EXTRA_LED_COUNT > 0
    // This assumes extra leds always go from RGB_MATRIX_EXTRA_LED_START -> RGB_MATRIX_EXTRA_LED_COUNT
    if (led_max > RGB_MATRIX_EXTRA_LED_START && count < RGB_MATRIX_LED_PROCESS_LIMIT) {
        // starting led, also used to keep a current led_idx
        uint8_t led_i = led_min < RGB_MATRIX_EXTRA_LED_START ? RGB_MATRIX_EXTRA_LED_START : led_min;
        for (uint8_t buf_i = led_i - RGB_MATRIX_EXTRA_LED_START; led_i < led_max; buf_i++, led_i++) {
            RGB rgb = rgb_matrix_hsv_to_rgb(extra_effect_func(&g_rgb_frame_buffer_extra[buf_i], led_i, decrease_heatmap_values params));
            rgb_matrix_set_color(led_i, rgb.r, rgb.g, rgb.b);
        }
    }
#endif // RGB_MATRIX_EXTRA_LED_COUNT > 0
    return rgb_matrix_check_finished_leds(led_max);
}
