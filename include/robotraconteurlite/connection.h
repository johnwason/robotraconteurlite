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

#ifndef ROBOTRACONTEURLITE_CONNECTION_H
#define ROBOTRACONTEURLITE_CONNECTION_H

#include "robotraconteurlite/message.h"
#include "robotraconteurlite/clock.h"
#include "robotraconteurlite/util.h"

/* robotraconteurlite_connection_config_flags */
#define ROBOTRACONTEURLITE_CONFIG_FLAGS_NULL 0U
#define ROBOTRACONTEURLITE_CONFIG_FLAGS_ISSERVER 0x1U
#define ROBOTRACONTEURLITE_CONFIG_FLAGS_ENABLE_REDUCED_HEADER4 0x2U

/* robotraconteurlite_connection_status_flags */
#define ROBOTRACONTEURLITE_STATUS_FLAGS_NULL 0U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_IDLE 0x1U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTED 0x2U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTING 0x4U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSING 0x8U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSED 0x10U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR 0x20U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_RECEIVE_REQUESTED 0x40U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_RECEIVING 0x80U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_SEND_REQUESTED 0x100U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_SENDING 0x200U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_RECEIVED 0x400U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_CONSUMED 0x800U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_BLOCK_SEND 0x1000U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_SENT 0x2000U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_ESTABLISHED 0x4000U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_CLIENT_ESTABLISHED 0x8000U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSE_REQUESTED 0x10000U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSED_CONSUMED 0x20000U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_SENT_CONSUMED 0x40000U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTED_CONSUMED 0x80000U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_SEND_MESSAGE4 0x100000U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSE_NOTIFIED 0x200000U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_CLIENT_SOCKET_CONNECTED 0x400000U
#define ROBOTRACONTEURLITE_STATUS_FLAGS_CLIENT_SOCKET_CONNECTED_CONSUMED 0x800000U

/* robotraconteurlite_connection_acceptor_status_flags */
#define ROBOTRACONTEURLITE_CONNECTION_ACCEPTOR_STATUS_FLAGS_NULL 0U

/* transport_capability_flags */
#define ROBOTRACONTEURLITE_TRANSPORT_CAPABILITY_CODE_PAGE_MASK 0xFFF00000U
#define ROBOTRACONTEURLITE_TRANSPORT_CAPABILITY_CODE_MESSAGE2_BASIC_PAGE 0x02000000U
#define ROBOTRACONTEURLITE_TRANSPORT_CAPABILITY_CODE_MESSAGE2_BASIC_ENABLE 0x00000001U
#define ROBOTRACONTEURLITE_TRANSPORT_CAPABILITY_CODE_MESSAGE4_BASIC_PAGE 0x04000000U
#define ROBOTRACONTEURLITE_TRANSPORT_CAPABILITY_CODE_MESSAGE4_BASIC_ENABLE 0x00000001U

/* socket_flags */
#define ROBOTRACONTEURLITE_SOCKET_FLAGS_ACTIVE 0x1U
#define ROBOTRACONTEURLITE_SOCKET_FLAGS_WANT_RECEIVE 0x2U
#define ROBOTRACONTEURLITE_SOCKET_FLAGS_WANT_SEND 0x4U
#define ROBOTRACONTEURLITE_SOCKET_FLAGS_RECEIVE_WOULD_BLOCK 0x8U
#define ROBOTRACONTEURLITE_SOCKET_FLAGS_SEND_WOULD_BLOCK 0x10U
#define ROBOTRACONTEURLITE_SOCKET_FLAGS_REGISTERED 0x100U
#define ROBOTRACONTEURLITE_SOCKET_FLAGS_WANT_RECEIVE_REGISTERED 0x200U
#define ROBOTRACONTEURLITE_SOCKET_FLAGS_WANT_SEND_REGISTERED 0x400U
#define ROBOTRACONTEURLITE_SOCKET_FLAGS_ERROR_REGISTERED 0x800U
#define ROBOTRACONTEURLITE_SOCKET_FLAGS_FAKE_SOCKET 0x8000U

#define ROBOTRACONTEURLITE_SOCKET_FAKE_SOCKET 0xFFFFFF

/* connection_object_type */
#define ROBOTRACONTEURLITE_CONNECTION_OBJECT_TYPE_CONNECTION 0x1U
#define ROBOTRACONTEURLITE_CONNECTION_OBJECT_TYPE_ACCEPTOR 0x2U
#define ROBOTRACONTEURLITE_CONNECTION_OBJECT_TYPE_LIST_HEAD 0x3U

