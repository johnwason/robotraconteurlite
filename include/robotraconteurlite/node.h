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

#ifndef ROBOTRACONTEURLITE_NODE_H
#define ROBOTRACONTEURLITE_NODE_H

#include "robotraconteurlite/message.h"
#include "robotraconteurlite/connection.h"
#include "robotraconteurlite/clock.h"
#include "robotraconteurlite/util.h"
#include "robotraconteurlite/poll.h"

#define ROBOTRACONTEURLITE_NODE_DEFAULT_SLEEP_TIME 5000

#ifdef __cplusplus
extern "C" {
#endif

enum robotraconteurlite_event_type
{
    ROBOTRACONTEURLITE_EVENT_TYPE_NOOP = 0,
    ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_CONNECTED,
    ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_CLOSED,
    ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_ERROR,
    ROBOTRACONTEURLITE_EVENT_TYPE_MESSAGE_RECEIVED,
    ROBOTRACONTEURLITE_EVENT_TYPE_MESSAGE_SEND_COMPLETE,
    ROBOTRACONTEURLITE_EVENT_TYPE_NEXT_CYCLE,
    ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_HEARTBEAT_TIMEOUT,
    ROBOTRACONTEURLITE_EVENT_TYPE_CONNECTION_TIMEOUT
};

struct robotraconteurlite_node
{
    /* Connections linked list */
    struct robotraconteurlite_connection_object* connections_head;
    struct robotraconteurlite_connection_object* connections_tail;
    struct robotraconteurlite_connection* connections_next;

    /* Node information */
    struct robotraconteurlite_nodeid nodeid;
    char nodename_char[ROBOTRACONTEURLITE_MESSAGE_STR_MAX_SIZE];
    struct robotraconteurlite_string nodename;

    /* Event information */
    robotraconteurlite_size_t events_serviced;
};

struct robotraconteurlite_node_send_messageentry_data
{
    /* Inputs */
    struct robotraconteurlite_node* node;
    struct robotraconteurlite_connection* connection;
    struct robotraconteurlite_messageentry_header* message_entry_header;
    /* Outputs */
    struct robotraconteurlite_messageelement_writer element_writer;
    robotraconteurlite_u32 request_id;
    /* Internal */
    struct robotraconteurlite_message_header message_header;
    struct robotraconteurlite_message_writer message_writer;
    struct robotraconteurlite_messageentry_writer entry_writer;
    struct robotraconteurlite_messageentry_header message_entry_header_storage;
    struct robotraconteurlite_buffer buffer_storage;
    struct robotraconteurlite_buffer_vec buffer_vec_storage;
};

struct robotraconteurlite_node_receive_messageentry_data
{
    /* Inputs */
    struct robotraconteurlite_node* node;
    struct robotraconteurlite_connection* connection;
    /* Outputs */
    struct robotraconteurlite_message_header received_message_header;
    struct robotraconteurlite_messageentry_header received_message_entry_header;
    struct robotraconteurlite_messageentry_reader entry_reader;
    /* Internal */
    struct robotraconteurlite_buffer buffer_storage;
    struct robotraconteurlite_buffer_vec buffer_vec_storage;
    char receiver_nodename_char[ROBOTRACONTEURLITE_MESSAGE_STR_MAX_SIZE];
    char sender_nodename_char[ROBOTRACONTEURLITE_MESSAGE_STR_MAX_SIZE];
    char service_path_char[ROBOTRACONTEURLITE_MESSAGE_STR_MAX_SIZE];
    char member_name_char[ROBOTRACONTEURLITE_MESSAGE_STR_MAX_SIZE];
    char extended_char[ROBOTRACONTEURLITE_MESSAGE_STR_MAX_SIZE];
};

struct robotraconteurlite_node_service_definition
{
    struct robotraconteurlite_string qualified_name;
    struct robotraconteurlite_string service_definition;
};

struct robotraconteurlite_node_service_object
{
    struct robotraconteurlite_string service_path;
    struct robotraconteurlite_string qualified_type;
    struct robotraconteurlite_node_service_definition* service_def;
};

struct robotraconteurlite_event
{
    enum robotraconteurlite_event_type event_type;
    struct robotraconteurlite_connection* connection;
    struct robotraconteurlite_node_receive_messageentry_data received_message;
    int event_error_code;
    robotraconteurlite_size_t events_serviced;
};

enum robotraconteurlite_client_handshake_state
{
    ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_INIT = 0,
    ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CONNECTED,
    ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CREATECONNECTION_SENT,
    ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CREATECONNECTION_COMPLETED,
    ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_OBJECTTYPE_SENT,
    ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_OBJECTTYPE_COMPLETED,
    ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CONNECTCLIENT_SENT,
    ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_CONNECTCLIENT_COMPLETED,
    ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_COMPLETED,
    ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_ERROR,
    ROBOTRACONTEURLITE_CLIENT_HANDSHAKE_FAILED
};

#define ROBOTRACONTEURLITE_CONNECTION_PARSE_CAPABILITY_MESSAGE2 0x1U
#define ROBOTRACONTEURLITE_CONNECTION_PARSE_CAPABILITY_MESSAGE4 0x2U

struct robotraconteurlite_client_handshake_data
{
    struct robotraconteurlite_node* node;
    struct robotraconteurlite_connection* connection;
    robotraconteurlite_u32 handshake_state;
    robotraconteurlite_u32 request_id;
    struct robotraconteurlite_string root_object_type;
    char root_object_type_char[ROBOTRACONTEURLITE_MESSAGE_STR_MAX_SIZE];
};

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_init(
    struct robotraconteurlite_node* node, struct robotraconteurlite_nodeid* nodeid,
    struct robotraconteurlite_string* nodename, struct robotraconteurlite_connection_object* connections_head);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_shutdown(struct robotraconteurlite_node* node);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_add_connection(
    struct robotraconteurlite_node* node, struct robotraconteurlite_connection* connection);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_remove_connection(
    struct robotraconteurlite_node* node, struct robotraconteurlite_connection_object* connection);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_next_event(
    struct robotraconteurlite_node* node, struct robotraconteurlite_event* event, robotraconteurlite_timespec now);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_node_consume_event(struct robotraconteurlite_node* node, struct robotraconteurlite_event* event);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_event_special_request(
    struct robotraconteurlite_node* node, struct robotraconteurlite_event* event);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_verify_incoming_message(
    struct robotraconteurlite_node* node, struct robotraconteurlite_connection* connection,
    struct robotraconteurlite_message_header* message_header);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_node_begin_send_messageentry(struct robotraconteurlite_node_send_messageentry_data* send_data);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_node_end_send_messageentry(struct robotraconteurlite_node_send_messageentry_data* send_data);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_node_abort_send_messageentry(struct robotraconteurlite_node_send_messageentry_data* send_data);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_node_receive_messageentry(struct robotraconteurlite_node_receive_messageentry_data* receive_data);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_receive_messageentry_consume(
    struct robotraconteurlite_node_receive_messageentry_data* receive_data);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_connection_send_messageentry_error_response(
    struct robotraconteurlite_node* node, struct robotraconteurlite_connection* connection,
    struct robotraconteurlite_messageentry_header* request_message_entry_header, robotraconteurlite_u16 error_code,
    const char* error_name, const char* error_message);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_begin_send_messageentry_response(
    struct robotraconteurlite_node_send_messageentry_data* send_data,
    struct robotraconteurlite_messageentry_header* request_message_entry_header);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_send_messageentry_empty_response(
    struct robotraconteurlite_node* node, struct robotraconteurlite_connection* connection,
    struct robotraconteurlite_messageentry_header* request_message_entry_header);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_event_special_request_service_definition(
    struct robotraconteurlite_node* node, struct robotraconteurlite_event* event,
    struct robotraconteurlite_node_service_object service_objects[], robotraconteurlite_size_t service_objects_len,
    struct robotraconteurlite_node_service_definition service_defs[], robotraconteurlite_size_t service_defs_len);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_event_special_request_object_type_name(
    struct robotraconteurlite_node* node, struct robotraconteurlite_event* event,
    struct robotraconteurlite_node_service_object service_objects[], robotraconteurlite_size_t service_objects_len);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_event_is_member(
    struct robotraconteurlite_event* event, const char* service_path, const char* member_name);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_client_is_connected(
    struct robotraconteurlite_node* node, struct robotraconteurlite_connection* connection);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_client_handshake(struct robotraconteurlite_client_handshake_data* handshake_data,
                                    struct robotraconteurlite_event* event, robotraconteurlite_timespec now);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_client_begin_request(
    struct robotraconteurlite_node_send_messageentry_data* send_data, robotraconteurlite_u16 entry_type,
    const char* membername, const char* servicepath);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_client_send_empty_request(
    struct robotraconteurlite_node_send_messageentry_data* send_data, robotraconteurlite_u16 entry_type,
    const char* membername, const char* servicepath);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_client_send_request(struct robotraconteurlite_node_send_messageentry_data* send_data);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_client_end_request(
    struct robotraconteurlite_node_send_messageentry_data* send_data, struct robotraconteurlite_event* event);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_client_send_heartbeat(
    struct robotraconteurlite_node* node, struct robotraconteurlite_connection* connection);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_next_wake(
    struct robotraconteurlite_node* node, robotraconteurlite_timespec now, robotraconteurlite_timespec* wake_time);

/*ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_client_process_request(struct
 * robotraconteurlite_node_send_messageentry_data* request_data, struct
 * robotraconteurlite_node_receive_messageentry_data* response_data);*/

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_transport_parse_capabilities(
    struct robotraconteurlite_messageentry_reader* entry_reader, robotraconteurlite_u32* parsed_flags);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_transport_populate_capabilities(
    struct robotraconteurlite_messageelement_writer* element_writer, robotraconteurlite_u32 capability_flags);

#ifdef __cplusplus
}
#endif

#endif /* ROBOTRACONTEURLITE_NODE_H */
