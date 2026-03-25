/* Copyright 2011-2024 Wason Technology, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <time.h>

#include "robotraconteurlite/clock.h"
#include "robotraconteurlite/err.h"

#include <windows.h>
#include <stdint.h>

robotraconteurlite_status robotraconteurlite_clock_init(struct robotraconteurlite_clock* clock)
{
    FILETIME ft;
    ULARGE_INTEGER ui;
    robotraconteurlite_i64 monotonic_us = 0;
    robotraconteurlite_i64 realtime_us = 0;

    // Get the current system time (realtime)
    GetSystemTimeAsFileTime(&ft);
    ui.LowPart = ft.dwLowDateTime;
    ui.HighPart = ft.dwHighDateTime;
    realtime_us = (ui.QuadPart / 10);

    // Get the current performance counter (monotonic)
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    monotonic_us = (counter.QuadPart * 1000000) / freq.QuadPart;

    // Calculate the offset
    clock->clock_epoch_offset = realtime_us - monotonic_us;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_clock_gettime(struct robotraconteurlite_clock* clock,
                                                           robotraconteurlite_timespec* now)
{
    LARGE_INTEGER freq, counter;
    robotraconteurlite_i64 monotonic_us = 0;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    monotonic_us = (counter.QuadPart * 1000000) / freq.QuadPart;

    *now = monotonic_us + clock->clock_epoch_offset;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}