#ifdef __cplusplus
extern "C" {
#endif

struct robotraconteurlite_transport_storage
{
    robotraconteurlite_byte _storage[ROBOTRACONTEURLITE_TRANSPORT_STORAGE_SIZE];
};

struct robotraconteurlite_user_storage
{
    robotraconteurlite_u32 user_data_type_code;
    void* user_data;
};

struct robotraconteurlite_connection_socket
{
    ROBOTRACONTEURLITE_SOCKET_HANDLE sock;
    robotraconteurlite_u16 flags;
    struct robotraconteurlite_connection_socket* next;
#ifdef _WIN32
    struct
    {
        robotraconteurlite_u8 reserved[64];
    } win32_storage;
#endif
};

#ifdef ROBOTRACONTEURLITE_HAVE_FUNCPTR

struct robotraconteurlite_connection_object;

struct robotraconteurlite_connection_object_ops
{
    robotraconteurlite_status (*communicate_recv)(struct robotraconteurlite_connection_object* connection,
                                                  struct robotraconteurlite_connection_object* connections_head,
                                                  robotraconteurlite_timespec now);
    robotraconteurlite_status (*communicate_send)(struct robotraconteurlite_connection_object* connection,
                                                  struct robotraconteurlite_connection_object* connections_head,
                                                  robotraconteurlite_timespec now);
    robotraconteurlite_status (*communicate_process_control)(
        struct robotraconteurlite_connection_object* connection,
        struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now);
    robotraconteurlite_status (*prepare_wait)(struct robotraconteurlite_connection_object* connection,
                                              struct robotraconteurlite_connection_object* connections_head,
                                              robotraconteurlite_timespec now);
    robotraconteurlite_status (*connection_close)(struct robotraconteurlite_connection_object* connection,
                                                  struct robotraconteurlite_connection_object* connections_head,
                                                  robotraconteurlite_timespec now);
};
#endif

struct robotraconteurlite_connection_object
{
    robotraconteurlite_u32 connection_object_type;
    robotraconteurlite_u32 transport_type;
    struct robotraconteurlite_connection_object* next;
    struct robotraconteurlite_connection_object* prev;
    struct robotraconteurlite_connection_socket sock;
#ifdef ROBOTRACONTEURLITE_HAVE_FUNCPTR
    const struct robotraconteurlite_connection_object_ops* ops;
#endif
};

struct robotraconteurlite_node_service;
struct robotraconteurlite_node_client;

/* NOLINTNEXTLINE(clang-analyzer-optin.performance.Padding) */
struct robotraconteurlite_connection
{
    struct robotraconteurlite_connection_object head;

    /* Send and receive buffers */
    robotraconteurlite_byte* send_buffer;
    robotraconteurlite_byte* recv_buffer;
    robotraconteurlite_size_t send_buffer_len;
    robotraconteurlite_size_t recv_buffer_len;
    robotraconteurlite_size_t send_buffer_pos;
    robotraconteurlite_size_t recv_buffer_pos;

    /* Control flags */
    robotraconteurlite_u32 config_flags;
    robotraconteurlite_u32 connection_state;

    /* Parameters */
    robotraconteurlite_i32 heartbeat_period_ms;
    robotraconteurlite_i32 heartbeat_timeout_ms;
    robotraconteurlite_timespec heartbeat_next_check_ms;

    /* Robot Raconteur information */
    robotraconteurlite_u32 local_endpoint;
    robotraconteurlite_u32 remote_endpoint;
    struct robotraconteurlite_nodeid remote_nodeid;
    char remote_nodename_char[ROBOTRACONTEURLITE_MESSAGE_STR_MAX_SIZE];
    struct robotraconteurlite_const_string remote_nodename;
    char remote_service_name_char[ROBOTRACONTEURLITE_MESSAGE_STR_MAX_SIZE];
    struct robotraconteurlite_const_string remote_service_name;
    robotraconteurlite_timespec last_recv_message_time;
    robotraconteurlite_timespec last_send_message_time;
    robotraconteurlite_u32 last_request_id;
    robotraconteurlite_u8 message_flags_inv_mask;

    /* Message information */
    robotraconteurlite_u32 recv_message_len;
    robotraconteurlite_u32 send_message_len;

    /* Transport storage */
    struct robotraconteurlite_transport_storage transport_storage;

    /* User storage */
    struct robotraconteurlite_user_storage user_storage;

    /* Transport next wake request time */
    robotraconteurlite_timespec transport_next_wake;

    /* Storage for associated service */
    struct robotraconteurlite_node_service* service;

    /* Storage for associated client */
    struct robotraconteurlite_node_client* client;
};

struct robotraconteurlite_connection_acceptor
{
    struct robotraconteurlite_connection_object head;

    /* Control flags */
    robotraconteurlite_u32 config_flags;
    robotraconteurlite_u32 acceptor_state;
};

struct robotraconteurlite_sockaddr_storage
{
    robotraconteurlite_byte _storage[ROBOTRACONTEURLITE_SOCKADDR_STORAGE_SIZE];
};

/* robotraconteurlite_addr_flags */
#define ROBOTRACONTEURLITE_ADDR_FLAGS_NULL 0x0U
#define ROBOTRACONTEURLITE_ADDR_FLAGS_WEBSOCKET 0x1U

struct robotraconteurlite_addr
{
    robotraconteurlite_u32 transport_type;
    struct robotraconteurlite_sockaddr_storage socket_addr;
    struct robotraconteurlite_nodeid nodeid;
    struct robotraconteurlite_const_string nodename;
    struct robotraconteurlite_const_string service_name;
    robotraconteurlite_u32 flags;
    struct robotraconteurlite_const_string http_host;
    struct robotraconteurlite_const_string http_path;
};

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_connection_reset(struct robotraconteurlite_connection* connection);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connection_verify_preamble(
    struct robotraconteurlite_connection* connection, robotraconteurlite_u32* message_len);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connection_begin_send_message(
    struct robotraconteurlite_connection* connection, struct robotraconteurlite_message_writer* message_writer,
    struct robotraconteurlite_buffer_vec* buffer_storage);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connection_end_send_message(
    struct robotraconteurlite_connection* connection, robotraconteurlite_size_t message_len);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_connection_abort_send_message(struct robotraconteurlite_connection* connection);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connection_message_receive(
    struct robotraconteurlite_connection* connection, struct robotraconteurlite_message_reader* message_reader,
    struct robotraconteurlite_buffer_vec* buffer_storage);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_connection_message_receive_consume(struct robotraconteurlite_connection* connection);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_connection_close(struct robotraconteurlite_connection* connection);

static int robotraconteurlite_connection_is_closed(const struct robotraconteurlite_connection* connection)
{
    return ROBOTRACONTEURLITE_FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSED);
}

