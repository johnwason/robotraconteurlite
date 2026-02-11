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

#ifndef ROBOTRACONTEURLITE_ARRAY_H
#define ROBOTRACONTEURLITE_ARRAY_H

#include "robotraconteurlite/err.h"
#include "robotraconteurlite/config.h"
#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

struct robotraconteurlite_array_storage
{
    void* data;
    robotraconteurlite_size_t len;
};

struct robotraconteurlite_buffer
{
    robotraconteurlite_byte* data;
    robotraconteurlite_size_t len;
};

struct robotraconteurlite_buffer_vec
{
    struct robotraconteurlite_buffer* buffer_vec;
    robotraconteurlite_size_t buffer_vec_cnt;

    struct robotraconteurlite_array_storage _scalar_storage;
};

/* Complex types */
struct robotraconteurlite_cdouble
{
    robotraconteurlite_double real;
    robotraconteurlite_double imag;
};

struct robotraconteurlite_csingle
{
    robotraconteurlite_single real;
    robotraconteurlite_single imag;
};

struct robotraconteurlite_bool
{
    robotraconteurlite_u8 logical;
};

/* All strings are utf8! */
struct robotraconteurlite_string
{
    char* data;
    robotraconteurlite_size_t len;
};

struct robotraconteurlite_string_vec
{
    struct robotraconteurlite_string* string_vec;
    robotraconteurlite_size_t string_vec_cnt;

    struct robotraconteurlite_array_storage _scalar_storage;
};

struct robotraconteurlite_const_string
{
    const char* data;
    robotraconteurlite_size_t len;
};

struct robotraconteurlite_const_string_vec
{
    struct robotraconteurlite_const_string* string_vec;
    robotraconteurlite_size_t string_vec_cnt;

    struct robotraconteurlite_array_storage _scalar_storage;
};

struct robotraconteurlite_array_double
{
    robotraconteurlite_double* data;
    robotraconteurlite_size_t len;
};

struct robotraconteurlite_array_vec_double
{
    struct robotraconteurlite_array_double* array_vec;
    robotraconteurlite_size_t array_vec_cnt;

    struct robotraconteurlite_array_storage _scalar_storage;
};

struct robotraconteurlite_array_single
{
    robotraconteurlite_single* data;
    robotraconteurlite_size_t len;
};

struct robotraconteurlite_array_vec_single
{
    struct robotraconteurlite_array_single* array_vec;
    robotraconteurlite_size_t array_vec_cnt;

    struct robotraconteurlite_array_storage _scalar_storage;
};

struct robotraconteurlite_array_int8
{
    robotraconteurlite_i8* data;
    robotraconteurlite_size_t len;
};

struct robotraconteurlite_array_vec_int8
{
    struct robotraconteurlite_array_int8* array_vec;
    robotraconteurlite_size_t array_vec_cnt;

    struct robotraconteurlite_array_storage _scalar_storage;
};

struct robotraconteurlite_array_uint8
{
    robotraconteurlite_u8* data;
    robotraconteurlite_size_t len;
};

struct robotraconteurlite_array_vec_uint8
{
    struct robotraconteurlite_array_uint8* array_vec;
    robotraconteurlite_size_t array_vec_cnt;

    struct robotraconteurlite_array_storage _scalar_storage;
};

struct robotraconteurlite_array_int16
{
    robotraconteurlite_i16* data;
    robotraconteurlite_size_t len;
};

struct robotraconteurlite_array_vec_int16
{
    struct robotraconteurlite_array_int16* array_vec;
    robotraconteurlite_size_t array_vec_cnt;

    struct robotraconteurlite_array_storage _scalar_storage;
};

struct robotraconteurlite_array_uint16
{
    robotraconteurlite_u16* data;
    robotraconteurlite_size_t len;
};

struct robotraconteurlite_array_vec_uint16
{
    struct robotraconteurlite_array_uint16* array_vec;
    robotraconteurlite_size_t array_vec_cnt;

    struct robotraconteurlite_array_storage _scalar_storage;
};

struct robotraconteurlite_array_int32
{
    robotraconteurlite_i32* data;
    robotraconteurlite_size_t len;
};

struct robotraconteurlite_array_vec_int32
{
    struct robotraconteurlite_array_int32* array_vec;
    robotraconteurlite_size_t array_vec_cnt;

    struct robotraconteurlite_array_storage _scalar_storage;
};

