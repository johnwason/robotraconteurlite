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
/* NOLINTNEXTLINE(bugprone-reserved-identifier) */
#define _XOPEN_SOURCE 500
#endif
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#ifndef _WIN32
#include <netinet/in.h>
#include <unistd.h>
#else
#include <winsock2.h>
#endif

#include <robotraconteurlite/robotraconteurlite.h>

#define NUM_CONNECTIONS 4
#define CONNECTION_BUFFER_SIZE 8096

/* Win32 defines FAILED macro. Use RRLITE_FAILED if Windows support required */
#define RRLITE_FAILED ROBOTRACONTEURLITE_FAILED
#define RETRY ROBOTRACONTEURLITE_RETRY

const char* node_name_str = "example.tiny_service";
const robotraconteurlite_u16 node_port = 22228;
/* Node ID should be different for all instances. Hard coded for example only. */
const char* default_nodeid_str = "c22551ad-f41e-43b8-9f78-2fb80118ea3c";

const char* service_name = "tiny_service";
const char* service_def_str = "service example.tiny_service\n\n"
                              "option version 0.10\n\n"
                              "object tiny_object\n"
                              "property double d1\n"
                              "end\n\n";
const char* service_def_qualified_name = "example.tiny_service";
const char* root_object_type = "example.tiny_service.tiny_object";

int handle_message(struct robotraconteurlite_node* node, struct robotraconteurlite_event* event,
                   struct robotraconteurlite_node_service* services_head,
                   struct robotraconteurlite_node_service_definition* service_defs_head)
{
    robotraconteurlite_status rv = robotraconteurlite_node_event_special_request(node, event);
    if (RRLITE_FAILED(rv))
    {
        if (rv == ROBOTRACONTEURLITE_ERROR_CONSUMED || rv == ROBOTRACONTEURLITE_ERROR_RETRY)
        {
            return 0;
        }
        printf("Could not handle special request\n");
        return -1;
    }

