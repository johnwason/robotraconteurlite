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
#include <arpa/inet.h>
#include <errno.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <robotraconteurlite/robotraconteurlite.h>

#define NUM_CONNECTIONS 1
#define CONNECTION_BUFFER_SIZE 8096

/* Win32 defines FAILED and SUCCEEDED macro. Use RRLITE_FAILED
and RRLITE_SUCCEEDED if Windows support required */
#define RRLITE_FAILED ROBOTRACONTEURLITE_FAILED
#define RRLITE_SUCCEEDED ROBOTRACONTEURLITE_SUCCEEDED
#define RETRY ROBOTRACONTEURLITE_RETRY

/* #define TINY_CLIENT_WEBSOCKET 1 */

const robotraconteurlite_u16 default_service_port = 22229;
const char* default_service_ip_str = "127.0.0.1";
const char* service_name = "tiny_service";
const char* expected_root_object_type = "example.tiny_service.tiny_object";

struct tiny_client_data
{
    struct robotraconteurlite_node_client* client;
    robotraconteurlite_i32 keep_going;
    robotraconteurlite_i32 done;
};

static void client_connected(struct robotraconteurlite_node_client_event* event);
static void client_disconnected(struct robotraconteurlite_node_client_event* event);

const struct robotraconteurlite_node_client_ops client_ops = {client_connected, client_disconnected, NULL, NULL, NULL};

struct robotraconteurlite_clock rr_clock;

