#pragma once

#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP)

typedef HSV (*heatmap_f)(uint8_t *buffer_val, bool decrease_heatmap_values, effect_params_t *params, rgb_config_t *config);

// A timer to track the last time we decremented all heatmap values.
static uint16_t heatmap_decrease_timer;
// Whether we should decrement the heatmap values during the next update.
static bool    decrease_heatmap_values;
static uint8_t last_frame_id;

#    ifndef RGB_MATRIX_TYPING_HEATMAP_DECREASE_DELAY_MS
#        define RGB_MATRIX_TYPING_HEATMAP_DECREASE_DELAY_MS 25
#    endif

bool effect_runner_heatmap(effect_params_t *params, rgb_config_t *config, heatmap_f effect_func, bool include_extra_leds) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    if (params->init) {
        memset(g_rgb_frame_buffer, 0, sizeof g_rgb_frame_buffer);
    }

    // The heatmap animation might run in several iterations depending on
    // `RGB_MATRIX_LED_PROCESS_LIMIT`, therefore we only want to update the
    // timer when the animation starts.
    if (params->frame_id != last_frame_id) {
        last_frame_id = params->frame_id;
        // setup for frame
        decrease_heatmap_values = timer_elapsed(heatmap_decrease_timer) >= RGB_MATRIX_TYPING_HEATMAP_DECREASE_DELAY_MS;

        // Restart the timer if we are going to decrease the heatmap this frame.
        if (decrease_heatmap_values) {
            heatmap_decrease_timer = timer_read();
        }
    }

    // Render heatmap & decrease
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        RGB rgb = rgb_matrix_hsv_to_rgb(effect_func(&g_rgb_frame_buffer[i], decrease_heatmap_values, params, config));
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }

    return rgb_matrix_check_finished_leds(led_max);
}

#endif
