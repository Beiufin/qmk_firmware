// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// clang-format off

// heatmap
#if defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP) || \
    defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP_LEDON) || \
    defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP_SOLID) || \
    defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP_LEDON_APM)
// if any of the typing heatmaps are enabled, the process function should be called
#    define RGB_MATRIX_PROCESS_HEATMAP
#endif

// framebuffer
#if defined(RGB_MATRIX_PROCESS_HEATMAP) || \
    defined(ENABLE_RGB_MATRIX_DIGITAL_RAIN)
#    define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#endif

#if defined(ENABLE_RGB_MATRIX_APM) || defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP_LEDON_APM)
// if any of the typing heatmaps are enabled, the process function should be called
#    define TRACK_APM
#endif

// reactive
#if defined(ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE) || \
    defined(ENABLE_RGB_MATRIX_SOLID_REACTIVE) || \
    defined(ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE) || \
    defined(ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE) || \
    defined(ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS) || \
    defined(ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS) || \
    defined(ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS) || \
    defined(ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS) || \
    defined(ENABLE_RGB_MATRIX_SPLASH) || \
    defined(ENABLE_RGB_MATRIX_MULTISPLASH) || \
    defined(ENABLE_RGB_MATRIX_SOLID_SPLASH) || \
    defined(ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS) || \
    defined(ENABLE_RGB_MATRIX_SOLID_MULTISPLASH)
#    define RGB_MATRIX_KEYPRESSES
#endif
