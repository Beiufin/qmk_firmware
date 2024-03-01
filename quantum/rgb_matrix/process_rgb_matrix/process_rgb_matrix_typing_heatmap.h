#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(RGB_MATRIX_PROCESS_HEATMAP)
// shared heatmap code
#    ifndef RGB_MATRIX_TYPING_HEATMAP_INCREASE_STEP
#        define RGB_MATRIX_TYPING_HEATMAP_INCREASE_STEP 32
#    endif

#    ifndef RGB_MATRIX_TYPING_HEATMAP_SPREAD
#        define RGB_MATRIX_TYPING_HEATMAP_SPREAD 40
#    endif

#    ifndef RGB_MATRIX_TYPING_HEATMAP_AREA_LIMIT
#        define RGB_MATRIX_TYPING_HEATMAP_AREA_LIMIT 16
#    endif

#    ifdef RGB_MATRIX_TYPING_HEATMAP_SLIM
void process_rgb_matrix_typing_heatmap(uint8_t row, uint8_t col) {
    // could/should use rgb_matrix_map_row_column_to_led, but it requires an array of an unknown size as a parameter
    uint8_t led_i = g_led_config.matrix_co[row][col];
    if (led_i == NO_LED) { // skip as pressed key doesn't have an led position
        return;
    }
    // Limit effect to pressed keys
    g_rgb_frame_buffer[led_i] = qadd8(g_rgb_frame_buffer[led_i], RGB_MATRIX_TYPING_HEATMAP_INCREASE_STEP);
}
#    else // RGB_MATRIX_TYPING_HEATMAP_SLIM

void process_rgb_matrix_typing_heatmap(uint8_t row, uint8_t col) {
    // could/should use rgb_matrix_map_row_column_to_led, but it requires an array of an unknown size as a parameter
    uint8_t led_i = g_led_config.matrix_co[row][col];
    if (led_i == NO_LED) { // skip as pressed key doesn't have an led position
        return;
    }
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        if (i == led_i) {
            g_rgb_frame_buffer[i] = qadd8(g_rgb_frame_buffer[i], RGB_MATRIX_TYPING_HEATMAP_INCREASE_STEP);
        } else {
#        define LED_DISTANCE(led_a, led_b) sqrt16(((int16_t)(led_a.x - led_b.x) * (int16_t)(led_a.x - led_b.x)) + ((int16_t)(led_a.y - led_b.y) * (int16_t)(led_a.y - led_b.y)))
            uint8_t distance = LED_DISTANCE(g_led_config.point[i], g_led_config.point[led_i]);
#        undef LED_DISTANCE
            if (distance <= RGB_MATRIX_TYPING_HEATMAP_SPREAD) {
                uint8_t amount = qsub8(RGB_MATRIX_TYPING_HEATMAP_SPREAD, distance);
                if (amount > RGB_MATRIX_TYPING_HEATMAP_AREA_LIMIT) {
                    amount = RGB_MATRIX_TYPING_HEATMAP_AREA_LIMIT;
                }
                g_rgb_frame_buffer[i] = qadd8(g_rgb_frame_buffer[i], amount);
            }
        }
    }
}
#    endif
#endif
