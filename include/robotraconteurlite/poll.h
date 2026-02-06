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

#ifndef ROBOTRACONTEURLITE_POLL_H
#define ROBOTRACONTEURLITE_POLL_H

#include <stdlib.h>
#include "robotraconteurlite/clock.h"

#ifdef __cplusplus
extern "C" {
#endif

struct robotraconteurlite_pollfd
{
    ROBOTRACONTEURLITE_SOCKET fd;
    short int events;
    short int revents;
};

int robotraconteurlite_poll(struct robotraconteurlite_pollfd* fds, int nfds, int timeout);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_poll_next_wake(struct robotraconteurlite_clock* clock, struct robotraconteurlite_pollfd* pollfds,
                                  robotraconteurlite_size_t pollfd_count, robotraconteurlite_timespec wake_time);

#ifdef __cplusplus
}
#endif

#endif /* ROBOTRACONTEURLITE_POLL_H */
