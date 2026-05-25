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

#ifndef _XOPEN_SOURCE
/* cppcheck-suppress misra-c2012-21.1 */
/* NOLINTNEXTLINE(bugprone-reserved-identifier) */
#define _XOPEN_SOURCE 500
#endif

#include "robotraconteurlite/poll.h"
#include <poll.h>
#include <unistd.h>
#include <errno.h>

int robotraconteurlite_poll_impl(struct robotraconteurlite_pollfd* fds, int nfds, int timeout)
{
    int ret = -1;
    /* cppcheck-suppress misra-c2012-11.3 */
    ret = poll((struct pollfd*)fds, (nfds_t)nfds, (int)timeout);
    /* False positive cppcheck warning for errno not set */
    /* cppcheck-suppress [misra-c2012-22.10,misra-config] */
    if ((ret < 0) && (errno == EINTR))
    {
        return 0;
    }
    return ret;
}
