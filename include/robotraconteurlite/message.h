/* Copyright 2011-2019 Wason Technology, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ROBOTRACONTEURLITE_MESSAGE_H
#define ROBOTRACONTEURLITE_MESSAGE_H

#include "robotraconteurlite/config.h"
#include "robotraconteurlite/err.h"
#include "robotraconteurlite/array.h"
#include "robotraconteurlite/nodeid.h"


/* cppcheck-suppress-begin misra-c2012-2.5 */

/* Data Types */
#define ROBOTRACONTEURLITE_DATATYPE_VOID 0U
#define ROBOTRACONTEURLITE_DATATYPE_DOUBLE 1U
#define ROBOTRACONTEURLITE_DATATYPE_SINGLE 2U
#define ROBOTRACONTEURLITE_DATATYPE_INT8 3U
#define ROBOTRACONTEURLITE_DATATYPE_UINT8 4U
#define ROBOTRACONTEURLITE_DATATYPE_INT16 5U
#define ROBOTRACONTEURLITE_DATATYPE_UINT16 6U
#define ROBOTRACONTEURLITE_DATATYPE_INT32 7U
#define ROBOTRACONTEURLITE_DATATYPE_UINT32 8U
#define ROBOTRACONTEURLITE_DATATYPE_INT64 9U
#define ROBOTRACONTEURLITE_DATATYPE_UINT64 10U
#define ROBOTRACONTEURLITE_DATATYPE_STRING 11U
#define ROBOTRACONTEURLITE_DATATYPE_CDOUBLE 12U
#define ROBOTRACONTEURLITE_DATATYPE_CSINGLE 13U
#define ROBOTRACONTEURLITE_DATATYPE_BOOL 14U
#define ROBOTRACONTEURLITE_DATATYPE_STRUCTURE 101U
#define ROBOTRACONTEURLITE_DATATYPE_MAP_INT32 102U
#define ROBOTRACONTEURLITE_DATATYPE_MAP_STRING 103U
#define ROBOTRACONTEURLITE_DATATYPE_OBJECT 104U
#define ROBOTRACONTEURLITE_DATATYPE_VARVALUE 105U
#define ROBOTRACONTEURLITE_DATATYPE_VAROBJECT 106U
/* #define ROBOTRACONTEURLITE_DATATYPE_MULTIDIMARRAY - Deprecated! */
#define ROBOTRACONTEURLITE_DATATYPE_LIST 108U
#define ROBOTRACONTEURLITE_DATATYPE_POD 109U
#define ROBOTRACONTEURLITE_DATATYPE_POD_ARRAY 110U
#define ROBOTRACONTEURLITE_DATATYPE_POD_MULTIDIMARRAY 111U
#define ROBOTRACONTEURLITE_DATATYPE_ENUM 112U
#define ROBOTRACONTEURLITE_DATATYPE_NAMEDTYPE 113U
#define ROBOTRACONTEURLITE_DATATYPE_NAMEDARRAY 114U
#define ROBOTRACONTEURLITE_DATATYPE_NAMEDARRAY_ARRAY 115U
#define ROBOTRACONTEURLITE_DATATYPE_NAMEDARRAY_MULTIDIMARRAY 116U
/* Use a new data type code for numeric arrays to avoid confusion */
#define ROBOTRACONTEURLITE_DATATYPE_MULTIDIMARRAY 117U