static int robotraconteurlite_connection_is_closed_event(const struct robotraconteurlite_connection* connection)
{
    return (ROBOTRACONTEURLITE_FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSED)) &&
           (!ROBOTRACONTEURLITE_FLAGS_CHECK(connection->connection_state,
                                            ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSED_CONSUMED));
}

static void robotraconteurlite_connection_consume_closed(struct robotraconteurlite_connection* connection)
{
    ROBOTRACONTEURLITE_FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSED_CONSUMED);
}

static int robotraconteurlite_connection_is_error(const struct robotraconteurlite_connection* connection)
{
    /* Check for error, but don't report if already closed or closing */
    return (ROBOTRACONTEURLITE_FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR)) &&
           (!ROBOTRACONTEURLITE_FLAGS_CHECK(
               connection->connection_state,
               (ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSED | ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSE_REQUESTED)));
}

static int robotraconteurlite_connection_is_connected(const struct robotraconteurlite_connection* connection)
{
    return ROBOTRACONTEURLITE_FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTED);
}

static void robotraconteurlite_connection_consume_connected(struct robotraconteurlite_connection* connection)
{
    ROBOTRACONTEURLITE_FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTED_CONSUMED);
}

static int robotraconteurlite_connection_is_connected_event(const struct robotraconteurlite_connection* connection)
{
    return (ROBOTRACONTEURLITE_FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTED)) &&
           (!ROBOTRACONTEURLITE_FLAGS_CHECK(connection->connection_state,
                                            ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTED_CONSUMED));
}

static int robotraconteurlite_connection_is_message_received(const struct robotraconteurlite_connection* connection)
{
    return ROBOTRACONTEURLITE_FLAGS_CHECK(connection->connection_state,
                                          ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_RECEIVED);
}

