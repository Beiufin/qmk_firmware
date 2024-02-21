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

/* The APM calculation works by taking an APM reading every APM_UPDATE_INTERVAL
 * ms and applying it to a rolling average.
 */
#define APM_MAX_SAMPLES (1000 * APM_SAMPLE_SECONDS / APM_UPDATE_INTERVAL)

// APM Stuff
static uint16_t current_apm = 0;
static uint32_t apm_timer   = 0;
static uint16_t presses = 0;

uint16_t get_current_apm(void) {
    return current_apm;
}

uint8_t get_relative_apm(void) {
    return (UINT8_MAX * current_apm + (APM_MAX_VAL)/2)/(APM_MAX_VAL);
}

void increment_apm(void) {
    if (presses < UINT16_MAX) {
        presses++;
    }
}

void decay_apm(void) {
    uint32_t elapsed  = timer_elapsed32(apm_timer);
    if (elapsed < APM_UPDATE_INTERVAL) {
        return;
    }
    // calc apm for this reading
    uint32_t apm_now  = (60000 * presses) / elapsed;

    // reset
    apm_timer = timer_read32();
    presses = 0;

    // update rolling avg
    current_apm = (current_apm * (APM_MAX_SAMPLES-1) + apm_now)/ APM_MAX_SAMPLES;
    if (current_apm > APM_MAX_VAL) current_apm = APM_MAX_VAL; // set some reasonable APM measurement limits
}
