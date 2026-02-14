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

struct robotraconteurlite_node_service;
struct robotraconteurlite_node_service_definition;

#ifdef ROBOTRACONTEURLITE_HAVE_FUNCPTR
struct robotraconteurlite_node_ops;
struct robotraconteurlite_node_service_ops;
struct robotraconteurlite_node_service_objects_ops;
#endif

struct robotraconteurlite_node
{
    /* Connections linked list */
    struct robotraconteurlite_connection_object* connections_head;
    struct robotraconteurlite_connection_object* connections_tail;
    struct robotraconteurlite_connection* connections_next;

    /* Node information */
    struct robotraconteurlite_nodeid nodeid;
    char nodename_char[ROBOTRACONTEURLITE_MESSAGE_STR_MAX_SIZE];
    struct robotraconteurlite_const_string nodename;

    /* Event information */
    robotraconteurlite_size_t events_serviced;

    /* Services */
    struct robotraconteurlite_node_service* services_head;
    struct robotraconteurlite_node_service_definition* service_defs_head;

    /* ops */
#ifdef ROBOTRACONTEURLITE_HAVE_FUNCPTR
    const struct robotraconteurlite_node_ops* node_ops;
#endif
};

struct robotraconteurlite_node_send_messageentry_data
{
    /* Inputs */
    struct robotraconteurlite_node* node;
    struct robotraconteurlite_connection* connection;
    struct robotraconteurlite_messageentry_const_header* message_entry_header;
    /* Outputs */
    struct robotraconteurlite_messageelement_writer element_writer;
    robotraconteurlite_u32 request_id;
    /* Internal */
    struct robotraconteurlite_message_const_header message_header;
    struct robotraconteurlite_message_writer message_writer;
    struct robotraconteurlite_messageentry_writer entry_writer;
    struct robotraconteurlite_messageentry_const_header message_entry_header_storage;
    struct robotraconteurlite_buffer buffer_storage;
    struct robotraconteurlite_buffer_vec buffer_vec_storage;
};

struct robotraconteurlite_node_receive_messageentry_data
{
    /* Inputs */
    struct robotraconteurlite_node* node;
    struct robotraconteurlite_connection* connection;
    /* Outputs */
    struct robotraconteurlite_message_const_header received_message_header;
    struct robotraconteurlite_messageentry_const_header received_message_entry_header;
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
    struct robotraconteurlite_const_string qualified_name;
    robotraconteurlite_u32 qualified_name_hash;
    struct robotraconteurlite_const_string service_definition;
    struct robotraconteurlite_const_string imported_qualified_names;
    struct robotraconteurlite_node_service_definition* prev;
    struct robotraconteurlite_node_service_definition* next;
};

struct robotraconteurlite_node_service_object
{
    struct robotraconteurlite_const_string service_path;
    robotraconteurlite_u32 service_path_hash;
    struct robotraconteurlite_const_string qualified_type;
    struct robotraconteurlite_const_string implemented_qualified_types;
    struct robotraconteurlite_node_service_object* prev;
    struct robotraconteurlite_node_service_object* next;
    struct robotraconteurlite_user_storage* user_storage;

#ifdef ROBOTRACONTEURLITE_HAVE_FUNCPTR
    const struct robotraconteurlite_node_service_object_ops* service_object_ops;
#endif
};

struct robotraconteurlite_node_service
{
    struct robotraconteurlite_const_string service_name;
    robotraconteurlite_u32 service_name_hash;
    struct robotraconteurlite_node_service_object service_objects_head;
    struct robotraconteurlite_node_service_object* root_service_object;
    struct robotraconteurlite_node_service* prev;
    struct robotraconteurlite_node_service* next;
    struct robotraconteurlite_user_storage* user_storage;

#ifdef ROBOTRACONTEURLITE_HAVE_FUNCPTR
    const struct robotraconteurlite_node_service_ops* service_ops;
#endif
};

