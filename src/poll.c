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
#include "robotraconteurlite/connection.h"
#include <limits.h>

#define FLAGS_CHECK_ALL ROBOTRACONTEURLITE_FLAGS_CHECK_ALL
#define FLAGS_CHECK ROBOTRACONTEURLITE_FLAGS_CHECK
#define FLAGS_SET ROBOTRACONTEURLITE_FLAGS_SET
#define FLAGS_CLEAR ROBOTRACONTEURLITE_FLAGS_CLEAR

#define FAILED ROBOTRACONTEURLITE_FAILED
#define RETRY ROBOTRACONTEURLITE_RETRY

/* cppcheck-suppress constParameterPointer */
robotraconteurlite_status robotraconteurlite_poll_pollfds_add_socket(struct robotraconteurlite_connection_socket* sock,
                                                                     struct robotraconteurlite_pollfd* pollfds,
                                                                     robotraconteurlite_size_t* pollfd_count,
                                                                     robotraconteurlite_size_t max_pollfds)
{
    if (!FLAGS_CHECK(sock->flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_ACTIVE) ||
        FLAGS_CHECK(sock->flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_FAKE_SOCKET))
    {
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    if (sock->sock == (ROBOTRACONTEURLITE_SOCKET_HANDLE)0)
    {
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    return robotraconteurlite_poll_impl_add_fd(sock->sock, sock->flags, pollfds, pollfd_count, max_pollfds);
}

robotraconteurlite_status robotraconteurlite_poll_pollfds_add_sockets(struct robotraconteurlite_connection_socket* sock,
                                                                      struct robotraconteurlite_pollfd* pollfds,
                                                                      robotraconteurlite_size_t* pollfd_count,
                                                                      robotraconteurlite_size_t max_pollfds)
{
    struct robotraconteurlite_connection_socket* s = sock;
    robotraconteurlite_status rv = -1;
    while (s != NULL)
    {
        rv = robotraconteurlite_poll_pollfds_add_socket(s, pollfds, pollfd_count, max_pollfds);
        if (FAILED(rv))
        {
            return rv;
        }
        s = s->next;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_poll_pollfds_next_wake(struct robotraconteurlite_clock* clock,
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

    rv = robotraconteurlite_poll_impl(pollfds, (int)pollfd_count, timeout);
    if (FAILED(rv))
    {
        return ROBOTRACONTEURLITE_ERROR_SYSTEM_ERROR;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_poll_connections_next_wake(
    struct robotraconteurlite_connection_object* connections_head, struct robotraconteurlite_clock* clock,
    struct robotraconteurlite_pollfd* pollfds_storage, robotraconteurlite_size_t pollfds_storage_count,
    robotraconteurlite_timespec wake_time)
{
    struct robotraconteurlite_connection_object* c = connections_head;
    robotraconteurlite_status rv = -1;
    robotraconteurlite_size_t pollfd_count = 0;
    while (c != NULL)
    {
        rv = robotraconteurlite_poll_pollfds_add_sockets(&c->sock, pollfds_storage, &pollfd_count,
                                                         pollfds_storage_count);
        if (FAILED(rv))
        {
            return rv;
        }
        c = c->next;
    }

    return robotraconteurlite_poll_pollfds_next_wake(clock, pollfds_storage, pollfd_count, wake_time);
}