/* Message Entry Type */
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_NULL 0U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_STREAMOP 1U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_STREAMOPRET 2U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_STREAMCHECKCAPABILITY 3U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_STREAMCHECKCAPABILITYRET 4U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_STRINGTABLEOP 5U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_STRINGTABLEOPRET 6U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_GETSERVICEDESC 101U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_GETSERVICEDESCRET 102U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_OBJECTTYPENAME 103U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_OBJECTTYPENAMERET 104U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_SERVICECLOSED 105U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_SERVICECLOSEDRET 106U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CONNECTCLIENT 107U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CONNECTCLIENTRET 108U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_DISCONNECTCLIENT 109U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_DISCONNECTCLIENTRET 110U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CONNECTIONTEST 111U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CONNECTIONTESTRET 112U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_GETNODEINFO 113U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_GETNODEINFORET 114U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_RECONNECTCLIENT 115U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_RECONNECTCLIENTRET 116U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_NODECHECKCAPABILITY 117U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_NODECHECKCAPABILITYRET 118U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_GETSERVICEATTRIBUTES 119U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_GETSERVICEATTRIBUTESRET 120U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CONNECTCLIENTCOMBINED 121U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CONNECTCLIENTCOMBINEDRET 122U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_ENDPOINTCHECKCAPABILITY 501U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_ENDPOINTCHECKCAPABILITYRET 502U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_SERVICECHECKCAPABILITYREQ 1101U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_SERVICECHECKCAPABILITYRET 1102U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CLIENTKEEPALIVEREQ 1105U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CLIENTKEEPALIVERET 1106U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CLIENTSESSIONOPREQ 1107U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CLIENTSESSIONOPRET 1108U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_SERVICEPATHRELEASEDREQ 1109U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_SERVICEPATHRELEASEDRET 1110U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PROPERTYGETREQ 1111U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PROPERTYGETRES 1112U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PROPERTYSETREQ 1113U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PROPERTYSETRES 1114U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_FUNCTIONCALLREQ 1121U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_FUNCTIONCALLRES 1122U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_GENERATORNEXTREQ 1123U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_GENERATORNEXTRES 1124U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_EVENTREQ 1131U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_EVENTRES 1132U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PIPEPACKET 1141U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PIPEPACKETRET 1142U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PIPECONNECTREQ 1143U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PIPECONNECTRET 1144U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PIPEDISCONNECTREQ 1145U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PIPEDISCONNECTRET 1146U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PIPECLOSED 1147U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PIPECLOSEDRET 1148U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CALLBACKCALLREQ 1151U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_CALLBACKCALLRET 1152U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_WIREPACKET 1161U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_WIREPACKETRET 1162U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_WIRECONNECTREQ 1163U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_WIRECONNECTRET 1164U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_WIREDISCONNECTREQ 1165U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_WIREDISCONNECTRET 1166U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_WIRECLOSED 1167U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_WIRECLOSEDRET 1168U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_MEMORYREAD 1171U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_MEMORYREADRET 1172U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_MEMORYWRITE 1173U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_MEMORYWRITERET 1174U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_MEMORYGETPARAM 1175U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_MEMORYGETPARAMRET 1176U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_WIREPEEKINVALUEREQ 1181U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_WIREPEEKINVALUERET 1182U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_WIREPEEKOUTVALUEREQ 1183U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_WIREPEEKOUTVALUERET 1184U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_WIREPOKEOUTVALUEREQ 1185U
#define ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_WIREPOKEOUTVALUERET 1186U

/* Message Error Type */
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_NONE 0U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_CONNECTIONERROR 1U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_PROTOCOLERROR 2U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_SERVICENOTFOUND 3U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_OBJECTNOTFOUND 4U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_INVALIDENDPOINT 5U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_ENDPOINTCOMMUNICATIONFATALERROR 6U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_NODENOTFOUND 7U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_SERVICEERROR 8U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_MEMBERNOTFOUND 9U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_MEMBERFORMATMISMATCH 10U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_DATATYPEMISMATCH 11U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_DATATYPEERROR 12U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_DATASERIALIZATIONERROR 13U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_MESSAGEENTRYNOTFOUND 14U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_MESSAGEELEMENTNOTFOUND 15U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_UNKNOWNERROR 16U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_INVALIDOPERATION 17U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_INVALIDARGUMENT 18U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_OPERATIONFAILED 19U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_NULLVALUE 20U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_INTERNALERROR 21U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_SYSTEMRESOURCEPERMISSIONDENIED 22U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_OUTOFSYSTEMRESOURCE 23U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_SYSTEMRESOURCEERROR 24U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_RESOURCENOTFOUND 25U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_IOERROR 26U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_BUFFERLIMITVIOLATION 27U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_SERVICEDEFINITIONERROR 28U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_OUTOFRANGE 29U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_KEYNOTFOUND 30U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_REMOTEERROR 100U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_REQUESTTIMEOUT 101U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_READONLYMEMBER 102U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_WRITEONLYMEMBER 103U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_NOTIMPLEMENTEDERROR 104U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_MEMBERBUSY 105U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_VALUENOTSET 106U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_ABORTOPERATION 107U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_OPERATIONABORTED 108U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_STOPITERATION 109U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_AUTHENTICATIONERROR 150U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_OBJECTLOCKEDERROR 151U
#define ROBOTRACONTEURLITE_MESSAGEERRORTYPE_PERMISSIONDENIED 152U