static int robotraconteurlite_connection_is_message_received_event(
    const struct robotraconteurlite_connection* connection)
{
    return (ROBOTRACONTEURLITE_FLAGS_CHECK(connection->connection_state,
                                           ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_RECEIVED)) &&
           (!ROBOTRACONTEURLITE_FLAGS_CHECK(connection->connection_state,
                                            ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_CONSUMED));
}

static void robotraconteurlite_connection_consume_message_received(struct robotraconteurlite_connection* connection)
{
    ROBOTRACONTEURLITE_FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_CONSUMED);
}

static int robotraconteurlite_connection_is_message_sent(const struct robotraconteurlite_connection* connection)
{
    return ROBOTRACONTEURLITE_FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_SENT);
}

static int robotraconteurlite_connection_is_message_sent_event(const struct robotraconteurlite_connection* connection)
{
    return (ROBOTRACONTEURLITE_FLAGS_CHECK(connection->connection_state,
                                           ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_SENT)) &&
           (!ROBOTRACONTEURLITE_FLAGS_CHECK(connection->connection_state,
                                            ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_SENT_CONSUMED));
}

static void robotraconteurlite_connection_consume_message_sent(struct robotraconteurlite_connection* connection)
{
    ROBOTRACONTEURLITE_FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_SENT_CONSUMED);
}

static void robotraconteurlite_connection_error(struct robotraconteurlite_connection* connection)
{
    ROBOTRACONTEURLITE_FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR);
}

static int robotraconteurlite_connection_is_server(const struct robotraconteurlite_connection* connection)
{
    return ROBOTRACONTEURLITE_FLAGS_CHECK(connection->config_flags, ROBOTRACONTEURLITE_CONFIG_FLAGS_ISSERVER);
}

ROBOTRACONTEURLITE_API void robotraconteurlite_connections_construct_from_array(
    struct robotraconteurlite_connection connections_fixed_storage[],
    robotraconteurlite_size_t connections_fixed_storage_len, robotraconteurlite_byte buffers[],
    robotraconteurlite_size_t buffer_size, robotraconteurlite_size_t buffer_count,
    struct robotraconteurlite_connection_object* connections_head);

static int robotraconteurlite_connection_is_heartbeat_timeout(const struct robotraconteurlite_connection* connection,
                                                              robotraconteurlite_timespec now)
{
    robotraconteurlite_i64 recv_diff_ms = now - connection->last_recv_message_time;
    robotraconteurlite_i64 send_diff_ms = now - connection->last_send_message_time;

    if ((recv_diff_ms > connection->heartbeat_timeout_ms) || (send_diff_ms > connection->heartbeat_timeout_ms))
    {
        return 2;
    }

    if ((recv_diff_ms > connection->heartbeat_period_ms) || (send_diff_ms > connection->heartbeat_period_ms))
    {
        return 1;
    }

    return 0;
}

static int robotraconteurlite_connection_is_idle(const struct robotraconteurlite_connection* connection)
{
    return ROBOTRACONTEURLITE_FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_IDLE);
}

static int robotraconteurlite_connection_is_client_socket_connected(
    const struct robotraconteurlite_connection* connection)
{
    return ROBOTRACONTEURLITE_FLAGS_CHECK(connection->connection_state,
                                          ROBOTRACONTEURLITE_STATUS_FLAGS_CLIENT_SOCKET_CONNECTED);
}

static int robotraconteurlite_connection_is_client_socket_connected_event(
    const struct robotraconteurlite_connection* connection)
{
    return (ROBOTRACONTEURLITE_FLAGS_CHECK(connection->connection_state,
                                           ROBOTRACONTEURLITE_STATUS_FLAGS_CLIENT_SOCKET_CONNECTED)) &&
           (!ROBOTRACONTEURLITE_FLAGS_CHECK(connection->connection_state,
                                            ROBOTRACONTEURLITE_STATUS_FLAGS_CLIENT_SOCKET_CONNECTED_CONSUMED));
}

static void robotraconteurlite_connection_consume_client_socket_connected_event(
    struct robotraconteurlite_connection* connection)
{
    ROBOTRACONTEURLITE_FLAGS_SET(connection->connection_state,
                                 ROBOTRACONTEURLITE_STATUS_FLAGS_CLIENT_SOCKET_CONNECTED_CONSUMED);
}

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_connection_next_wake(struct robotraconteurlite_connection* connection,
                                        robotraconteurlite_timespec now, robotraconteurlite_timespec* next_wake);

