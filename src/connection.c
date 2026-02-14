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

#include "robotraconteurlite/connection.h"
#include "robotraconteurlite/util.h"
#include <assert.h>

#define FLAGS_CHECK_ALL ROBOTRACONTEURLITE_FLAGS_CHECK_ALL
#define FLAGS_CHECK ROBOTRACONTEURLITE_FLAGS_CHECK
#define FLAGS_SET ROBOTRACONTEURLITE_FLAGS_SET
#define FLAGS_CLEAR ROBOTRACONTEURLITE_FLAGS_CLEAR

#define FAILED ROBOTRACONTEURLITE_FAILED
#define RETRY ROBOTRACONTEURLITE_RETRY

robotraconteurlite_status robotraconteurlite_connection_reset(struct robotraconteurlite_connection* connection)
{
    connection->head.transport_type = 0;
    connection->config_flags = ROBOTRACONTEURLITE_CONFIG_FLAGS_ENABLE_REDUCED_HEADER4;
    connection->connection_state = ROBOTRACONTEURLITE_STATUS_FLAGS_IDLE;
    connection->recv_buffer_pos = 0;
    connection->recv_message_len = 0;
    connection->send_buffer_pos = 0;
    connection->send_message_len = 0;
    (void)memset(&connection->head.sock.sock, 0, sizeof(struct robotraconteurlite_connection_socket));
    connection->local_endpoint = 0;
    connection->remote_endpoint = 0;
    if (robotraconteurlite_nodeid_reset(&connection->remote_nodeid) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    connection->message_flags_inv_mask = 0;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connection_verify_preamble(
    struct robotraconteurlite_connection* connection, robotraconteurlite_u32* message_len)
{
    const char rrac_magic[4] = {'R', 'R', 'A', 'C'};

    if ((connection->recv_buffer_pos >= 12U) && (connection->recv_message_len == 0U))
    {
        robotraconteurlite_u16 message_version = 0U;
        /* Check the message RRAC */
        if (memcmp(connection->recv_buffer, rrac_magic, sizeof(rrac_magic)) != 0)
        {
            FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR);
            FLAGS_CLEAR(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_RECEIVE_REQUESTED);
            return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
        }
        connection->recv_message_len = robotraconteurlite_util_read_uint32(&connection->recv_buffer[4]);
        *message_len = connection->recv_message_len;

        /* Check the message version */
        message_version = robotraconteurlite_util_read_uint16(&connection->recv_buffer[8]);
        if ((message_version != 2U) && (message_version != 4U))
        {
            FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR);
            FLAGS_CLEAR(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_RECEIVE_REQUESTED);
            return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
        }
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connection_message_receive(
    struct robotraconteurlite_connection* connection, struct robotraconteurlite_message_reader* message_reader,
    struct robotraconteurlite_buffer_vec* buffer_storage)
{

    if ((!FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_RECEIVED)) ||
        FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_CONSUMED))
    {
        return ROBOTRACONTEURLITE_ERROR_RETRY;
    }

    assert(buffer_storage->buffer_vec_cnt >= 1U);
    if (robotraconteurlite_buffer_init_scalar(&buffer_storage->buffer_vec[0], connection->recv_buffer,
                                              connection->recv_buffer_pos) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    buffer_storage->buffer_vec_cnt = 1U;
    if (robotraconteurlite_message_reader_init(message_reader, buffer_storage, 0U, connection->recv_buffer_pos) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connection_message_receive_consume(
    struct robotraconteurlite_connection* connection)
{
    if (!FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_RECEIVED))
    {
        return ROBOTRACONTEURLITE_ERROR_INVALID_OPERATION;
    }

    FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_CONSUMED);
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connection_begin_send_message(
    struct robotraconteurlite_connection* connection, struct robotraconteurlite_message_writer* message_writer,
    struct robotraconteurlite_buffer_vec* buffer_storage)
{
    robotraconteurlite_u16 message_version = 2;
    if (FLAGS_CHECK(connection->connection_state,
                    (ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR | ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSED |
                     ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSE_REQUESTED | ROBOTRACONTEURLITE_STATUS_FLAGS_IDLE)) ||
        (!FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTED)))
    {
        return ROBOTRACONTEURLITE_ERROR_INVALID_OPERATION;
    }

    if (FLAGS_CHECK(connection->connection_state,
                    (ROBOTRACONTEURLITE_STATUS_FLAGS_SEND_REQUESTED | ROBOTRACONTEURLITE_STATUS_FLAGS_SENDING |
                     ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_SENT |
                     ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_SENT_CONSUMED |
                     ROBOTRACONTEURLITE_STATUS_FLAGS_BLOCK_SEND)))
    {
        return ROBOTRACONTEURLITE_ERROR_RETRY;
    }

    if (robotraconteurlite_buffer_init_scalar(&buffer_storage->buffer_vec[0], connection->send_buffer,
                                              connection->send_buffer_len) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }

    if (FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_SEND_MESSAGE4))
    {
        message_version = 4;
    }

    if (robotraconteurlite_message_writer_init(message_writer, buffer_storage, 0U, connection->send_buffer_pos,
                                               message_version) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connection_end_send_message(
    struct robotraconteurlite_connection* connection, robotraconteurlite_size_t message_len)
{
    connection->send_message_len = (robotraconteurlite_u32)message_len;
    FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_SEND_REQUESTED);

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connection_abort_send_message(
    struct robotraconteurlite_connection* connection)
{
    ROBOTRACONTEURLITE_UNUSED(connection);
    /* Don't need to do anything, for future use */
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connection_close(struct robotraconteurlite_connection* connection)
{
    if (FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_IDLE))
    {
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSE_REQUESTED);

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

void robotraconteurlite_connections_construct_from_array(
    struct robotraconteurlite_connection connections_fixed_storage[],
    robotraconteurlite_size_t connections_fixed_storage_len, robotraconteurlite_byte buffers[],
    robotraconteurlite_size_t buffer_size, robotraconteurlite_size_t buffer_count,
    struct robotraconteurlite_connection_object* connections_head)
{
    robotraconteurlite_size_t i = 0U;
    assert(connections_fixed_storage_len > 0U);
    assert(buffer_count >= (connections_fixed_storage_len * 2U));
    assert(buffer_size > 1024U);

    for (i = 0; i < connections_fixed_storage_len; i++)
    {
        robotraconteurlite_connection_construct(&connections_fixed_storage[i], connections_head);
        connections_fixed_storage[i].recv_buffer = &buffers[(i * 2U * buffer_size)];
        connections_fixed_storage[i].send_buffer = &buffers[(i * 2U * buffer_size) + buffer_size];
        connections_fixed_storage[i].recv_buffer_len = buffer_size;
        connections_fixed_storage[i].send_buffer_len = buffer_size;
    }
}

/* cppcheck-suppress constParameterPointer*/
robotraconteurlite_status robotraconteurlite_connection_next_wake(struct robotraconteurlite_connection* connection,
                                                                  robotraconteurlite_timespec now,
                                                                  robotraconteurlite_timespec* next_wake)
{
    if (FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_IDLE))
    {
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    if (FLAGS_CHECK(connection->connection_state,
                    (ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSE_REQUESTED | ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR)))
    {
        *next_wake = now;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    if (FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_SENT))
    {
        *next_wake = now;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    if (FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_RECEIVED))
    {
        *next_wake = now;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    if (connection->heartbeat_next_check_ms > 0)
    {
        if ((now > connection->heartbeat_next_check_ms) ||
            /* cppcheck-suppress knownConditionTrueFalse */
            ((connection->heartbeat_next_check_ms == 0) && (connection->heartbeat_period_ms > 0)))
        {
            *next_wake = now;
            return ROBOTRACONTEURLITE_ERROR_SUCCESS;
        }

        if (connection->heartbeat_next_check_ms < *next_wake)
        {
            *next_wake = connection->heartbeat_next_check_ms;
        }
    }

    if (connection->transport_next_wake > 0)
    {
        if (connection->transport_next_wake < *next_wake)
        {
            *next_wake = connection->transport_next_wake;
        }
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

struct robotraconteurlite_connection* robotraconteurlite_connection_find_idle(
    struct robotraconteurlite_connection_object* connections_head)
{
    struct robotraconteurlite_connection* connection = robotraconteurlite_connection_first(connections_head);
    while (connection != NULL)
    {
        if (FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_IDLE))
        {
            return connection;
        }
        connection = robotraconteurlite_connection_next(&connection->head);
    }
    return NULL;
}

void robotraconteurlite_connection_list_head_construct(struct robotraconteurlite_connection_object* connections_head)
{
    (void)memset(connections_head, 0, sizeof(struct robotraconteurlite_connection_object));
    connections_head->connection_object_type = ROBOTRACONTEURLITE_CONNECTION_OBJECT_TYPE_LIST_HEAD;
}

void robotraconteurlite_connection_list_append(struct robotraconteurlite_connection_object* connections_head,
                                               struct robotraconteurlite_connection_object* value)
{
    struct robotraconteurlite_connection_object* c = connections_head;
    while (c->next != NULL)
    {
        c = c->next;
    }
    c->next = value;
    value->prev = c;
}

void robotraconteurlite_connection_list_remove(struct robotraconteurlite_connection_object* connections_head,
                                               struct robotraconteurlite_connection_object* value)
{
    ROBOTRACONTEURLITE_UNUSED(connections_head);
    if (value->prev != NULL)
    {
        value->prev->next = value->next;
    }
    if (value->next != NULL)
    {
        value->next->prev = value->prev;
    }
    value->next = NULL;
    value->prev = NULL;
}

void robotraconteurlite_connection_construct(struct robotraconteurlite_connection* connection,
                                             struct robotraconteurlite_connection_object* connections_head)
{
    (void)memset(connection, 0, sizeof(struct robotraconteurlite_connection));
    connection->head.connection_object_type = ROBOTRACONTEURLITE_CONNECTION_OBJECT_TYPE_CONNECTION;
    connection->heartbeat_period_ms = 5000;
    connection->heartbeat_timeout_ms = 15000;

    if (connections_head != NULL)
    {
        robotraconteurlite_connection_list_append(connections_head, &connection->head);
    }
}

void robotraconteurlite_connection_init(struct robotraconteurlite_connection* connection)
{
    (void)robotraconteurlite_connection_reset(connection);
    connection->heartbeat_period_ms = 5000;
    connection->heartbeat_timeout_ms = 15000;
}

void robotraconteurlite_connection_init_connections(struct robotraconteurlite_connection_object* connections_head)
{
    struct robotraconteurlite_connection* c = robotraconteurlite_connection_first(connections_head);
    while (c != NULL)
    {
        robotraconteurlite_connection_init(c);
        c = robotraconteurlite_connection_next(&c->head);
    }
}

robotraconteurlite_status robotraconteurlite_connection_impl_communicate_process_control(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now,
    robotraconteurlite_u32 transport_type, robotraconteurlite_u8* close_request)
{
    ROBOTRACONTEURLITE_UNUSED(now);

    if ((connection->head.transport_type != transport_type))
    {
        return ROBOTRACONTEURLITE_ERROR_CONSUMED;
    }

    if (FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSED_CONSUMED))
    {
        connection->connection_state = ROBOTRACONTEURLITE_STATUS_FLAGS_IDLE;
        if (robotraconteurlite_connection_reset(connection) != 0)
        {
            return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
        }
    }

    if (FLAGS_CHECK(connection->connection_state,
                    (ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSED | ROBOTRACONTEURLITE_STATUS_FLAGS_IDLE)))
    {
        return ROBOTRACONTEURLITE_ERROR_CONSUMED;
    }

    if (FLAGS_CHECK(connection->connection_state,
                    (ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSE_REQUESTED | ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSING)))
    {
        /* TODO: graceful shutdown */
        close_request[0] = 1;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connection_impl_communicate_after_close_requested(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now,
    robotraconteurlite_status close_rv)
{
    ROBOTRACONTEURLITE_UNUSED(now);
    ROBOTRACONTEURLITE_UNUSED(close_rv);

    connection->connection_state = ROBOTRACONTEURLITE_STATUS_FLAGS_CLOSED;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connection_impl_communicate_recv1(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now,
    robotraconteurlite_size_t* recv_op_len)
{
    ROBOTRACONTEURLITE_UNUSED(now);

    /* If the connection is in an error state, return error */
    if (FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR))
    {
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    /* If the message has been consumed, move the receive buffer to the beginning */
    if (FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_CONSUMED))
    {
        if (connection->recv_buffer_pos > connection->recv_message_len)
        {
            (void)memmove(connection->recv_buffer, &connection->recv_buffer[connection->recv_message_len],
                          connection->recv_buffer_pos - connection->recv_message_len);
        }
        connection->recv_buffer_pos -= connection->recv_message_len;
        connection->recv_message_len = 0;
        FLAGS_CLEAR(connection->connection_state, (ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_CONSUMED |
                                                   ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_RECEIVED));
        FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_RECEIVE_REQUESTED);
    }

    if (FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_RECEIVE_REQUESTED))
    {

        /* Receive data */
        recv_op_len[0] =
            (connection->recv_message_len == 0U) ? 64U : (connection->recv_buffer_len - connection->recv_buffer_pos);
    }
    else
    {
        recv_op_len[0] = 0;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connection_impl_communicate_recv2(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now,
    robotraconteurlite_status recv_op_rv)
{
    if (FAILED(recv_op_rv))
    {
        FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR);
        FLAGS_CLEAR(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_RECEIVE_REQUESTED);
        return recv_op_rv;
    }

    /* If this is the first receive, parse the message length */

    if (robotraconteurlite_connection_verify_preamble(connection, &connection->recv_message_len) !=
        ROBOTRACONTEURLITE_ERROR_SUCCESS)
    {
        FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR);
        FLAGS_CLEAR(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_RECEIVE_REQUESTED);
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    /* If we have received the entire message, set the message received flag */
    if ((connection->recv_message_len > 0U) && (connection->recv_buffer_pos >= connection->recv_message_len))
    {
        FLAGS_CLEAR(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_RECEIVE_REQUESTED);
        FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_RECEIVED);
        connection->last_recv_message_time = now;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connection_impl_communicate_send1(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now,
    robotraconteurlite_size_t* send_op_len)
{
    ROBOTRACONTEURLITE_UNUSED(now);

    /* If the connection is in an error state, return error */
    if (FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR))
    {
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    if (FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_SENT_CONSUMED))
    {
        FLAGS_CLEAR(connection->connection_state, (ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_SENT_CONSUMED |
                                                   ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_SENT));
        connection->send_buffer_pos = 0;
        connection->send_message_len = 0;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    if (FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_SEND_REQUESTED))
    {
        /* Clear send requested flag */
        FLAGS_CLEAR(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_SEND_REQUESTED);

        /* Send data */
        send_op_len[0] = connection->send_message_len;
    }
    else if (FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_SENDING))
    {
        /* Send data */
        send_op_len[0] = (connection->send_message_len - connection->send_buffer_pos);
    }
    else
    {
        send_op_len[0] = 0;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connection_impl_communicate_send2(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now,
    robotraconteurlite_status send_op_rv)
{
    if (FAILED(send_op_rv))
    {
        FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR);
        return send_op_rv;
    }

    /* If we have not sent the entire message, set the message sending flag */
    if ((connection->send_message_len > 0U) && (connection->send_buffer_pos < connection->send_message_len))
    {
        FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_SENDING);
    }
    else
    {
        FLAGS_CLEAR(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_SENDING);
        FLAGS_SET(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_MESSAGE_SENT);
        connection->last_send_message_time = now;
    }
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connection_impl_connect2(
    struct robotraconteurlite_connection* connection, robotraconteurlite_timespec now,
    robotraconteurlite_u32 transport_type, const struct robotraconteurlite_addr* addr,
    const struct robotraconteurlite_connection_socket* sock)
{
    struct robotraconteurlite_connection* c = connection;

    c->head.connection_object_type = ROBOTRACONTEURLITE_CONNECTION_OBJECT_TYPE_CONNECTION;
    c->head.transport_type = transport_type;
    FLAGS_CLEAR(c->config_flags, ROBOTRACONTEURLITE_CONFIG_FLAGS_ISSERVER);
    (void)memcpy(&c->head.sock, sock, sizeof(struct robotraconteurlite_connection_socket));
    c->head.sock.flags = ROBOTRACONTEURLITE_SOCKET_FLAGS_ACTIVE;
    c->connection_state = (robotraconteurlite_u32)ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTING |
                          ROBOTRACONTEURLITE_STATUS_FLAGS_RECEIVE_REQUESTED;
    (void)memset(&c->transport_storage, 0, sizeof(c->transport_storage));

    c->last_recv_message_time = now;
    c->last_send_message_time = now;

    if (robotraconteurlite_nodeid_copy_to(&addr->nodeid, &c->remote_nodeid) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    if (robotraconteurlite_string_copy_to_buffer_storage(&addr->nodename, &c->remote_nodename, c->remote_nodename_char,
                                                         sizeof(c->remote_nodename_char)) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    if (robotraconteurlite_string_copy_to_buffer_storage(&addr->service_name, &c->remote_service_name,
                                                         c->remote_service_name_char,
                                                         sizeof(c->remote_service_name_char)) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }

    c->last_recv_message_time = now;
    c->last_send_message_time = now;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connection_impl_accept2(struct robotraconteurlite_connection* connection,
                                                                     robotraconteurlite_timespec now,
                                                                     robotraconteurlite_u32 transport_type,
                                                                     /* cppcheck-suppress constParameterPointer*/
                                                                     struct robotraconteurlite_connection_socket* sock)
{

    struct robotraconteurlite_connection* c = connection;
    c->head.connection_object_type = ROBOTRACONTEURLITE_CONNECTION_OBJECT_TYPE_CONNECTION;
    (void)memcpy(&c->head.sock, sock, sizeof(struct robotraconteurlite_connection_socket));
    c->head.sock.flags = ROBOTRACONTEURLITE_SOCKET_FLAGS_ACTIVE;
    c->head.transport_type = transport_type;
    FLAGS_SET(c->config_flags, ROBOTRACONTEURLITE_CONFIG_FLAGS_ISSERVER);
    c->connection_state = ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTING;
    c->last_recv_message_time = now;
    c->last_send_message_time = now;
    (void)memset(&c->transport_storage, 0, sizeof(c->transport_storage));

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

void robotraconteurlite_connection_init_connection_acceptor(struct robotraconteurlite_connection_acceptor* acceptor)
{
    ROBOTRACONTEURLITE_UNUSED(acceptor);
}

struct robotraconteurlite_connection* robotraconteurlite_connection_cast(
    struct robotraconteurlite_connection_object* connection_obj)
{
    if (connection_obj == NULL)
    {
        return NULL;
    }

    if (connection_obj->connection_object_type != ROBOTRACONTEURLITE_CONNECTION_OBJECT_TYPE_CONNECTION)
    {
        return NULL;
    }

    /* cppcheck-suppress misra-c2012-11.3 */
    return (struct robotraconteurlite_connection*)connection_obj;
}

struct robotraconteurlite_connection* robotraconteurlite_connection_first(
    struct robotraconteurlite_connection_object* connection_obj)
{
    struct robotraconteurlite_connection_object* c1 = connection_obj;
    while (c1 != NULL)
    {
        struct robotraconteurlite_connection* c2 = robotraconteurlite_connection_cast(c1);
        if (c2 != NULL)
        {
            return c2;
        }
        c1 = c1->next;
    }
    return NULL;
}

struct robotraconteurlite_connection* robotraconteurlite_connection_first_2(
    struct robotraconteurlite_connection_object* connection_obj, robotraconteurlite_u32 transport_type)
{
    struct robotraconteurlite_connection_object* c1 = connection_obj;
    while (c1 != NULL)
    {
        struct robotraconteurlite_connection* c2 = robotraconteurlite_connection_cast(c1);
        if ((c2 != NULL) && (c2->head.transport_type == transport_type))
        {
            return c2;
        }
        c1 = c1->next;
    }
    return NULL;
}

struct robotraconteurlite_connection* robotraconteurlite_connection_next(
    struct robotraconteurlite_connection_object* connection_obj)
{
    struct robotraconteurlite_connection_object* c1 = NULL;
    if ((connection_obj == NULL) || (connection_obj->next == NULL))
    {
        return NULL;
    }
    c1 = connection_obj->next;
    while (c1 != NULL)
    {
        struct robotraconteurlite_connection* c2 = robotraconteurlite_connection_cast(c1);
        if (c2 != NULL)
        {
            return c2;
        }
        c1 = c1->next;
    }
    return NULL;
}

struct robotraconteurlite_connection* robotraconteurlite_connection_next_2(
    struct robotraconteurlite_connection_object* connection_obj, robotraconteurlite_u32 transport_type)
{
    struct robotraconteurlite_connection_object* c1 = NULL;
    if ((connection_obj == NULL) || (connection_obj->next == NULL))
    {
        return NULL;
    }
    c1 = connection_obj->next;
    while (c1 != NULL)
    {
        struct robotraconteurlite_connection* c2 = robotraconteurlite_connection_cast(c1);
        if ((c2 != NULL) && (c2->head.transport_type == transport_type))
        {
            return c2;
        }
        c1 = c1->next;
    }
    return NULL;
}

struct robotraconteurlite_connection_acceptor* robotraconteurlite_connection_acceptor_cast(
    struct robotraconteurlite_connection_object* connection_obj)
{
    if (connection_obj == NULL)
    {
        return NULL;
    }

    if (connection_obj->connection_object_type != ROBOTRACONTEURLITE_CONNECTION_OBJECT_TYPE_ACCEPTOR)
    {
        return NULL;
    }

    /* cppcheck-suppress misra-c2012-11.3 */
    return (struct robotraconteurlite_connection_acceptor*)connection_obj;
}

robotraconteurlite_status robotraconteurlite_connection_impl_prepare_wait(
    struct robotraconteurlite_connection* connection)
{

    if ((!FLAGS_CHECK(connection->head.sock.flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_ACTIVE)) ||
        (connection->head.sock.sock == (ROBOTRACONTEURLITE_SOCKET_HANDLE)0))
    {
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    if (FLAGS_CHECK(connection->connection_state,
                    (ROBOTRACONTEURLITE_STATUS_FLAGS_SEND_REQUESTED | ROBOTRACONTEURLITE_STATUS_FLAGS_SENDING)))
    {
        FLAGS_SET(connection->head.sock.flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_WANT_SEND);
    }
    else
    {
        FLAGS_CLEAR(connection->head.sock.flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_WANT_SEND);
    }

    if (FLAGS_CHECK(connection->connection_state,
                    (ROBOTRACONTEURLITE_STATUS_FLAGS_RECEIVE_REQUESTED | ROBOTRACONTEURLITE_STATUS_FLAGS_RECEIVING |
                     ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTING)))
    {
        FLAGS_SET(connection->head.sock.flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_WANT_RECEIVE);
    }
    else
    {
        FLAGS_CLEAR(connection->head.sock.flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_WANT_RECEIVE);
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connection_acceptor_impl_prepare_wait(
    struct robotraconteurlite_connection_acceptor* acceptor,
    struct robotraconteurlite_connection_object* connection_head)
{
    struct robotraconteurlite_connection* c = robotraconteurlite_connection_first(connection_head);
    FLAGS_CLEAR(acceptor->head.sock.flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_WANT_RECEIVE);
    while (c != NULL)
    {
        if (FLAGS_CHECK(c->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_IDLE))
        {
            FLAGS_SET(acceptor->head.sock.flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_WANT_RECEIVE);
            break;
        }
        c = robotraconteurlite_connection_next(c->head.next);
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

#ifdef ROBOTRACONTEURLITE_HAVE_FUNCPTR
robotraconteurlite_status robotraconteurlite_connections_communicate(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now)
{
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_connection_object* c = connections_head;
    while (c != NULL)
    {
        if ((c->ops != NULL) && (c->ops->communicate_process_control != NULL))
        {
            rv = c->ops->communicate_process_control(c, connections_head, now);
            if (FAILED(rv))
            {
                if (rv == ROBOTRACONTEURLITE_ERROR_CONSUMED)
                {
                    c = c->next;
                    continue;
                }
                /* TODO: report error*/
                /*return rv;*/
                c = c->next;
                continue;
            }
        }

        if ((c->ops != NULL) && (c->ops->communicate_recv != NULL))
        {
            rv = c->ops->communicate_recv(c, connections_head, now);
            if (FAILED(rv))
            {
                if (rv == ROBOTRACONTEURLITE_ERROR_CONSUMED)
                {
                    c = c->next;
                    continue;
                }
                /* TODO: report error*/
                /*return rv;*/
                c = c->next;
                continue;
            }
        }

        if ((c->ops != NULL) && (c->ops->communicate_send != NULL))
        {
            rv = c->ops->communicate_send(c, connections_head, now);
            if (FAILED(rv))
            {
                if (rv == ROBOTRACONTEURLITE_ERROR_CONSUMED)
                {
                    c = c->next;
                    continue;
                }
                /* TODO: report error*/
                /*return rv;*/
                c = c->next;
                continue;
            }
        }

        c = c->next;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connections_communicate_recv(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now)
{
    struct robotraconteurlite_connection_object* c = connections_head;
    while (c != NULL)
    {
        if ((c->ops != NULL) && (c->ops->communicate_recv != NULL))
        {
            robotraconteurlite_status rv = -1;
            rv = c->ops->communicate_recv(c, connections_head, now);
            if (rv == ROBOTRACONTEURLITE_ERROR_CONSUMED)
            {
                c = c->next;
                continue;
            }
            /* TODO: report error */
            /* return rv; */
        }
        c = c->next;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connections_communicate_send(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now)
{
    struct robotraconteurlite_connection_object* c = connections_head;
    while (c != NULL)
    {
        if ((c->ops != NULL) && (c->ops->communicate_send != NULL))
        {
            robotraconteurlite_status rv = -1;
            rv = c->ops->communicate_send(c, connections_head, now);
            if (FAILED(rv))
            {
                if (rv == ROBOTRACONTEURLITE_ERROR_CONSUMED)
                {
                    c = c->next;
                    continue;
                }
                /* TODO: report error */
                /* return rv; */
            }
        }
        c = c->next;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connections_communicate_process_control(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now)
{
    struct robotraconteurlite_connection_object* c = connections_head;
    while (c != NULL)
    {
        if ((c->ops != NULL) && (c->ops->communicate_process_control != NULL))
        {
            robotraconteurlite_status rv = -1;
            rv = c->ops->communicate_process_control(c, connections_head, now);
            if (FAILED(rv))
            {
                if (rv == ROBOTRACONTEURLITE_ERROR_CONSUMED)
                {
                    c = c->next;
                    continue;
                }
                /* TODO: report error */
                /* return rv; */
            }
        }
        c = c->next;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connections_prepare_wait(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now)
{
    struct robotraconteurlite_connection_object* c = connections_head;
    while (c != NULL)
    {
        if ((c->ops != NULL) && (c->ops->prepare_wait != NULL))
        {
            robotraconteurlite_status rv = -1;
            rv = c->ops->prepare_wait(c, connections_head, now);
            if (FAILED(rv))
            {
                return rv;
            }
        }
        c = c->next;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connections_close(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now)
{
    struct robotraconteurlite_connection_object* c = connections_head;
    while (c != NULL)
    {
        if ((c->ops != NULL) && (c->ops->connection_close != NULL))
        {
            robotraconteurlite_status rv = -1;
            rv = c->ops->connection_close(c, connections_head, now);
            if (FAILED(rv))
            {
                return rv;
            }
        }
        c = c->next;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_connections_communicate_drain(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now)
{
    return robotraconteurlite_connections_communicate_send(connections_head, now);
}

robotraconteurlite_size_t robotraconteurlite_connections_communicate_drain_pending(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now)
{
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_connection_object* c = NULL;
    robotraconteurlite_size_t o = 0;
    rv = robotraconteurlite_connections_prepare_wait(connections_head, now);
    if (FAILED(rv))
    {
        return 0;
    }

    c = connections_head->next;
    while (c != NULL)
    {
        if (c->connection_object_type == ROBOTRACONTEURLITE_CONNECTION_OBJECT_TYPE_CONNECTION)
        {
            if (FLAGS_CHECK(c->sock.flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_ACTIVE) &&
                (FLAGS_CHECK(c->sock.flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_WANT_SEND) &&
                 (!FLAGS_CHECK(c->sock.flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_SEND_WOULD_BLOCK))))
            {
                o++;
            }
        }
        c = c->next;
    }
    return o;
}

robotraconteurlite_size_t robotraconteurlite_connections_communicate_available(
    struct robotraconteurlite_connection_object* connections_head, robotraconteurlite_timespec now)
{
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_connection_object* c = NULL;
    robotraconteurlite_size_t o = 0;
    rv = robotraconteurlite_connections_prepare_wait(connections_head, now);
    if (FAILED(rv))
    {
        return 0;
    }

    c = connections_head->next;
    while (c != NULL)
    {
        if (c->connection_object_type == ROBOTRACONTEURLITE_CONNECTION_OBJECT_TYPE_CONNECTION)
        {
            if (FLAGS_CHECK(c->sock.flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_ACTIVE) &&
                (FLAGS_CHECK(c->sock.flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_WANT_SEND) &&
                 (!FLAGS_CHECK(c->sock.flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_SEND_WOULD_BLOCK))) &&
                (FLAGS_CHECK(c->sock.flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_WANT_RECEIVE) &&
                 (!FLAGS_CHECK(c->sock.flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_RECEIVE_WOULD_BLOCK))))
            {
                o++;
            }
        }
        c = c->next;
    }
    return o;
}

#endif