struct robotraconteurlite_event
{
    enum robotraconteurlite_event_type event_type;
    struct robotraconteurlite_node* node;
    struct robotraconteurlite_connection* connection;
    robotraconteurlite_timespec event_time;
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

enum robotraconteurlite_node_service_event_type
{
    ROBOTRACONTEURLITE_NODE_SERVICE_EVENT_TYPE_NOOP = 0,
    ROBOTRACONTEURLITE_NODE_SERVICE_EVENT_TYPE_MESSAGE,
    ROBOTRACONTEURLITE_NODE_SERVICE_EVENT_TYPE_CLIENT_CONNECTED,
    ROBOTRACONTEURLITE_NODE_SERVICE_EVENT_TYPE_CLIENT_DISCONNECTED
};

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_node_init(struct robotraconteurlite_node* node, const struct robotraconteurlite_nodeid* nodeid,
                             const struct robotraconteurlite_const_string* nodename,
                             struct robotraconteurlite_connection_object* connections_head);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_shutdown(struct robotraconteurlite_node* node);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_add_connection(
    struct robotraconteurlite_node* node, struct robotraconteurlite_connection* connection);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_remove_connection(
    struct robotraconteurlite_node* node, struct robotraconteurlite_connection_object* connection);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_next_event(
    struct robotraconteurlite_node* node, struct robotraconteurlite_event* event, robotraconteurlite_timespec now);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_node_consume_event(struct robotraconteurlite_event* event);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_node_event_special_request(struct robotraconteurlite_event* event);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_verify_incoming_message(
    struct robotraconteurlite_node* node, struct robotraconteurlite_connection* connection,
    struct robotraconteurlite_message_const_header* message_header);

ROBOTRACONTEURLITE_API void robotraconteurlite_node_event_construct_send_data(
    struct robotraconteurlite_event* event, struct robotraconteurlite_node_send_messageentry_data* send_data);

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
    struct robotraconteurlite_messageentry_const_header* request_message_entry_header,
    robotraconteurlite_u16 error_code, const char* error_name, const char* error_message);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_begin_send_messageentry_response(
    struct robotraconteurlite_node_send_messageentry_data* send_data,
    struct robotraconteurlite_messageentry_const_header* request_message_entry_header);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_send_messageentry_empty_response(
    struct robotraconteurlite_node* node, struct robotraconteurlite_connection* connection,
    struct robotraconteurlite_messageentry_const_header* request_message_entry_header);

ROBOTRACONTEURLITE_API void robotraconteurlite_node_service_definition_list_head_construct(
    struct robotraconteurlite_node_service_definition* service_defs_head);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_service_definition_construct(
    struct robotraconteurlite_node_service_definition* service_def,
    const struct robotraconteurlite_const_string* qualified_name_str,
    const struct robotraconteurlite_const_string* service_definition_str,
    const struct robotraconteurlite_const_string* imported_qualified_names_str,
    struct robotraconteurlite_node_service_definition* service_defs_head);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_split_qualified_type(
    const struct robotraconteurlite_const_string* qualified_type, struct robotraconteurlite_const_string* service_type,
    struct robotraconteurlite_const_string* service_entry_type);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_service_definition_construct_c_str(
    struct robotraconteurlite_node_service_definition* service_def, const char* qualified_name_str,
    const char* service_definition_str, const char* imported_qualified_names_str,
    struct robotraconteurlite_node_service_definition* service_defs_head);

ROBOTRACONTEURLITE_API struct robotraconteurlite_node_service_definition*
robotraconteurlite_node_find_service_definition(struct robotraconteurlite_node_service_definition* service_defs_head,
                                                const struct robotraconteurlite_const_string* qualified_name_str);

ROBOTRACONTEURLITE_API struct robotraconteurlite_node_service_definition*
robotraconteurlite_node_find_service_definition_for_entry(
    struct robotraconteurlite_node_service_definition* service_defs_head,
    const struct robotraconteurlite_const_string* qualified_entry_name_str);

ROBOTRACONTEURLITE_API void robotraconteurlite_node_service_object_list_head_construct(
    struct robotraconteurlite_node_service_object* service_objects_head);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_service_object_construct(
    struct robotraconteurlite_node_service_object* service_object,
    const struct robotraconteurlite_const_string* service_path,
    const struct robotraconteurlite_const_string* qualified_type,
    const struct robotraconteurlite_const_string* implemented_qualified_types,
    struct robotraconteurlite_node_service* service);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_service_object_construct_c_str(
    struct robotraconteurlite_node_service_object* service_object, const char* service_path, const char* qualified_type,
    const char* implemented_qualified_types, struct robotraconteurlite_node_service* service);

ROBOTRACONTEURLITE_API void robotraconteurlite_node_service_list_head_construct(
    struct robotraconteurlite_node_service* service_objects_head);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_service_construct(
    struct robotraconteurlite_node_service* service, const struct robotraconteurlite_const_string* service_name,
    struct robotraconteurlite_node_service* services_head);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_service_construct_c_str(
    struct robotraconteurlite_node_service* service, const char* service_name,
    struct robotraconteurlite_node_service* services_head);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_service_set_root_object(
    struct robotraconteurlite_node_service* service, struct robotraconteurlite_node_service_object* service_object);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_service_add_service_object(
    struct robotraconteurlite_node_service* service, struct robotraconteurlite_node_service_object* service_object);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_service_remove_service_object(
    struct robotraconteurlite_node_service* service, struct robotraconteurlite_node_service_object* service_object);

