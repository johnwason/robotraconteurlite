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

#ifndef ROBOTRACONTEURLITE_CLOCK_H
#define ROBOTRACONTEURLITE_CLOCK_H

#include "robotraconteurlite/config.h"
#include "robotraconteurlite/err.h"

#ifdef __cplusplus
extern "C" {
#endif

struct robotraconteurlite_clock
{
    robotraconteurlite_i64 clock_epoch_offset;
};

typedef robotraconteurlite_i64 robotraconteurlite_timespec;

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_clock_init(struct robotraconteurlite_clock* clock);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_clock_gettime(struct robotraconteurlite_clock* clock, robotraconteurlite_timespec* now);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_clock_timeout_from_now(struct robotraconteurlite_clock* clock, robotraconteurlite_i32 timeout_ms,
                                          robotraconteurlite_timespec* timeout_out);

#ifdef __cplusplus
}
#endif

#endif /*ROBOTRACONTEURLITE_CLOCK_H */