struct robotraconteurlite_array_uint32
{
    robotraconteurlite_u32* data;
    robotraconteurlite_size_t len;
};

struct robotraconteurlite_array_vec_uint32
{
    struct robotraconteurlite_array_uint32* array_vec;
    robotraconteurlite_size_t array_vec_cnt;

    struct robotraconteurlite_array_storage _scalar_storage;
};

struct robotraconteurlite_array_int64
{
    robotraconteurlite_i64* data;
    robotraconteurlite_size_t len;
};

struct robotraconteurlite_array_vec_int64
{
    struct robotraconteurlite_array_int64* array_vec;
    robotraconteurlite_size_t array_vec_cnt;

    struct robotraconteurlite_array_storage _scalar_storage;
};

struct robotraconteurlite_array_uint64
{
    robotraconteurlite_u64* data;
    robotraconteurlite_size_t len;
};

struct robotraconteurlite_array_vec_uint64
{
    struct robotraconteurlite_array_uint64* array_vec;
    robotraconteurlite_size_t array_vec_cnt;

    struct robotraconteurlite_array_storage _scalar_storage;
};

struct robotraconteurlite_array_cdouble
{
    struct robotraconteurlite_cdouble* data;
    robotraconteurlite_size_t len;
};

struct robotraconteurlite_array_vec_cdouble
{
    struct robotraconteurlite_array_cdouble* array_vec;
    robotraconteurlite_size_t array_vec_cnt;

    struct robotraconteurlite_array_storage _scalar_storage;
};

struct robotraconteurlite_array_csingle
{
    struct robotraconteurlite_csingle* data;
    robotraconteurlite_size_t len;
};

struct robotraconteurlite_array_vec_csingle
{
    struct robotraconteurlite_array_csingle* array_vec;
    robotraconteurlite_size_t array_vec_cnt;

    struct robotraconteurlite_array_storage _scalar_storage;
};

struct robotraconteurlite_array_bool
{
    struct robotraconteurlite_bool* data;
    robotraconteurlite_size_t len;
};

struct robotraconteurlite_array_vec_bool
{
    struct robotraconteurlite_array_bool* array_vec;
    robotraconteurlite_size_t array_vec_cnt;

