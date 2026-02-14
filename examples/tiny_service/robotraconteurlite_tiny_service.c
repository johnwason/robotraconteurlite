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

robotraconteurlite_status tiny_object_handle_message(struct robotraconteurlite_node_service_event* s_evt)
{
    switch (s_evt->event->received_message.received_message_entry_header.entry_type)
    {
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PROPERTYGETREQ: {
        if (robotraconteurlite_node_event_is_member(s_evt->event, "d1"))
        {
            robotraconteurlite_double d1 = 1.234;
            struct robotraconteurlite_node_send_messageentry_data send_data;
            robotraconteurlite_status rv = -1;
            send_data.node = s_evt->event->node;
            send_data.connection = s_evt->event->connection;
            rv = robotraconteurlite_node_begin_send_messageentry_response(
                &send_data, &s_evt->event->received_message.received_message_entry_header);
            if (RETRY(rv))
            {
                return ROBOTRACONTEURLITE_ERROR_RETRY;
            }
            if (RRLITE_FAILED(rv))
            {
                printf("Could not begin send message entry response\n");
                return -1;
            }

            if (robotraconteurlite_messageelement_writer_write_double_c_str(&send_data.element_writer, "value", d1))
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
                return rv;
            }
            return rv;
        }
        else
        {
            printf("Unknown property get request, responding with error\n");
            /* Send error response */
            return robotraconteurlite_node_event_respond_member_not_found(s_evt->event);
        }
    }
    case ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PROPERTYSETREQ: {
        if (robotraconteurlite_node_event_is_member(s_evt->event, "d1"))
        {
            /* Find "value" message element */
            struct robotraconteurlite_messageelement_reader element_reader;
            robotraconteurlite_status rv = -1;
            robotraconteurlite_double d1 = 0.0;

            rv = robotraconteurlite_messageentry_reader_find_element_verify_scalar_c_str(
                &s_evt->event->received_message.entry_reader, "value", &element_reader,
                ROBOTRACONTEURLITE_DATATYPE_DOUBLE);

            if (RRLITE_FAILED(rv))
            {
                printf("Could not find element or type mismatch\n");
                return robotraconteurlite_node_event_respond_element_read_error(s_evt->event, rv);
            }

            rv = robotraconteurlite_messageelement_reader_read_data_double(&element_reader, &d1);
            if (RRLITE_FAILED(rv))
            {
                printf("Could not read double\n");
                /* Send error response */
                return robotraconteurlite_node_event_respond_element_read_error(s_evt->event, rv);
            }

            printf("Got set d1=%f\n", d1);

            /* Send empty response */
            rv = robotraconteurlite_node_send_messageentry_empty_response(
                s_evt->event->node, s_evt->event->connection,
                &s_evt->event->received_message.received_message_entry_header);
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
            return robotraconteurlite_node_event_respond_member_not_found(s_evt->event);
        }
    }
    default: {
        printf("Could not handle message, responding with error\n");
        /* Send error response */
        return robotraconteurlite_node_event_respond_member_not_found(s_evt->event);
    }
    }

    return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
}

void tiny_service_service_client_event(struct robotraconteurlite_node_service_event* event,
                                       enum robotraconteurlite_node_service_event_type event_type)
{
    ROBOTRACONTEURLITE_UNUSED(event);
    switch (event_type)
    {
    case ROBOTRACONTEURLITE_NODE_SERVICE_EVENT_TYPE_CLIENT_CONNECTED:
        printf("Client connected\n");
        break;
    case ROBOTRACONTEURLITE_NODE_SERVICE_EVENT_TYPE_CLIENT_DISCONNECTED:
        printf("Client disconnected\n");
        break;
    default:
        break;
    }
}

/* cppcheck-suppress constParameterCallback */
robotraconteurlite_status tiny_service_connection_event(struct robotraconteurlite_event* event)
{
    printf("Connection event: %d\n", event->event_type);
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

/* cppcheck-suppress constParameterCallback */
robotraconteurlite_status tiny_service_send_complete(struct robotraconteurlite_event* event)
{
    ROBOTRACONTEURLITE_UNUSED(event);
    printf("Send complete event\n");
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

/* cppcheck-suppress constParameterPointer */
robotraconteurlite_status tiny_service_event_error_returned(struct robotraconteurlite_event* event)
{
    printf("Event error returned: %d\n", event->event_error_code);
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

const struct robotraconteurlite_node_service_object_ops tiny_object_ops = {tiny_object_handle_message};

const struct robotraconteurlite_node_service_ops tiny_service_ops = {tiny_service_service_client_event};

const struct robotraconteurlite_node_ops tiny_node_ops = {tiny_service_connection_event, tiny_service_send_complete,
                                                          tiny_service_event_error_returned};

volatile sig_atomic_t signal_received = 0;

void signal_handler(int signum)
{
    ROBOTRACONTEURLITE_UNUSED(signum);
    signal_received = 1;
}

int main(int argc, const char* argv[])
{
    /* Variable storage */
    struct robotraconteurlite_connection connections_storage[NUM_CONNECTIONS];
    robotraconteurlite_byte connection_buffers[NUM_CONNECTIONS * 2 * CONNECTION_BUFFER_SIZE];
    struct robotraconteurlite_connection_object connections_head;
    struct robotraconteurlite_connection_acceptor tcp_acceptor;
    struct robotraconteurlite_node node;
    struct sockaddr_in listen_addr;
    struct robotraconteurlite_nodeid node_id;
    struct robotraconteurlite_const_string node_name;
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
        struct robotraconteurlite_const_string nodeid_str_s;
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
    robotraconteurlite_node_service_set_ops(&service, &tiny_service_ops);
    robotraconteurlite_node_service_object_set_ops(&service_obj, &tiny_object_ops);

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

    robotraconteurlite_node_set_ops(&node, &tiny_node_ops);
    (void)robotraconteurlite_node_set_services(&node, &services_head, &service_defs_head);

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

        robotraconteurlite_clock_gettime(&clock, &now);

        rv = robotraconteurlite_poll_connections_run(&node, &clock, pollfds, NUM_CONNECTIONS + 2, now + 1000000);
        if (RRLITE_FAILED(rv))
        {
            printf("Run poll connections failed\n");
            return 1;
        }

        rv = robotraconteurlite_node_run_events_available(&node, now, 100, 10);
        if (RRLITE_FAILED(rv))
        {
            printf("Node events failed\n");
            return 1;
        }

        if (signal_received)
        {
            break;
        }
    } while (1);

    robotraconteurlite_clock_gettime(&clock, &now);

    /* Close all connection objects */
    robotraconteurlite_connections_close(&connections_head, now);
    printf("robotraconteur_tiny_service shut down\n");
    return 0;
}
