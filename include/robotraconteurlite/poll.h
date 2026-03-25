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
    ROBOTRACONTEURLITE_SOCKET_HANDLE fd;
    short int events;
    short int revents;
};

struct robotraconteurlite_connection_socket;
struct robotraconteurlite_connection_object;
struct robotraconteurlite_node;

int robotraconteurlite_poll_impl(struct robotraconteurlite_pollfd* fds, int nfds, int timeout);

robotraconteurlite_status robotraconteurlite_poll_impl_add_fd(ROBOTRACONTEURLITE_SOCKET_HANDLE sock_handle,
                                                              robotraconteurlite_u16 sock_flags,
                                                              struct robotraconteurlite_pollfd* pollfds,
                                                              robotraconteurlite_size_t* pollfd_count,
                                                              robotraconteurlite_size_t max_pollfds);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_poll_pollfds_add_socket(
    struct robotraconteurlite_connection_socket* sock, struct robotraconteurlite_pollfd* pollfds,
    robotraconteurlite_size_t* pollfd_count, robotraconteurlite_size_t max_pollfds);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_poll_pollfds_add_sockets(
    struct robotraconteurlite_connection_socket* sock, struct robotraconteurlite_pollfd* pollfds,
    robotraconteurlite_size_t* pollfd_count, robotraconteurlite_size_t max_pollfds);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_poll_pollfds_next_wake(
    struct robotraconteurlite_clock* clock, struct robotraconteurlite_pollfd* pollfds,
    robotraconteurlite_size_t pollfd_count, robotraconteurlite_timespec wake_time);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_poll_connections_next_wake(
    struct robotraconteurlite_connection_object* connections_head, struct robotraconteurlite_clock* clock,
    struct robotraconteurlite_pollfd* pollfds_storage, robotraconteurlite_size_t pollfds_storage_count,
    robotraconteurlite_timespec wake_time);

#ifdef ROBOTRACONTEURLITE_HAVE_FUNCPTR
ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_poll_connections_run(
    struct robotraconteurlite_node* node, struct robotraconteurlite_clock* clock,
    struct robotraconteurlite_pollfd* pollfds_storage, robotraconteurlite_size_t pollfds_storage_count,
    robotraconteurlite_timespec wake_time);
#endif

#ifdef __cplusplus
}
#endif

#endif /* ROBOTRACONTEURLITE_POLL_H */
