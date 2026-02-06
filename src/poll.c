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

#include "robotraconteurlite/poll.h"
#include "robotraconteurlite/err.h"
#include "robotraconteurlite/util.h"
#include <limits.h>

#define FAILED ROBOTRACONTEURLITE_FAILED

robotraconteurlite_status robotraconteurlite_poll_next_wake(struct robotraconteurlite_clock* clock,
                                                            struct robotraconteurlite_pollfd* pollfds,
                                                            robotraconteurlite_size_t pollfd_count,
                                                            robotraconteurlite_timespec wake_time)
{
    robotraconteurlite_timespec now = 0;
    robotraconteurlite_status rv = -1;
    int timeout = 0;
    robotraconteurlite_i64 timeout_i64 = 0;

    rv = robotraconteurlite_clock_gettime(clock, &now);
    if (FAILED(rv))
    {
        return rv;
    }

    if (wake_time < now)
    {
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    timeout_i64 = wake_time - now;
    if (timeout_i64 > INT_MAX)
    {
        /* This should never happen! */
        timeout = INT_MAX;
    }
    else
    {
        timeout = (int)timeout_i64;
    }

    rv = robotraconteurlite_poll(pollfds, (int)pollfd_count, timeout);
    if (FAILED(rv))
    {
        return ROBOTRACONTEURLITE_ERROR_SYSTEM_ERROR;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}
