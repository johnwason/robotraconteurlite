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

#include "robotraconteurlite/message.h"
#include <string.h>
#include <assert.h>
#include <math.h>

/* readers */
/* double */
robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_double_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_double* dest_array)
{
    return robotraconteurlite_messageelement_reader_read_data_ex(element_reader,
                                                                 /* cppcheck-suppress invalidPointerCast */
                                                                 (robotraconteurlite_byte*)dest_array->data,
                                                                 &dest_array->len, ROBOTRACONTEURLITE_DATATYPE_DOUBLE,
                                                                 sizeof(robotraconteurlite_double));
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_double(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_double* value)
{
    return robotraconteurlite_messageelement_reader_read_data_scalar_ex(
        /* cppcheck-suppress invalidPointerCast */
        element_reader, (robotraconteurlite_byte*)value, ROBOTRACONTEURLITE_DATATYPE_DOUBLE,
        sizeof(robotraconteurlite_double));
}
/* single */
robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_single_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_single* dest_array)
{
    return robotraconteurlite_messageelement_reader_read_data_ex(element_reader,
                                                                 /* cppcheck-suppress invalidPointerCast */
                                                                 (robotraconteurlite_byte*)dest_array->data,
                                                                 &dest_array->len, ROBOTRACONTEURLITE_DATATYPE_SINGLE,
                                                                 sizeof(robotraconteurlite_single));
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_single(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_single* value)
{
    return robotraconteurlite_messageelement_reader_read_data_scalar_ex(
        /* cppcheck-suppress invalidPointerCast */
        element_reader, (robotraconteurlite_byte*)value, ROBOTRACONTEURLITE_DATATYPE_SINGLE,
        sizeof(robotraconteurlite_single));
}
/* int8 */
robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_int8_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_int8* dest_array)
{
    return robotraconteurlite_messageelement_reader_read_data_ex(
        element_reader, (robotraconteurlite_byte*)dest_array->data, &dest_array->len, ROBOTRACONTEURLITE_DATATYPE_INT8,
        sizeof(robotraconteurlite_i8));
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_int8(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_i8* value)
{
    return robotraconteurlite_messageelement_reader_read_data_scalar_ex(element_reader, (robotraconteurlite_byte*)value,
                                                                        ROBOTRACONTEURLITE_DATATYPE_INT8,
                                                                        sizeof(robotraconteurlite_i8));
}
/* uint8 */
robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_uint8_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_uint8* dest_array)
{
    return robotraconteurlite_messageelement_reader_read_data_ex(
        element_reader, (robotraconteurlite_byte*)dest_array->data, &dest_array->len, ROBOTRACONTEURLITE_DATATYPE_UINT8,
        sizeof(robotraconteurlite_u8));
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_uint8(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_u8* value)
{
    return robotraconteurlite_messageelement_reader_read_data_scalar_ex(element_reader, (robotraconteurlite_byte*)value,
                                                                        ROBOTRACONTEURLITE_DATATYPE_UINT8,
                                                                        sizeof(robotraconteurlite_u8));
}
/* int16 */
robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_int16_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_int16* dest_array)
{
    return robotraconteurlite_messageelement_reader_read_data_ex(
        element_reader, (robotraconteurlite_byte*)dest_array->data, &dest_array->len, ROBOTRACONTEURLITE_DATATYPE_INT16,
        sizeof(robotraconteurlite_i16));
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_int16(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_i16* value)
{
    return robotraconteurlite_messageelement_reader_read_data_scalar_ex(element_reader, (robotraconteurlite_byte*)value,
                                                                        ROBOTRACONTEURLITE_DATATYPE_INT16,
                                                                        sizeof(robotraconteurlite_i16));
}
/* uint16 */
robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_uint16_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_uint16* dest_array)
{
    return robotraconteurlite_messageelement_reader_read_data_ex(
        element_reader, (robotraconteurlite_byte*)dest_array->data, &dest_array->len,
        ROBOTRACONTEURLITE_DATATYPE_UINT16, sizeof(robotraconteurlite_u16));
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_uint16(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_u16* value)
{
    return robotraconteurlite_messageelement_reader_read_data_scalar_ex(element_reader, (robotraconteurlite_byte*)value,
                                                                        ROBOTRACONTEURLITE_DATATYPE_UINT16,
                                                                        sizeof(robotraconteurlite_u16));
}
/* int32 */
robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_int32_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_int32* dest_array)
{
    return robotraconteurlite_messageelement_reader_read_data_ex(
        element_reader, (robotraconteurlite_byte*)dest_array->data, &dest_array->len, ROBOTRACONTEURLITE_DATATYPE_INT32,
        sizeof(robotraconteurlite_i32));
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_int32(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_i32* value)
{
    return robotraconteurlite_messageelement_reader_read_data_scalar_ex(element_reader, (robotraconteurlite_byte*)value,
                                                                        ROBOTRACONTEURLITE_DATATYPE_INT32,
                                                                        sizeof(robotraconteurlite_i32));
}
/* uint32 */
robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_uint32_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_uint32* dest_array)
{
    return robotraconteurlite_messageelement_reader_read_data_ex(
        element_reader, (robotraconteurlite_byte*)dest_array->data, &dest_array->len,
        ROBOTRACONTEURLITE_DATATYPE_UINT32, sizeof(robotraconteurlite_u32));
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_uint32(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_u32* value)
{
    return robotraconteurlite_messageelement_reader_read_data_scalar_ex(element_reader, (robotraconteurlite_byte*)value,
                                                                        ROBOTRACONTEURLITE_DATATYPE_UINT32,
                                                                        sizeof(robotraconteurlite_u32));
}
/* int64 */
robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_int64_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_int64* dest_array)
{
    return robotraconteurlite_messageelement_reader_read_data_ex(
        element_reader, (robotraconteurlite_byte*)dest_array->data, &dest_array->len, ROBOTRACONTEURLITE_DATATYPE_INT64,
        sizeof(robotraconteurlite_i64));
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_int64(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_i64* value)
{
    return robotraconteurlite_messageelement_reader_read_data_scalar_ex(element_reader, (robotraconteurlite_byte*)value,
                                                                        ROBOTRACONTEURLITE_DATATYPE_INT64,
                                                                        sizeof(robotraconteurlite_i64));
}
/* uint64 */
robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_uint64_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_uint64* dest_array)
{
    return robotraconteurlite_messageelement_reader_read_data_ex(
        element_reader, (robotraconteurlite_byte*)dest_array->data, &dest_array->len,
        ROBOTRACONTEURLITE_DATATYPE_UINT64, sizeof(robotraconteurlite_u64));
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_uint64(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_u64* value)
{
    return robotraconteurlite_messageelement_reader_read_data_scalar_ex(element_reader, (robotraconteurlite_byte*)value,
                                                                        ROBOTRACONTEURLITE_DATATYPE_UINT64,
                                                                        sizeof(robotraconteurlite_u64));
}
/* cdouble */
robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_cdouble_array(
    struct robotraconteurlite_messageelement_reader* element_reader,
    struct robotraconteurlite_array_cdouble* dest_array)
{
    return robotraconteurlite_messageelement_reader_read_data_ex(
        element_reader, (robotraconteurlite_byte*)dest_array->data, &dest_array->len,
        ROBOTRACONTEURLITE_DATATYPE_CDOUBLE, sizeof(struct robotraconteurlite_cdouble));
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_cdouble(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_cdouble* value)
{
    return robotraconteurlite_messageelement_reader_read_data_scalar_ex(element_reader, (robotraconteurlite_byte*)value,
                                                                        ROBOTRACONTEURLITE_DATATYPE_CDOUBLE,
                                                                        sizeof(struct robotraconteurlite_cdouble));
}
/* csingle */
robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_csingle_array(
    struct robotraconteurlite_messageelement_reader* element_reader,
    struct robotraconteurlite_array_csingle* dest_array)
{
    return robotraconteurlite_messageelement_reader_read_data_ex(
        element_reader, (robotraconteurlite_byte*)dest_array->data, &dest_array->len,
        ROBOTRACONTEURLITE_DATATYPE_CSINGLE, sizeof(struct robotraconteurlite_csingle));
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_csingle(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_csingle* value)
{
    return robotraconteurlite_messageelement_reader_read_data_scalar_ex(element_reader, (robotraconteurlite_byte*)value,
                                                                        ROBOTRACONTEURLITE_DATATYPE_CSINGLE,
                                                                        sizeof(struct robotraconteurlite_csingle));
}
/* bool */
robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_bool_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_bool* dest_array)
{
    return robotraconteurlite_messageelement_reader_read_data_ex(
        element_reader, (robotraconteurlite_byte*)dest_array->data, &dest_array->len, ROBOTRACONTEURLITE_DATATYPE_BOOL,
        sizeof(struct robotraconteurlite_bool));
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_bool(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_bool* value)
{
    return robotraconteurlite_messageelement_reader_read_data_scalar_ex(element_reader, (robotraconteurlite_byte*)value,
                                                                        ROBOTRACONTEURLITE_DATATYPE_BOOL,
                                                                        sizeof(struct robotraconteurlite_bool));
}

/* writers */
/* double */
robotraconteurlite_status robotraconteurlite_messageelement_writer_write_double(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_double value)
{
    struct robotraconteurlite_array_double a;
    a.data = &value;
    a.len = 1;
    return robotraconteurlite_messageelement_writer_write_raw(element_writer, element_name,
                                                              /* cppcheck-suppress invalidPointerCast */
                                                              (const robotraconteurlite_byte*)a.data, a.len,
                                                              ROBOTRACONTEURLITE_DATATYPE_DOUBLE,
                                                              sizeof(robotraconteurlite_double));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_double_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_double* value)
{
    return robotraconteurlite_messageelement_writer_write_raw(element_writer, element_name,
                                                              /* cppcheck-suppress invalidPointerCast */
                                                              (const robotraconteurlite_byte*)value->data, value->len,
                                                              ROBOTRACONTEURLITE_DATATYPE_DOUBLE,
                                                              sizeof(robotraconteurlite_double));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_double_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_double value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_double(element_writer, &element_name_str, value);
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_double_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_double* value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_double_array(element_writer, &element_name_str, value);
}
/* single */
robotraconteurlite_status robotraconteurlite_messageelement_writer_write_single(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_single value)
{
    struct robotraconteurlite_array_single a;
    a.data = &value;
    a.len = 1;
    return robotraconteurlite_messageelement_writer_write_raw(element_writer, element_name,
                                                              /* cppcheck-suppress invalidPointerCast */
                                                              (const robotraconteurlite_byte*)a.data, a.len,
                                                              ROBOTRACONTEURLITE_DATATYPE_SINGLE,
                                                              sizeof(robotraconteurlite_single));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_single_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_single* value)
{
    return robotraconteurlite_messageelement_writer_write_raw(element_writer, element_name,
                                                              /* cppcheck-suppress invalidPointerCast */
                                                              (const robotraconteurlite_byte*)value->data, value->len,
                                                              ROBOTRACONTEURLITE_DATATYPE_SINGLE,
                                                              sizeof(robotraconteurlite_single));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_single_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_single value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_single(element_writer, &element_name_str, value);
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_single_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_single* value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_single_array(element_writer, &element_name_str, value);
}
/* int8 */
robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int8(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_i8 value)
{
    struct robotraconteurlite_array_int8 a;
    a.data = &value;
    a.len = 1;
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)a.data, a.len, ROBOTRACONTEURLITE_DATATYPE_INT8,
        sizeof(robotraconteurlite_i8));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int8_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_int8* value)
{
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)value->data, value->len,
        ROBOTRACONTEURLITE_DATATYPE_INT8, sizeof(robotraconteurlite_i8));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int8_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_i8 value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_int8(element_writer, &element_name_str, value);
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int8_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_int8* value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_int8_array(element_writer, &element_name_str, value);
}
/* uint8 */
robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint8(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_u8 value)
{
    struct robotraconteurlite_array_uint8 a;
    a.data = &value;
    a.len = 1;
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)a.data, a.len, ROBOTRACONTEURLITE_DATATYPE_UINT8,
        sizeof(robotraconteurlite_u8));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint8_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_uint8* value)
{
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)value->data, value->len,
        ROBOTRACONTEURLITE_DATATYPE_UINT8, sizeof(robotraconteurlite_u8));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint8_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_u8 value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_uint8(element_writer, &element_name_str, value);
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint8_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_uint8* value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_uint8_array(element_writer, &element_name_str, value);
}
/* int16 */
robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int16(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_i16 value)
{
    struct robotraconteurlite_array_int16 a;
    a.data = &value;
    a.len = 1;
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)a.data, a.len, ROBOTRACONTEURLITE_DATATYPE_INT16,
        sizeof(robotraconteurlite_i16));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int16_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_int16* value)
{
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)value->data, value->len,
        ROBOTRACONTEURLITE_DATATYPE_INT16, sizeof(robotraconteurlite_i16));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int16_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_i16 value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_int16(element_writer, &element_name_str, value);
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int16_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_int16* value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_int16_array(element_writer, &element_name_str, value);
}
/* uint16 */
robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint16(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_u16 value)
{
    struct robotraconteurlite_array_uint16 a;
    a.data = &value;
    a.len = 1;
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)a.data, a.len, ROBOTRACONTEURLITE_DATATYPE_UINT16,
        sizeof(robotraconteurlite_u16));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint16_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_uint16* value)
{
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)value->data, value->len,
        ROBOTRACONTEURLITE_DATATYPE_UINT16, sizeof(robotraconteurlite_u16));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint16_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_u16 value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_uint16(element_writer, &element_name_str, value);
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint16_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_uint16* value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_uint16_array(element_writer, &element_name_str, value);
}
/* int32 */
robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int32(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_i32 value)
{
    struct robotraconteurlite_array_int32 a;
    a.data = &value;
    a.len = 1;
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)a.data, a.len, ROBOTRACONTEURLITE_DATATYPE_INT32,
        sizeof(robotraconteurlite_i32));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int32_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_int32* value)
{
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)value->data, value->len,
        ROBOTRACONTEURLITE_DATATYPE_INT32, sizeof(robotraconteurlite_i32));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int32_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_i32 value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_int32(element_writer, &element_name_str, value);
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int32_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_int32* value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_int32_array(element_writer, &element_name_str, value);
}
/* uint32 */
robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint32(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_u32 value)
{
    struct robotraconteurlite_array_uint32 a;
    a.data = &value;
    a.len = 1;
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)a.data, a.len, ROBOTRACONTEURLITE_DATATYPE_UINT32,
        sizeof(robotraconteurlite_u32));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint32_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_uint32* value)
{
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)value->data, value->len,
        ROBOTRACONTEURLITE_DATATYPE_UINT32, sizeof(robotraconteurlite_u32));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint32_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_u32 value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_uint32(element_writer, &element_name_str, value);
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint32_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_uint32* value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_uint32_array(element_writer, &element_name_str, value);
}
/* int64 */
robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int64(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_i64 value)
{
    struct robotraconteurlite_array_int64 a;
    a.data = &value;
    a.len = 1;
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)a.data, a.len, ROBOTRACONTEURLITE_DATATYPE_INT64,
        sizeof(robotraconteurlite_i64));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int64_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_int64* value)
{
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)value->data, value->len,
        ROBOTRACONTEURLITE_DATATYPE_INT64, sizeof(robotraconteurlite_i64));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int64_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_i64 value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_int64(element_writer, &element_name_str, value);
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int64_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_int64* value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_int64_array(element_writer, &element_name_str, value);
}
/* uint64 */
robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint64(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_u64 value)
{
    struct robotraconteurlite_array_uint64 a;
    a.data = &value;
    a.len = 1;
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)a.data, a.len, ROBOTRACONTEURLITE_DATATYPE_UINT64,
        sizeof(robotraconteurlite_u64));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint64_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_uint64* value)
{
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)value->data, value->len,
        ROBOTRACONTEURLITE_DATATYPE_UINT64, sizeof(robotraconteurlite_u64));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint64_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_u64 value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_uint64(element_writer, &element_name_str, value);
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint64_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_uint64* value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_uint64_array(element_writer, &element_name_str, value);
}
/* cdouble */
robotraconteurlite_status robotraconteurlite_messageelement_writer_write_cdouble(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, struct robotraconteurlite_cdouble value)
{
    struct robotraconteurlite_array_cdouble a;
    a.data = &value;
    a.len = 1;
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)a.data, a.len,
        ROBOTRACONTEURLITE_DATATYPE_CDOUBLE, sizeof(struct robotraconteurlite_cdouble));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_cdouble_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_cdouble* value)
{
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)value->data, value->len,
        ROBOTRACONTEURLITE_DATATYPE_CDOUBLE, sizeof(struct robotraconteurlite_cdouble));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_cdouble_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    struct robotraconteurlite_cdouble value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_cdouble(element_writer, &element_name_str, value);
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_cdouble_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_cdouble* value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_cdouble_array(element_writer, &element_name_str, value);
}
/* csingle */
robotraconteurlite_status robotraconteurlite_messageelement_writer_write_csingle(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, struct robotraconteurlite_csingle value)
{
    struct robotraconteurlite_array_csingle a;
    a.data = &value;
    a.len = 1;
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)a.data, a.len,
        ROBOTRACONTEURLITE_DATATYPE_CSINGLE, sizeof(struct robotraconteurlite_csingle));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_csingle_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_csingle* value)
{
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)value->data, value->len,
        ROBOTRACONTEURLITE_DATATYPE_CSINGLE, sizeof(struct robotraconteurlite_csingle));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_csingle_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    struct robotraconteurlite_csingle value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_csingle(element_writer, &element_name_str, value);
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_csingle_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_csingle* value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_csingle_array(element_writer, &element_name_str, value);
}
/* bool */
robotraconteurlite_status robotraconteurlite_messageelement_writer_write_bool(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, struct robotraconteurlite_bool value)
{
    struct robotraconteurlite_array_bool a;
    a.data = &value;
    a.len = 1;
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)a.data, a.len, ROBOTRACONTEURLITE_DATATYPE_BOOL,
        sizeof(struct robotraconteurlite_bool));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_bool_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_bool* value)
{
    return robotraconteurlite_messageelement_writer_write_raw(
        element_writer, element_name, (const robotraconteurlite_byte*)value->data, value->len,
        ROBOTRACONTEURLITE_DATATYPE_BOOL, sizeof(struct robotraconteurlite_bool));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_bool_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    struct robotraconteurlite_bool value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_bool(element_writer, &element_name_str, value);
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_bool_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_bool* value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_bool_array(element_writer, &element_name_str, value);
}

/* string */

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_string(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_string* dest_str)
{
    return robotraconteurlite_messageelement_reader_read_data_ex(
        element_reader, (robotraconteurlite_byte*)dest_str->data, &dest_str->len, ROBOTRACONTEURLITE_DATATYPE_STRING,
        sizeof(char));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_data_string(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_const_string* value)
{
    return robotraconteurlite_messageelement_writer_write_raw(element_writer, element_name,
                                                              (const robotraconteurlite_byte*)value->data, value->len,
                                                              ROBOTRACONTEURLITE_DATATYPE_STRING, sizeof(char));
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_data_string_c_str2(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name, const char* value)
{
    struct robotraconteurlite_const_string element_name_str;
    struct robotraconteurlite_const_string value_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    robotraconteurlite_string_from_c_str(value, &value_str);
    return robotraconteurlite_messageelement_writer_write_data_string(element_writer, &element_name_str, &value_str);
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_data_string_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_const_string* value)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageelement_writer_write_data_string(element_writer, &element_name_str, value);
}