    struct robotraconteurlite_array_storage _scalar_storage;
};

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_init_scalar(
    struct robotraconteurlite_buffer* buffer, void* data, robotraconteurlite_size_t len);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_init_scalar(
    struct robotraconteurlite_buffer_vec* buffer_vec, struct robotraconteurlite_buffer* buffer);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_len(const struct robotraconteurlite_buffer* source, robotraconteurlite_size_t* len);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_len(const struct robotraconteurlite_buffer_vec* source, robotraconteurlite_size_t* len);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_copy(
    const struct robotraconteurlite_buffer* source, robotraconteurlite_size_t source_pos,
    const struct robotraconteurlite_buffer* dest, robotraconteurlite_size_t dest_pos, robotraconteurlite_size_t count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_vec(
    const struct robotraconteurlite_buffer_vec* source, robotraconteurlite_size_t source_pos,
    const struct robotraconteurlite_buffer_vec* dest, robotraconteurlite_size_t dest_pos,
    robotraconteurlite_size_t count);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_copy_ex(const struct robotraconteurlite_buffer* source, robotraconteurlite_size_t source_pos,
                                  robotraconteurlite_size_t source_elem_size, robotraconteurlite_size_t source_count,
                                  const struct robotraconteurlite_buffer* dest, robotraconteurlite_size_t dest_pos,
                                  robotraconteurlite_size_t dest_elem_size, robotraconteurlite_size_t dest_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_vec_ex(
    const struct robotraconteurlite_buffer_vec* source, robotraconteurlite_size_t source_pos,
    robotraconteurlite_size_t source_elem_size, robotraconteurlite_size_t source_count,
    const struct robotraconteurlite_buffer_vec* dest, robotraconteurlite_size_t dest_pos,
    robotraconteurlite_size_t dest_elem_size, robotraconteurlite_size_t dest_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_to_mem(
    const struct robotraconteurlite_buffer_vec* source, robotraconteurlite_size_t source_pos,
    robotraconteurlite_byte* dest, robotraconteurlite_size_t dest_len, robotraconteurlite_size_t dest_pos,
    robotraconteurlite_size_t dest_elem_size, robotraconteurlite_size_t dest_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_mem(
    struct robotraconteurlite_buffer_vec* dest, robotraconteurlite_size_t dest_pos,
    const robotraconteurlite_byte* source, robotraconteurlite_size_t source_len, robotraconteurlite_size_t source_pos,
    robotraconteurlite_size_t source_elem_size, robotraconteurlite_size_t source_count);

/* double */
ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_to_double_array(
    const struct robotraconteurlite_buffer_vec* source_buf, robotraconteurlite_size_t source_buf_pos,
    struct robotraconteurlite_array_double* dest, robotraconteurlite_size_t dest_pos,
    robotraconteurlite_size_t dest_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_double_array(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    const struct robotraconteurlite_array_double* source, robotraconteurlite_size_t source_pos,
    robotraconteurlite_size_t source_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_copy_to_double(const struct robotraconteurlite_buffer_vec* source_buf,
                                             robotraconteurlite_size_t source_buf_pos, robotraconteurlite_double* dest);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_double(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    robotraconteurlite_double source);

/* single */
ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_to_single_array(
    const struct robotraconteurlite_buffer_vec* source_buf, robotraconteurlite_size_t source_buf_pos,
    struct robotraconteurlite_array_single* dest, robotraconteurlite_size_t dest_pos,
    robotraconteurlite_size_t dest_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_single_array(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    const struct robotraconteurlite_array_single* source, robotraconteurlite_size_t source_pos,
    robotraconteurlite_size_t source_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_copy_to_single(const struct robotraconteurlite_buffer_vec* source_buf,
                                             robotraconteurlite_size_t source_buf_pos, robotraconteurlite_single* dest);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_single(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    robotraconteurlite_single source);

/* int8 */
ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_to_int8_array(
    const struct robotraconteurlite_buffer_vec* source_buf, robotraconteurlite_size_t source_buf_pos,
    struct robotraconteurlite_array_int8* dest, robotraconteurlite_size_t dest_pos,
    robotraconteurlite_size_t dest_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_int8_array(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    const struct robotraconteurlite_array_int8* source, robotraconteurlite_size_t source_pos,
    robotraconteurlite_size_t source_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_copy_to_int8(const struct robotraconteurlite_buffer_vec* source_buf,
                                           robotraconteurlite_size_t source_buf_pos, robotraconteurlite_i8* dest);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_copy_from_int8(struct robotraconteurlite_buffer_vec* dest_buf,
                                             robotraconteurlite_size_t dest_buf_pos, robotraconteurlite_i8 source);

/* uint8 */
ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_to_uint8_array(
    const struct robotraconteurlite_buffer_vec* source_buf, robotraconteurlite_size_t source_buf_pos,
    struct robotraconteurlite_array_uint8* dest, robotraconteurlite_size_t dest_pos,
    robotraconteurlite_size_t dest_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_uint8_array(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    const struct robotraconteurlite_array_uint8* source, robotraconteurlite_size_t source_pos,
    robotraconteurlite_size_t source_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_copy_to_uint8(const struct robotraconteurlite_buffer_vec* source_buf,
                                            robotraconteurlite_size_t source_buf_pos, robotraconteurlite_u8* dest);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_copy_from_uint8(struct robotraconteurlite_buffer_vec* dest_buf,
                                              robotraconteurlite_size_t dest_buf_pos, robotraconteurlite_u8 source);

/* int16 */
ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_to_int16_array(
    const struct robotraconteurlite_buffer_vec* source_buf, robotraconteurlite_size_t source_buf_pos,
    struct robotraconteurlite_array_int16* dest, robotraconteurlite_size_t dest_pos,
    robotraconteurlite_size_t dest_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_int16_array(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    const struct robotraconteurlite_array_int16* source, robotraconteurlite_size_t source_pos,
    robotraconteurlite_size_t source_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_copy_to_int16(const struct robotraconteurlite_buffer_vec* source_buf,
                                            robotraconteurlite_size_t source_buf_pos, robotraconteurlite_i16* dest);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_copy_from_int16(struct robotraconteurlite_buffer_vec* dest_buf,
                                              robotraconteurlite_size_t dest_buf_pos, robotraconteurlite_i16 source);

/* uint16 */
ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_to_uint16_array(
    const struct robotraconteurlite_buffer_vec* source_buf, robotraconteurlite_size_t source_buf_pos,
    struct robotraconteurlite_array_uint16* dest, robotraconteurlite_size_t dest_pos,
    robotraconteurlite_size_t dest_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_uint16_array(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    const struct robotraconteurlite_array_uint16* source, robotraconteurlite_size_t source_pos,
    robotraconteurlite_size_t source_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_copy_to_uint16(const struct robotraconteurlite_buffer_vec* source_buf,
                                             robotraconteurlite_size_t source_buf_pos, robotraconteurlite_u16* dest);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_copy_from_uint16(struct robotraconteurlite_buffer_vec* dest_buf,
                                               robotraconteurlite_size_t dest_buf_pos, robotraconteurlite_u16 source);

/* int32 */
ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_to_int32_array(
    const struct robotraconteurlite_buffer_vec* source_buf, robotraconteurlite_size_t source_buf_pos,
    struct robotraconteurlite_array_int32* dest, robotraconteurlite_size_t dest_pos,
    robotraconteurlite_size_t dest_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_int32_array(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    const struct robotraconteurlite_array_int32* source, robotraconteurlite_size_t source_pos,
    robotraconteurlite_size_t source_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_copy_to_int32(const struct robotraconteurlite_buffer_vec* source_buf,
                                            robotraconteurlite_size_t source_buf_pos, robotraconteurlite_i32* dest);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_copy_from_int32(struct robotraconteurlite_buffer_vec* dest_buf,
                                              robotraconteurlite_size_t dest_buf_pos, robotraconteurlite_i32 source);

/* uint32 */
ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_to_uint32_array(
    const struct robotraconteurlite_buffer_vec* source_buf, robotraconteurlite_size_t source_buf_pos,
    struct robotraconteurlite_array_uint32* dest, robotraconteurlite_size_t dest_pos,
    robotraconteurlite_size_t dest_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_uint32_array(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    const struct robotraconteurlite_array_uint32* source, robotraconteurlite_size_t source_pos,
    robotraconteurlite_size_t source_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_copy_to_uint32(const struct robotraconteurlite_buffer_vec* source_buf,
                                             robotraconteurlite_size_t source_buf_pos, robotraconteurlite_u32* dest);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_copy_from_uint32(struct robotraconteurlite_buffer_vec* dest_buf,
                                               robotraconteurlite_size_t dest_buf_pos, robotraconteurlite_u32 source);

/* int64 */
ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_to_int64_array(
    const struct robotraconteurlite_buffer_vec* source_buf, robotraconteurlite_size_t source_buf_pos,
    struct robotraconteurlite_array_int64* dest, robotraconteurlite_size_t dest_pos,
    robotraconteurlite_size_t dest_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_int64_array(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    const struct robotraconteurlite_array_int64* source, robotraconteurlite_size_t source_pos,
    robotraconteurlite_size_t source_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_copy_to_int64(const struct robotraconteurlite_buffer_vec* source_buf,
                                            robotraconteurlite_size_t source_buf_pos, robotraconteurlite_i64* dest);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_copy_from_int64(struct robotraconteurlite_buffer_vec* dest_buf,
                                              robotraconteurlite_size_t dest_buf_pos, robotraconteurlite_i64 source);

/* uint64 */
ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_to_uint64_array(
    const struct robotraconteurlite_buffer_vec* source_buf, robotraconteurlite_size_t source_buf_pos,
    struct robotraconteurlite_array_uint64* dest, robotraconteurlite_size_t dest_pos,
    robotraconteurlite_size_t dest_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_uint64_array(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    const struct robotraconteurlite_array_uint64* source, robotraconteurlite_size_t source_pos,
    robotraconteurlite_size_t source_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_copy_to_uint64(const struct robotraconteurlite_buffer_vec* source_buf,
                                             robotraconteurlite_size_t source_buf_pos, robotraconteurlite_u64* dest);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_buffer_vec_copy_from_uint64(struct robotraconteurlite_buffer_vec* dest_buf,
                                               robotraconteurlite_size_t dest_buf_pos, robotraconteurlite_u64 source);

/* cdouble */
ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_to_cdouble_array(
    const struct robotraconteurlite_buffer_vec* source_buf, robotraconteurlite_size_t source_buf_pos,
    struct robotraconteurlite_array_cdouble* dest, robotraconteurlite_size_t dest_pos,
    robotraconteurlite_size_t dest_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_cdouble_array(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    const struct robotraconteurlite_array_cdouble* source, robotraconteurlite_size_t source_pos,
    robotraconteurlite_size_t source_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_to_cdouble(
    const struct robotraconteurlite_buffer_vec* source_buf, robotraconteurlite_size_t source_buf_pos,
    struct robotraconteurlite_cdouble* dest);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_cdouble(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    struct robotraconteurlite_cdouble source);

/* csingle */
ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_to_csingle_array(
    const struct robotraconteurlite_buffer_vec* source_buf, robotraconteurlite_size_t source_buf_pos,
    struct robotraconteurlite_array_csingle* dest, robotraconteurlite_size_t dest_pos,
    robotraconteurlite_size_t dest_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_csingle_array(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    const struct robotraconteurlite_array_csingle* source, robotraconteurlite_size_t source_pos,
    robotraconteurlite_size_t source_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_to_csingle(
    const struct robotraconteurlite_buffer_vec* source_buf, robotraconteurlite_size_t source_buf_pos,
    struct robotraconteurlite_csingle* dest);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_csingle(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    struct robotraconteurlite_csingle source);

/* bool */
ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_to_bool_array(
    const struct robotraconteurlite_buffer_vec* source_buf, robotraconteurlite_size_t source_buf_pos,
    struct robotraconteurlite_array_bool* dest, robotraconteurlite_size_t dest_pos,
    robotraconteurlite_size_t dest_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_bool_array(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    const struct robotraconteurlite_array_bool* source, robotraconteurlite_size_t source_pos,
    robotraconteurlite_size_t source_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_to_bool(
    const struct robotraconteurlite_buffer_vec* source_buf, robotraconteurlite_size_t source_buf_pos,
    struct robotraconteurlite_bool* dest);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_bool(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    struct robotraconteurlite_bool source);

/* string */
ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_to_string(
    const struct robotraconteurlite_buffer_vec* source_buf, robotraconteurlite_size_t source_buf_pos,
    struct robotraconteurlite_string* dest, robotraconteurlite_size_t dest_pos, robotraconteurlite_size_t dest_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_buffer_vec_copy_from_string(
    struct robotraconteurlite_buffer_vec* dest_buf, robotraconteurlite_size_t dest_buf_pos,
    const struct robotraconteurlite_const_string* source, robotraconteurlite_size_t source_pos,
    robotraconteurlite_size_t source_count);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_string_cmp(
    const struct robotraconteurlite_const_string* str1, const struct robotraconteurlite_const_string* str2);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_string_cmp_mutable(
    const struct robotraconteurlite_string* str1, const struct robotraconteurlite_const_string* str2);

static int robotraconteurlite_string_cmp_c_str(const struct robotraconteurlite_const_string* str1, const char* str2)
{
    robotraconteurlite_size_t strlen_str2 = strlen(str2);
    if ((strlen_str2 == 0U) && (str1->len == 0U))
    {
        return 0;
    }

    if (str1->len == 0U)
    {
        return -1;
    }

    if (strlen_str2 == 0U)
    {
        return 1;
    }

    if (str1->len < strlen_str2)
    {
        return -1;
    }

    if (str1->len > strlen_str2)
    {
        return 1;
    }

    /* The char null terminator is not used by this function */
    /* cppcheck-suppress misra-c2012-21.14 */
    return memcmp(str1->data, str2, str1->len);
}

ROBOTRACONTEURLITE_API robotraconteurlite_u32
robotraconteurlite_string_hash(const struct robotraconteurlite_const_string* str);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_string_copy_to(
    const struct robotraconteurlite_const_string* source, struct robotraconteurlite_string* dest);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_string_copy_to_buffer_storage(
    const struct robotraconteurlite_const_string* source, struct robotraconteurlite_const_string* dest,
    char* buffer_storage, robotraconteurlite_size_t buffer_storage_len);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_string_shallow_copy_to(
    const struct robotraconteurlite_const_string* source, struct robotraconteurlite_const_string* dest);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_string_shallow_copy_from_mutable(
    const struct robotraconteurlite_string* source, struct robotraconteurlite_const_string* dest);

static void robotraconteurlite_string_from_c_str(const char* source, struct robotraconteurlite_const_string* dest)
{
    if (source == NULL)
    {
        dest->data = NULL;
        dest->len = 0;
        return;
    }

    dest->data = source;
    dest->len = strlen(source);
}

#ifdef __cplusplus
}
#endif

#endif /* ROBOTRACONTEURLITE_ARRAY_H */