    switch (event->received_message.received_message_entry_header.entry_type)
    {
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_GETSERVICEDESC: {
        return robotraconteurlite_node_event_special_request_service_definition(node, event, services_head,
                                                                                service_defs_head);
    }
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_OBJECTTYPENAME: {
        return robotraconteurlite_node_event_special_request_object_type_name2(node, event, services_head);
    }
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CLIENTKEEPALIVEREQ: {
        robotraconteurlite_status rv = robotraconteurlite_node_send_messageentry_empty_response(
            node, event->connection, &event->received_message.received_message_entry_header);
        if (RETRY(rv))
        {
            return ROBOTRACONTEURLITE_ERROR_RETRY;
        }

        return rv;
    }
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_SERVICEPATHRELEASEDREQ: {
        /* Don't need to do anything. Consume and return */
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PROPERTYGETREQ: {
        if (robotraconteurlite_event_is_member(event, "tiny_service", "d1"))
        {
            robotraconteurlite_double d1 = 1.234;
            struct robotraconteurlite_node_send_messageentry_data send_data;
            struct robotraconteurlite_string element_name;
            robotraconteurlite_status rv = -1;
            send_data.node = node;
            send_data.connection = event->connection;
            rv = robotraconteurlite_node_begin_send_messageentry_response(
                &send_data, &event->received_message.received_message_entry_header);
            if (RETRY(rv))
            {
                return ROBOTRACONTEURLITE_ERROR_RETRY;
            }
            if (RRLITE_FAILED(rv))
            {
                printf("Could not begin send message entry response\n");
                return -1;
            }

            robotraconteurlite_string_from_c_str("value", &element_name);
            if (robotraconteurlite_messageelement_writer_write_double(&send_data.element_writer, &element_name, d1))
            {
                printf("Could not write double\n");
                return -1;
            }
            rv = robotraconteurlite_node_end_send_messageentry(&send_data);
            if (RETRY(rv))
            {
                return ROBOTRACONTEURLITE_ERROR_RETRY;
            }
            if (RRLITE_FAILED(rv))
            {
                printf("Could not end send message entry response\n");
                return -1;
            }
            return rv;
        }
        else
        {
            if (robotraconteurlite_node_consume_event(node, event))
            {
                printf("Could not consume event\n");
                return -1;
            }
            printf("Unknown property get request, responding with error\n");
            /* Send error response */
            return robotraconteurlite_connection_send_messageentry_error_response(
                node, event->connection, &event->received_message.received_message_entry_header,
                ROBOTRACONTEURLITE_MESSAGEERRORTYPE_INVALIDOPERATION, "RobotRaconteur.InvalidOperation",
                "Invalid operation");
        }
    }
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PROPERTYSETREQ: {
        if (robotraconteurlite_event_is_member(event, "tiny_service", "d1"))
        {
            /* Find "value" message element */

            struct robotraconteurlite_string element_name;
            struct robotraconteurlite_messageelement_reader element_reader;
            robotraconteurlite_status rv = -1;
            robotraconteurlite_double d1 = 0.0;
            robotraconteurlite_string_from_c_str("value", &element_name);

            rv = robotraconteurlite_messageentry_reader_find_element_verify_scalar(
                &event->received_message.entry_reader, &element_name, &element_reader,
                ROBOTRACONTEURLITE_DATATYPE_DOUBLE);

            if (rv == ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_NOT_FOUND ||
                rv == ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_TYPE_MISMATCH)
            {
                printf("Could not find element or type mismatch\n");
                /* Send error response */
                return robotraconteurlite_connection_send_messageentry_error_response(
                    node, event->connection, &event->received_message.received_message_entry_header,
                    ROBOTRACONTEURLITE_MESSAGEERRORTYPE_INVALIDOPERATION, "RobotRaconteur.InvalidOperation",
                    "Invalid operation");
            }

            if (robotraconteurlite_messageelement_reader_read_data_double(&element_reader, &d1))
            {
                printf("Could not read double\n");
                /* Send error response */
                return robotraconteurlite_connection_send_messageentry_error_response(
                    node, event->connection, &event->received_message.received_message_entry_header,
                    ROBOTRACONTEURLITE_MESSAGEERRORTYPE_INVALIDOPERATION, "RobotRaconteur.InvalidOperation",
                    "Invalid operation");
            }

            printf("Got set d1=%f\n", d1);

            /* Send empty response */
            rv = robotraconteurlite_node_send_messageentry_empty_response(
                node, event->connection, &event->received_message.received_message_entry_header);
            if (RETRY(rv))
            {
                return ROBOTRACONTEURLITE_ERROR_RETRY;
            }
            return rv;
        }
        else
        {
            printf("Unknown property set request, responding with error\n");

            /* Send error response */
            return robotraconteurlite_connection_send_messageentry_error_response(
                node, event->connection, &event->received_message.received_message_entry_header,
                ROBOTRACONTEURLITE_MESSAGEERRORTYPE_INVALIDOPERATION, "RobotRaconteur.InvalidOperation",
                "Invalid operation");
        }
    }
    default: {
        printf("Could not handle message, responding with error\n");
        /* Send error response */
        return robotraconteurlite_connection_send_messageentry_error_response(
            node, event->connection, &event->received_message.received_message_entry_header,
            ROBOTRACONTEURLITE_MESSAGEERRORTYPE_INVALIDOPERATION, "RobotRaconteur.InvalidOperation",
            "Invalid operation");
    }
    }

    return 0;
}

int handle_event(struct robotraconteurlite_node* node, struct robotraconteurlite_event* event,
                 struct robotraconteurlite_node_service* services_head,
                 struct robotraconteurlite_node_service_definition* service_defs_head)
{
    switch (event->event_type)
    {
    case ROBOTRACONTEURLITE_EVENT_TYPE_NOOP: {
        printf("Noop event\n");
        /* Consume the event */
        if (robotraconteurlite_node_consume_event(node, event))
        {
            printf("Could not consume event\n");
            return -1;
        }
        break;
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_NEXT_CYCLE: {
        if (robotraconteurlite_node_consume_event(node, event))
        {
            printf("Could not consume event\n");
            return -1;
        }
        /* End of cycle, exit loop */
        return 1;
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_MESSAGE_RECEIVED: {
        robotraconteurlite_status rv = -1;
        printf("Message received\n");
        /* Handle the message */
        rv = handle_message(node, event, services_head, service_defs_head);
        if (RETRY(rv))
        {
            return 0;
        }
        if (RRLITE_FAILED(rv))
        {
            printf("Could not handle message\n");
            return -1;
        }
        /* Consume the event */
        if (robotraconteurlite_node_consume_event(node, event))
        {
            printf("Could not consume event\n");
            return -1;
        }
        break;
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_MESSAGE_SEND_COMPLETE: {
        printf("Message sent\n");
        /* Consume the event */
        if (robotraconteurlite_node_consume_event(node, event))
        {
            printf("Could not consume event\n");
            return -1;
        }
        break;
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_HEARTBEAT_TIMEOUT: {
        printf("Connection heartbeat timeout, sending heartbeat\n");
        robotraconteurlite_client_send_heartbeat(node, event->connection);
        if (robotraconteurlite_node_consume_event(node, event))
        {
            printf("Could not consume event\n");
            return -1;
        }
        break;
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_TIMEOUT: {
        printf("Connection timed out\n");
        /* Close the connection on timeout */
        if (robotraconteurlite_connection_close(event->connection))
        {
            printf("Could not close connection\n");
            return -1;
        }
        if (robotraconteurlite_node_consume_event(node, event))
        {
            printf("Could not consume event\n");
            return -1;
        }
        break;
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_ERROR: {
        /* Close the connection on error */
        if (robotraconteurlite_connection_close(event->connection))
        {
            printf("Could not close connection\n");
            return -1;
        }
        if (robotraconteurlite_node_consume_event(node, event))
        {
            printf("Could not consume event\n");
            return -1;
        }
        break;
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_CONNECTED: {
        printf("Connection connected!\n");
        /* Consume the event */
        if (robotraconteurlite_node_consume_event(node, event))
        {
            printf("Could not consume event\n");
            return -1;
        }
        break;
    }
    case ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_CLOSED: {
        printf("Connection closed!\n");
        /* Consume the event */
        if (robotraconteurlite_node_consume_event(node, event))
        {
            printf("Could not consume event\n");
            return -1;
        }
        break;
    }
    default: {
        printf("Unexpected event type\n");
        return -1;
    }
    }
    return 0;
}

volatile sig_atomic_t signal_received = 0;

void signal_handler(int signum)
{
    ROBOTRACONTEURLITE_UNUSED(signum);
    signal_received = 1;
}

int main(int argc, char* argv[])
{
    /* Variable storage */
    struct robotraconteurlite_connection connections_storage[NUM_CONNECTIONS];
    robotraconteurlite_byte connection_buffers[NUM_CONNECTIONS * 2 * CONNECTION_BUFFER_SIZE];
    struct robotraconteurlite_connection_object connections_head;
    struct robotraconteurlite_connection_acceptor tcp_acceptor;
    struct robotraconteurlite_node node;
    struct sockaddr_in listen_addr;
    struct robotraconteurlite_nodeid node_id;
    struct robotraconteurlite_string node_name;
    struct robotraconteurlite_clock clock;
    robotraconteurlite_timespec now = 0;
    const char* nodeid_str = default_nodeid_str;
    struct robotraconteurlite_node_service_definition service_defs_head;
    struct robotraconteurlite_node_service_definition service_def;
    struct robotraconteurlite_node_service services_head;
    struct robotraconteurlite_node_service service;
    struct robotraconteurlite_node_service_object service_obj;

#ifndef _WIN32
    struct sigaction sa;
#endif

    if (argc > 1)
    {
        nodeid_str = argv[1];
    }

#ifndef _WIN32
    /* Disable sigpipe. This is a common source of errors. Some libraries will disable this for you, but not all. */
    /* robotraconteurlite does not automatically disable sigpipe. */
    signal(SIGPIPE, SIG_IGN);

    /* Listen for SIGINT and SIGTERM to break loop */
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
#else
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Could not initialize Winsock\n");
        return -1;
    }
#endif

    /* Seed rand with the current time */
    srand(time(NULL));

    /* Initialize the clock */
    robotraconteurlite_clock_init(&clock);

    /* Read the clock */
    robotraconteurlite_clock_gettime(&clock, &now);

    /* Load the nodeid from string */
    {
        struct robotraconteurlite_string nodeid_str_s;
        robotraconteurlite_string_from_c_str(nodeid_str, &nodeid_str_s);
        if (RRLITE_FAILED(robotraconteurlite_nodeid_parse(&nodeid_str_s, &node_id)))
        {
            printf("Could not parse nodeid\n");
            return -1;
        }
    }

    /* Construct service definitions */
    robotraconteurlite_node_service_definition_list_head_construct(&service_defs_head);
    robotraconteurlite_node_service_definition_construct_c_str(
        &service_def, service_def_qualified_name, service_def_str, service_def_qualified_name, &service_defs_head);

    /* Construct service objects */
    robotraconteurlite_node_service_list_head_construct(&services_head);
    robotraconteurlite_node_service_construct_c_str(&service, service_name, &services_head);
    robotraconteurlite_node_service_object_construct_c_str(&service_obj, service_name, root_object_type, NULL, NULL);
    robotraconteurlite_node_service_set_root_object(&service, &service_obj);

    /* Construct the connection object head */
    robotraconteurlite_connection_list_head_construct(&connections_head);

    /* Initialize connections and TCP transport */
    robotraconteurlite_connections_construct_from_array(
        connections_storage, NUM_CONNECTIONS, connection_buffers, CONNECTION_BUFFER_SIZE,
        (robotraconteurlite_size_t)(NUM_CONNECTIONS * 2), &connections_head);

    robotraconteurlite_tcp_acceptor_construct(&tcp_acceptor, &connections_head);

    /* Init connection objects*/
    robotraconteurlite_connection_init_connection_acceptor(&tcp_acceptor);
    robotraconteurlite_connection_init_connections(&connections_head);

    /* Initialize the node */

    robotraconteurlite_string_from_c_str(node_name_str, &node_name);

    if (robotraconteurlite_node_init(&node, &node_id, &node_name, &connections_head))
    {
        printf("Could not initialize node\n");
        return -1;
    }

    /* Start TCP acceptor */
    (void)memset(&listen_addr, 0, sizeof(listen_addr));
    /* Implicit listen address of 0.0.0.0, or all interfaces */
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(node_port);

    if (robotraconteurlite_tcp_acceptor_listen(&tcp_acceptor, (struct sockaddr_storage*)&listen_addr, 4))
    {
        printf("Could not start TCP acceptor\n");
        return -1;
    }

    printf("robotraconteur_tiny_service started\n");

    do
    {
        /* One socket per connection plus acceptor and node. May vary, check documentation */
        struct robotraconteurlite_pollfd pollfds[NUM_CONNECTIONS + 2];
        robotraconteurlite_status rv = -1;
        robotraconteurlite_timespec next_wake = 0;

        robotraconteurlite_clock_gettime(&clock, &now);

        /* Communicate with all connections */
        if (robotraconteurlite_tcp_connections_communicate(&connections_head, now))
        {
            printf("Could not communicate with connections\n");
            return -1;
        }
        rv = robotraconteurlite_node_next_wake(&node, now, &next_wake);
        if (RRLITE_FAILED(rv))
        {
            printf("Could not get next wake\n");
            return -1;
        }

        if (next_wake > now)
        {
            rv = robotraconteurlite_tcp_connections_prepare_wait(&connections_head);
            if (RRLITE_FAILED(rv))
            {
                printf("Could not add acceptor to poll\n");
                return -1;
            }

            rv = robotraconteurlite_poll_connections_next_wake(&connections_head, &clock, pollfds, NUM_CONNECTIONS + 2,
                                                               next_wake);
            if (RRLITE_FAILED(rv))
            {
                if (signal_received)
                {
                    printf("Exiting\n");
                    return 0;
                }
                printf("Could not wait for next wake\n");
                return -1;
            }
            robotraconteurlite_clock_gettime(&clock, &now);

            /* Communicate with all connections */
            if (robotraconteurlite_tcp_connections_communicate(&connections_head, now))
            {
                printf("Could not communicate with connections\n");
                return -1;
            }
        }

        /* Run the event loop. Exit if no events are available*/

        do
        {
            struct robotraconteurlite_event event;
            robotraconteurlite_status rv = -1;
            robotraconteurlite_clock_gettime(&clock, &now);
            rv = robotraconteurlite_node_next_event(&node, &event, now);
            if (RRLITE_FAILED(rv))
            {
                printf("Could not get event\n");
                return -1;
            }

            rv = handle_event(&node, &event, &services_head, &service_defs_head);
            if (rv == 1)
            {
                break;
            }
            else if (RRLITE_FAILED(rv))
            {
                if (rv != ROBOTRACONTEURLITE_ERROR_RETRY)
                {
                    printf("Could not handle event\n");
                    return -1;
                }
            }
        } while (1);

        if (signal_received)
        {
            break;
        }

    } while (1);

    /* Close all connections */
    robotraconteurlite_tcp_connections_close(&connections_head);

    /* Close the acceptor */
    robotraconteurlite_tcp_acceptor_close(&tcp_acceptor);

    printf("robotraconteur_tiny_service shut down\n");

    return 0;
}
