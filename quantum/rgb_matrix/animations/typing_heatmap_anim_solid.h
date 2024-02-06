#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP_SOLID)
RGB_MATRIX_EFFECT(TYPING_HEATMAP_SOLID)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
// A timer to track the last time we decremented all heatmap values.
static uint16_t heatmap_decrease_timer;
// Whether we should decrement the heatmap values during the next update.
static bool decrease_heatmap_values;

static inline void anim_heatmap_solid(uint8_t *buffer_val, uint8_t led_idx, uint8_t v_min, effect_params_t* params) {
    uint8_t val = *buffer_val;
    if (!HAS_ANY_FLAGS(g_led_config.flags[led_idx], params->flags)) return;

    HSV hsv = {rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, qadd8(v_min, scale8(val, qsub8(rgb_matrix_config.hsv.v, v_min)))};
    RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
    rgb_matrix_set_color(led_idx, rgb.r, rgb.g, rgb.b);

    if (decrease_heatmap_values) {
        *buffer_val = qsub8(val, 1);
    }
}

bool TYPING_HEATMAP_SOLID(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    uint8_t v_min = rgb_matrix_config.speed;

    if (params->init) {
        HSV hsv = {rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, v_min};
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color_all(rgb.r, rgb.g, rgb.b);
        memset(g_rgb_frame_buffer, 0, sizeof g_rgb_frame_buffer);
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
                anim_heatmap_solid(&g_rgb_frame_buffer[row][col], g_led_config.matrix_co[row][col], v_min, params);
            }
        }
    }
#        if RGB_MATRIX_EXTRA_LED_COUNT > 0
    // This assumes extra leds always go from RGB_MATRIX_EXTRA_LED_START -> RGB_MATRIX_EXTRA_LED_COUNT
    if (led_max > RGB_MATRIX_EXTRA_LED_START) {
        // starting led, also used to keep a current led_idx
        uint8_t led_i = led_min < RGB_MATRIX_EXTRA_LED_START ? RGB_MATRIX_EXTRA_LED_START : led_min;
        for (uint8_t buf_i = led_i - RGB_MATRIX_EXTRA_LED_START; led_i < led_max; buf_i++, led_i++) {
            anim_heatmap_solid(&g_rgb_frame_buffer_extra[buf_i], led_i, v_min, params);
        }
    }
#        endif // RGB_MATRIX_EXTRA_LED_COUNT > 0

    return rgb_matrix_check_finished_leds(led_max);
}

#    endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif      // defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP_SOLID)