ROBOTRACONTEURLITE_API struct robotraconteurlite_connection* robotraconteurlite_connection_find_idle(
    struct robotraconteurlite_connection_object* connections_head);

ROBOTRACONTEURLITE_API void robotraconteurlite_connection_list_head_construct(
    struct robotraconteurlite_connection_object* connections_head);

ROBOTRACONTEURLITE_API void robotraconteurlite_connection_list_append(
    struct robotraconteurlite_connection_object* connections_head, struct robotraconteurlite_connection_object* value);

ROBOTRACONTEURLITE_API void robotraconteurlite_connection_list_remove(
    struct robotraconteurlite_connection_object* connections_head, struct robotraconteurlite_connection_object* value);

ROBOTRACONTEURLITE_API void robotraconteurlite_connection_construct(
    struct robotraconteurlite_connection* connection, struct robotraconteurlite_connection_object* connections_head);

ROBOTRACONTEURLITE_API void robotraconteurlite_connection_init(struct robotraconteurlite_connection* connection);

ROBOTRACONTEURLITE_API void robotraconteurlite_connection_init_connections(
    struct robotraconteurlite_connection_object* connections_head);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connection_impl_communicate_process_control(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now,
    robotraconteurlite_u32 transport_type, robotraconteurlite_u8* close_request);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connection_impl_communicate_after_close_requested(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now,
    robotraconteurlite_status close_rv);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connection_impl_communicate_recv1(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now,
    robotraconteurlite_size_t* recv_op_len);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connection_impl_communicate_recv2(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now,
    robotraconteurlite_status recv_op_rv);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connection_impl_communicate_send1(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now,
    robotraconteurlite_size_t* send_op_len);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connection_impl_communicate_send2(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now,
    robotraconteurlite_status send_op_rv);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connection_impl_connect2(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now,
    robotraconteurlite_u32 transport_type, const struct robotraconteurlite_addr* addr,
    const struct robotraconteurlite_connection_socket* sock);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connection_impl_accept2(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now,
    robotraconteurlite_u32 transport_type, struct robotraconteurlite_connection_socket* sock);

ROBOTRACONTEURLITE_API void robotraconteurlite_connection_init_connection_acceptor(
    struct robotraconteurlite_connection_acceptor* acceptor);

ROBOTRACONTEURLITE_API struct robotraconteurlite_connection* robotraconteurlite_connection_cast(
    struct robotraconteurlite_connection_object* connection_obj);

ROBOTRACONTEURLITE_API struct robotraconteurlite_connection* robotraconteurlite_connection_first(
    struct robotraconteurlite_connection_object* connection_obj);

ROBOTRACONTEURLITE_API struct robotraconteurlite_connection* robotraconteurlite_connection_first_2(
    struct robotraconteurlite_connection_object* connection_obj, robotraconteurlite_u32 transport_type);

ROBOTRACONTEURLITE_API struct robotraconteurlite_connection* robotraconteurlite_connection_next(
    struct robotraconteurlite_connection_object* connection_obj);

ROBOTRACONTEURLITE_API struct robotraconteurlite_connection* robotraconteurlite_connection_next_2(
    struct robotraconteurlite_connection_object* connection_obj, robotraconteurlite_u32 transport_type);

ROBOTRACONTEURLITE_API struct robotraconteurlite_connection_acceptor* robotraconteurlite_connection_acceptor_cast(
    struct robotraconteurlite_connection_object* connection_obj);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_connection_impl_prepare_wait(struct robotraconteurlite_connection* connection);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_connection_acceptor_impl_prepare_wait(struct robotraconteurlite_connection_acceptor* acceptor,
                                                         struct robotraconteurlite_connection_object* connection_head);

#ifdef ROBOTRACONTEURLITE_HAVE_FUNCPTR
ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connections_communicate(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connections_communicate_recv(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connections_communicate_send(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connections_communicate_process_control(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connections_prepare_wait(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connections_close(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connections_communicate_drain(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now);

ROBOTRACONTEURLITE_API robotraconteurlite_size_t robotraconteurlite_connections_communicate_drain_pending(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now);

ROBOTRACONTEURLITE_API robotraconteurlite_size_t robotraconteurlite_connections_communicate_available(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now);

#endif

#ifdef __cplusplus
}
#endif

#endif /*ROBOTRACONTEURLITE_CONNECTION_H*/