int main(int argc, const char* argv[])
{
    /* Variable storage */
    struct robotraconteurlite_connection connections_storage[NUM_CONNECTIONS];
    robotraconteurlite_byte connection_buffers[NUM_CONNECTIONS * 2 * CONNECTION_BUFFER_SIZE];
    struct robotraconteurlite_connection_object connections_head;
    struct robotraconteurlite_node node;
    struct robotraconteurlite_nodeid node_id;
    struct robotraconteurlite_addr service_addr;
    struct sockaddr_in* service_sockaddr = NULL;
    struct robotraconteurlite_node_client client;
    robotraconteurlite_timespec now = 0;
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_const_string nodename_str;
    struct tiny_client_data data;
    struct robotraconteurlite_user_storage data_storage;
    struct robotraconteurlite_node_request requests_head;

    const char* service_ip_str = NULL;
    robotraconteurlite_u16 service_port = 0;
    int use_ws = 0;

    if (argc > 1)
    {
        service_ip_str = argv[1];
    }
    else
    {
        service_ip_str = default_service_ip_str;
    }

    if (argc > 2)
    {
        long temp_port = 0;
        errno = 0;
        temp_port = strtol(argv[2], NULL, 10);
        if (errno != 0 || temp_port < 0 || temp_port > 65535)
        {
            printf("Invalid port number\n");
            return -1;
        }
        service_port = (robotraconteurlite_u16)temp_port;
    }
    else
    {
        service_port = default_service_port;
    }

    if (argc > 3)
    {
        if (strcmp(argv[3], "ws") == 0)
        {
            use_ws = 1;
        }
    }

#ifndef _WIN32
    /* Disable sigpipe. This is a common source of errors. Some libraries will disable this for you, but not all. */
    /* robotraconteurlite does not automatically disable sigpipe. */
    signal(SIGPIPE, SIG_IGN);
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
    robotraconteurlite_clock_init(&rr_clock);

    /* Use a random nodeid. In practice this should be fixed and unique for every service */
    robotraconteurlite_nodeid_newrandom(&node_id);

    /* Initialize connections and TCP transport */
    robotraconteurlite_connection_list_head_construct(&connections_head);
    robotraconteurlite_connections_construct_from_array(
        connections_storage, NUM_CONNECTIONS, connection_buffers, CONNECTION_BUFFER_SIZE,
        (robotraconteurlite_size_t)(NUM_CONNECTIONS * 2), &connections_head);

    robotraconteurlite_connection_init_connections(&connections_head);

    /* Initialize the node */
    robotraconteurlite_string_from_c_str("tiny_client", &nodename_str);
    robotraconteurlite_node_init(&node, &node_id, &nodename_str, &connections_head);
    robotraconteurlite_node_request_list_head_construct(&requests_head);
    robotraconteurlite_node_set_requests_head(&node, &requests_head);

    /* Connect to the service */
    (void)memset(&service_addr, 0, sizeof(service_addr));
    robotraconteurlite_string_from_c_str(service_name, &service_addr.service_name);
    service_sockaddr = (struct sockaddr_in*)&service_addr.socket_addr;
    service_sockaddr->sin_family = AF_INET;
    service_sockaddr->sin_port = robotraconteurlite_htons(service_port);
    if (inet_pton(AF_INET, service_ip_str, &service_sockaddr->sin_addr) != 1)
    {
        printf("Could not convert service IP address\n");
        return -1;
    }

    if (use_ws != 0)
    {
        /* Use websocket connection */
        ROBOTRACONTEURLITE_FLAGS_SET(service_addr.flags, ROBOTRACONTEURLITE_ADDR_FLAGS_WEBSOCKET);
        robotraconteurlite_string_from_c_str("127.0.0.1", &service_addr.http_host);
        robotraconteurlite_string_from_c_str("/", &service_addr.http_path);
    }

    (void)memset(&data, 0, sizeof(data));

    (void)memset(&client, 0, sizeof(client));
    client.node = &node;
    client.service_address = &service_addr;
    robotraconteurlite_string_from_c_str(expected_root_object_type, &client.expected_root_object_type);
    data.client = &client;
    data.keep_going = 1;
    (void)memset(&data_storage, 0, sizeof(data_storage));
    data_storage.user_data = &data;
    client.user_storage = &data_storage;

    robotraconteurlite_node_client_set_ops(&client, &client_ops);

    printf("Begin connecting to service\n");

    robotraconteurlite_clock_gettime(&rr_clock, &now);
    rv = robotraconteurlite_tcp_connect_service(&client, now);

    if (RRLITE_FAILED(rv))
    {
        printf("Could not connect to service\n");
        return -1;
    }

    while (data.keep_going)
    {
        struct robotraconteurlite_pollfd pollfds[NUM_CONNECTIONS + 2];

        robotraconteurlite_clock_gettime(&rr_clock, &now);

        rv = robotraconteurlite_poll_connections_run(&node, &rr_clock, pollfds, NUM_CONNECTIONS + 2, now + 1000000);
        if (RRLITE_FAILED(rv))
        {
            printf("Run poll connections failed\n");
            return 1;
        }

        robotraconteurlite_clock_gettime(&rr_clock, &now);
        rv = robotraconteurlite_node_run_events_available(&node, now, 100, 10);
        if (RRLITE_FAILED(rv))
        {
            printf("Node events failed\n");
            return 1;
        }
    }

    /* TODO: Drain the connection */
    robotraconteurlite_clock_gettime(&rr_clock, &now);
    robotraconteurlite_connections_communicate(&connections_head, now);

    if (!data.done)
    {
        printf("Test failure occurred");
        return 1;
    }
    else
    {
        printf("Done!\n");
    }

    return 0;
}

static struct tiny_client_data* get_tiny_client_data(struct robotraconteurlite_user_storage* storage)
{
    return (struct tiny_client_data*)storage->user_data;
}

struct robotraconteurlite_node_request request;

static robotraconteurlite_status request_error(struct robotraconteurlite_node_request* request_res,
                                               robotraconteurlite_status request_rv);

static robotraconteurlite_status get_d1_response(struct robotraconteurlite_event* event,
                                                 struct robotraconteurlite_node_request* request_res);
static robotraconteurlite_status set_d1_response(struct robotraconteurlite_event* event,
                                                 struct robotraconteurlite_node_request* request_res);

const struct robotraconteurlite_node_request_ops get_d1_ops = {get_d1_response, request_error};

const struct robotraconteurlite_node_request_ops set_d1_ops = {set_d1_response, request_error};