ROBOTRACONTEURLITE_API struct robotraconteurlite_bool robotraconteurlite_node_is_service_path_prefix(
    const struct robotraconteurlite_const_string* path_prefix,
    const struct robotraconteurlite_const_string* service_path);

ROBOTRACONTEURLITE_API struct robotraconteurlite_node_service* robotraconteurlite_node_find_service_for_path(
    struct robotraconteurlite_node_service* services_head, const struct robotraconteurlite_const_string* service_path);

ROBOTRACONTEURLITE_API struct robotraconteurlite_node_service_object*
robotraconteurlite_node_find_service_object_for_path(
    struct robotraconteurlite_node_service_object* service_objects_head,
    const struct robotraconteurlite_const_string* service_path);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_event_special_request_service_definition(
    struct robotraconteurlite_event* event, struct robotraconteurlite_node_service* services_head,
    struct robotraconteurlite_node_service_definition* service_defs_head);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_event_special_request_object_type_name(
    struct robotraconteurlite_event* event, struct robotraconteurlite_node_service_object* service_objects_head);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_event_special_request_object_type_name2(
    struct robotraconteurlite_event* event, struct robotraconteurlite_node_service* services_head);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_node_event_is_member(struct robotraconteurlite_event* event, const char* member_name);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_event_is_member2(
    struct robotraconteurlite_event* event, const char* service_path, const char* member_name);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_node_event_respond_member_not_found(struct robotraconteurlite_event* event);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_node_event_respond_invalid_operation(struct robotraconteurlite_event* event);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_event_respond_element_read_error(
    struct robotraconteurlite_event* event, robotraconteurlite_status read_rv);

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

ROBOTRACONTEURLITE_API robotraconteurlite_size_t
robotraconteurlite_node_events_pending(struct robotraconteurlite_node* node);

#ifdef ROBOTRACONTEURLITE_HAVE_FUNCPTR
ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_set_services(
    struct robotraconteurlite_node* node, struct robotraconteurlite_node_service* services_head,
    struct robotraconteurlite_node_service_definition* service_defs_head);

struct robotraconteurlite_node_service_event
{
    struct robotraconteurlite_event* event;
    struct robotraconteurlite_node_service* service;
    struct robotraconteurlite_node_service_object* service_object;
};

struct robotraconteurlite_node_ops
{
    robotraconteurlite_status (*connection_event)(struct robotraconteurlite_event* event);
    robotraconteurlite_status (*send_complete)(struct robotraconteurlite_event* event);
    robotraconteurlite_status (*event_error_returned)(struct robotraconteurlite_event* event);
};

struct robotraconteurlite_node_service_ops
{
    void (*client_event)(struct robotraconteurlite_node_service_event* event,
                         enum robotraconteurlite_node_service_event_type event_type);
};

struct robotraconteurlite_node_service_object_ops
{
    robotraconteurlite_status (*message_received)(struct robotraconteurlite_node_service_event* event);
};

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_node_set_ops(struct robotraconteurlite_node* node, const struct robotraconteurlite_node_ops* ops);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_service_set_ops(
    struct robotraconteurlite_node_service* service, const struct robotraconteurlite_node_service_ops* ops);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_node_service_object_set_ops(struct robotraconteurlite_node_service_object* service_object,
                                               const struct robotraconteurlite_node_service_object_ops* ops);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_node_run_next_event(struct robotraconteurlite_node* node, robotraconteurlite_timespec now,
                                       enum robotraconteurlite_event_type* handled_event_type);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_run_next_event2(
    struct robotraconteurlite_node* node, robotraconteurlite_timespec now, struct robotraconteurlite_event* event);

ROBOTRACONTEURLITE_API robotraconteurlite_size_t
robotraconteurlite_node_events_drain_pending(struct robotraconteurlite_node* node, robotraconteurlite_timespec now);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_run_next_event_cycle(
    struct robotraconteurlite_node* node, robotraconteurlite_timespec now, robotraconteurlite_size_t max_events);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_run_events_drain(
    struct robotraconteurlite_node* node, robotraconteurlite_timespec now,
    robotraconteurlite_size_t max_events_per_cycle, robotraconteurlite_size_t max_cycles);

ROBOTRACONTEURLITE_API robotraconteurlite_size_t
robotraconteurlite_node_events_available(struct robotraconteurlite_node* node, robotraconteurlite_timespec now);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_node_run_events_available(
    struct robotraconteurlite_node* node, robotraconteurlite_timespec now,
    robotraconteurlite_size_t max_events_per_cycle, robotraconteurlite_size_t max_cycles);

#endif

#ifdef __cplusplus
}
#endif

#endif /* ROBOTRACONTEURLITE_NODE_H */
