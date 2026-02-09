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

#include "robotraconteurlite/tcp_transport.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* memcmp is used between robotraconteurlite_byte* buffer and const char literals in this function
   The char null terminator is not used for this comparison, and the comparison between
   char and robotraconteurlite_byte is safe. Disabling misra warnings for this file.
*/

#define FLAGS_CHECK_ALL ROBOTRACONTEURLITE_FLAGS_CHECK_ALL
#define FLAGS_CHECK ROBOTRACONTEURLITE_FLAGS_CHECK
#define FLAGS_SET ROBOTRACONTEURLITE_FLAGS_SET
#define FLAGS_CLEAR ROBOTRACONTEURLITE_FLAGS_CLEAR

#define FAILED ROBOTRACONTEURLITE_FAILED
#define RETRY ROBOTRACONTEURLITE_RETRY

#ifdef ROBOTRACONTEURLITE_HAVE_FUNCPTR
/* connections ops definition */
static robotraconteurlite_status robotraconteurlite_tcp_connection_communicate_recv_op(
    struct robotraconteurlite_connection_object* connection,
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now)
{
    struct robotraconteurlite_connection* c = robotraconteurlite_connection_cast(connection);
    if (c == NULL)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    return robotraconteurlite_tcp_connection_communicate_recv(c, now);
}

static robotraconteurlite_status robotraconteurlite_tcp_connection_communicate_send_op(
    struct robotraconteurlite_connection_object* connection,
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now)
{
    struct robotraconteurlite_connection* c = robotraconteurlite_connection_cast(connection);
    if (c == NULL)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    return robotraconteurlite_tcp_connection_communicate_send(c, now);
}

static robotraconteurlite_status robotraconteurlite_tcp_connection_communicate_process_control_op(
    struct robotraconteurlite_connection_object* connection,
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now)
{
    struct robotraconteurlite_connection* c = robotraconteurlite_connection_cast(connection);
    if (c == NULL)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    return robotraconteurlite_tcp_connection_communicate_process_control(c, now);
}

static robotraconteurlite_status robotraconteurlite_tcp_connection_prepare_wait_op(
    struct robotraconteurlite_connection_object* connection,
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now)
{
    struct robotraconteurlite_connection* c = robotraconteurlite_connection_cast(connection);
    if (c == NULL)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    return robotraconteurlite_tcp_connection_prepare_wait(c);
}