/* cppcheck-suppress-end misra-c2012-2.5 */

struct robotraconteurlite_message_header
{
    uint32_t message_size;
    uint16_t message_version;
    uint16_t header_size;
    struct robotraconteurlite_nodeid sender_nodeid;
    struct robotraconteurlite_nodeid receiver_nodeid;
    uint32_t sender_endpoint;
    uint32_t receiver_endpoint;
    struct robotraconteurlite_string sender_nodename;
    struct robotraconteurlite_string receiver_nodename;
    struct robotraconteurlite_string extended;
    uint16_t entry_count;
    uint16_t message_id;
    uint16_t message_res_id;
};

struct robotraconteurlite_messageentry_header
{
    uint32_t entry_size;
    uint16_t entry_type;
    struct robotraconteurlite_string service_path;
    struct robotraconteurlite_string member_name;
    uint32_t request_id;
    uint16_t error;
    struct robotraconteurlite_string extended;
    uint16_t element_count;
};

struct robotraconteurlite_messageelement_header
{
    uint32_t element_size;
    struct robotraconteurlite_string element_name;
    uint16_t element_type;
    struct robotraconteurlite_string element_type_name;
    struct robotraconteurlite_string extended;
    uint32_t data_count;
};

struct robotraconteurlite_message_reader
{
    struct robotraconteurlite_buffer_vec* buffer;
    size_t buffer_offset;
    size_t buffer_count;
};

struct robotraconteurlite_messageentry_reader
{
    struct robotraconteurlite_buffer_vec* buffer;
    size_t buffer_offset;
    size_t buffer_count;
    size_t buffer_remaining;
    size_t entry_count_offset;
    size_t current_entry;
};

struct robotraconteurlite_messageelement_reader
{
    struct robotraconteurlite_buffer_vec* buffer;
    size_t buffer_offset;
    size_t buffer_count;
    size_t buffer_remaining;
    size_t element_count_offset;
    size_t element_count_uint32;
    size_t current_element;
};

struct robotraconteurlite_message_writer
{
    struct robotraconteurlite_buffer_vec* buffer;
    size_t buffer_offset;
    size_t buffer_count;
    /* uint8_t dry_run; */
};

struct robotraconteurlite_messageentry_writer
{
    struct robotraconteurlite_buffer_vec* buffer;
    size_t buffer_offset;
    size_t buffer_count;
    size_t entries_written_count;
    size_t entries_written_size;
    /* uint8_t dry_run; */
};

struct robotraconteurlite_messageelement_writer
{
    struct robotraconteurlite_buffer_vec* buffer;
    size_t buffer_offset;
    size_t buffer_count;
    size_t elements_written_count;
    size_t elements_written_size;
    /* uint8_t dry_run; */
};

/* Message Reader */

