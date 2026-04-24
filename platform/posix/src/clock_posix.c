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

#ifndef _POSIX_C_SOURCE
/* cppcheck-suppress misra-c2012-21.1 */
/* NOLINTNEXTLINE(bugprone-reserved-identifier) */
#define _POSIX_C_SOURCE 199309L
#endif

/* cppcheck-suppress misra-c2012-21.10 */
#include <time.h>

#include "robotraconteurlite/clock.h"
#include "robotraconteurlite/err.h"

robotraconteurlite_status robotraconteurlite_clock_init(struct robotraconteurlite_clock* clock)
{
    struct timespec monotonic_time;
    struct timespec realtime_time;
    robotraconteurlite_i64 monotonic_ms = 0;
    robotraconteurlite_i64 realtime_ms = 0;
    /* cppcheck-suppress misra-config */
    if (clock_gettime(CLOCK_MONOTONIC, &monotonic_time) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    /* cppcheck-suppress misra-config */
    if (clock_gettime(CLOCK_REALTIME, &realtime_time) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }

    /* Convert both times to robotraconteurlite_u64 us */
    monotonic_ms = (monotonic_time.tv_sec * 1000000) + (monotonic_time.tv_nsec / 1000);
    realtime_ms = (realtime_time.tv_sec * 1000000) + (realtime_time.tv_nsec / 1000);

    /* Calculate the offset */
    clock->clock_epoch_offset = realtime_ms - monotonic_ms;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

/* cppcheck-suppress constParameterPointer*/
robotraconteurlite_status robotraconteurlite_clock_gettime(struct robotraconteurlite_clock* clock,
                                                           robotraconteurlite_timespec* now)
{
    struct timespec monotonic_time;
    robotraconteurlite_timespec monotonic_ms = 0;
    /* cppcheck-suppress misra-config */
    if (clock_gettime(CLOCK_MONOTONIC, &monotonic_time) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }

    monotonic_ms = (((robotraconteurlite_timespec)monotonic_time.tv_sec) * 1000000) +
                   (((robotraconteurlite_timespec)monotonic_time.tv_nsec) / 1000);

    *now = monotonic_ms + clock->clock_epoch_offset;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}
