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

#include "robotraconteurlite/node.h"
#include "robotraconteurlite/util.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
/* TODO: don't use stdio.h */
/* cppcheck-suppress misra-c2012-21.6 */
#include <stdio.h>

#define FLAGS_CHECK_ALL ROBOTRACONTEURLITE_FLAGS_CHECK_ALL
#define FLAGS_CHECK ROBOTRACONTEURLITE_FLAGS_CHECK
#define FLAGS_SET ROBOTRACONTEURLITE_FLAGS_SET
#define FLAGS_CLEAR ROBOTRACONTEURLITE_FLAGS_CLEAR

#define FAILED ROBOTRACONTEURLITE_FAILED
#define SUCCEEDED ROBOTRACONTEURLITE_SUCCEEDED
#define RETRY ROBOTRACONTEURLITE_RETRY

robotraconteurlite_status robotraconteurlite_node_init(struct robotraconteurlite_node* node,
                                                       struct robotraconteurlite_nodeid* nodeid,
                                                       struct robotraconteurlite_const_string* nodename,
                                                       struct robotraconteurlite_connection_object* connections_head)
{
    (void)memset(node, 0, sizeof(struct robotraconteurlite_node));
    if (robotraconteurlite_nodeid_copy_to(nodeid, &node->nodeid) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    assert(nodename->len < sizeof(node->nodename_char));
    if (robotraconteurlite_string_copy_to_buffer_storage(nodename, &node->nodename, node->nodename_char,
                                                         sizeof(node->nodename_char)) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    if (connections_head != NULL)
    {
        struct robotraconteurlite_connection_object* c = NULL;
        node->connections_head = connections_head;
        /* Set tail */
        c = node->connections_head;
        while (c->next != NULL)
        {
            c = c->next;
        }
        node->connections_tail = c;
    }

    node->connections_next = robotraconteurlite_connection_first(connections_head);

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_shutdown(struct robotraconteurlite_node* node)
{
    /* Request close on all connections */
    struct robotraconteurlite_connection* c = robotraconteurlite_connection_first(node->connections_head);
    while (c != NULL)
    {
        (void)robotraconteurlite_connection_close(c);
        c = robotraconteurlite_connection_next(&c->head);
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_add_connection(struct robotraconteurlite_node* node,
                                                                 struct robotraconteurlite_connection* connection)
{
    /* Add to end */
    if (node->connections_head != NULL)
    {
        node->connections_tail->next = &connection->head;
        connection->head.prev = node->connections_tail;
        node->connections_tail = &connection->head;
    }
    else
    {
        node->connections_head = &connection->head;
        node->connections_tail = &connection->head;
        node->connections_next = connection;
    }
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_remove_connection(
    struct robotraconteurlite_node* node, struct robotraconteurlite_connection_object* connection)
{
    /* Remove from list */
    if (connection->prev != NULL)
    {
        connection->prev->next = connection->next;
    }
    else
    {
        node->connections_head = connection->next;
    }

    if (connection->next != NULL)
    {
        connection->next->prev = connection->prev;
    }
    else
    {
        node->connections_tail = connection->prev;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

void static robotraconteurlite_clear_event(struct robotraconteurlite_event* event)
{
    (void)memset(event, 0, sizeof(struct robotraconteurlite_event));
}

robotraconteurlite_status robotraconteurlite_node_next_event(struct robotraconteurlite_node* node,
                                                             struct robotraconteurlite_event* event,
                                                             robotraconteurlite_timespec now)
{
    struct robotraconteurlite_connection* c = NULL;
    event->node = node;
    if (!node->connections_next)
    {
        robotraconteurlite_clear_event(event);
        event->node = node;
        event->event_time = now;
        event->event_type = ROBOTRACONTEURLITE_EVENT_TYPE_NEXT_CYCLE;
        event->events_serviced = node->events_serviced;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    do
    {
        c = node->connections_next;
        node->connections_next = robotraconteurlite_connection_next(&c->head);

        /* Check for idle*/
        if (robotraconteurlite_connection_is_idle(c) != 0)
        {
            continue;
        }

        if (robotraconteurlite_connection_is_closed_event(c) != 0)
        {
            robotraconteurlite_clear_event(event);
            event->node = node;
            event->event_time = now;
            node->events_serviced++;
            event->event_type = ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_CLOSED;
            event->connection = c;
            return ROBOTRACONTEURLITE_ERROR_SUCCESS;
        }

        if (robotraconteurlite_connection_is_error(c) != 0)
        {
            robotraconteurlite_clear_event(event);
            event->node = node;
            event->event_time = now;
            node->events_serviced++;
            event->event_type = ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_ERROR;
            event->connection = c;
            return ROBOTRACONTEURLITE_ERROR_SUCCESS;
        }

        if (robotraconteurlite_connection_is_connected_event(c) != 0)
        {
            robotraconteurlite_clear_event(event);
            event->node = node;
            event->event_time = now;
            node->events_serviced++;
            event->event_type = ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_CONNECTED;
            event->connection = c;
            return ROBOTRACONTEURLITE_ERROR_SUCCESS;
        }

        if (robotraconteurlite_connection_is_message_sent_event(c) != 0)
        {
            robotraconteurlite_clear_event(event);
            event->node = node;
            event->event_time = now;
            node->events_serviced++;
            event->event_type = ROBOTRACONTEURLITE_EVENT_TYPE_MESSAGE_SEND_COMPLETE;
            event->connection = c;
            return ROBOTRACONTEURLITE_ERROR_SUCCESS;
        }

        if (robotraconteurlite_connection_is_message_received_event(c) != 0)
        {
            robotraconteurlite_clear_event(event);
            event->node = node;
            event->event_time = now;
            node->events_serviced++;
            event->event_type = ROBOTRACONTEURLITE_EVENT_TYPE_MESSAGE_RECEIVED;
            event->connection = c;
            event->received_message.node = node;
            event->received_message.connection = c;
            event->event_error_code = robotraconteurlite_node_receive_messageentry(&event->received_message);
            return ROBOTRACONTEURLITE_ERROR_SUCCESS;
        }

        if (c->heartbeat_next_check_ms < now)
        {
            int heartbeat_ret = -1;
            c->heartbeat_next_check_ms = now + c->heartbeat_period_ms;

            heartbeat_ret = robotraconteurlite_connection_is_heartbeat_timeout(c, now);

            if (heartbeat_ret != 0)
            {
                robotraconteurlite_clear_event(event);
                event->node = node;
                event->event_time = now;
                node->events_serviced++;
                event->event_type = (heartbeat_ret == 1) ? ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_HEARTBEAT_TIMEOUT
                                                         : ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_TIMEOUT;
                event->connection = c;
                return ROBOTRACONTEURLITE_ERROR_SUCCESS;
            }
        }

    } while (node->connections_next != NULL);

    (void)memset(event, 0, sizeof(struct robotraconteurlite_event));
    event->event_type = ROBOTRACONTEURLITE_EVENT_TYPE_NEXT_CYCLE;
    event->node = node;
    event->event_time = now;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_consume_event(struct robotraconteurlite_event* event)
{
    struct robotraconteurlite_node* node = event->node;
    switch (event->event_type)
    {
    case ROBOTRACONTEURLITE_EVENT_TYPE_NEXT_CYCLE: {
        node->connections_next = robotraconteurlite_connection_next(node->connections_head);
        node->events_serviced = 0;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_CLOSED: {
        robotraconteurlite_connection_consume_closed(event->connection);
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_ERROR: {
        /* Error cannot be cleared or consumed */
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_CONNECTED: {
        robotraconteurlite_connection_consume_connected(event->connection);
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_MESSAGE_RECEIVED: {
        robotraconteurlite_connection_consume_message_received(event->connection);
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_MESSAGE_SEND_COMPLETE: {
        robotraconteurlite_connection_consume_message_sent(event->connection);
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_HEARTBEAT_TIMEOUT:
    case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_TIMEOUT: {
        /* Timeouts cannot be cleared or consumed */
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
        {
            /* Error cannot be cleared or consumed */
            return ROBOTRACONTEURLITE_ERROR_SUCCESS;
        }

    default:
        return ROBOTRACONTEURLITE_ERROR_INVALID_OPERATION;
    }
}

static robotraconteurlite_status robotraconteurlite_node_event_special_request_handle_error(
    struct robotraconteurlite_event* event, int err)
{
    if (err == ROBOTRACONTEURLITE_ERROR_RETRY)
    {
        return ROBOTRACONTEURLITE_ERROR_RETRY;
    }
    robotraconteurlite_connection_error(event->connection);
    /* Consume event */
    (void)robotraconteurlite_node_consume_event(event);
    return ROBOTRACONTEURLITE_ERROR_CONSUMED;
}

static robotraconteurlite_status
robotraconteurlite_node_event_special_request_service_definition_reply_service_not_found(
    struct robotraconteurlite_event* event)
{
    /* Return ServiceNotFound error message */
    return robotraconteurlite_connection_send_messageentry_error_response(
        event->node, event->connection, &event->received_message.received_message_entry_header,
        ROBOTRACONTEURLITE_MESSAGEERRORTYPE_SERVICENOTFOUND, "RobotRaconteur.ServiceNotFound", "Service not found");
}

robotraconteurlite_status robotraconteurlite_node_event_special_request(struct robotraconteurlite_event* event)
{
    if (event->received_message.received_message_entry_header.entry_type > 500U)
    {
        /* Special requests are below entry type 500 */
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    if (event->event_error_code != ROBOTRACONTEURLITE_ERROR_SUCCESS)
    {
        robotraconteurlite_connection_error(event->connection);
        /* Consume event */
        (void)robotraconteurlite_node_consume_event(event);
        return ROBOTRACONTEURLITE_ERROR_CONSUMED;
    }

    switch (event->received_message.received_message_entry_header.entry_type)
    {
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_STREAMOP: {
        if ((robotraconteurlite_string_cmp_c_str(&event->received_message.received_message_entry_header.member_name,
                                                 "CreateConnection") == 0) &&
            robotraconteurlite_connection_is_server(event->connection) &&
            (!FLAGS_CHECK(event->connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ESTABLISHED)))
        {
            robotraconteurlite_status rv = -1;
            robotraconteurlite_u32 caps_flags = 0;
            /* TODO: Check the incoming target address and sender address */
            if (robotraconteurlite_nodeid_copy_to(&event->received_message.received_message_header.sender_nodeid,
                                                  &event->connection->remote_nodeid) != 0)
            {
                return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
            }

            if (event->received_message.received_message_entry_header.element_count > 0U)
            {
                (void)robotraconteurlite_node_transport_parse_capabilities(&event->received_message.entry_reader,
                                                                           &caps_flags);
                if (FLAGS_CHECK(caps_flags, ROBOTRACONTEURLITE_CONNECTION_PARSE_CAPABILITY_MESSAGE4))
                {
                    FLAGS_SET(event->connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_SEND_MESSAGE4);
                }
            }

            /* TODO: sender nodename */
            if (caps_flags == 0U)
            {
                rv = robotraconteurlite_node_send_messageentry_empty_response(
                    event->node, event->connection, &event->received_message.received_message_entry_header);
                if (FAILED(rv))
                {
                    return robotraconteurlite_node_event_special_request_handle_error(event, rv);
                }
            }
            else
            {
                struct robotraconteurlite_node_send_messageentry_data send_data;
                send_data.node = event->received_message.node;
                send_data.connection = event->connection;
                rv = robotraconteurlite_node_begin_send_messageentry_response(
                    &send_data, &event->received_message.received_message_entry_header);
                if (FAILED(rv))
                {
                    return robotraconteurlite_node_event_special_request_handle_error(event, rv);
                }

                rv = robotraconteurlite_node_transport_populate_capabilities(&send_data.element_writer, caps_flags);
                if (FAILED(rv))
                {
                    return robotraconteurlite_node_event_special_request_handle_error(
                        event, ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR);
                }

                rv = robotraconteurlite_node_end_send_messageentry(&send_data);
                if (FAILED(rv))
                {
                    return robotraconteurlite_node_event_special_request_handle_error(event, rv);
                }
            }

            /* Set the ESTABLISHED flag */
            FLAGS_SET(event->connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ESTABLISHED);

            /* Consume event */
            (void)robotraconteurlite_node_consume_event(event);
            return ROBOTRACONTEURLITE_ERROR_CONSUMED;
        }
        else
        {
            break;
        }
    }
    /* False positive cppcheck warning */
    /* cppcheck-suppress misra-c2012-16.3 */
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_GETSERVICEDESC:
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_OBJECTTYPENAME: {
        /* These need to be handled by the user to avoid memory handling */
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CONNECTCLIENT: {
        robotraconteurlite_status rv = -1;
        struct robotraconteurlite_node_service* connected_service = NULL;
        {
            /* check that requested service exists! */
            if (event->node->services_head != NULL)
            {
                struct robotraconteurlite_node_service* s = event->node->services_head->next;
                while (s != NULL)
                {
                    if (robotraconteurlite_string_cmp(
                            &event->received_message.received_message_entry_header.service_path, &s->service_name) == 0)
                    {
                        connected_service = s;
                        break;
                    }
                    s = s->next;
                }

                if (connected_service == NULL)
                {
                    /* return service not found error */
                    rv =
                        robotraconteurlite_node_event_special_request_service_definition_reply_service_not_found(event);
                    if (FAILED(rv))
                    {
                        return robotraconteurlite_node_event_special_request_handle_error(event, rv);
                    }
                    (void)robotraconteurlite_node_consume_event(event);
                    return ROBOTRACONTEURLITE_ERROR_CONSUMED;
                }
            }
        }
        if (event->connection->local_endpoint == 0U)
        {
            event->connection->local_endpoint = (robotraconteurlite_u32)rand();
        }
        event->connection->remote_endpoint = event->received_message.received_message_header.sender_endpoint;

        rv = robotraconteurlite_node_send_messageentry_empty_response(
            event->node, event->connection, &event->received_message.received_message_entry_header);
        if (FAILED(rv))
        {
            return robotraconteurlite_node_event_special_request_handle_error(event, rv);
        }

        /* Set the CLIENT_ESTABLISHED flag */
        FLAGS_SET(event->connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CLIENT_ESTABLISHED);

        /* Set the service pointer */
        event->connection->service = connected_service;

        if (FLAGS_CHECK(event->connection->config_flags, ROBOTRACONTEURLITE_CONFIG_FLAGS_ENABLE_REDUCED_HEADER4))
        {
            /* Enable reduced address info in message 4 headers */
            event->connection->message_flags_inv_mask =
                (ROBOTRACONTEURLITE_MESSAGE_FLAGS_ROUTING_INFO | ROBOTRACONTEURLITE_MESSAGE_FLAGS_ENDPOINT_INFO);
        }

        /* Consume event */
        (void)robotraconteurlite_node_consume_event(event);
        return ROBOTRACONTEURLITE_ERROR_CONSUMED;
    }
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_DISCONNECTCLIENT: {
        robotraconteurlite_status rv = -1;
        /* Clear the client established flag */
        FLAGS_CLEAR(event->connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CLIENT_ESTABLISHED);
        /* Clear the associated service */
        event->connection->service = NULL;

        if (!robotraconteurlite_connection_is_server(event->connection))
        {
            (void)robotraconteurlite_node_consume_event(event);
            break;
        }
        /* TODO: handle disconnect client */
        rv = robotraconteurlite_node_send_messageentry_empty_response(
            event->node, event->connection, &event->received_message.received_message_entry_header);
        if (FAILED(rv))
        {
            return robotraconteurlite_node_event_special_request_handle_error(event, rv);
        }

        /* Consume event */
        (void)robotraconteurlite_node_consume_event(event);
        return ROBOTRACONTEURLITE_ERROR_CONSUMED;
    }
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CONNECTIONTEST:
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_GETNODEINFO: {
        robotraconteurlite_status rv = -1;
        if (!robotraconteurlite_connection_is_server(event->connection))
        {
            (void)robotraconteurlite_node_consume_event(event);
            break;
        }
        /* TODO: handle disconnect client */
        rv = robotraconteurlite_node_send_messageentry_empty_response(
            event->node, event->connection, &event->received_message.received_message_entry_header);
        if (FAILED(rv))
        {
            return robotraconteurlite_node_event_special_request_handle_error(event, rv);
        }

        /* Consume event */
        (void)robotraconteurlite_node_consume_event(event);
        return ROBOTRACONTEURLITE_ERROR_CONSUMED;
    }

    /* Client special request return messages*/
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_STREAMOPRET: {
        if ((robotraconteurlite_string_cmp_c_str(&event->received_message.received_message_entry_header.member_name,
                                                 "CreateConnection") == 0) &&
            !robotraconteurlite_connection_is_server(event->connection) &&
            (!FLAGS_CHECK(event->connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ESTABLISHED)))
        {
            if (robotraconteurlite_nodeid_copy_to(&event->received_message.received_message_header.sender_nodeid,
                                                  &event->connection->remote_nodeid) != 0)
            {
                return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
            }

            /* Set the ESTABLISHED flag */
            FLAGS_SET(event->connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ESTABLISHED);

            {
                robotraconteurlite_u32 caps_flags = 0;
                (void)robotraconteurlite_node_transport_parse_capabilities(&event->received_message.entry_reader,
                                                                           &caps_flags);
                if (FLAGS_CHECK(caps_flags, ROBOTRACONTEURLITE_CONNECTION_PARSE_CAPABILITY_MESSAGE4))
                {
                    FLAGS_SET(event->connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_SEND_MESSAGE4);
                    if (FLAGS_CHECK(event->connection->config_flags,
                                    ROBOTRACONTEURLITE_CONFIG_FLAGS_ENABLE_REDUCED_HEADER4))
                    {
                        /* Enable reduced address info in message 4 headers */
                        event->connection->message_flags_inv_mask = (ROBOTRACONTEURLITE_MESSAGE_FLAGS_ROUTING_INFO |
                                                                     ROBOTRACONTEURLITE_MESSAGE_FLAGS_ENDPOINT_INFO);
                    }
                }
            }

            return ROBOTRACONTEURLITE_ERROR_SUCCESS;
        }
        else
        {
            break;
        }
    }
    /* False positive cppcheck warning */
    /* cppcheck-suppress misra-c2012-16.3 */
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CONNECTCLIENTRET: {
        if (!robotraconteurlite_connection_is_server(event->connection) &&
            (!FLAGS_CHECK(event->connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CLIENT_ESTABLISHED)))
        {
            event->connection->remote_endpoint = event->received_message.received_message_header.sender_endpoint;

            /* Set the CLIENT_ESTABLISHED flag */
            FLAGS_SET(event->connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CLIENT_ESTABLISHED);

            return ROBOTRACONTEURLITE_ERROR_SUCCESS;
        }
        break;
    }
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CONNECTIONTESTRET: {
        /* Consume the return, handled by the connection */
        (void)robotraconteurlite_node_consume_event(event);
        return ROBOTRACONTEURLITE_ERROR_CONSUMED;
    }

    default:
        break;
    }

    /* If message is odd, it is a request, respond with error. Otherwise pass it on to the client */
    if ((event->received_message.received_message_entry_header.entry_type % 2U) == 1U)
    {
        robotraconteurlite_status rv = robotraconteurlite_connection_send_messageentry_error_response(
            event->node, event->connection, &event->received_message.received_message_entry_header,
            ROBOTRACONTEURLITE_ERROR_INVALID_OPERATION, "RobotRaconteur.InvalidOperation", "Invalid operation");
        if (RETRY(rv))
        {
            return ROBOTRACONTEURLITE_ERROR_RETRY;
        }
        /* Consume event */
        (void)robotraconteurlite_node_consume_event(event);
        return ROBOTRACONTEURLITE_ERROR_CONSUMED;
    }
    else
    {
        /* robotraconteurlite_connection_close(event->connection); */
        /* Consume event */
        /* robotraconteurlite_node_consume_event(node, event); */
        /* return ROBOTRACONTEURLITE_ERROR_CONSUMED; */

        /* For the client, pass message back to user to handle */
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
}

robotraconteurlite_status robotraconteurlite_node_verify_incoming_message(
    struct robotraconteurlite_node* node, struct robotraconteurlite_connection* connection,
    struct robotraconteurlite_message_const_header* message_header)
{
    ROBOTRACONTEURLITE_UNUSED(node);
    ROBOTRACONTEURLITE_UNUSED(connection);
    ROBOTRACONTEURLITE_UNUSED(message_header);
    /* TODO: verify address information */
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_begin_send_messageentry(
    struct robotraconteurlite_node_send_messageentry_data* send_data)
{
    robotraconteurlite_status rv = -1;
    robotraconteurlite_u8 message_flags_mask = 0;
    send_data->buffer_storage.data = NULL;
    send_data->buffer_storage.len = 0;
    send_data->buffer_vec_storage.buffer_vec_cnt = 1;
    send_data->buffer_vec_storage.buffer_vec = &send_data->buffer_storage;
    rv = robotraconteurlite_connection_begin_send_message(send_data->connection, &send_data->message_writer,
                                                          &send_data->buffer_vec_storage);
    if (FAILED(rv))
    {
        return rv;
    }

    (void)memset(&send_data->message_header, 0, sizeof(struct robotraconteurlite_message_const_header));
    send_data->message_header.message_version = 2;
    if (robotraconteurlite_nodeid_copy_to(&send_data->node->nodeid, &send_data->message_header.sender_nodeid) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    if (robotraconteurlite_nodeid_copy_to(&send_data->connection->remote_nodeid,
                                          &send_data->message_header.receiver_nodeid) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    send_data->message_header.sender_endpoint = send_data->connection->local_endpoint;
    send_data->message_header.receiver_endpoint = send_data->connection->remote_endpoint;
    if (robotraconteurlite_string_shallow_copy_to(&send_data->node->nodename,
                                                  &send_data->message_header.sender_nodename) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    if (robotraconteurlite_string_shallow_copy_to(&send_data->connection->remote_nodename,
                                                  &send_data->message_header.receiver_nodename) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }

    message_flags_mask = ~send_data->connection->message_flags_inv_mask;

    rv = robotraconteurlite_message_writer_begin_message_ex(&send_data->message_writer, &send_data->message_header,
                                                            &send_data->entry_writer, message_flags_mask);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_messageentry_writer_begin_entry(&send_data->entry_writer, send_data->message_entry_header,
                                                            &send_data->element_writer);
    return rv;
}

robotraconteurlite_status robotraconteurlite_node_end_send_messageentry(
    struct robotraconteurlite_node_send_messageentry_data* send_data)
{
    robotraconteurlite_status rv = robotraconteurlite_messageentry_writer_end_entry(
        &send_data->entry_writer, send_data->message_entry_header, &send_data->element_writer);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_writer_end_message(&send_data->message_writer, &send_data->message_header,
                                                       &send_data->entry_writer);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_connection_end_send_message(send_data->connection, send_data->message_header.message_size);
    return rv;
}

robotraconteurlite_status robotraconteurlite_node_abort_send_messageentry(
    struct robotraconteurlite_node_send_messageentry_data* send_data)
{
    robotraconteurlite_status rv = robotraconteurlite_connection_abort_send_message(send_data->connection);
    return rv;
}

robotraconteurlite_status robotraconteurlite_node_send_messageentry_empty_response(
    struct robotraconteurlite_node* node, struct robotraconteurlite_connection* connection,
    struct robotraconteurlite_messageentry_const_header* request_message_entry_header)
{
    struct robotraconteurlite_node_send_messageentry_data send_data;
    struct robotraconteurlite_messageentry_const_header send_message_header;
    robotraconteurlite_status rv = -1;
    (void)memset(&send_data, 0, sizeof(struct robotraconteurlite_node_send_messageentry_data));
    (void)memcpy(&send_message_header, request_message_entry_header,
                 sizeof(struct robotraconteurlite_messageentry_const_header));
    send_message_header.entry_type++;
    send_data.node = node;
    send_data.connection = connection;
    send_data.message_entry_header = &send_message_header;
    rv = robotraconteurlite_node_begin_send_messageentry(&send_data);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_node_end_send_messageentry(&send_data);
    if (FAILED(rv))
    {
        return rv;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_begin_send_messageentry_response(
    struct robotraconteurlite_node_send_messageentry_data* send_data,
    struct robotraconteurlite_messageentry_const_header* request_message_entry_header)
{
    robotraconteurlite_status rv = -1;
    (void)memcpy(&send_data->message_entry_header_storage, request_message_entry_header,
                 sizeof(struct robotraconteurlite_messageentry_const_header));
    send_data->message_entry_header_storage.entry_type++;
    send_data->message_entry_header = &send_data->message_entry_header_storage;
    rv = robotraconteurlite_node_begin_send_messageentry(send_data);
    return rv;
}

robotraconteurlite_status robotraconteurlite_connection_send_messageentry_error_response(
    struct robotraconteurlite_node* node, struct robotraconteurlite_connection* connection,
    struct robotraconteurlite_messageentry_const_header* request_message_entry_header,
    robotraconteurlite_u16 error_code, const char* error_name, const char* error_message)
{
    struct robotraconteurlite_node_send_messageentry_data send_data;
    struct robotraconteurlite_messageentry_const_header send_message_entry_header;
    robotraconteurlite_status rv = -1;
    (void)memcpy(&send_message_entry_header, request_message_entry_header,
                 sizeof(struct robotraconteurlite_messageentry_const_header));
    send_message_entry_header.entry_type++;
    send_message_entry_header.error = error_code;
    (void)memset(&send_data, 0, sizeof(struct robotraconteurlite_node_send_messageentry_data));
    send_data.node = node;
    send_data.connection = connection;
    send_data.message_entry_header = &send_message_entry_header;
    rv = robotraconteurlite_node_begin_send_messageentry(&send_data);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_messageelement_writer_write_data_string_c_str(&send_data.element_writer, "errorname",
                                                                          error_name);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_messageelement_writer_write_data_string_c_str(&send_data.element_writer, "errorstring",
                                                                          error_message);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_node_end_send_messageentry(&send_data);
    if (FAILED(rv))
    {
        return rv;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_receive_messageentry(
    struct robotraconteurlite_node_receive_messageentry_data* receive_data)
{
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_message_reader message_reader;
    struct robotraconteurlite_message_header message_header_temp;
    struct robotraconteurlite_messageentry_header messageentry_header_temp;
    (void)memset(&message_reader, 0, sizeof(struct robotraconteurlite_message_reader));
    (void)memset(&message_header_temp, 0, sizeof(struct robotraconteurlite_message_header));
    (void)memset(&messageentry_header_temp, 0, sizeof(struct robotraconteurlite_messageentry_header));
    receive_data->buffer_storage.data = NULL;
    receive_data->buffer_storage.len = 0;
    receive_data->buffer_vec_storage.buffer_vec = &receive_data->buffer_storage;
    receive_data->buffer_vec_storage.buffer_vec_cnt = 1;
    rv = robotraconteurlite_connection_message_receive(receive_data->connection, &message_reader,
                                                       &receive_data->buffer_vec_storage);
    if (FAILED(rv))
    {
        return rv;
    }

    /* Apply storage buffers for header strings */
    message_header_temp.receiver_nodename.data = receive_data->receiver_nodename_char;
    message_header_temp.receiver_nodename.len = sizeof(receive_data->receiver_nodename_char);
    message_header_temp.sender_nodename.data = receive_data->sender_nodename_char;
    message_header_temp.sender_nodename.len = sizeof(receive_data->sender_nodename_char);

    rv = robotraconteurlite_message_reader_read_header(&message_reader, &message_header_temp);
    if (FAILED(rv))
    {
        return rv;
    }

    robotraconteurlite_message_header_shallow_copy_from_mutable(&message_header_temp,
                                                                &receive_data->received_message_header);

    rv = robotraconteurlite_node_verify_incoming_message(receive_data->node, receive_data->connection,
                                                         &receive_data->received_message_header);
    if (FAILED(rv))
    {
        return rv;
    }

    /* TODO: Support multiple entries in one message */
    rv = robotraconteurlite_message_reader_begin_read_entries(&message_reader, &receive_data->entry_reader);
    if (FAILED(rv))
    {
        return rv;
    }

    /* Apply storage buffer for entry header strings */
    messageentry_header_temp.member_name.data = receive_data->member_name_char;
    messageentry_header_temp.member_name.len = sizeof(receive_data->member_name_char);
    messageentry_header_temp.service_path.data = receive_data->service_path_char;
    messageentry_header_temp.service_path.len = sizeof(receive_data->service_path_char);
    messageentry_header_temp.metadata.data = receive_data->extended_char;
    messageentry_header_temp.metadata.len = sizeof(receive_data->extended_char);

    rv = robotraconteurlite_messageentry_reader_read_header(&receive_data->entry_reader, &messageentry_header_temp);
    if (FAILED(rv))
    {
        return rv;
    }

    robotraconteurlite_messageentry_header_shallow_copy_from_mutable(&messageentry_header_temp,
                                                                     &receive_data->received_message_entry_header);

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_receive_messageentry_consume(
    struct robotraconteurlite_node_receive_messageentry_data* receive_data)
{
    return robotraconteurlite_connection_message_receive_consume(receive_data->connection);
}

robotraconteurlite_status robotraconteurlite_node_split_qualified_type(
    const struct robotraconteurlite_const_string* qualified_type, struct robotraconteurlite_const_string* service_type,
    struct robotraconteurlite_const_string* service_entry_type)
{
    robotraconteurlite_size_t i = 0;
    assert(qualified_type);
    if ((qualified_type->len < 3U) || (qualified_type->data == NULL))
    {
        return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
    }

    i = qualified_type->len - 1U;

    do
    {
        i--;

        if (qualified_type->data[i] == ((char)'.'))
        {
            if (service_type != NULL)
            {
                service_type->data = qualified_type->data;
                service_type->len = i;
            }

            if (service_entry_type != NULL)
            {
                i++;
                service_type->data = &qualified_type->data[i];
                service_type->len = qualified_type->len - i;
            }
            return ROBOTRACONTEURLITE_ERROR_SUCCESS;
        }

    } while (i > 1U);

    return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
}

void robotraconteurlite_node_service_definition_list_head_construct(
    struct robotraconteurlite_node_service_definition* service_defs_head)
{
    (void)memset(service_defs_head, 0, sizeof(struct robotraconteurlite_node_service_definition));
}

robotraconteurlite_status robotraconteurlite_node_service_definition_construct(
    struct robotraconteurlite_node_service_definition* service_def,
    const struct robotraconteurlite_const_string* qualified_name_str,
    const struct robotraconteurlite_const_string* service_definition_str,
    const struct robotraconteurlite_const_string* imported_qualified_names_str,
    struct robotraconteurlite_node_service_definition* service_defs_head)
{
    robotraconteurlite_status rv = -1;
    (void)memset(service_def, 0, sizeof(struct robotraconteurlite_node_service_definition));

    rv = robotraconteurlite_string_shallow_copy_to(qualified_name_str, &service_def->qualified_name);
    if (FAILED(rv))
    {
        return rv;
    }

    service_def->qualified_name_hash = robotraconteurlite_string_hash(qualified_name_str);

    rv = robotraconteurlite_string_shallow_copy_to(service_definition_str, &service_def->service_definition);
    if (FAILED(rv))
    {
        return rv;
    }
    if (imported_qualified_names_str != NULL)
    {
        rv = robotraconteurlite_string_shallow_copy_to(imported_qualified_names_str,
                                                       &service_def->imported_qualified_names);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (service_defs_head != NULL)
    {
        struct robotraconteurlite_node_service_definition* c = service_defs_head;
        while (c->next != NULL)
        {
            c = c->next;
        }
        c->next = service_def;
        service_def->prev = c;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

struct robotraconteurlite_node_service_definition* robotraconteurlite_node_find_service_definition(
    struct robotraconteurlite_node_service_definition* service_defs_head,
    const struct robotraconteurlite_const_string* qualified_name_str)
{
    robotraconteurlite_u32 hash = 0;
    assert(service_defs_head != NULL);
    assert(qualified_name_str != NULL);

    if ((qualified_name_str->len == 0U) || (qualified_name_str->data == NULL))
    {
        return NULL;
    }

    hash = robotraconteurlite_string_hash(qualified_name_str);

    {
        struct robotraconteurlite_node_service_definition* c = service_defs_head->next;
        while (c != NULL)
        {
            if ((c->qualified_name_hash == hash) &&
                (robotraconteurlite_string_cmp(&c->qualified_name, qualified_name_str) == 0))
            {
                return c;
            }
            c = c->next;
        }
    }

    return NULL;
}

struct robotraconteurlite_node_service_definition* robotraconteurlite_node_find_service_definition_for_entry(
    struct robotraconteurlite_node_service_definition* service_defs_head,
    const struct robotraconteurlite_const_string* qualified_entry_name_str)
{
    struct robotraconteurlite_const_string qualified_name_str;
    robotraconteurlite_status rv = -1;

    rv = robotraconteurlite_node_split_qualified_type(qualified_entry_name_str, &qualified_name_str, NULL);
    if (FAILED(rv))
    {
        return NULL;
    }

    return robotraconteurlite_node_find_service_definition(service_defs_head, &qualified_name_str);
}

robotraconteurlite_status robotraconteurlite_node_service_definition_construct_c_str(
    struct robotraconteurlite_node_service_definition* service_def, const char* qualified_name_str,
    const char* service_definition_str, const char* imported_qualified_names_str,
    struct robotraconteurlite_node_service_definition* service_defs_head)
{
    struct robotraconteurlite_const_string qualified_name_rrstr;
    struct robotraconteurlite_const_string service_definition_rrstr;
    struct robotraconteurlite_const_string imported_qualified_names_rrstr;

    robotraconteurlite_string_from_c_str(qualified_name_str, &qualified_name_rrstr);
    robotraconteurlite_string_from_c_str(service_definition_str, &service_definition_rrstr);
    robotraconteurlite_string_from_c_str(imported_qualified_names_str, &imported_qualified_names_rrstr);

    return robotraconteurlite_node_service_definition_construct(service_def, &qualified_name_rrstr,
                                                                &service_definition_rrstr,
                                                                &imported_qualified_names_rrstr, service_defs_head);
}

void robotraconteurlite_node_service_object_list_head_construct(
    struct robotraconteurlite_node_service_object* service_objects_head)
{
    (void)memset(service_objects_head, 0, sizeof(struct robotraconteurlite_node_service_object));
}

robotraconteurlite_status robotraconteurlite_node_service_object_construct(
    struct robotraconteurlite_node_service_object* service_object,
    const struct robotraconteurlite_const_string* service_path,
    const struct robotraconteurlite_const_string* qualified_type,
    const struct robotraconteurlite_const_string* implemented_qualified_types,
    struct robotraconteurlite_node_service* service)
{
    robotraconteurlite_status rv = -1;
    (void)memset(service_object, 0, sizeof(struct robotraconteurlite_node_service_object));

    rv = robotraconteurlite_string_shallow_copy_to(service_path, &service_object->service_path);
    if (FAILED(rv))
    {
        return rv;
    }

    service_object->service_path_hash = robotraconteurlite_string_hash(service_path);

    rv = robotraconteurlite_string_shallow_copy_to(qualified_type, &service_object->qualified_type);
    if (FAILED(rv))
    {
        return rv;
    }

    if (implemented_qualified_types != NULL)
    {
        rv = robotraconteurlite_string_shallow_copy_to(implemented_qualified_types,
                                                       &service_object->implemented_qualified_types);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (service != NULL)
    {
        struct robotraconteurlite_node_service_object* c = &service->service_objects_head;
        while (c->next != NULL)
        {
            c = c->next;
        }
        c->next = service_object;
        service_object->prev = c;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_service_object_construct_c_str(
    struct robotraconteurlite_node_service_object* service_object, const char* service_path, const char* qualified_type,
    const char* implemented_qualified_types, struct robotraconteurlite_node_service* service)
{
    struct robotraconteurlite_const_string service_path_str;
    struct robotraconteurlite_const_string qualified_type_str;
    struct robotraconteurlite_const_string implemented_qualified_types_str;

    robotraconteurlite_string_from_c_str(service_path, &service_path_str);
    robotraconteurlite_string_from_c_str(qualified_type, &qualified_type_str);
    robotraconteurlite_string_from_c_str(implemented_qualified_types, &implemented_qualified_types_str);

    return robotraconteurlite_node_service_object_construct(service_object, &service_path_str, &qualified_type_str,
                                                            &implemented_qualified_types_str, service);
}

robotraconteurlite_status robotraconteurlite_node_service_construct(
    struct robotraconteurlite_node_service* service, const struct robotraconteurlite_const_string* service_name,
    struct robotraconteurlite_node_service* services_head)
{
    robotraconteurlite_status rv = -1;
    (void)memset(service, 0, sizeof(struct robotraconteurlite_node_service));

    rv = robotraconteurlite_string_shallow_copy_to(service_name, &service->service_name);
    if (FAILED(rv))
    {
        return rv;
    }

    service->service_name_hash = robotraconteurlite_string_hash(service_name);
    if (services_head != NULL)
    {
        struct robotraconteurlite_node_service* c = services_head;
        while (c->next != NULL)
        {
            c = c->next;
        }
        c->next = service;
        service->prev = c;
    }
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_service_construct_c_str(
    struct robotraconteurlite_node_service* service, const char* service_name,
    struct robotraconteurlite_node_service* services_head)
{
    struct robotraconteurlite_const_string service_name_str;
    robotraconteurlite_string_from_c_str(service_name, &service_name_str);
    return robotraconteurlite_node_service_construct(service, &service_name_str, services_head);
}

void robotraconteurlite_node_service_list_head_construct(struct robotraconteurlite_node_service* service_objects_head)
{
    (void)memset(service_objects_head, 0, sizeof(struct robotraconteurlite_node_service));
}

robotraconteurlite_status robotraconteurlite_node_service_set_root_object(
    struct robotraconteurlite_node_service* service, struct robotraconteurlite_node_service_object* service_object)
{
    if ((service->service_objects_head.next != NULL) || (service->root_service_object != NULL))
    {
        /* root service object must be set first */
        return ROBOTRACONTEURLITE_ERROR_INVALID_OPERATION;
    }

    if (robotraconteurlite_string_cmp(&service->service_name, &service_object->service_path) != 0)
    {
        /* root service path must match service name */
        return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
    }

    service->service_objects_head.next = service_object;
    service_object->prev = &service->service_objects_head;
    service->root_service_object = service_object;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_service_add_service_object(
    struct robotraconteurlite_node_service* service, struct robotraconteurlite_node_service_object* service_object)
{
    if ((service->service_objects_head.next == NULL) || (service->root_service_object == NULL))
    {
        /* root service object must be set first */
        return ROBOTRACONTEURLITE_ERROR_INVALID_OPERATION;
    }

    if (robotraconteurlite_node_is_service_path_prefix(&service->service_name, &service_object->service_path).logical ==
        0U)
    {
        /* service object must have root service name in service path */
        return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
    }

    {
        struct robotraconteurlite_node_service_object* c = service->service_objects_head.next;
        struct robotraconteurlite_node_service_object* c_last = service->service_objects_head.next;
        while (c != NULL)
        {
            if (robotraconteurlite_string_cmp(&service_object->service_path, &c->service_path) == 0)
            {
                /* duplicate service path! */
                return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
            }
            c_last = c;
            c = c->next;
        }

        c_last->next = service_object;
        service_object->prev = c_last;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_service_remove_service_object(
    struct robotraconteurlite_node_service* service, struct robotraconteurlite_node_service_object* service_object)
{
    assert(service != NULL);
    assert(service_object != NULL);
    assert(service_object->prev != NULL);

    if ((service->service_objects_head.next == service_object) || (service->root_service_object == service_object))
    {
        /* cannot remove root object */
        return ROBOTRACONTEURLITE_ERROR_INVALID_OPERATION;
    }

    {
        struct robotraconteurlite_node_service_object* c = &service->service_objects_head;
        do
        {
            if (c->next == service_object)
            {
                break;
            }
            if (c->next == NULL)
            {
                /* service object not in service */
                return ROBOTRACONTEURLITE_ERROR_INVALID_OPERATION;
            }
            c = c->next;
        } while (c != NULL);
    }

    service_object->prev->next = service_object->next;
    if (service_object->next != NULL)
    {
        service_object->next->prev = service_object->prev;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

struct robotraconteurlite_bool robotraconteurlite_node_is_service_path_prefix(
    const struct robotraconteurlite_const_string* path_prefix,
    const struct robotraconteurlite_const_string* service_path)
{
    struct robotraconteurlite_bool ret;
    ret.logical = 0;

    assert(path_prefix != NULL);
    assert(service_path != NULL);
    assert(path_prefix->data != NULL);
    assert(service_path->data != NULL);
    assert(path_prefix->len > 0U);
    assert(service_path->len > 0U);

    if (service_path->len < path_prefix->len)
    {
        return ret;
    }

    if (service_path->len == path_prefix->len)
    {
        ret.logical = (memcmp(service_path->data, path_prefix->data, path_prefix->len) == 0) ? 1 : 0;
        return ret;
    }

    ret.logical = ((memcmp(service_path->data, path_prefix->data, path_prefix->len) == 0) &&
                   (service_path->data[path_prefix->len] == ((char)'.')))
                      ? 1
                      : 0;
    return ret;
}

struct robotraconteurlite_node_service* robotraconteurlite_node_find_service_for_path(
    struct robotraconteurlite_node_service* services_head, const struct robotraconteurlite_const_string* service_path)
{
    robotraconteurlite_size_t l = 1;
    struct robotraconteurlite_const_string service_path_prefix;
    robotraconteurlite_u32 hash = 0;
    assert(services_head != NULL);
    assert(service_path != NULL);

    service_path_prefix.data = service_path->data;
    service_path_prefix.len = service_path->len;

    if ((service_path->len == 0U) || (service_path->data == NULL))
    {
        return NULL;
    }

    while (l < service_path->len)
    {
        if (service_path->data[l] == ((char)'.'))
        {
            break;
        }
        l++;
    }

    if (l < service_path->len)
    {
        service_path_prefix.len = l;
    }

    hash = robotraconteurlite_string_hash(&service_path_prefix);

    {
        struct robotraconteurlite_node_service* c = services_head->next;
        while (c != NULL)
        {
            if ((c->service_name_hash == hash) &&
                (robotraconteurlite_string_cmp(&c->service_name, &service_path_prefix) == 0))
            {
                return c;
            }
            c = c->next;
        }
    }

    return NULL;
}

struct robotraconteurlite_node_service_object* robotraconteurlite_node_find_service_object_for_path(
    struct robotraconteurlite_node_service_object* service_objects_head,
    const struct robotraconteurlite_const_string* service_path)
{
    robotraconteurlite_u32 hash = 0;
    assert(service_objects_head != NULL);
    assert(service_path != NULL);

    if ((service_path->len == 0U) || (service_path->data == NULL))
    {
        return NULL;
    }

    hash = robotraconteurlite_string_hash(service_path);

    {
        struct robotraconteurlite_node_service_object* c = service_objects_head->next;
        while (c != NULL)
        {
            if ((c->service_path_hash == hash) && (robotraconteurlite_string_cmp(&c->service_path, service_path) == 0))
            {
                return c;
            }
            c = c->next;
        }
    }

    return NULL;
}

static robotraconteurlite_status robotraconteurlite_node_event_special_request_service_definition_reply_service_def(
    struct robotraconteurlite_event* event, const struct robotraconteurlite_const_string* service_def_str)
{
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_node_send_messageentry_data send_data;
    robotraconteurlite_u32 old_remote_endpoint = 0;

    assert(event);
    assert(service_def_str);
    assert(service_def_str->len > 0U);
    assert(service_def_str->data);

    send_data.node = event->received_message.node;
    send_data.connection = event->connection;
    old_remote_endpoint = event->connection->remote_endpoint;
    event->connection->remote_endpoint = event->received_message.received_message_header.sender_endpoint;
    rv = robotraconteurlite_node_begin_send_messageentry_response(
        &send_data, &event->received_message.received_message_entry_header);
    event->connection->remote_endpoint = old_remote_endpoint;
    if (FAILED(rv))
    {
        return rv;
    }
    {
        /* Write service definition */
        struct robotraconteurlite_const_string element_name_str;
        robotraconteurlite_string_from_c_str("servicedef", &element_name_str);
        rv = robotraconteurlite_messageelement_writer_write_data_string(&send_data.element_writer, &element_name_str,
                                                                        service_def_str);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    {
        /* Write empty attributes */
        /* TODO: Support attributes */
        struct robotraconteurlite_messageelement_writer attr_element_writer;
        struct robotraconteurlite_messageelement_const_header attr_element_header;
        (void)memset(&attr_element_header, 0, sizeof(struct robotraconteurlite_messageelement_const_header));
        attr_element_header.element_type = ROBOTRACONTEURLITE_DATATYPE_MAP_STRING;
        robotraconteurlite_string_from_c_str("attributes", &attr_element_header.element_name);
        rv = robotraconteurlite_messageelement_writer_begin_nested_element(&send_data.element_writer,
                                                                           &attr_element_header, &attr_element_writer);
        if (FAILED(rv))
        {
            return rv;
        }
        rv = robotraconteurlite_messageelement_writer_end_nested_element(&send_data.element_writer,
                                                                         &attr_element_header, &attr_element_writer);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    rv = robotraconteurlite_node_end_send_messageentry(&send_data);
    if (FAILED(rv))
    {
        return rv;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_event_special_request_service_definition(
    struct robotraconteurlite_event* event, struct robotraconteurlite_node_service* services_head,
    struct robotraconteurlite_node_service_definition* service_defs_head)
{

    assert(event != NULL);
    assert(event->received_message.received_message_entry_header.entry_type ==
           ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_GETSERVICEDESC);
    assert(services_head != NULL);
    assert(service_defs_head != NULL);

    {
        /* find if "servicetype" or "ServiceType" elements exist */
        robotraconteurlite_status rv = -1;
        struct robotraconteurlite_const_string element_name;
        struct robotraconteurlite_messageelement_reader element_reader;
        robotraconteurlite_string_from_c_str("servicetype", &element_name);
        rv = robotraconteurlite_messageentry_reader_find_element(&event->received_message.entry_reader, &element_name,
                                                                 &element_reader);
        if (FAILED(rv))
        {
            if (rv != ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_NOT_FOUND)
            {
                return robotraconteurlite_node_event_special_request_service_definition_reply_service_not_found(event);
            }
        }

        robotraconteurlite_string_from_c_str("ServiceType", &element_name);
        rv = robotraconteurlite_messageentry_reader_find_element(&event->received_message.entry_reader, &element_name,
                                                                 &element_reader);
        if (FAILED(rv))
        {
            if (rv != ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_NOT_FOUND)
            {
                return robotraconteurlite_node_event_special_request_service_definition_reply_service_not_found(event);
            }
        }

        if (SUCCEEDED(rv))
        {
            char servicetype_storage[ROBOTRACONTEURLITE_MESSAGE_STR_MAX_SIZE];
            struct robotraconteurlite_string servicetype_str;
            struct robotraconteurlite_const_string servicetype_const_str;
            struct robotraconteurlite_node_service_definition* def = NULL;
            servicetype_str.data = servicetype_storage;
            servicetype_str.len = sizeof(servicetype_storage);
            rv = robotraconteurlite_messageelement_reader_read_data_string(&element_reader, &servicetype_str);
            if (FAILED(rv))
            {
                return robotraconteurlite_node_event_special_request_service_definition_reply_service_not_found(event);
            }

            (void)robotraconteurlite_string_shallow_copy_from_mutable(&servicetype_str, &servicetype_const_str);
            def = robotraconteurlite_node_find_service_definition(service_defs_head, &servicetype_const_str);
            if (def == NULL)
            {
                return robotraconteurlite_node_event_special_request_service_definition_reply_service_not_found(event);
            }

            return robotraconteurlite_node_event_special_request_service_definition_reply_service_def(
                event, &def->service_definition);
        }
    }

    {
        struct robotraconteurlite_node_service* srv = NULL;
        srv = robotraconteurlite_node_find_service_for_path(
            services_head, &event->received_message.received_message_entry_header.service_path);
        if (srv != NULL)
        {
            struct robotraconteurlite_node_service_definition* def = NULL;
            def = robotraconteurlite_node_find_service_definition_for_entry(service_defs_head,
                                                                            &srv->root_service_object->qualified_type);
            if (def != NULL)
            {
                return robotraconteurlite_node_event_special_request_service_definition_reply_service_def(
                    event, &def->service_definition);
            }
        }
    }

    /* TODO: handle clientversion field? */

    return robotraconteurlite_node_event_special_request_service_definition_reply_service_not_found(event);
}

robotraconteurlite_status robotraconteurlite_node_event_special_request_object_type_name(
    struct robotraconteurlite_event* event, struct robotraconteurlite_node_service_object* service_objects_head)
{
    struct robotraconteurlite_node_service_object* service_object = NULL;
    assert(event);
    assert(event->received_message.received_message_entry_header.entry_type ==
           ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_OBJECTTYPENAME);

    service_object = robotraconteurlite_node_find_service_object_for_path(
        service_objects_head, &event->received_message.received_message_entry_header.service_path);

    if (service_object != NULL)
    {
        robotraconteurlite_status rv = -1;
        struct robotraconteurlite_node_send_messageentry_data send_data;
        robotraconteurlite_u32 old_remote_endpoint = 0;
        send_data.node = event->received_message.node;
        send_data.connection = event->connection;
        old_remote_endpoint = event->connection->remote_endpoint;
        event->connection->remote_endpoint = event->received_message.received_message_header.sender_endpoint;
        rv = robotraconteurlite_node_begin_send_messageentry_response(
            &send_data, &event->received_message.received_message_entry_header);
        event->connection->remote_endpoint = old_remote_endpoint;
        if (FAILED(rv))
        {
            return rv;
        }
        {
            /* Write object type name */
            struct robotraconteurlite_const_string element_name_str;
            robotraconteurlite_string_from_c_str("objecttype", &element_name_str);
            rv = robotraconteurlite_messageelement_writer_write_data_string(
                &send_data.element_writer, &element_name_str, &service_object->qualified_type);
            if (FAILED(rv))
            {
                return rv;
            }

            if (service_object->implemented_qualified_types.len > 0U)
            {
                struct robotraconteurlite_messageelement_writer nested_element_writer;
                struct robotraconteurlite_messageelement_const_header nested_element_header;
                robotraconteurlite_size_t i = 0;
                robotraconteurlite_size_t k = 0;
                robotraconteurlite_u32 list_i = 0;

                (void)memset(&nested_element_header, 0, sizeof(struct robotraconteurlite_messageelement_const_header));

                robotraconteurlite_string_from_c_str("objectimplements", &nested_element_header.element_name);
                nested_element_header.element_type = ROBOTRACONTEURLITE_DATATYPE_LIST;

                rv = robotraconteurlite_messageelement_writer_begin_nested_element(
                    &send_data.element_writer, &nested_element_header, &nested_element_writer);
                if (FAILED(rv))
                {
                    return rv;
                }

                for (i = 0; i < service_object->implemented_qualified_types.len; i++)
                {

                    if ((service_object->implemented_qualified_types.data[i] == ((char)';')) ||
                        (i >= service_object->implemented_qualified_types.len))
                    {
                        if ((i - k) > 0U)
                        {
                            struct robotraconteurlite_const_string o;
                            struct robotraconteurlite_const_string list_i_str;
                            char list_i_str_buf[16];
                            (void)memset(list_i_str_buf, 0, sizeof(list_i_str_buf));
                            /* TODO: snprintf? */
                            (void)sprintf(list_i_str_buf, "%u", list_i);
                            o.data = &service_object->implemented_qualified_types.data[k];
                            o.len = (i - k);
                            list_i_str.data = list_i_str_buf;
                            list_i_str.len = strlen(list_i_str_buf);
                            rv = robotraconteurlite_messageelement_writer_write_data_string(&nested_element_writer,
                                                                                            &list_i_str, &o);
                            if (FAILED(rv))
                            {
                                return rv;
                            }
                            list_i++;
                        }
                        k = i + 1U;
                    }
                }

                rv = robotraconteurlite_messageelement_writer_end_nested_element(
                    &send_data.element_writer, &nested_element_header, &nested_element_writer);
                if (FAILED(rv))
                {
                    return rv;
                }
            }
        }

        rv = robotraconteurlite_node_end_send_messageentry(&send_data);
        if (FAILED(rv))
        {
            return rv;
        }

        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    /* Return ObjectNotFound error message */
    return robotraconteurlite_connection_send_messageentry_error_response(
        event->node, event->connection, &event->received_message.received_message_entry_header,
        ROBOTRACONTEURLITE_MESSAGEERRORTYPE_OBJECTNOTFOUND, "RobotRaconteur.ObjectNotFound", "Object not found");
}

robotraconteurlite_status robotraconteurlite_node_event_special_request_object_type_name2(
    struct robotraconteurlite_event* event, struct robotraconteurlite_node_service* services_head)
{
    struct robotraconteurlite_node_service* obj = NULL;

    obj = robotraconteurlite_node_find_service_for_path(
        services_head, &event->received_message.received_message_entry_header.service_path);
    if (obj != NULL)
    {
        return robotraconteurlite_node_event_special_request_object_type_name(event, &obj->service_objects_head);
    }

    /* Return ObjectNotFound error message */
    return robotraconteurlite_connection_send_messageentry_error_response(
        event->node, event->connection, &event->received_message.received_message_entry_header,
        ROBOTRACONTEURLITE_MESSAGEERRORTYPE_OBJECTNOTFOUND, "RobotRaconteur.ObjectNotFound", "Object not found");
}

robotraconteurlite_status robotraconteurlite_node_event_is_member(struct robotraconteurlite_event* event,
                                                                  const char* member_name)
{
    assert(event->event_type == ROBOTRACONTEURLITE_EVENT_TYPE_MESSAGE_RECEIVED);
    if (robotraconteurlite_string_cmp_c_str(&event->received_message.received_message_entry_header.member_name,
                                            member_name) != 0)
    {
        return 0;
    }
    return 1;
}

robotraconteurlite_status robotraconteurlite_node_event_is_member2(struct robotraconteurlite_event* event,
                                                                   const char* service_path, const char* member_name)
{
    assert(event->event_type == ROBOTRACONTEURLITE_EVENT_TYPE_MESSAGE_RECEIVED);
    if (robotraconteurlite_string_cmp_c_str(&event->received_message.received_message_entry_header.service_path,
                                            service_path) != 0)
    {
        return 0;
    }
    if (robotraconteurlite_string_cmp_c_str(&event->received_message.received_message_entry_header.member_name,
                                            member_name) != 0)
    {
        return 0;
    }
    return 1;
}

robotraconteurlite_status robotraconteurlite_node_event_respond_member_not_found(struct robotraconteurlite_event* event)
{
    return robotraconteurlite_connection_send_messageentry_error_response(
        event->node, event->connection, &event->received_message.received_message_entry_header,
        ROBOTRACONTEURLITE_MESSAGEERRORTYPE_MEMBERNOTFOUND, "RobotRaconteur.MemberNotFound", "Member not found");
}

robotraconteurlite_status robotraconteurlite_node_event_respond_invalid_operation(
    struct robotraconteurlite_event* event)
{
    return robotraconteurlite_connection_send_messageentry_error_response(
        event->node, event->connection, &event->received_message.received_message_entry_header,
        ROBOTRACONTEURLITE_MESSAGEERRORTYPE_INVALIDOPERATION, "RobotRaconteur.InvalidOperation", "Invalid operation");
}

robotraconteurlite_status robotraconteurlite_client_is_connected(struct robotraconteurlite_node* node,
                                                                 struct robotraconteurlite_connection* connection)
{
    ROBOTRACONTEURLITE_UNUSED(node);
    if (FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_ERROR))
    {
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    if (!FLAGS_CHECK(connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTED))
    {
        return ROBOTRACONTEURLITE_ERROR_RETRY;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_client_handshake_error(
    struct robotraconteurlite_client_handshake_data* handshake_data)
{
    if (robotraconteurlite_connection_close(handshake_data->connection) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }
    handshake_data->handshake_state = ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_ERROR;
    /* Error cannot be cleared or consumed */
    return ROBOTRACONTEURLITE_ERROR_RETRY;
}

static robotraconteurlite_status robotraconteurlite_client_handshake_handle_error(
    struct robotraconteurlite_client_handshake_data* handshake_data, robotraconteurlite_status rv)
{
    if (RETRY(rv))
    {
        return ROBOTRACONTEURLITE_ERROR_RETRY;
    }
    return robotraconteurlite_client_handshake_error(handshake_data);
}

static robotraconteurlite_status robotraconteurlite_client_handshake_begin_request(
    struct robotraconteurlite_client_handshake_data* handshake_data,
    struct robotraconteurlite_node_send_messageentry_data* send_data, robotraconteurlite_u16 entry_type,
    const char* membername)
{
    robotraconteurlite_status rv = -1;
    (void)memset(send_data, 0, sizeof(struct robotraconteurlite_node_send_messageentry_data));
    send_data->node = handshake_data->node;
    send_data->connection = handshake_data->connection;
    rv = robotraconteurlite_client_begin_request(send_data, entry_type, membername, NULL);
    handshake_data->request_id = send_data->message_entry_header->request_id;
    return rv;
}

robotraconteurlite_status robotraconteurlite_client_handshake(
    struct robotraconteurlite_client_handshake_data* handshake_data, struct robotraconteurlite_event* event,
    robotraconteurlite_timespec now)
{
    robotraconteurlite_status rv = -1;
    ROBOTRACONTEURLITE_UNUSED(now);

    if ((event->event_type != ROBOTRACONTEURLITE_EVENT_TYPE_NEXT_CYCLE) &&
        (event->connection != handshake_data->connection))
    {
        return ROBOTRACONTEURLITE_ERROR_UNHANDLED_EVENT;
    }

    if (event->event_type != ROBOTRACONTEURLITE_EVENT_TYPE_NEXT_CYCLE)
    {
        rv = robotraconteurlite_node_event_special_request(event);
        if (rv == ROBOTRACONTEURLITE_ERROR_CONSUMED)
        {
            return ROBOTRACONTEURLITE_ERROR_RETRY;
        }
        else if (FAILED(rv))
        {
            return robotraconteurlite_client_handshake_handle_error(handshake_data, rv);
        }
        else
        {
            /* noop */
        }

        switch (event->event_type)
        {
        case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_CLOSED: {
            robotraconteurlite_connection_consume_closed(event->connection);
            handshake_data->handshake_state = ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_FAILED;
            return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
        }
        case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_ERROR:
        case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_TIMEOUT: {
            (void)robotraconteurlite_connection_close(event->connection);
            handshake_data->handshake_state = ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_ERROR;
            /* Error cannot be cleared or consumed */
            return ROBOTRACONTEURLITE_ERROR_RETRY;
        }
        case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_CONNECTED: {
            /*if (handshake_data->handshake_state != ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_INIT)
            {
                return robotraconteurlite_client_handshake_error(handshake_data);
            }*/
            /*robotraconteurlite_connection_consume_connected(event->connection);
            handshake_data->handshake_state = ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CONNECTED;*/
            (void)robotraconteurlite_node_consume_event(event);
            break;
        }
        case ROBOTRACONTEURLITE_EVENT_TYPE_MESSAGE_SEND_COMPLETE: {
            switch (handshake_data->handshake_state)
            {
            case ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CREATECONNECTION_SENT:
            case ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_OBJECTTYPE_SENT:
            case ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CONNECTCLIENT_SENT: {
                (void)robotraconteurlite_node_consume_event(event);
                break;
            }
            default: {
                return ROBOTRACONTEURLITE_ERROR_UNHANDLED_EVENT;
            }
            }
            break;
        }
        case ROBOTRACONTEURLITE_EVENT_TYPE_MESSAGE_RECEIVED: {
            switch (handshake_data->handshake_state)
            {
            case ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CREATECONNECTION_SENT: {
                if (/*(event->received_message.received_message_entry_header.request_id != handshake_data->request_id)
                       || */
                    (event->received_message.received_message_entry_header.entry_type !=
                     ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_STREAMOPRET))
                {
                    return robotraconteurlite_client_handshake_error(handshake_data);
                }
                /* Set to connection "connected" connection_state flag if still connecting */
                if (FLAGS_CHECK(handshake_data->connection->connection_state,
                                ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTING))
                {
                    FLAGS_CLEAR(handshake_data->connection->connection_state,
                                ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTING);
                    FLAGS_SET(handshake_data->connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTED);
                }

                handshake_data->handshake_state = ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CREATECONNECTION_COMPLETED;
                (void)robotraconteurlite_node_consume_event(event);
                break;
            }
            case ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_OBJECTTYPE_SENT: {

                struct robotraconteurlite_const_string element_name;
                struct robotraconteurlite_messageelement_reader element_reader;
                if (/*(event->received_message.received_message_entry_header.request_id != handshake_data->request_id)
                       || */
                    (event->received_message.received_message_entry_header.entry_type !=
                     ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_OBJECTTYPENAMERET))
                {
                    return robotraconteurlite_client_handshake_error(handshake_data);
                }
                robotraconteurlite_string_from_c_str("objecttype", &element_name);
                rv = robotraconteurlite_messageentry_reader_find_element_verify_string(
                    &event->received_message.entry_reader, &element_name, &element_reader, 128);
                if (FAILED(rv))
                {
                    return robotraconteurlite_client_handshake_error(handshake_data);
                }

                handshake_data->root_object_type.data = handshake_data->root_object_type_char;
                handshake_data->root_object_type.len = sizeof(handshake_data->root_object_type_char);
                rv = robotraconteurlite_messageelement_reader_read_data_string(&element_reader,
                                                                               &handshake_data->root_object_type);
                if (FAILED(rv))
                {
                    return robotraconteurlite_client_handshake_error(handshake_data);
                }
                /* Consume event */
                (void)robotraconteurlite_node_consume_event(event);
                handshake_data->handshake_state = ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_OBJECTTYPE_COMPLETED;
                break;
            }
            case ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CONNECTCLIENT_SENT: {
                if ((event->received_message.received_message_entry_header.request_id != handshake_data->request_id) ||
                    (event->received_message.received_message_entry_header.entry_type !=
                     ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CONNECTCLIENTRET))
                {
                    return robotraconteurlite_client_handshake_error(handshake_data);
                }
                handshake_data->handshake_state = ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CONNECTCLIENT_COMPLETED;
                (void)robotraconteurlite_node_consume_event(event);
                break;
            }
            default: {
                return ROBOTRACONTEURLITE_ERROR_UNHANDLED_EVENT;
            }
            }
            break;
        case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_HEARTBEAT_TIMEOUT: {
            /* Ignore heartbeat timeouts */
            return ROBOTRACONTEURLITE_ERROR_RETRY;
        }
        }
        default: {
            return ROBOTRACONTEURLITE_ERROR_UNHANDLED_EVENT;
        }
        }
    }
    else
    {
        /* Consume next cycle */
        (void)robotraconteurlite_node_consume_event(event);
    }

    switch (handshake_data->handshake_state)
    {
    /*case ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_INIT:
    {
        if (handshake_data->connection->local_endpoint == 0)
        {
            handshake_data->connection->local_endpoint = rand();
        }
        return ROBOTRACONTEURLITE_ERROR_RETRY;
    }
    case ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CONNECTED:*/

    /* Send opening request immediately */
    case ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_INIT: {
        struct robotraconteurlite_node_send_messageentry_data send_data;
        robotraconteurlite_u32 old_connection_state = 0;
        if (handshake_data->connection->local_endpoint == 0U)
        {
            handshake_data->connection->local_endpoint = rand();
        }
        handshake_data->connection->last_request_id = 100 + (rand() % 100000);
        /* Spoof being connected to avoid error... */
        old_connection_state = handshake_data->connection->connection_state;
        FLAGS_CLEAR(handshake_data->connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTING);
        FLAGS_SET(handshake_data->connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CONNECTED);
        rv = robotraconteurlite_client_handshake_begin_request(
            handshake_data, &send_data, ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_STREAMOP, "CreateConnection");
        handshake_data->connection->connection_state = old_connection_state;
        if (FAILED(rv))
        {
            return robotraconteurlite_client_handshake_handle_error(handshake_data, rv);
        }

        rv = robotraconteurlite_node_transport_populate_capabilities(
            &send_data.element_writer, (ROBOTRACONTEURLITE_CONNECTION_PARSE_CAPABILITY_MESSAGE2 |
                                        ROBOTRACONTEURLITE_CONNECTION_PARSE_CAPABILITY_MESSAGE4));
        if (FAILED(rv))
        {
            return robotraconteurlite_client_handshake_handle_error(handshake_data, rv);
        }

        rv = robotraconteurlite_node_end_send_messageentry(&send_data);
        if (FAILED(rv))
        {
            return robotraconteurlite_client_handshake_handle_error(handshake_data, rv);
        }

        handshake_data->handshake_state = ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CREATECONNECTION_SENT;
        return ROBOTRACONTEURLITE_ERROR_RETRY;
    }
    case ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CREATECONNECTION_SENT: {
        return ROBOTRACONTEURLITE_ERROR_RETRY;
    }
    case ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CREATECONNECTION_COMPLETED: {
        struct robotraconteurlite_node_send_messageentry_data send_data;
        rv = robotraconteurlite_client_handshake_begin_request(
            handshake_data, &send_data, ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_OBJECTTYPENAME, NULL);
        if (FAILED(rv))
        {
            return robotraconteurlite_client_handshake_handle_error(handshake_data, rv);
        }
        rv = robotraconteurlite_node_end_send_messageentry(&send_data);
        if (FAILED(rv))
        {
            return robotraconteurlite_client_handshake_handle_error(handshake_data, rv);
        }

        handshake_data->handshake_state = ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_OBJECTTYPE_SENT;
        return ROBOTRACONTEURLITE_ERROR_RETRY;
    }
    case ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_OBJECTTYPE_SENT: {
        return ROBOTRACONTEURLITE_ERROR_RETRY;
    }
    case ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_OBJECTTYPE_COMPLETED: {
        struct robotraconteurlite_node_send_messageentry_data send_data;
        rv = robotraconteurlite_client_handshake_begin_request(handshake_data, &send_data,
                                                               ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CONNECTCLIENT, NULL);
        if (FAILED(rv))
        {
            return robotraconteurlite_client_handshake_handle_error(handshake_data, rv);
        }
        rv = robotraconteurlite_node_end_send_messageentry(&send_data);
        if (FAILED(rv))
        {
            return robotraconteurlite_client_handshake_handle_error(handshake_data, rv);
        }

        handshake_data->handshake_state = ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CONNECTCLIENT_SENT;
        return ROBOTRACONTEURLITE_ERROR_RETRY;
    }
    case ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CONNECTCLIENT_SENT: {
        return ROBOTRACONTEURLITE_ERROR_RETRY;
    }
    case ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CONNECTCLIENT_COMPLETED: {
        handshake_data->handshake_state = ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_COMPLETED;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    default: {
        return robotraconteurlite_client_handshake_error(handshake_data);
    }
    }
}

robotraconteurlite_status robotraconteurlite_client_begin_request(
    struct robotraconteurlite_node_send_messageentry_data* send_data, robotraconteurlite_u16 entry_type,
    const char* membername, const char* servicepath)
{
    send_data->message_entry_header = &send_data->message_entry_header_storage;
    (void)memset(send_data->message_entry_header, 0, sizeof(struct robotraconteurlite_messageentry_const_header));
    send_data->message_entry_header->entry_type = entry_type;
    send_data->connection->last_request_id++;
    send_data->message_entry_header->request_id = send_data->connection->last_request_id;
    if (servicepath != NULL)
    {
        robotraconteurlite_string_from_c_str(servicepath, &send_data->message_entry_header->service_path);
    }
    else
    {
        robotraconteurlite_status rv = robotraconteurlite_string_shallow_copy_to(
            &send_data->connection->remote_service_name, &send_data->message_entry_header->service_path);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    robotraconteurlite_string_from_c_str(membername, &send_data->message_entry_header->member_name);
    return robotraconteurlite_node_begin_send_messageentry(send_data);
}

robotraconteurlite_status robotraconteurlite_client_send_request(
    struct robotraconteurlite_node_send_messageentry_data* send_data)
{
    return robotraconteurlite_node_end_send_messageentry(send_data);
}

robotraconteurlite_status robotraconteurlite_client_end_request(
    struct robotraconteurlite_node_send_messageentry_data* send_data, struct robotraconteurlite_event* event)
{
    if (event->event_type != ROBOTRACONTEURLITE_EVENT_TYPE_MESSAGE_RECEIVED)
    {
        return ROBOTRACONTEURLITE_ERROR_UNHANDLED_EVENT;
    }
    if (event->received_message.received_message_entry_header.request_id == send_data->message_entry_header->request_id)
    {
        if (event->received_message.received_message_entry_header.error != 0U)
        {
            return ROBOTRACONTEURLITE_ERROR_REQUEST_REMOTE_ERROR;
        }
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    return ROBOTRACONTEURLITE_ERROR_UNHANDLED_EVENT;
}

robotraconteurlite_status robotraconteurlite_client_send_empty_request(
    struct robotraconteurlite_node_send_messageentry_data* send_data, robotraconteurlite_u16 entry_type,
    const char* membername, const char* servicepath)
{
    robotraconteurlite_status rv =
        robotraconteurlite_client_begin_request(send_data, entry_type, membername, servicepath);
    if (FAILED(rv))
    {
        return rv;
    }
    return robotraconteurlite_node_end_send_messageentry(send_data);
}

robotraconteurlite_status robotraconteurlite_client_send_heartbeat(struct robotraconteurlite_node* node,
                                                                   struct robotraconteurlite_connection* connection)
{
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_node_send_messageentry_data send_data;
    send_data.node = node;
    send_data.connection = connection;
    rv = robotraconteurlite_client_begin_request(&send_data, ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CONNECTIONTEST, NULL,
                                                 NULL);
    if (FAILED(rv))
    {
        return rv;
    }
    send_data.message_entry_header->request_id = 0;
    rv = robotraconteurlite_node_end_send_messageentry(&send_data);
    if (FAILED(rv))
    {
        return rv;
    }
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_next_wake(struct robotraconteurlite_node* node,
                                                            robotraconteurlite_timespec now,
                                                            robotraconteurlite_timespec* wake_time)
{
    /* Check each connection */
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_connection* c = robotraconteurlite_connection_first(node->connections_head);
    if (*wake_time == 0)
    {
        *wake_time = now + ROBOTRACONTEURLITE_NODE_DEFAULT_SLEEP_TIME;
    }
    while (c != NULL)
    {
        rv = robotraconteurlite_connection_next_wake(c, now, wake_time);
        if (FAILED(rv))
        {
            return rv;
        }
        c = robotraconteurlite_connection_next(&c->head);
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_transport_parse_capabilities(
    struct robotraconteurlite_messageentry_reader* entry_reader, robotraconteurlite_u32* parsed_flags)
{
    robotraconteurlite_status rv = ROBOTRACONTEURLITE_ERROR_SUCCESS;
    struct robotraconteurlite_const_string capabilities_element_name_str;
    struct robotraconteurlite_messageelement_reader capabilities_element_reader;
    robotraconteurlite_string_from_c_str("capabilities", &capabilities_element_name_str);

    *parsed_flags = 0;

    rv = robotraconteurlite_messageentry_reader_find_element_verify_array(entry_reader, &capabilities_element_name_str,
                                                                          &capabilities_element_reader,
                                                                          ROBOTRACONTEURLITE_DATATYPE_UINT32, 1024, 1);

    if (rv == 0)
    {
        struct robotraconteurlite_messageelement_header capabilities_element_header;
        struct robotraconteurlite_messageelement_buffer_info capabilities_element_buffer_info;
        (void)memset(&capabilities_element_header, 0, sizeof(capabilities_element_header));
        (void)memset(&capabilities_element_buffer_info, 0, sizeof(capabilities_element_buffer_info));
        rv = robotraconteurlite_messageelement_reader_read_header_ex(
            &capabilities_element_reader, &capabilities_element_header, &capabilities_element_buffer_info);
        if (rv == 0)
        {
            robotraconteurlite_size_t i = 0;
            for (i = 0; i < capabilities_element_header.data_count; i++)
            {
                robotraconteurlite_u32 c = 0;
                rv = robotraconteurlite_buffer_vec_copy_to_uint32(
                    capabilities_element_reader.buffer,
                    capabilities_element_buffer_info.data_start_offset + (i * sizeof(robotraconteurlite_u32)), &c);
                if (FAILED(rv))
                {
                    return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
                }
                if (((c & ROBOTRACONTEURLITE_TRANSPORT_CAPABILITY_CODE_PAGE_MASK) ==
                     ROBOTRACONTEURLITE_TRANSPORT_CAPABILITY_CODE_MESSAGE2_BASIC_PAGE) &&
                    (FLAGS_CHECK(c, ROBOTRACONTEURLITE_TRANSPORT_CAPABILITY_CODE_MESSAGE2_BASIC_ENABLE)))
                {
                    FLAGS_SET(*parsed_flags, ROBOTRACONTEURLITE_CONNECTION_PARSE_CAPABILITY_MESSAGE2);
                }
                if (((c & ROBOTRACONTEURLITE_TRANSPORT_CAPABILITY_CODE_PAGE_MASK) ==
                     ROBOTRACONTEURLITE_TRANSPORT_CAPABILITY_CODE_MESSAGE4_BASIC_PAGE) &&
                    (FLAGS_CHECK(c, ROBOTRACONTEURLITE_TRANSPORT_CAPABILITY_CODE_MESSAGE4_BASIC_ENABLE)))
                {
                    /* We have message 4 capability! */

                    FLAGS_SET(*parsed_flags, ROBOTRACONTEURLITE_CONNECTION_PARSE_CAPABILITY_MESSAGE4);
                }
            }
        }
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_transport_populate_capabilities(
    struct robotraconteurlite_messageelement_writer* element_writer, robotraconteurlite_u32 capability_flags)
{
    robotraconteurlite_u32 caps_reply[2];
    robotraconteurlite_size_t caps_reply_len = 0;
    robotraconteurlite_status rv = -1;
    if (FLAGS_CHECK(capability_flags, ROBOTRACONTEURLITE_CONNECTION_PARSE_CAPABILITY_MESSAGE2))
    {
        caps_reply[caps_reply_len] = ROBOTRACONTEURLITE_TRANSPORT_CAPABILITY_CODE_MESSAGE2_BASIC_PAGE |
                                     ROBOTRACONTEURLITE_TRANSPORT_CAPABILITY_CODE_MESSAGE2_BASIC_ENABLE;
        caps_reply_len++;
    }
    if (FLAGS_CHECK(capability_flags, ROBOTRACONTEURLITE_CONNECTION_PARSE_CAPABILITY_MESSAGE4))
    {
        caps_reply[caps_reply_len] = ROBOTRACONTEURLITE_TRANSPORT_CAPABILITY_CODE_MESSAGE4_BASIC_PAGE |
                                     ROBOTRACONTEURLITE_TRANSPORT_CAPABILITY_CODE_MESSAGE4_BASIC_ENABLE;
        caps_reply_len++;
    }

    if (caps_reply_len > 0U)
    {
        struct robotraconteurlite_const_string capabilities_element_name_str;
        struct robotraconteurlite_array_uint32 capabilities_array;
        capabilities_array.data = caps_reply;
        capabilities_array.len = caps_reply_len;
        robotraconteurlite_string_from_c_str("capabilities", &capabilities_element_name_str);
        rv = robotraconteurlite_messageelement_writer_write_uint32_array(element_writer, &capabilities_element_name_str,
                                                                         &capabilities_array);
        return rv;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

#ifdef ROBOTRACONTEURLITE_HAVE_FUNCPTR
robotraconteurlite_status robotraconteurlite_node_set_services(
    struct robotraconteurlite_node* node, struct robotraconteurlite_node_service* services_head,
    struct robotraconteurlite_node_service_definition* service_defs_head)
{
    assert(node != NULL);
    node->services_head = services_head;
    node->service_defs_head = service_defs_head;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_set_ops(struct robotraconteurlite_node* node,
                                                          const struct robotraconteurlite_node_ops* ops)
{
    node->node_ops = ops;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_service_set_ops(struct robotraconteurlite_node_service* service,
                                                                  const struct robotraconteurlite_node_service_ops* ops)
{
    service->service_ops = ops;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_node_service_object_set_ops(
    struct robotraconteurlite_node_service_object* service_object,
    const struct robotraconteurlite_node_service_object_ops* ops)
{
    service_object->service_object_ops = ops;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_node_run_next_event__consume_event(
    struct robotraconteurlite_event* event, robotraconteurlite_status returned_rv)
{
    /* gracefully handle returned errors and retry */
    if (FAILED(returned_rv))
    {
        if (returned_rv == ROBOTRACONTEURLITE_ERROR_RETRY)
        {
            return ROBOTRACONTEURLITE_ERROR_SUCCESS;
        }
        /* TODO: already in error state, can't add to error */
        (void)robotraconteurlite_node_consume_event(event);
        return returned_rv;
    }
    else
    {
        return robotraconteurlite_node_consume_event(event);
    }
}

static robotraconteurlite_status robotraconteurlite_node_run_next_event__client(struct robotraconteurlite_event* event)
{
    /* TODO: implement */
    return robotraconteurlite_node_consume_event(event);
}

static robotraconteurlite_status robotraconteurlite_node_run_next_event__service(struct robotraconteurlite_event* event)
{
    struct robotraconteurlite_node_service* s = event->connection->service;
    struct robotraconteurlite_node_service_event s_evt;
    robotraconteurlite_status rv = -1;
    (void)memset(&s_evt, 0, sizeof(struct robotraconteurlite_node_service_event));
    s_evt.event = event;
    s_evt.service = s;
    rv = robotraconteurlite_node_event_special_request(event);
    if (FAILED(rv))
    {
        switch (rv)
        {
        case ROBOTRACONTEURLITE_ERROR_RETRY:
            return ROBOTRACONTEURLITE_ERROR_SUCCESS;
        case ROBOTRACONTEURLITE_ERROR_CONSUMED: {
            switch (event->received_message.received_message_entry_header.entry_type)
            {
            case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CONNECTCLIENT: {
                s = event->connection->service;
                if ((s != NULL) && (s->service_ops != NULL) && (s->service_ops->client_event != NULL))
                {
                    s_evt.service = s;
                    s->service_ops->client_event(&s_evt, ROBOTRACONTEURLITE_NODE_SERVICE_EVENT_TYPE_CLIENT_CONNECTED);
                }
                break;
            }
            case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_DISCONNECTCLIENT: {
                if ((s != NULL) && (s->service_ops != NULL) && (s->service_ops->client_event != NULL))
                {
                    s->service_ops->client_event(&s_evt,
                                                 ROBOTRACONTEURLITE_NODE_SERVICE_EVENT_TYPE_CLIENT_DISCONNECTED);
                }
                break;
            }
            default:
                break;
            }
            /* TODO: call service event handlers */
            return ROBOTRACONTEURLITE_ERROR_SUCCESS;
        }
        default: {
            return rv;
        }
        }
    }

    switch (event->received_message.received_message_entry_header.entry_type)
    {
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_GETSERVICEDESC: {
        (void)robotraconteurlite_node_consume_event(event);
        return robotraconteurlite_node_event_special_request_service_definition(event, event->node->services_head,
                                                                                event->node->service_defs_head);
    }
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_OBJECTTYPENAME: {
        (void)robotraconteurlite_node_consume_event(event);
        return robotraconteurlite_node_event_special_request_object_type_name2(event, event->node->services_head);
    }
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CLIENTKEEPALIVEREQ: {
        robotraconteurlite_status rv = robotraconteurlite_node_send_messageentry_empty_response(
            event->node, event->connection, &event->received_message.received_message_entry_header);
        if (RETRY(rv))
        {
            return ROBOTRACONTEURLITE_ERROR_RETRY;
        }

        (void)robotraconteurlite_node_consume_event(event);
        return rv;
    }
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_SERVICEPATHRELEASEDREQ: {
        (void)robotraconteurlite_node_consume_event(event);
        /* Don't need to do anything. Consume and return */
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    default:
        break;
    }

    if ((!FLAGS_CHECK(event->connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CLIENT_ESTABLISHED)) ||
        (event->connection->service == NULL))

    {
        /* not connected */
        (void)robotraconteurlite_node_consume_event(event);
        return robotraconteurlite_connection_send_messageentry_error_response(
            event->node, event->connection, &event->received_message.received_message_entry_header,
            ROBOTRACONTEURLITE_MESSAGEERRORTYPE_PROTOCOLERROR, "RobotRaconteur.ProtocolError",
            "Service connection not established");
    }
    {
        struct robotraconteurlite_node_service_object* s_obj = robotraconteurlite_node_find_service_object_for_path(
            &s->service_objects_head, &event->received_message.received_message_entry_header.service_path);

        if (s_obj == NULL)
        {
            (void)robotraconteurlite_node_consume_event(event);
            return robotraconteurlite_connection_send_messageentry_error_response(
                event->node, event->connection, &event->received_message.received_message_entry_header,
                ROBOTRACONTEURLITE_MESSAGEERRORTYPE_OBJECTNOTFOUND, "RobotRaconteur.ObjectNotFound",
                "Object not found");
        }

        if ((s_obj->service_object_ops != NULL) && (s_obj->service_object_ops->message_received != NULL))
        {
            s_evt.service_object = s_obj;
            rv = s_obj->service_object_ops->message_received(&s_evt);
            return robotraconteurlite_node_run_next_event__consume_event(event, rv);
        }
        else
        {
            (void)robotraconteurlite_node_consume_event(event);
            return ROBOTRACONTEURLITE_ERROR_NOT_IMPLEMENTED;
        }
    }
}

static robotraconteurlite_status robotraconteurlite_node_run_next_event__client_connection_error(
    struct robotraconteurlite_event* event)
{
    /* TODO: implement */
    return robotraconteurlite_node_consume_event(event);
}

static robotraconteurlite_status robotraconteurlite_node_run_next_event__service_connection_error(
    struct robotraconteurlite_event* event)
{

    if (FLAGS_CHECK(event->connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CLIENT_ESTABLISHED))
    {
        struct robotraconteurlite_node_service* s = event->connection->service;
        FLAGS_CLEAR(event->connection->connection_state, ROBOTRACONTEURLITE_STATUS_FLAGS_CLIENT_ESTABLISHED);
        event->connection->service = NULL;

        /* TODO: fail requests */

        if ((s != NULL) && (s->service_ops != NULL) && (s->service_ops->client_event != NULL))
        {
            struct robotraconteurlite_node_service_event s_evt;
            (void)memset(&s_evt, 0, sizeof(struct robotraconteurlite_node_service_event));
            s_evt.event = event;
            s_evt.service = s;
            s->service_ops->client_event(&s_evt, ROBOTRACONTEURLITE_NODE_SERVICE_EVENT_TYPE_CLIENT_DISCONNECTED);
        }
    }

    /* TODO: report unhandled message? */
    return robotraconteurlite_node_consume_event(event);
}

robotraconteurlite_status robotraconteurlite_node_run_next_event(struct robotraconteurlite_node* node,
                                                                 robotraconteurlite_timespec now,
                                                                 enum robotraconteurlite_event_type* handled_event_type)
{
    struct robotraconteurlite_event event;
    robotraconteurlite_status rv = -1;

    rv = robotraconteurlite_node_next_event(node, &event, now);
    if (FAILED(rv))
    {
        return rv;
    }

    *handled_event_type = event.event_type;

    switch (event.event_type)
    {
    case ROBOTRACONTEURLITE_EVENT_TYPE_NOOP:
    case ROBOTRACONTEURLITE_EVENT_TYPE_NEXT_CYCLE: {
        /* do nothing, return to user */
        return robotraconteurlite_node_consume_event(&event);
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_ERROR:
    case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_TIMEOUT: {
        /* report to service or client */
        (void)robotraconteurlite_connection_close(event.connection);
        if (robotraconteurlite_connection_is_server(event.connection) != 0)
        {
            (void)robotraconteurlite_node_run_next_event__service_connection_error(&event);
        }
        else
        {
            (void)robotraconteurlite_node_run_next_event__client_connection_error(&event);
        }
        if ((node->node_ops != NULL) && (node->node_ops->connection_event != NULL))
        {
            rv = node->node_ops->connection_event(&event);
            return robotraconteurlite_node_run_next_event__consume_event(&event, rv);
        }
        else
        {
            /* no ops, return to user */
            return robotraconteurlite_node_consume_event(&event);
        }
        break;
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_CONNECTED:
    case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_CLOSED: {
        if ((node->node_ops != NULL) && (node->node_ops->connection_event != NULL))
        {
            rv = node->node_ops->connection_event(&event);
            return robotraconteurlite_node_run_next_event__consume_event(&event, rv);
        }
        else
        {
            /* no ops, return to user */
            return robotraconteurlite_node_consume_event(&event);
        }
        break;
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_HEARTBEAT_TIMEOUT: {
        /* send a connection test packet */
        (void)robotraconteurlite_client_send_heartbeat(event.node, event.connection);
        return robotraconteurlite_node_consume_event(&event);
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_MESSAGE_SEND_COMPLETE: {
        if ((node->node_ops != NULL) && (node->node_ops->send_complete != NULL))
        {
            rv = node->node_ops->send_complete(&event);
            return robotraconteurlite_node_run_next_event__consume_event(&event, rv);
        }
        else
        {
            /* no ops, return to user */
            return robotraconteurlite_node_consume_event(&event);
        }
        break;
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_MESSAGE_RECEIVED: {
        if (robotraconteurlite_connection_is_server(event.connection) != 0)
        {
            return robotraconteurlite_node_run_next_event__service(&event);
        }
        else
        {
            return robotraconteurlite_node_run_next_event__client(&event);
        }
        break;
    }
    default:
        break;
    }

    /* Unknown event */
    (void)robotraconteurlite_node_consume_event(&event);
    return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
}

#endif
