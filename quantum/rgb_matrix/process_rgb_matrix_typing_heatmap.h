#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(RGB_MATRIX_PROCESS_HEATMAP)
// shared heatmap code
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#        ifndef RGB_MATRIX_TYPING_HEATMAP_INCREASE_STEP
#            define RGB_MATRIX_TYPING_HEATMAP_INCREASE_STEP 32
#        endif

#        ifndef RGB_MATRIX_TYPING_HEATMAP_SPREAD
#            define RGB_MATRIX_TYPING_HEATMAP_SPREAD 40
#        endif

#        ifndef RGB_MATRIX_TYPING_HEATMAP_AREA_LIMIT
#            define RGB_MATRIX_TYPING_HEATMAP_AREA_LIMIT 16
#        endif

#        ifdef RGB_MATRIX_TYPING_HEATMAP_SLIM
void process_rgb_matrix_typing_heatmap(uint8_t row, uint8_t col) {
    // Limit effect to pressed keys
    g_rgb_frame_buffer[row][col] = qadd8(g_rgb_frame_buffer[row][col], RGB_MATRIX_TYPING_HEATMAP_INCREASE_STEP);
}
#        else // RGB_MATRIX_TYPING_HEATMAP_SLIM

static inline void apply_dist(uint8_t *frame_buf, uint8_t led1_idx, uint8_t led2_idx) {
#           define LED_DISTANCE(led_a, led_b) sqrt16(((int16_t)(led_a.x - led_b.x) * (int16_t)(led_a.x - led_b.x)) + ((int16_t)(led_a.y - led_b.y) * (int16_t)(led_a.y - led_b.y)))
        uint8_t distance = LED_DISTANCE(g_led_config.point[led1_idx], g_led_config.point[led2_idx]);
#           undef LED_DISTANCE
        if (distance <= RGB_MATRIX_TYPING_HEATMAP_SPREAD) {
            uint8_t amount = qsub8(RGB_MATRIX_TYPING_HEATMAP_SPREAD, distance);
            if (amount > RGB_MATRIX_TYPING_HEATMAP_AREA_LIMIT) {
                amount = RGB_MATRIX_TYPING_HEATMAP_AREA_LIMIT;
            }
            *frame_buf = qadd8(*frame_buf, amount);
        }
}

void process_rgb_matrix_typing_heatmap(uint8_t row, uint8_t col) {
    if (g_led_config.matrix_co[row][col] == NO_LED) { // skip as pressed key doesn't have an led position
        return;
    }
#           if RGB_MATRIX_EXTRA_LED_COUNT > 0
    uint8_t led_idx = g_led_config.matrix_co[row][col];
    for (uint8_t i = 0, led_i = RGB_MATRIX_EXTRA_LED_START; i < RGB_MATRIX_EXTRA_LED_COUNT; i++, led_i++) {
        apply_dist(&g_rgb_frame_buffer_extra[i], led_idx, led_i);
    }
#           endif //RGB_MATRIX_EXTRA_LED_COUNT > 0
    for (uint8_t i_row = 0; i_row < MATRIX_ROWS; i_row++) {
        for (uint8_t i_col = 0; i_col < MATRIX_COLS; i_col++) {
            if (g_led_config.matrix_co[i_row][i_col] == NO_LED) { // skip as target key doesn't have an led position
                continue;
            }
            if (i_row == row && i_col == col) {
                g_rgb_frame_buffer[row][col] = qadd8(g_rgb_frame_buffer[row][col], RGB_MATRIX_TYPING_HEATMAP_INCREASE_STEP);
            } else {
                apply_dist(&g_rgb_frame_buffer[i_row][i_col], g_led_config.matrix_co[row][col], g_led_config.matrix_co[i_row][i_col]);
            }
        }
    }
}
#       endif
#    endif
#endif