static void client_connected(struct robotraconteurlite_node_client_event* event)
{
    struct robotraconteurlite_node_send_messageentry_data send_data;
    robotraconteurlite_status rv = -1;
    printf("Client connected\n");

    rv = robotraconteurlite_client_send_empty_request_c_str(
        event->client, &send_data, ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PROPERTYGETREQ, "d1", NULL, &request);
    if (RRLITE_FAILED(rv))
    {
        /* TODO: in real software, handle RETRY return error gracefully */
        printf("Send request error\n");
        exit(1);
    }

    rv = robotraconteurlite_node_request_set_timeout(&request, &rr_clock, 15000);
    if (RRLITE_FAILED(rv))
    {
        printf("Request \n");
        exit(1);
    }

    (void)robotraconteurlite_node_request_set_ops(&request, &get_d1_ops);
}
static void client_disconnected(struct robotraconteurlite_node_client_event* event)
{
    struct tiny_client_data* data = NULL;
    printf("Client disconnected\n");

    data = get_tiny_client_data(event->client->user_storage);
    data->keep_going = 0;
}

static robotraconteurlite_status request_error(struct robotraconteurlite_node_request* request_res,
                                               robotraconteurlite_status request_rv)
{
    ROBOTRACONTEURLITE_UNUSED(request_res);
    printf("request error %d\n", request_rv);
    exit(1);
    return 0;
}

static robotraconteurlite_status get_d1_response(struct robotraconteurlite_event* event,
                                                 struct robotraconteurlite_node_request* request_res)
{
    struct robotraconteurlite_node_send_messageentry_data send_data;
    robotraconteurlite_status rv = -1;
    robotraconteurlite_double d1_set_val = 42.2;
    robotraconteurlite_double d1_val = -1e9;
    struct robotraconteurlite_messageelement_reader element_reader;

    printf("get_d1_response called\n");

    rv = robotraconteurlite_client_end_request2(request_res, event);
    if (RRLITE_FAILED(rv))
    {
        printf("get_d1 request failed\n");
        exit(1);
        return 0;
    }

    rv = robotraconteurlite_messageentry_reader_find_element_verify_scalar_c_str(
        &event->received_message.entry_reader, "value", &element_reader, ROBOTRACONTEURLITE_DATATYPE_DOUBLE);
    if (RRLITE_FAILED(rv))
    {
        printf("get_d1 read value failed\n");
        exit(1);
        return 0;
    }

    rv = robotraconteurlite_messageelement_reader_read_data_double(&element_reader, &d1_val);
    if (RRLITE_FAILED(rv))
    {
        printf("Could not read double\n");
        return -1;
    }
    printf("Got d1 value: %f\n", d1_val);

    rv = robotraconteurlite_client_begin_request_c_str(event->connection->client, &send_data,
                                                       ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PROPERTYSETREQ, "d1", NULL,
                                                       &request);
    if (RRLITE_FAILED(rv))
    {
        /* TODO: in real software, handle RETRY return error gracefully */
        printf("Send request error\n");
        exit(1);
    }

    rv = robotraconteurlite_messageelement_writer_write_double_c_str(&send_data.element_writer, "value", d1_set_val);

    if (RRLITE_FAILED(rv))
    {
        printf("Could not write double\n");
        exit(1);
        return 0;
    }

    rv = robotraconteurlite_client_send_request(&send_data, &request);
    if (RRLITE_FAILED(rv))
    {
        /*TODO: handle retry */
        printf("Could not send set_d1\n");
        exit(1);
        return 0;
    }

    rv = robotraconteurlite_node_request_set_timeout(&request, &rr_clock, 15000);
    if (RRLITE_FAILED(rv))
    {
        printf("Request \n");
        exit(1);
        return 0;
    }

    (void)robotraconteurlite_node_request_set_ops(&request, &set_d1_ops);

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status set_d1_response(struct robotraconteurlite_event* event,
                                                 struct robotraconteurlite_node_request* request_res)
{
    struct tiny_client_data* data = NULL;
    robotraconteurlite_status rv = -1;
    printf("set_d1_response called\n");

    rv = robotraconteurlite_client_end_request2(request_res, event);
    if (RRLITE_FAILED(rv))
    {
        printf("set_d1 request failed\n");
        exit(1);
        return 0;
    }

    data = get_tiny_client_data(event->connection->client->user_storage);
    data->keep_going = 0;
    data->done = 1;

    return 0;
}