static robotraconteurlite_status robotraconteurlite_tcp_connection_close_op(
    struct robotraconteurlite_connection_object* connection,
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now)
{
    struct robotraconteurlite_connection* c = robotraconteurlite_connection_cast(connection);
    if (c == NULL)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    robotraconteurlite_tcp_connection_close(c);
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static const struct robotraconteurlite_connection_object_ops robotraconteurlite_tcp_connection_object_ops = {
    .communicate_recv = robotraconteurlite_tcp_connection_communicate_recv_op,
    .communicate_send = robotraconteurlite_tcp_connection_communicate_send_op,
    .communicate_process_control = robotraconteurlite_tcp_connection_communicate_process_control_op,
    .prepare_wait = robotraconteurlite_tcp_connection_prepare_wait_op,
    .connection_close = robotraconteurlite_tcp_connection_close_op};

/* acceptor ops definition */
static robotraconteurlite_status robotraconteurlite_tcp_acceptor_communicate_process_control_op(
    struct robotraconteurlite_connection_object* connection,
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now)
{
    struct robotraconteurlite_connection_acceptor* a = robotraconteurlite_connection_acceptor_cast(connection);
    if (a == NULL)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    return robotraconteurlite_tcp_acceptor_communicate(a, connections_head, now);
}

static robotraconteurlite_status robotraconteurlite_tcp_acceptor_prepare_wait_op(
    struct robotraconteurlite_connection_object* connection,
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now)
{
    struct robotraconteurlite_connection_acceptor* a = robotraconteurlite_connection_acceptor_cast(connection);
    if (a == NULL)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    return robotraconteurlite_tcp_acceptor_prepare_wait(a, connections_head);
}

static robotraconteurlite_status robotraconteurlite_tcp_acceptor_close_op(
    struct robotraconteurlite_connection_object* connection,
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now)
{
    struct robotraconteurlite_connection_acceptor* a = robotraconteurlite_connection_acceptor_cast(connection);
    if (a == NULL)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    return robotraconteurlite_tcp_acceptor_close(a);
}

static const struct robotraconteurlite_connection_object_ops robotraconteurlite_tcp_acceptor_object_ops = {
    .communicate_recv = NULL,
    .communicate_send = NULL,
    .communicate_process_control = robotraconteurlite_tcp_acceptor_communicate_process_control_op,
    .prepare_wait = robotraconteurlite_tcp_acceptor_prepare_wait_op,
    .connection_close = robotraconteurlite_tcp_acceptor_close_op};

#endif

void robotraconteurlite_tcp_acceptor_construct(struct robotraconteurlite_connection_acceptor* acceptor,
                                               struct robotraconteurlite_connection_object* connections_head)
{
    (void)memset(acceptor, 0, sizeof(struct robotraconteurlite_connection_acceptor));
    acceptor->head.connection_object_type = ROBOTRACONTEURLITE_CONNECTION_OBJECT_TYPE_ACCEPTOR;
    acceptor->head.transport_type = ROBOTRACONTEURLITE_TCP_TRANSPORT;

#ifdef ROBOTRACONTEURLITE_HAVE_FUNCPTR
    acceptor->head.ops = &robotraconteurlite_tcp_acceptor_object_ops;
#endif

    if (connections_head != NULL)
    {
        robotraconteurlite_connection_list_append(connections_head, &acceptor->head);
    }
}

robotraconteurlite_status robotraconteurlite_tcp_acceptor_listen(
    struct robotraconteurlite_connection_acceptor* acceptor, const struct sockaddr_storage* serv_addr, int backlog)
{

    int last_errno = -1;
    robotraconteurlite_status rv = -1;

    assert(sizeof(struct robotraconteurlite_tcp_transport_storage) <=
           sizeof(struct robotraconteurlite_transport_storage));

    acceptor->head.transport_type = ROBOTRACONTEURLITE_TCP_TRANSPORT;
    acceptor->acceptor_state = 0;

    rv = robotraconteurlite_tcp_socket_begin_server(serv_addr, backlog, &acceptor->head.sock, &last_errno);
    return rv;
}

robotraconteurlite_status robotraconteurlite_tcp_acceptor_close(struct robotraconteurlite_connection_acceptor* acceptor)
{
    /* cppcheck-suppress misra-c2012-10.4 */
    if (acceptor->head.sock.sock != 0)
    {
        (void)robotraconteurlite_tcp_server_socket_close(&acceptor->head.sock);
        FLAGS_CLEAR(acceptor->head.sock.flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_ACTIVE);
    }

    return 0;
}

robotraconteurlite_status robotraconteurlite_tcp_acceptor_communicate(
    struct robotraconteurlite_connection_acceptor* acceptor,
    struct robotraconteurlite_connection_object* connection_head, robotraconteurlite_timespec now)
{
    /* Find a connection that is idle */
    struct robotraconteurlite_connection* c = NULL;
    int errno_out = -1;
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_connection_socket sock;
    memset(&sock, 0, sizeof(struct robotraconteurlite_connection_socket));

    c = robotraconteurlite_connection_find_idle(connection_head);

    if (c == NULL)
    {
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    /* Accept connection */
    rv = robotraconteurlite_tcp_socket_accept(&acceptor->head.sock, &sock, &errno_out);
    if (FAILED(rv))
    {
        if (RETRY(rv))
        {
            return ROBOTRACONTEURLITE_ERROR_SUCCESS;
        }
        return rv;
    }

    rv = robotraconteurlite_connection_impl_accept2(c, now, ROBOTRACONTEURLITE_TCP_TRANSPORT, &sock);
    if (FAILED(rv))
    {
        return rv;
    }

#ifdef ROBOTRACONTEURLITE_HAVE_FUNCPTR
    c->head.ops = &robotraconteurlite_tcp_connection_object_ops;
#endif

    return rv;
}

static robotraconteurlite_size_t robotraconteurlite_tcp_connection_recv_websocket_header_size(
    robotraconteurlite_u8 byte_two)
{
    robotraconteurlite_size_t websocket_header_len = 2;
    robotraconteurlite_u8 len1 = byte_two & 0x7FU;
    robotraconteurlite_u8 masked = byte_two & 0x80U;
    if (len1 == 126U)
    {
        websocket_header_len += 2U;
    }
    if (len1 == 127U)
    {
        websocket_header_len += 8U;
    }

    if (masked != 0U)
    {
        websocket_header_len += 4U;
    }

    return websocket_header_len;
}

static struct robotraconteurlite_tcp_transport_storage* get_storage(struct robotraconteurlite_connection* connection)
{
    /* TODO: fix misra warning */
    /* cppcheck-suppress misra-c2012-11.3 */
    return (struct robotraconteurlite_tcp_transport_storage*)&connection->transport_storage;
}

static robotraconteurlite_status robotraconteurlite_tcp_connection_buffer_recv_websocket(
    struct robotraconteurlite_connection* connection, robotraconteurlite_size_t len)
{
    struct robotraconteurlite_tcp_transport_storage* storage = get_storage(connection);
    if ((storage->recv_websocket_frame_pos == storage->recv_websocket_frame_len) &&
        (FLAGS_CHECK(storage->tcp_transport_state, ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_RECV_WEBSOCKET_IN_FRAME)))
    {
        /* reset for next websocket frame */
        storage->recv_websocket_frame_pos = 0;
        storage->recv_websocket_frame_len = 0;
        storage->recv_websocket_header_pos = 0;
        FLAGS_CLEAR(storage->tcp_transport_state, (ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_RECV_WEBSOCKET_IN_FRAME));
    }

    if (!FLAGS_CHECK(storage->tcp_transport_state, ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_RECV_WEBSOCKET_IN_FRAME))
    {
        robotraconteurlite_size_t websocket_header_len = 2;
        robotraconteurlite_u8 len1 = 0;
        robotraconteurlite_u8 masked = 0;

        if (storage->recv_websocket_header_pos < 2U)
        {
            int last_errno = -1;
            robotraconteurlite_status rv = robotraconteurlite_tcp_socket_recv_nonblocking(
                &connection->head.sock, storage->recv_websocket_header_buffer, &storage->recv_websocket_header_pos,
                2U - storage->recv_websocket_header_pos, &last_errno);
            if (FAILED(rv))
            {
                FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR);
                return rv;
            }

            if (storage->recv_websocket_header_pos < 2U)
            {
                return ROBOTRACONTEURLITE_ERROR_SUCCESS;
            }
        }

        websocket_header_len =
            robotraconteurlite_tcp_connection_recv_websocket_header_size(storage->recv_websocket_header_buffer[1]);

        if (storage->recv_websocket_header_pos < websocket_header_len)
        {
            int last_errno = -1;
            robotraconteurlite_status rv = robotraconteurlite_tcp_socket_recv_nonblocking(
                &connection->head.sock, storage->recv_websocket_header_buffer, &storage->recv_websocket_header_pos,
                websocket_header_len - storage->recv_websocket_header_pos, &last_errno);
            if (FAILED(rv))
            {
                FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR);
                return rv;
            }

            if (storage->recv_websocket_header_pos < websocket_header_len)
            {
                return ROBOTRACONTEURLITE_ERROR_SUCCESS;
            }
        }

        len1 = storage->recv_websocket_header_buffer[1] & 0x7FU;
        masked = storage->recv_websocket_header_buffer[1] & 0x80U;
        if (len1 == 126U)
        {
            robotraconteurlite_u16 frame_len_be = 0;
            (void)memcpy((robotraconteurlite_byte*)&frame_len_be, &storage->recv_websocket_header_buffer[2], 2);
            storage->recv_websocket_frame_len = robotraconteurlite_ntohs(frame_len_be);
        }
        else if (len1 == 127U)
        {
            robotraconteurlite_u64 frame_len_be = 0;
            (void)memcpy((robotraconteurlite_byte*)&frame_len_be, &storage->recv_websocket_header_buffer[2], 8);
            storage->recv_websocket_frame_len = robotraconteurlite_be64toh(frame_len_be);
        }
        else
        {
            storage->recv_websocket_frame_len = len1;
        }

        if (masked != 0U)
        {
            (void)memcpy(storage->recv_websocket_mask,
                         &storage->recv_websocket_header_buffer[websocket_header_len - 4U], 4U);
            FLAGS_SET(storage->tcp_transport_state, ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_RECV_WEBSOCKET_ENABLE_MASK);
        }
        else
        {
            FLAGS_CLEAR(storage->tcp_transport_state,
                        ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_RECV_WEBSOCKET_ENABLE_MASK);
        }

        FLAGS_SET(storage->tcp_transport_state, ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_RECV_WEBSOCKET_IN_FRAME);
    }
    if (FLAGS_CHECK(storage->tcp_transport_state, ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_RECV_WEBSOCKET_IN_FRAME))
    {
        robotraconteurlite_u32 recv_len =
            storage->recv_websocket_frame_len + (connection->recv_buffer_pos - storage->recv_websocket_frame_pos);
        int last_errno = -1;
        robotraconteurlite_u32 prev_recv_buffer_pos = 0;
        robotraconteurlite_status rv = -1;
        robotraconteurlite_size_t n = 0;
        robotraconteurlite_size_t i = 0;
        if (recv_len > len)
        {
            recv_len = (robotraconteurlite_u32)len;
        }

        prev_recv_buffer_pos = connection->recv_buffer_pos;
        rv = robotraconteurlite_tcp_socket_recv_nonblocking(&connection->head.sock, connection->recv_buffer,
                                                            &connection->recv_buffer_pos, recv_len, &last_errno);
        if (FAILED(rv))
        {
            FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR);
            return rv;
        }
        n = connection->recv_buffer_pos - prev_recv_buffer_pos;
        for (i = 0; i < n; i++)
        {
            connection->recv_buffer[prev_recv_buffer_pos + i] ^=
                storage->recv_websocket_mask[(storage->recv_websocket_frame_pos + i) % 4U];
        }
        storage->recv_websocket_frame_pos += n;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_tcp_connection_buffer_recv(
    struct robotraconteurlite_connection* connection, robotraconteurlite_size_t len)
{
    int last_errno = -1;
    struct robotraconteurlite_tcp_transport_storage* storage = get_storage(connection);
    if (FLAGS_CHECK(storage->tcp_transport_state, ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_IS_WEBSOCKET))
    {
        return robotraconteurlite_tcp_connection_buffer_recv_websocket(connection, len);
    }

    return robotraconteurlite_tcp_socket_recv_nonblocking(&connection->head.sock, connection->recv_buffer,
                                                          &connection->recv_buffer_pos, len, &last_errno);
}

robotraconteurlite_status robotraconteurlite_tcp_connection_communicate_recv(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now)
{
    robotraconteurlite_size_t recv_op_len = 0;
    robotraconteurlite_status rv = -1;

    rv = robotraconteurlite_connection_impl_communicate_recv1(connection, now, &recv_op_len);

    if (FAILED(rv) || (recv_op_len == 0U))
    {
        return rv;
    }

    rv = robotraconteurlite_tcp_connection_buffer_recv(connection, recv_op_len);

    return robotraconteurlite_connection_impl_communicate_recv2(connection, now, rv);
}

static robotraconteurlite_status robotraconteurlite_tcp_websocket_random_mask(
    struct robotraconteurlite_connection* connection, robotraconteurlite_u8 mask[4])
{
    /* TODO: use better random source */
    robotraconteurlite_u32 random_val = rand();
    ROBOTRACONTEURLITE_UNUSED(connection);
    mask[0] = (random_val >> 24) & 0xFFU;
    mask[1] = (random_val >> 16) & 0xFFU;
    mask[2] = (random_val >> 8) & 0xFFU;
    mask[3] = random_val & 0xFFU;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_tcp_connection_buffer_send_websocket(
    struct robotraconteurlite_connection* connection, robotraconteurlite_size_t len)
{
    struct robotraconteurlite_tcp_transport_storage* storage = get_storage(connection);
    robotraconteurlite_size_t send_len = len;
    int last_errno = -1;
    robotraconteurlite_status rv = -1;
    if ((storage->send_websocket_frame_len == 0U) && (send_len > 0U))
    {
        /* Prepare new frame */
        if (send_len >= ROBOTRACONTEURLITE_UINT16_MAX)
        {
            send_len = ROBOTRACONTEURLITE_UINT16_MAX;
        }

        storage->send_websocket_header_len = 2;
        storage->send_websocket_header_buffer[0] = 0x80U | ROBOTRACONTEURLITE_TCP_TRANSPORT_WEBSOCKET_FLAGS_BINARY;
        if (len > 125U)
        {
            robotraconteurlite_u16 len_be = robotraconteurlite_htons((robotraconteurlite_u16)len);
            storage->send_websocket_header_len += 2U;
            (void)memcpy(&storage->send_websocket_header_buffer[2], (robotraconteurlite_u8*)&len_be, 2);
            storage->send_websocket_header_buffer[1] = 126;
        }
        else
        {
            storage->send_websocket_header_buffer[1] = (robotraconteurlite_u8)len;
        }

        /* Create mask if client */
        if (!FLAGS_CHECK(connection->config_flags, ROBOTRACONTEURLITE_CONFIG_FLAGS_ISSERVER))
        {
            storage->send_websocket_header_len += 4U;
            FLAGS_SET(storage->tcp_transport_state, ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_SEND_WEBSOCKET_ENABLE_MASK);
            if (robotraconteurlite_tcp_websocket_random_mask(connection, storage->send_websocket_mask) != 0)
            {
                return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
            }
            storage->send_websocket_header_buffer[1] |= 0x80U;
            (void)memcpy(&storage->send_websocket_header_buffer[storage->send_websocket_header_len - 4U],
                         storage->send_websocket_mask, 4);
        }

        storage->send_websocket_frame_len = send_len;
        storage->send_websocket_frame_buffer_end = send_len + connection->send_buffer_pos;
        storage->send_websocket_frame_buffer_pos = connection->send_buffer_pos;

        /* Apply mask to send buffer */
        if (FLAGS_CHECK(storage->tcp_transport_state,
                        ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_SEND_WEBSOCKET_ENABLE_MASK))
        {
            robotraconteurlite_size_t i = 0;
            for (i = 0; i < send_len; i++)
            {
                connection->send_buffer[connection->send_buffer_pos + i] ^= storage->send_websocket_mask[i % 4U];
            }
        }
    }

    /* Send header nonblocking */
    if ((storage->send_websocket_header_len > 0U) &&
        (storage->send_websocket_header_pos < storage->send_websocket_header_len))
    {
        int last_errno = -1;
        robotraconteurlite_status rv = robotraconteurlite_tcp_socket_send_nonblocking(
            &connection->head.sock, storage->send_websocket_header_buffer, &storage->send_websocket_header_pos,
            storage->send_websocket_header_len - storage->send_websocket_header_pos, &last_errno);
        if (FAILED(rv))
        {
            return rv;
        }
        if (storage->send_websocket_header_pos < storage->send_websocket_header_len)
        {
            return ROBOTRACONTEURLITE_ERROR_SUCCESS;
        }
    }

    /* Send data nonblocking */
    if (len < storage->send_websocket_frame_len)
    {
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    rv = robotraconteurlite_tcp_socket_send_nonblocking(
        &connection->head.sock, connection->send_buffer, &storage->send_websocket_frame_buffer_pos,
        storage->send_websocket_frame_buffer_end - storage->send_websocket_frame_buffer_pos, &last_errno);

    if (FAILED(rv))
    {
        return rv;
    }

    if (storage->send_websocket_frame_buffer_pos < storage->send_websocket_frame_buffer_end)
    {
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    /* Increment buffer position */
    connection->send_buffer_pos = storage->send_websocket_frame_buffer_end;

    /* Reset frame */
    storage->send_websocket_frame_len = 0;
    storage->send_websocket_header_len = 0;
    storage->send_websocket_header_pos = 0;
    FLAGS_CLEAR(storage->tcp_transport_state, ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_SEND_WEBSOCKET_ENABLE_MASK);

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_tcp_connection_buffer_send(
    struct robotraconteurlite_connection* connection, robotraconteurlite_size_t len)
{
    int last_errno = -1;
    struct robotraconteurlite_tcp_transport_storage* storage = get_storage(connection);
    if (FLAGS_CHECK(storage->tcp_transport_state, ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_IS_WEBSOCKET))
    {
        return robotraconteurlite_tcp_connection_buffer_send_websocket(connection, len);
    }

    return robotraconteurlite_tcp_socket_send_nonblocking(&connection->head.sock, connection->send_buffer,
                                                          &connection->send_buffer_pos, len, &last_errno);
}

robotraconteurlite_status robotraconteurlite_tcp_connection_communicate_send(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now)
{
    robotraconteurlite_size_t send_op_len = 0;
    robotraconteurlite_status rv = -1;

    rv = robotraconteurlite_connection_impl_communicate_send1(connection, now, &send_op_len);
    if (FAILED(rv) || (send_op_len == 0U))
    {
        return rv;
    }

    rv = robotraconteurlite_tcp_connection_buffer_send(connection, send_op_len);

    return robotraconteurlite_connection_impl_communicate_send2(connection, now, rv);
}

static robotraconteurlite_status robotraconteurlite_tcp_connection_handshake_http_handshake_find_next_line(
    const char* recv_data, robotraconteurlite_size_t recv_data_len, robotraconteurlite_size_t* i_end,
    robotraconteurlite_size_t* i_next)
{
    robotraconteurlite_size_t i = *i_next;
    while ((i < recv_data_len) && (recv_data[i] != '\n') && (recv_data[i] != '\r'))
    {
        i++;
    }

    if (i >= recv_data_len)
    {
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }
    *i_end = i;
    while ((i < recv_data_len) && ((recv_data[i] == '\n') || (recv_data[i] == '\r')))
    {
        i++;
    }
    *i_next = i;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

#define STRCONST_HTTP_SEC_WEBSOCKET_KEY_LEN 18U
#define STRCONST_HTTP_SEC_WEBSOCKET_KEY "Sec-WebSocket-Key:"
#define STRCONST_HTTP_KEY_UUID_LEN 36U
#define STRCONST_HTTP_KEY_UUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
#define WEBSOCKET_KEY_BASE64_LEN 24U
#define STRCONST_HTTP_RESPONSE_1                                                                                       \
    "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Protocol: "        \
    "robotraconteur.robotraconteur.com\r\nSec-WebSocket-Version: 13\r\nSec-WebSocket-Accept: "
#define STRCONST_HTTP_RESPONSE_1_LEN 183U
#define STRCONST_HTTP_RESPONSE_2 "\r\n\r\n"
#define STRCONST_HTTP_RESPONSE_2_LEN 4U

static robotraconteurlite_status robotraconteurlite_tcp_connection_handshake_http_handshake(
    struct robotraconteurlite_connection* connection)
{
    const char* recv_data = (const char*)connection->recv_buffer;
    robotraconteurlite_size_t recv_data_len = connection->recv_buffer_pos;

    /* Skip first line of receive data */
    robotraconteurlite_size_t i_end = 0;
    robotraconteurlite_size_t i_next = 0;
    robotraconteurlite_size_t i_sec_websocket = 0;
    robotraconteurlite_size_t i_sec_websocket_end = 0;
    struct robotraconteurlite_tcp_sha1_storage sha1_storage;
    robotraconteurlite_byte sha1_input_bytes[60];
    char sha1_base64[28];
    robotraconteurlite_size_t sha1_base64_len = sizeof(sha1_base64);
    int last_errno = -1;
    robotraconteurlite_status rv = robotraconteurlite_tcp_connection_handshake_http_handshake_find_next_line(
        recv_data, recv_data_len, &i_end, &i_next);
    if (FAILED(rv))
    {
        return rv;
    }

    /* Find the websocket sec key line */
    while (i_next < recv_data_len)
    {
        if (recv_data[i_next] == 'S')
        {
            if (((i_next + STRCONST_HTTP_SEC_WEBSOCKET_KEY_LEN) < recv_data_len) &&
                /* cppcheck-suppress [misra-c2012-21.14, misra-c2012-21.16] */
                (memcmp(&recv_data[i_next], STRCONST_HTTP_SEC_WEBSOCKET_KEY, STRCONST_HTTP_SEC_WEBSOCKET_KEY_LEN) == 0))
            {
                i_sec_websocket = i_next + STRCONST_HTTP_SEC_WEBSOCKET_KEY_LEN;
                break;
            }
        }
        rv = robotraconteurlite_tcp_connection_handshake_http_handshake_find_next_line(recv_data, recv_data_len, &i_end,
                                                                                       &i_next);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (i_sec_websocket == 0U)
    {
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    /* Find end of sec key line */
    rv = robotraconteurlite_tcp_connection_handshake_http_handshake_find_next_line(recv_data, recv_data_len, &i_end,
                                                                                   &i_next);
    if (FAILED(rv))
    {
        return rv;
    }

    /* Skip any whitespace */
    while ((i_sec_websocket < i_end) && ((recv_data[i_sec_websocket] == ' ') || (recv_data[i_sec_websocket] == '\t')))
    {
        i_sec_websocket++;
    }

    /* Find end of sec key */
    i_sec_websocket_end = i_sec_websocket;
    while ((i_sec_websocket_end < i_end) && (recv_data[i_sec_websocket_end] != ' ') &&
           (recv_data[i_sec_websocket_end] != '\t'))
    {
        i_sec_websocket_end++;
    }

    /* Check the client key length */
    if ((i_sec_websocket_end - i_sec_websocket) != WEBSOCKET_KEY_BASE64_LEN)
    {
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    /* Generate the return key */

    /* NOLINTBEGIN(bugprone-not-null-terminated-result) */
    /* Copy the client key to the buffer */
    (void)memcpy(sha1_input_bytes, &recv_data[i_sec_websocket], WEBSOCKET_KEY_BASE64_LEN);
    /* Copy the UUID to the buffer */
    (void)memcpy(&sha1_input_bytes[WEBSOCKET_KEY_BASE64_LEN], STRCONST_HTTP_KEY_UUID, STRCONST_HTTP_KEY_UUID_LEN);
    /* NOLINTEND(bugprone-not-null-terminated-result) */

    /* Compute the SHA1 hash */
    if (robotraconteurlite_tcp_sha1(sha1_input_bytes, sizeof(sha1_input_bytes), &sha1_storage) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    /* Convert the SHA1 hash to base64 */
    if (robotraconteurlite_tcp_base64_encode(sha1_storage.sha1_bytes, sizeof(sha1_storage.sha1_bytes), sha1_base64,
                                             &sha1_base64_len) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    assert(sha1_base64_len == sizeof(sha1_base64));

    connection->recv_buffer_pos = 0;
    /* Send the response using send_buffer */
    connection->send_buffer_pos = 0;

    /* Send the first part of the response */
    /* NOLINTBEGIN(bugprone-not-null-terminated-result) */
    (void)memcpy(connection->send_buffer, STRCONST_HTTP_RESPONSE_1, STRCONST_HTTP_RESPONSE_1_LEN);
    /* Send the accept hash */
    (void)memcpy(&connection->send_buffer[STRCONST_HTTP_RESPONSE_1_LEN], sha1_base64, sha1_base64_len);
    /* Send the second part of the response */
    (void)memcpy(&connection->send_buffer[STRCONST_HTTP_RESPONSE_1_LEN + sha1_base64_len], STRCONST_HTTP_RESPONSE_2,
                 STRCONST_HTTP_RESPONSE_2_LEN);
    /* NOLINTEND(bugprone-not-null-terminated-result) */

    /* Set the send message length */
    connection->send_message_len = STRCONST_HTTP_RESPONSE_1_LEN + sha1_base64_len + STRCONST_HTTP_RESPONSE_2_LEN;
    assert(connection->send_message_len <= connection->send_buffer_len);

    /* Send the response */
    rv = robotraconteurlite_tcp_socket_send_nonblocking(
        &connection->head.sock, connection->send_buffer, &connection->send_buffer_pos,
        connection->send_message_len - connection->send_buffer_pos, &last_errno);
    return rv;
}

static robotraconteurlite_status robotraconteurlite_tcp_connection_handshake_http_recv_header(
    struct robotraconteurlite_connection* connection)
{
    int newline_found = 0;
    robotraconteurlite_status rv = -1;
    /* Receive byte by byte until two newlines character */
    while (connection->recv_buffer_pos < connection->recv_buffer_len)
    {
        int last_errno = -1;
        robotraconteurlite_size_t last_recv_buffer_pos = connection->recv_buffer_pos;
        rv = robotraconteurlite_tcp_socket_recv_nonblocking(&connection->head.sock, connection->recv_buffer,
                                                            &connection->recv_buffer_pos,
                                                            connection->recv_buffer_pos + 1U, &last_errno);
        if (FAILED(rv))
        {
            FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR);
            return rv;
        }
        if (connection->recv_buffer_pos == last_recv_buffer_pos)
        {
            return ROBOTRACONTEURLITE_ERROR_CONSUMED;
        }
        /* Check for ending of \r\n\r\n or \n\n or \r\r or \n\r\n\r */
        if (connection->recv_buffer_pos >= 4U)
        {
            robotraconteurlite_byte* end_minus_4 = &connection->recv_buffer[connection->recv_buffer_pos - 4U];
            /* cppcheck-suppress [misra-c2012-21.14, misra-c2012-21.16] */
            if ((memcmp(end_minus_4, "\r\n\r\n", 4) == 0) || (memcmp(end_minus_4, "\n\r\n\r", 4) == 0))
            {
                newline_found = 1;
            }
        }

        if (connection->recv_buffer_pos >= 2U)
        {
            robotraconteurlite_byte* end_minus_2 = &connection->recv_buffer[connection->recv_buffer_pos - 2U];
            /* cppcheck-suppress [misra-c2012-21.14, misra-c2012-21.16] */
            if ((memcmp(end_minus_2, "\n\n", 2) == 0) || (memcmp(end_minus_2, "\r\r", 2) == 0))
            {
                newline_found = 1;
            }
        }

        if (newline_found != 0)
        {
            break;
        }
    }

    /* Check that two newlines were found */
    if (!newline_found)
    {
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_tcp_connection_handshake_server(
    struct robotraconteurlite_connection* connection)
{
    const char rrac_magic[4] = {'R', 'R', 'A', 'C'};

    struct robotraconteurlite_tcp_transport_storage* storage = get_storage(connection);
    robotraconteurlite_status rv = -1;

    if (!FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTING))
    {
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    if (connection->send_message_len > 0U)
    {
        int last_errno = -1;
        robotraconteurlite_status rv = robotraconteurlite_tcp_socket_send_nonblocking(
            &connection->head.sock, connection->send_buffer, &connection->send_buffer_pos,
            connection->send_message_len - connection->send_buffer_pos, &last_errno);
        if (FAILED(rv))
        {
            if (RETRY(rv))
            {
                return ROBOTRACONTEURLITE_ERROR_RETRY;
            }
            FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR);
            return rv;
        }
        if (connection->send_buffer_pos < connection->send_message_len)
        {
            return ROBOTRACONTEURLITE_ERROR_CONSUMED;
        }

        /* We are now connected using websocket! */
        FLAGS_SET(storage->tcp_transport_state, ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_IS_WEBSOCKET);
        FLAGS_SET(connection->connection_state,
                  ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTED | ROBOTRACONTEURLITE_STATUS_FLAGS_RECEIVE_REQUESTED);
        FLAGS_CLEAR(connection->connection_state,
                    (ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTING | ROBOTRACONTEURLITE_STATUS_FLAGS_BLOCK_SEND |
                     ROBOTRACONTEURLITE_STATUS_FLAGS_SENDING | ROBOTRACONTEURLITE_STATUS_FLAGS_SEND_REQUESTED));
        /* Clear send buffer */
        connection->send_buffer_pos = 0;
        connection->send_message_len = 0;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    if (connection->recv_buffer_pos < 4U)
    {
        int last_errno = -1;
        robotraconteurlite_status rv = robotraconteurlite_tcp_socket_recv_nonblocking(
            &connection->head.sock, connection->recv_buffer, &connection->recv_buffer_pos, 4, &last_errno);
        if (FAILED(rv))
        {
            FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR);
            return rv;
        }
        if (connection->recv_buffer_pos < 4U)
        {
            return ROBOTRACONTEURLITE_ERROR_CONSUMED;
        }
    }

    /* Check if first 4 bytes are RRAC */
    if (memcmp(connection->recv_buffer, rrac_magic, sizeof(rrac_magic)) == 0)
    {
        /* We are connected with a non-websocket client */
        FLAGS_CLEAR(storage->tcp_transport_state, ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_IS_WEBSOCKET);
        FLAGS_SET(connection->connection_state,
                  ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTED | ROBOTRACONTEURLITE_STATUS_FLAGS_RECEIVE_REQUESTED);
        FLAGS_CLEAR(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTING);
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    /* Check if first 4 bytes are GET */
    /* cppcheck-suppress [misra-c2012-21.14, misra-c2012-21.16] */
    if (memcmp(connection->recv_buffer, "GET ", 4) == 0)
    {
        /* We are connected with a websocket client, begin handshake */
        FLAGS_SET(storage->tcp_transport_state, ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_IS_WEBSOCKET);
    }
    else
    {
        /* Unknown header */
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    rv = robotraconteurlite_tcp_connection_handshake_http_recv_header(connection);
    if (FAILED(rv))
    {
        if (RETRY(rv))
        {
            return ROBOTRACONTEURLITE_ERROR_RETRY;
        }
        FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR);
        return rv;
    }

    /* Handshake */
    rv = robotraconteurlite_tcp_connection_handshake_http_handshake(connection);
    if (FAILED(rv))
    {
        FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR);
        return rv;
    }

    FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_SENDING);

    /* Check if the response was sent */
    if ((connection->send_message_len > 0U) && (connection->send_buffer_pos == connection->send_message_len))
    {
        /* We are connected using websockets! */
        FLAGS_SET(connection->connection_state,
                  ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTED | ROBOTRACONTEURLITE_STATUS_FLAGS_RECEIVE_REQUESTED);
        FLAGS_CLEAR(connection->connection_state,
                    (ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTING | ROBOTRACONTEURLITE_STATUS_FLAGS_BLOCK_SEND |
                     ROBOTRACONTEURLITE_STATUS_FLAGS_SENDING | ROBOTRACONTEURLITE_STATUS_FLAGS_SEND_REQUESTED));
        FLAGS_SET(storage->tcp_transport_state, ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_IS_WEBSOCKET);
        /* Clear send buffer */
        connection->send_buffer_pos = 0;
        connection->send_message_len = 0;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    return ROBOTRACONTEURLITE_ERROR_CONSUMED;
}

static robotraconteurlite_status robotraconteurlite_tcp_connection_handshake_client(
    struct robotraconteurlite_connection* connection)
{
    struct robotraconteurlite_tcp_transport_storage* storage = get_storage(connection);
    robotraconteurlite_status rv = -1;

    if (!FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTING))
    {
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    if (!FLAGS_CHECK(storage->tcp_transport_state, ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_IS_WEBSOCKET))
    {
        /* No handshake for raw sockets */
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    /* Send data left in buffer */
    if (connection->send_message_len > 0U)
    {
        int last_errno = -1;
        robotraconteurlite_status rv = robotraconteurlite_tcp_socket_send_nonblocking(
            &connection->head.sock, connection->send_buffer, &connection->send_buffer_pos,
            connection->send_message_len - connection->send_buffer_pos, &last_errno);
        if (FAILED(rv))
        {
            FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR);
            return rv;
        }
        if (connection->send_buffer_pos < connection->send_message_len)
        {
            FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_SENDING);
            return ROBOTRACONTEURLITE_ERROR_CONSUMED;
        }
    }

    FLAGS_CLEAR(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_SENDING);

    rv = robotraconteurlite_tcp_connection_handshake_http_recv_header(connection);
    if (FAILED(rv))
    {
        if ((RETRY(rv)) || (rv == ROBOTRACONTEURLITE_ERROR_CONSUMED))
        {
            return rv;
        }
        FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR);
        return rv;
    }

    /* Make sure we actually upgraded */
    /* cppcheck-suppress [misra-c2012-21.14, misra-c2012-21.16] */
    if (memcmp(connection->recv_buffer, "HTTP/1.1 101", 12) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    /* TODO: We should check more of the HTTP response, but just ignore for now and begin using the connection! */

    /* We are connected using websockets! */
    FLAGS_SET(connection->connection_state,
              ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTED | ROBOTRACONTEURLITE_STATUS_FLAGS_RECEIVE_REQUESTED);
    FLAGS_CLEAR(connection->connection_state,
                (ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTING | ROBOTRACONTEURLITE_STATUS_FLAGS_BLOCK_SEND |
                 ROBOTRACONTEURLITE_STATUS_FLAGS_SENDING | ROBOTRACONTEURLITE_STATUS_FLAGS_SEND_REQUESTED));
    FLAGS_SET(storage->tcp_transport_state, ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_IS_WEBSOCKET);
    FLAGS_CLEAR(storage->tcp_transport_state, ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_IN_HTTP_HEADER);
    /* Clear send buffer */
    connection->send_buffer_pos = 0;
    connection->send_message_len = 0;
    connection->recv_buffer_pos = 0;
    connection->recv_message_len = 0;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_tcp_connection_handshake(struct robotraconteurlite_connection* connection)
{
    if (!FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTING))
    {
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    if (FLAGS_CHECK(connection->config_flags, ROBOTRACONTEURLITE_CONFIG_FLAGS_ISSERVER))
    {
        return robotraconteurlite_tcp_connection_handshake_server(connection);
    }
    else
    {
        return robotraconteurlite_tcp_connection_handshake_client(connection);
    }
}

robotraconteurlite_status robotraconteurlite_tcp_connection_communicate_process_control(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now)
{
    robotraconteurlite_status rv = -1;
    robotraconteurlite_u8 close_requested = 0;

    rv = robotraconteurlite_connection_impl_communicate_process_control(
        connection, now, ROBOTRACONTEURLITE_TCP_TRANSPORT, &close_requested);
    if (FAILED(rv))
    {
        return rv;
    }

    if (close_requested != 0U)
    {
        rv = robotraconteurlite_tcp_socket_close(&connection->head.sock);
        return robotraconteurlite_connection_impl_communicate_after_close_requested(connection, now, rv);
    }

    /* Do handshake */
    return robotraconteurlite_tcp_connection_handshake(connection);
}
robotraconteurlite_status robotraconteurlite_tcp_connection_communicate(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now)
{
    robotraconteurlite_status rv = -1;

    /* Process control */
    rv = robotraconteurlite_tcp_connection_communicate_process_control(connection, now);
    if (FAILED(rv))
    {
        if (rv == ROBOTRACONTEURLITE_ERROR_CONSUMED)
        {
            return ROBOTRACONTEURLITE_ERROR_SUCCESS;
        }
        return rv;
    }

    rv = robotraconteurlite_tcp_connection_communicate_recv(connection, now);
    if (FAILED(rv))
    {
        return rv;
    }

    return robotraconteurlite_tcp_connection_communicate_send(connection, now);
}

robotraconteurlite_status robotraconteurlite_tcp_connections_communicate(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now)
{
    struct robotraconteurlite_connection_object* c = connections_head;
    robotraconteurlite_status rv = -1;
    while (c != NULL)
    {
        if (c->transport_type == ROBOTRACONTEURLITE_TCP_TRANSPORT)
        {
            if (c->connection_object_type == ROBOTRACONTEURLITE_CONNECTION_OBJECT_TYPE_CONNECTION)
            {
                struct robotraconteurlite_connection* c1 = robotraconteurlite_connection_cast(c);
                if (c1 == NULL)
                {
                    return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
                }
                rv = robotraconteurlite_tcp_connection_communicate(c1, now);
                if (FAILED(rv))
                {
                    return rv;
                }
            }

            if (c->connection_object_type == ROBOTRACONTEURLITE_CONNECTION_OBJECT_TYPE_ACCEPTOR)
            {
                struct robotraconteurlite_connection_acceptor* c2 = robotraconteurlite_connection_acceptor_cast(c);
                if (c2 == NULL)
                {
                    return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
                }
                rv = robotraconteurlite_tcp_acceptor_communicate(c2, connections_head, now);
                if (FAILED(rv))
                {
                    return rv;
                }
            }
        }
        c = c->next;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

void robotraconteurlite_tcp_connection_close(struct robotraconteurlite_connection* connection)
{
    if (connection->head.transport_type != ROBOTRACONTEURLITE_TCP_TRANSPORT)
    {
        return;
    }

    /* cppcheck-suppress misra-c2012-10.4 */
    if (connection->head.sock.sock != 0)
    {
        (void)robotraconteurlite_tcp_socket_close(&connection->head.sock);
        connection->head.sock.sock = 0;
    }
}

void robotraconteurlite_tcp_connections_close(struct robotraconteurlite_connection_object* connections_head)
{
    struct robotraconteurlite_connection* c =
        robotraconteurlite_connection_first_2(connections_head, ROBOTRACONTEURLITE_TCP_TRANSPORT);
    while (c != NULL)
    {
        robotraconteurlite_tcp_connection_close(c);
        c = robotraconteurlite_connection_next_2(&c->head, ROBOTRACONTEURLITE_TCP_TRANSPORT);
    }
}

#define STRCONST_HTTP_REQUEST_1 "GET "
#define STRCONST_HTTP_REQUEST_1_LEN 4U
#define STRCONST_HTTP_REQUEST_2 " HTTP/1.1\r\nHost: "
#define STRCONST_HTTP_REQUEST_2_LEN 17U
#define STRCONST_HTTP_REQUEST_3 "\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key: "
#define STRCONST_HTTP_REQUEST_3_LEN 62U
#define STRCONST_HTTP_REQUEST_4                                                                                        \
    "\r\nSec-WebSocket-Protocol: robotraconteur.robotraconteur.com\r\nSec-WebSocket-Version: 13\r\n\r\n"
#define STRCONST_HTTP_REQUEST_4_LEN 90U

static robotraconteurlite_status robotraconteurlite_tcp_connect_service_send_websocket_http_header(
    struct robotraconteurlite_tcp_connect_service_data* connect_data)
{
    robotraconteurlite_byte* send_buf = NULL;
    int i = 0;
    robotraconteurlite_byte websocket_key[STRCONST_HTTP_SEC_WEBSOCKET_KEY_LEN];
    robotraconteurlite_u32 send_len = STRCONST_HTTP_REQUEST_1_LEN + connect_data->service_address->http_path.len +
                                      STRCONST_HTTP_REQUEST_2_LEN + connect_data->service_address->http_host.len +
                                      STRCONST_HTTP_REQUEST_3_LEN + WEBSOCKET_KEY_BASE64_LEN +
                                      STRCONST_HTTP_REQUEST_4_LEN;
    robotraconteurlite_size_t sec_b64_send_len = send_len;
    assert(connect_data->client_out->head.transport_type == ROBOTRACONTEURLITE_TCP_TRANSPORT);
    assert(connect_data->client_out->send_buffer_pos == 0U);

    for (i = 0; i < (int)sizeof(websocket_key); i++)
    {
        websocket_key[i] = (robotraconteurlite_byte)((robotraconteurlite_u32)rand() % 256U);
    }

    if (send_len > connect_data->client_out->send_buffer_len)
    {
        return ROBOTRACONTEURLITE_ERROR_INVALID_PARAMETER;
    }

    if ((connect_data->service_address->http_path.len == 0U) || (connect_data->service_address->http_host.len == 0U))
    {
        return ROBOTRACONTEURLITE_ERROR_INVALID_PARAMETER;
    }

    /* NOLINTBEGIN(bugprone-not-null-terminated-result) */
    send_buf = connect_data->client_out->send_buffer;
    (void)memcpy(send_buf, STRCONST_HTTP_REQUEST_1, STRCONST_HTTP_REQUEST_1_LEN);
    send_buf = &send_buf[STRCONST_HTTP_REQUEST_1_LEN];
    (void)memcpy(send_buf, connect_data->service_address->http_path.data, connect_data->service_address->http_path.len);
    send_buf = &send_buf[connect_data->service_address->http_path.len];
    (void)memcpy(send_buf, STRCONST_HTTP_REQUEST_2, STRCONST_HTTP_REQUEST_2_LEN);
    send_buf = &send_buf[STRCONST_HTTP_REQUEST_2_LEN];
    (void)memcpy(send_buf, connect_data->service_address->http_host.data, connect_data->service_address->http_host.len);
    send_buf = &send_buf[connect_data->service_address->http_host.len];
    (void)memcpy(send_buf, STRCONST_HTTP_REQUEST_3, STRCONST_HTTP_REQUEST_3_LEN);
    send_buf = &send_buf[STRCONST_HTTP_REQUEST_3_LEN];

    if (robotraconteurlite_tcp_base64_encode(websocket_key, sizeof(websocket_key), (char*)send_buf,
                                             &sec_b64_send_len) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    if (sec_b64_send_len != WEBSOCKET_KEY_BASE64_LEN)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    send_buf = &send_buf[WEBSOCKET_KEY_BASE64_LEN];
    (void)memcpy(send_buf, STRCONST_HTTP_REQUEST_4, STRCONST_HTTP_REQUEST_4_LEN);
    send_buf = &send_buf[STRCONST_HTTP_REQUEST_4_LEN];
    /* NOLINTEND(bugprone-not-null-terminated-result) */

    /* cppcheck-suppress misra-c2012-18.4 */
    assert((send_buf - connect_data->client_out->send_buffer) == (int)send_len);
    connect_data->client_out->send_message_len = send_len;
    connect_data->client_out->send_buffer_pos = 0;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_tcp_connect_service(
    struct robotraconteurlite_tcp_connect_service_data* connect_data, robotraconteurlite_timespec now)
{
    struct robotraconteurlite_connection* c = NULL;
    struct robotraconteurlite_connection_socket sock;
    robotraconteurlite_status rv = -1;
    int last_errno = -1;
    (void)memset(&sock, 0, sizeof(struct robotraconteurlite_connection_socket));

    c = robotraconteurlite_connection_find_idle(connect_data->connections_head);

    if (!c)
    {
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    rv = robotraconteurlite_tcp_socket_connect(&connect_data->service_address->socket_addr, &sock, &last_errno);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_connection_impl_connect2(c, now, ROBOTRACONTEURLITE_TCP_TRANSPORT,
                                                     connect_data->service_address, &sock);

    if (FAILED(rv))
    {
        return rv;
    }

#ifdef ROBOTRACONTEURLITE_HAVE_FUNCPTR
    c->head.ops = &robotraconteurlite_tcp_connection_object_ops;
#endif

    connect_data->client_out = c;

    if (FLAGS_CHECK(connect_data->service_address->flags, ROBOTRACONTEURLITE_ADDR_FLAGS_WEBSOCKET))
    {
        struct robotraconteurlite_tcp_transport_storage* storage = get_storage(c);
        FLAGS_SET(storage->tcp_transport_state, ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_IS_WEBSOCKET |
                                                    ROBOTRACONTEURLITE_TCP_TRANSPORT_STATE_IN_HTTP_HEADER);
        FLAGS_SET(c->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_BLOCK_SEND);
        return robotraconteurlite_tcp_connect_service_send_websocket_http_header(connect_data);
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_tcp_connection_prepare_wait(
    struct robotraconteurlite_connection* connection)
{
    return robotraconteurlite_connection_impl_prepare_wait(connection);
}

robotraconteurlite_status robotraconteurlite_tcp_acceptor_prepare_wait(
    struct robotraconteurlite_connection_acceptor* acceptor,
    struct robotraconteurlite_connection_object* connection_head)
{
    return robotraconteurlite_connection_acceptor_impl_prepare_wait(acceptor, connection_head);
}

robotraconteurlite_status robotraconteurlite_tcp_connections_prepare_wait(
    struct robotraconteurlite_connection_object* connection_head)

{
    struct robotraconteurlite_connection_object* c = connection_head;
    robotraconteurlite_status rv = -1;
    while (c != NULL)
    {
        if (c->transport_type == ROBOTRACONTEURLITE_TCP_TRANSPORT)
        {
            if (c->connection_object_type == ROBOTRACONTEURLITE_CONNECTION_OBJECT_TYPE_CONNECTION)
            {
                struct robotraconteurlite_connection* c1 = robotraconteurlite_connection_cast(c);
                if (c1 == NULL)
                {
                    return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
                }
                rv = robotraconteurlite_tcp_connection_prepare_wait(c1);
                if (FAILED(rv))
                {
                    return rv;
                }
            }

            if (c->connection_object_type == ROBOTRACONTEURLITE_CONNECTION_OBJECT_TYPE_ACCEPTOR)
            {
                struct robotraconteurlite_connection_acceptor* c2 = robotraconteurlite_connection_acceptor_cast(c);
                if (c2 == NULL)
                {
                    return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
                }
                rv = robotraconteurlite_tcp_acceptor_prepare_wait(c2, connection_head);
                if (FAILED(rv))
                {
                    return rv;
                }
            }
        }
        c = c->next;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}
