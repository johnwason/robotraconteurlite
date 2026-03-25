/* Copyright 2011-2026 Wason Technology, LLC
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

#ifndef ROBOTRACONTEURLITE_NODE_MACROS_H
#define ROBOTRACONTEURLITE_NODE_MACROS_H

#define robotraconteurlite_node_service_is_member(member_name)                                                         \
    robotraconteurlite_node_event_is_member(s_evt->event, member_name)

#define robotraconteurlite_node_service_begin_send()                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        robotraconteurlite_node_event_construct_send_data(s_evt->event, &send_data);                                   \
        rv = robotraconteurlite_node_begin_send_messageentry_response(                                                 \
            &send_data, &s_evt->event->received_message.received_message_entry_header);                                \
        if (ROBOTRACONTEURLITE_FAILED(rv))                                                                             \
        {                                                                                                              \
            return rv;                                                                                                 \
        }                                                                                                              \
    } while (0);

#define robotraconteurlite_node_service_end_send()                                                                     \
    do                                                                                                                 \
    {                                                                                                                  \
        rv = robotraconteurlite_node_end_send_messageentry(&send_data);                                                \
        if (ROBOTRACONTEURLITE_FAILED(rv))                                                                             \
        {                                                                                                              \
            return rv;                                                                                                 \
        }                                                                                                              \
    } while (0);

#define robotraconteurlite_node_service_send_empty_response()                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        rv = robotraconteurlite_node_send_messageentry_empty_response(                                                 \
            s_evt->event->node, s_evt->event->connection,                                                              \
            &s_evt->event->received_message.received_message_entry_header);                                            \
        if (ROBOTRACONTEURLITE_FAILED(rv))                                                                             \
        {                                                                                                              \
            return rv;                                                                                                 \
        }                                                                                                              \
    } while (0);

#define robotraconteurlite_node_service_send_member_not_found()                                                        \
    return robotraconteurlite_node_event_respond_member_not_found(s_evt->event);

#define robotraconteurlite_node_service_read_double(element_name_c_str, value)                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        struct robotraconteurlite_messageelement_reader __node_macro_temp_element_reader;                              \
        rv = robotraconteurlite_messageentry_reader_find_element_verify_scalar_c_str(                                  \
            &s_evt->event->received_message.entry_reader, element_name_c_str, &__node_macro_temp_element_reader,       \
            ROBOTRACONTEURLITE_DATATYPE_DOUBLE);                                                                       \
                                                                                                                       \
        if (ROBOTRACONTEURLITE_FAILED(rv))                                                                             \
        {                                                                                                              \
            return robotraconteurlite_node_event_respond_element_read_error(s_evt->event, rv);                         \
        }                                                                                                              \
                                                                                                                       \
        rv = robotraconteurlite_messageelement_reader_read_data_double(&__node_macro_temp_element_reader, value);      \
        if (ROBOTRACONTEURLITE_FAILED(rv))                                                                             \
        {                                                                                                              \
            return robotraconteurlite_node_event_respond_element_read_error(s_evt->event, rv);                         \
        }                                                                                                              \
    } while (0);

#define robotraconteurlite_node_service_read_nested_double(reader, element_name_c_str, value)                          \
    do                                                                                                                 \
    {                                                                                                                  \
        struct robotraconteurlite_messageelement_reader __node_macro_temp_element_reader;                              \
        rv = robotraconteurlite_messageentry_reader_find_nested_element_verify_scalar_c_str(                           \
            reader, element_name_c_str, &__node_macro_temp_element_reader, ROBOTRACONTEURLITE_DATATYPE_DOUBLE);        \
                                                                                                                       \
        if (ROBOTRACONTEURLITE_FAILED(rv))                                                                             \
        {                                                                                                              \
            return robotraconteurlite_node_event_respond_element_read_error(s_evt->event, rv);                         \
        }                                                                                                              \
                                                                                                                       \
        rv = robotraconteurlite_messageelement_reader_read_data_double(&__node_macro_temp_element_reader, value);      \
        if (ROBOTRACONTEURLITE_FAILED(rv))                                                                             \
        {                                                                                                              \
            return robotraconteurlite_node_event_respond_element_read_error(s_evt->event, rv);                         \
        }                                                                                                              \
    } while (0);

#define robotraconteurlite_node_service_read_double_array(element_name_c_str, value, expected_len, var_len)            \
    do                                                                                                                 \
    {                                                                                                                  \
        struct robotraconteurlite_messageelement_reader __node_macro_temp_element_reader;                              \
        rv = robotraconteurlite_messageentry_reader_find_element_verify_array_c_str(                                   \
            &s_evt->event->received_message.entry_reader, element_name_c_str, &__node_macro_temp_element_reader,       \
            ROBOTRACONTEURLITE_DATATYPE_DOUBLE, expected_len, var_len);                                                \
                                                                                                                       \
        if (ROBOTRACONTEURLITE_FAILED(rv))                                                                             \
        {                                                                                                              \
            return robotraconteurlite_node_event_respond_element_read_error(s_evt->event, rv);                         \
        }                                                                                                              \
                                                                                                                       \
        rv =                                                                                                           \
            robotraconteurlite_messageelement_reader_read_data_double_array(&__node_macro_temp_element_reader, value); \
        if (ROBOTRACONTEURLITE_FAILED(rv))                                                                             \
        {                                                                                                              \
            return robotraconteurlite_node_event_respond_element_read_error(s_evt->event, rv);                         \
        }                                                                                                              \
    } while (0);

#define robotraconteurlite_node_service_read_nested_double_array(reader, element_name_c_str, value, expected_len,      \
                                                                 var_len)                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        struct robotraconteurlite_messageelement_reader __node_macro_temp_element_reader;                              \
        rv = robotraconteurlite_messageentry_reader_find_nested_element_verify_array_c_str(                            \
            reader, element_name_c_str, &__node_macro_temp_element_reader, ROBOTRACONTEURLITE_DATATYPE_DOUBLE,         \
            expected_len, var_len);                                                                                    \
                                                                                                                       \
        if (ROBOTRACONTEURLITE_FAILED(rv))                                                                             \
        {                                                                                                              \
            return robotraconteurlite_node_event_respond_element_read_error(s_evt->event, rv);                         \
        }                                                                                                              \
                                                                                                                       \
        rv =                                                                                                           \
            robotraconteurlite_messageelement_reader_read_data_double_array(&__node_macro_temp_element_reader, value); \
        if (ROBOTRACONTEURLITE_FAILED(rv))                                                                             \
        {                                                                                                              \
            return robotraconteurlite_node_event_respond_element_read_error(s_evt->event, rv);                         \
        }                                                                                                              \
    } while (0);

#define robotraconteurlite_node_service_write_nested_double(writer, element_name_c_str, value)                         \
    do                                                                                                                 \
    {                                                                                                                  \
        rv = robotraconteurlite_messageelement_writer_write_double_c_str(writer, element_name_c_str, value);           \
        if (ROBOTRACONTEURLITE_FAILED(rv))                                                                             \
        {                                                                                                              \
            return rv;                                                                                                 \
        }                                                                                                              \
    } while (0);

#define robotraconteurlite_node_service_write_double(element_name_c_str, value)                                        \
    robotraconteurlite_node_service_write_nested_double(&send_data.element_writer, element_name_c_str, value)

#define robotraconteurlite_node_service_write_nested_double_array(writer, element_name_c_str, value)                   \
    do                                                                                                                 \
    {                                                                                                                  \
        rv = robotraconteurlite_messageelement_writer_write_double_array_c_str(writer, element_name_c_str, value);     \
        if (ROBOTRACONTEURLITE_FAILED(rv))                                                                             \
        {                                                                                                              \
            return rv;                                                                                                 \
        }                                                                                                              \
    } while (0);

#define robotraconteurlite_node_service_write_double_array(element_name_c_str, value)                                  \
    robotraconteurlite_node_service_write_nested_double_array(&send_data.element_writer, element_name_c_str, value)

#ifdef ROBOTRACONTEURLITE_NODE_SHORTHAND_MACROS

#define rrl_s_is_member robotraconteurlite_node_service_is_member
#define rrl_s_begin_send robotraconteurlite_node_service_begin_send
#define rrl_s_end_send robotraconteurlite_node_service_end_send
#define rrl_s_send_empty_response robotraconteurlite_node_service_send_empty_response
#define rrl_s_send_member_not_found robotraconteurlite_node_service_send_member_not_found
#define rrl_s_read_double robotraconteurlite_node_service_read_double
#define rrl_s_write_double robotraconteurlite_node_service_write_double
#define rrl_s_read_double2 robotraconteurlite_node_service_read_nested_double
#define rrl_s_write_double2 robotraconteurlite_node_service_write_nested_double
#define rrl_s_read_double_array robotraconteurlite_node_service_read_double_array
#define rrl_s_write_double_array robotraconteurlite_node_service_write_double_array
#define rrl_s_read_double_array2 robotraconteurlite_node_service_read_nested_double_array
#define rrl_s_write_double_array2 robotraconteurlite_node_service_write_nested_double_array

#endif /* ROBOTRACONTEURLITE_NODE_SHORTHAND_MACROS */

#endif /* ROBOTRACONTEURLITE_NODE_MACROS_H */
