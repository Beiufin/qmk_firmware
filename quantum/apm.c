/*
 * Copyright 2024 Beiufin (Beiufin@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "apm.h"
#include "timer.h"
#include <math.h>

// This is basically a simplified version of the WPM feature.

// APM Stuff
static uint8_t  current_apm = 0;
static uint32_t apm_timer   = 0;

/* The APM calculation works by specifying a certain number of 'periods' inside
 * a ring buffer, and we count the number of keypresses which occur in each of
 * those periods. Then to calculate APM, we add up all of the keypresses in
 * the whole ring buffer, and then adjust for how much time is captured by our
 * ring buffer. The size of the ring buffer can be configured using the keymap
 * configuration value `APM_SAMPLE_PERIODS`.
 *
 */
#define APM_MAX_PERIODS (APM_SAMPLE_PERIODS)
#define APM_PERIOD_DURATION (1000 * APM_SAMPLE_SECONDS / APM_MAX_PERIODS)

static int16_t period_presses[APM_MAX_PERIODS] = {0};
static uint8_t current_period              = 0;
static uint8_t periods                     = 1;

/* APM_LATENCY is used as part of filtering, and controls how quickly the reported
 * APM trails behind our actual instantaneous measured APM value, and is
 * defined in milliseconds. So for APM_LATENCY == 100, the displayed APM is
 * smoothed out over periods of 0.1 seconds. This results in a nice,
 * smoothly-moving reported APM value which nevertheless is never more than
 * 0.1 seconds behind the user's actual current APM.
 */
#define APM_LATENCY (100)
static uint32_t smoothing_timer = 0;
static uint8_t  prev_apm        = 0;
static uint8_t  next_apm        = 0;


uint8_t get_current_apm(void) {
    return current_apm;
}

void increment_apm() {
    if (period_presses[current_period] > INT16_MIN) {
        period_presses[current_period]--;
    }
}

void decay_apm(void) {
    uint32_t presses = period_presses[0];
    for (int i = 1; i <= periods; i++) {
        presses += period_presses[i];
    }

    uint32_t elapsed  = timer_elapsed32(apm_timer);
    uint32_t duration = (((periods)*APM_PERIOD_DURATION) + elapsed);
    uint32_t apm_now  = (60000 * presses) / duration;

    if (apm_now > 1000) apm_now = 1000; // set some reasonable APM measurement limits

    if (elapsed > APM_PERIOD_DURATION) {
        current_period                 = (current_period + 1) % APM_MAX_PERIODS;
        period_presses[current_period] = 0;
        periods                        = (periods < APM_MAX_PERIODS - 1) ? periods + 1 : APM_MAX_PERIODS - 1;
        elapsed                        = 0;
        apm_timer                      = timer_read32();
    }

    uint32_t latency = timer_elapsed32(smoothing_timer);
    if (latency > APM_LATENCY) {
        smoothing_timer = timer_read32();
        prev_apm        = current_apm;
        next_apm        = apm_now;
    }

    current_apm = prev_apm + (latency * ((int)next_apm - (int)prev_apm) / APM_LATENCY);
}
