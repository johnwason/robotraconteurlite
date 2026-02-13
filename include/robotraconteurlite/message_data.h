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

#ifndef ROBOTRACONTEURLITE_MESSAGE_DATA_H
#define ROBOTRACONTEURLITE_MESSAGE_DATA_H

#include "robotraconteurlite/message.h"

#ifdef __cplusplus
extern "C" {
#endif

struct robotraconteurlite_messageelement_reader;
struct robotraconteurlite_messageelement_writer;

/* reader */

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_double_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_double* array);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_double(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_double* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_single_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_single* array);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_single(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_single* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_int8_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_int8* array);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_int8(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_i8* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_uint8_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_uint8* array);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_uint8(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_u8* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_int16_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_int16* array);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_int16(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_i16* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_uint16_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_uint16* array);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_uint16(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_u16* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_int32_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_int32* array);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_int32(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_i32* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_uint32_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_uint32* array);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_uint32(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_u32* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_int64_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_int64* array);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_int64(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_i64* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_uint64_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_uint64* array);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_uint64(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_u64* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_cdouble_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_cdouble* array);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_cdouble(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_cdouble* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_csingle_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_csingle* array);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_csingle(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_csingle* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_bool_array(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_array_bool* array);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_bool(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_bool* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_string(
    struct robotraconteurlite_messageelement_reader* element_reader, struct robotraconteurlite_string* dest_str);

/* writer */

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_double(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_double value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_double_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_double* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_double_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_double value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_double_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_double* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_single(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_single value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_single_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_single* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_single_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_single value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_single_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_single* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int8(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_i8 value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int8_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_int8* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int8_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_i8 value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int8_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_int8* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint8(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_u8 value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint8_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_uint8* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint8_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_u8 value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint8_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_uint8* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int16(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_i16 value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int16_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_int16* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int16_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_i16 value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int16_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_int16* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint16(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_u16 value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint16_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_uint16* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint16_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_u16 value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint16_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_uint16* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int32(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_i32 value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int32_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_int32* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int32_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_i32 value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int32_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_int32* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint32(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_u32 value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint32_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_uint32* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint32_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_u32 value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint32_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_uint32* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int64(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_i64 value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int64_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_int64* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int64_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_i64 value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_int64_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_int64* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint64(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_u64 value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint64_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_uint64* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint64_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    robotraconteurlite_u64 value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_uint64_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_uint64* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_cdouble(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, struct robotraconteurlite_cdouble value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_cdouble_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_cdouble* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_cdouble_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    struct robotraconteurlite_cdouble value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_cdouble_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_cdouble* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_csingle(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, struct robotraconteurlite_csingle value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_csingle_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_csingle* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_csingle_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    struct robotraconteurlite_csingle value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_csingle_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_csingle* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_bool(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, struct robotraconteurlite_bool value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_bool_array(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_array_bool* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_bool_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    struct robotraconteurlite_bool value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_bool_array_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_array_bool* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_data_string(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const struct robotraconteurlite_const_string* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_data_string_c_str(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name,
    const struct robotraconteurlite_const_string* value);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_messageelement_writer_write_data_string_c_str2(
    struct robotraconteurlite_messageelement_writer* element_writer, const char* element_name, const char* value);

#ifdef __cplusplus
}
#endif

#endif /* ROBOTRACONTEURLITE_MESSAGE_DATA_H */
