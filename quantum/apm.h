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

#pragma once

#include <stdbool.h>
#include <stdint.h>

// The window size used to calculate the APM
#ifndef APM_SAMPLE_SECONDS
#    define APM_SAMPLE_SECONDS 5
#endif

/**
 * How frequently a new reading will be taken.
 * Lower values make the average more volitile, but also
 * more responsive.
 */
#ifndef APM_UPDATE_INTERVAL
#    define APM_UPDATE_INTERVAL 50
#endif

#if APM_MAX_VAL > UINT16_MAX
#    undef APM_MAX_VAL
#endif

// The max apm to allow.
#ifndef APM_MAX_VAL
#    define APM_MAX_VAL 500
#endif

uint16_t get_current_apm(void);
uint8_t  get_relative_apm(void);
void     increment_apm(void);

void apm_task(void);