ROBOTRACONTEURLITE_DECL int robotraconteurlite_message_reader_init(struct robotraconteurlite_message_reader* reader,
                                                                   struct robotraconteurlite_buffer_vec* buffer,
                                                                   size_t offset, size_t count);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_message_reader_read_header(
    struct robotraconteurlite_message_reader* reader, struct robotraconteurlite_message_header* header);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_message_reader_begin_read_entries(
    struct robotraconteurlite_message_reader* reader, struct robotraconteurlite_messageentry_reader* entry_reader);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageentry_reader_move_next(
    struct robotraconteurlite_messageentry_reader* entry_reader);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageentry_reader_read_header(
    struct robotraconteurlite_messageentry_reader* entry_reader, struct robotraconteurlite_messageentry_header* header);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageentry_reader_begin_read_elements(
    struct robotraconteurlite_messageentry_reader* entry_reader,
    struct robotraconteurlite_messageelement_reader* element_reader);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_move_next(
    struct robotraconteurlite_messageelement_reader* element_reader);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageentry_reader_find_element(
    struct robotraconteurlite_messageentry_reader* entry_reader, const struct robotraconteurlite_string* element_name,
    struct robotraconteurlite_messageelement_reader* element_reader);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageentry_reader_find_element_verify_scalar(
    struct robotraconteurlite_messageentry_reader* entry_reader, const struct robotraconteurlite_string* element_name,
    struct robotraconteurlite_messageelement_reader* element_reader, uint16_t expected_data_type);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageentry_reader_find_element_verify_array(
    struct robotraconteurlite_messageentry_reader* entry_reader, const struct robotraconteurlite_string* element_name,
    struct robotraconteurlite_messageelement_reader* element_reader, uint16_t expected_data_type, uint32_t expected_len,
    uint8_t var_length);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageentry_reader_find_element_verify_string(
    struct robotraconteurlite_messageentry_reader* entry_reader, const struct robotraconteurlite_string* element_name,
    struct robotraconteurlite_messageelement_reader* element_reader, uint32_t expected_max_len);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_header(
    struct robotraconteurlite_messageelement_reader* element_reader,
    struct robotraconteurlite_messageelement_header* header);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_double_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_double* array);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_double(
    struct robotraconteurlite_messageelement_reader* element_reader, double* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_single_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_single* array);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_single(
    struct robotraconteurlite_messageelement_reader* element_reader, float* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_int8_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_int8* array);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_int8(
    struct robotraconteurlite_messageelement_reader* element_reader, int8_t* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_uint8_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_uint8* array);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_uint8(
    struct robotraconteurlite_messageelement_reader* element_reader, uint8_t* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_int16_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_int16* array);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_int16(
    struct robotraconteurlite_messageelement_reader* element_reader, int16_t* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_uint16_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_uint16* array);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_uint16(
    struct robotraconteurlite_messageelement_reader* element_reader, uint16_t* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_int32_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_int32* array);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_int32(
    struct robotraconteurlite_messageelement_reader* element_reader, int32_t* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_uint32_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_uint32* array);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_uint32(
    struct robotraconteurlite_messageelement_reader* element_reader, uint32_t* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_int64_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_int64* array);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_int64(
    struct robotraconteurlite_messageelement_reader* element_reader, int64_t* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_uint64_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_uint64* array);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_uint64(
    struct robotraconteurlite_messageelement_reader* element_reader, uint64_t* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_cdouble_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_cdouble* array);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_cdouble(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_cdouble* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_csingle_array(
    struct robotraconteurlite_messageelement_reader* element_reader,
    struct robotraconteurlite_array_csingle* dest_array);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_csingle(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_csingle* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_bool_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_bool* dest_array);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_bool(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_bool* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_read_data_string(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_string* dest_str);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_begin_read_nested_elements(
    struct robotraconteurlite_messageelement_reader* element_reader,
    struct robotraconteurlite_messageelement_reader* nested_element_reader);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_find_nested_element(
    struct robotraconteurlite_messageelement_reader* element_reader,
    const struct robotraconteurlite_string* nested_element_name,
    struct robotraconteurlite_messageelement_reader* nested_element_reader);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_find_nested_element_verify_scalar(
    struct robotraconteurlite_messageelement_reader* element_reader,
    const struct robotraconteurlite_string* nested_element_name,
    struct robotraconteurlite_messageelement_reader* nested_element_reader, uint16_t expected_data_type);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_find_nested_element_verify_array(
    struct robotraconteurlite_messageelement_reader* element_reader,
    const struct robotraconteurlite_string* nested_element_name,
    struct robotraconteurlite_messageelement_reader* nested_element_reader, uint16_t expected_data_type,
    uint32_t expected_len, uint8_t var_length);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_reader_find_nested_element_verify_string(
    struct robotraconteurlite_messageelement_reader* element_reader,
    const struct robotraconteurlite_string* nested_element_name,
    struct robotraconteurlite_messageelement_reader* nested_element_reader, uint32_t expected_max_len);

/* Message Writer */

ROBOTRACONTEURLITE_DECL int robotraconteurlite_message_writer_init(struct robotraconteurlite_message_writer* writer,
                                                                   struct robotraconteurlite_buffer_vec* buffer,
                                                                   size_t offset, size_t count);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_message_writer_begin_message(
    struct robotraconteurlite_message_writer* writer, struct robotraconteurlite_message_header* header,
    struct robotraconteurlite_messageentry_writer* entry_writer);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_message_writer_end_message(
    struct robotraconteurlite_message_writer* writer, struct robotraconteurlite_message_header* header,
    struct robotraconteurlite_messageentry_writer* entry_writer);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageentry_writer_begin_entry(
    struct robotraconteurlite_messageentry_writer* entry_writer, struct robotraconteurlite_messageentry_header* header,
    struct robotraconteurlite_messageelement_writer* element_writer);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageentry_writer_end_entry(
    struct robotraconteurlite_messageentry_writer* entry_writer, struct robotraconteurlite_messageentry_header* header,
    struct robotraconteurlite_messageelement_writer* element_writer);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_begin_nested_element(
    struct robotraconteurlite_messageelement_writer* element_writer,
    struct robotraconteurlite_messageelement_header* header,
    struct robotraconteurlite_messageelement_writer* nested_element_writer);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_end_nested_element(
    struct robotraconteurlite_messageelement_writer* element_writer,
    struct robotraconteurlite_messageelement_header* header,
    struct robotraconteurlite_messageelement_writer* nested_element_writer);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_double(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, double value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_double_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, const struct robotraconteurlite_array_double* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_single(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, float value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_single_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, const struct robotraconteurlite_array_single* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_int8(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, int8_t value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_int8_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, const struct robotraconteurlite_array_int8* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_uint8(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, uint8_t value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_uint8_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, const struct robotraconteurlite_array_uint8* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_int16(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, int16_t value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_int16_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, const struct robotraconteurlite_array_int16* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_uint16(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, uint16_t value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_uint16_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, const struct robotraconteurlite_array_uint16* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_int32(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, int32_t value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_int32_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, const struct robotraconteurlite_array_int32* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_uint32(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, uint32_t value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_uint32_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, const struct robotraconteurlite_array_uint32* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_int64(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, int64_t value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_int64_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, const struct robotraconteurlite_array_int64* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_uint64(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, uint64_t value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_uint64_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, const struct robotraconteurlite_array_uint64* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_cdouble(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, struct robotraconteurlite_cdouble value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_cdouble_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, const struct robotraconteurlite_array_cdouble* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_csingle(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, struct robotraconteurlite_csingle value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_csingle_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, const struct robotraconteurlite_array_csingle* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_bool(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, struct robotraconteurlite_bool value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_bool_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, const struct robotraconteurlite_array_bool* value);

ROBOTRACONTEURLITE_DECL int robotraconteurlite_messageelement_writer_write_string(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_string* element_name, const struct robotraconteurlite_string* value);

static int robotraconteurlite_messageelement_writer_write_string_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name, const char* value)
{
    struct robotraconteurlite_string element_name_str;
    struct robotraconteurlite_string value_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    robotraconteurlite_string_from_c_str(value, &value_str);
    return robotraconteurlite_messageelement_writer_write_string(element_writer, &element_name_str, &value_str);
}

#endif /* ROBOTRACONTEURLITE_MESSAGE_H */
