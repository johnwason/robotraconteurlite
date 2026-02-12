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
#include "robotraconteurlite/util.h"

#define FLAGS_CHECK_ALL ROBOTRACONTEURLITE_FLAGS_CHECK_ALL
#define FLAGS_CHECK ROBOTRACONTEURLITE_FLAGS_CHECK
#define FLAGS_SET ROBOTRACONTEURLITE_FLAGS_SET
#define FLAGS_CLEAR ROBOTRACONTEURLITE_FLAGS_CLEAR

#define FAILED ROBOTRACONTEURLITE_FAILED
#define NO_MORE ROBOTRACONTEURLITE_NO_MORE

static const char rrac_magic[4] = {'R', 'R', 'A', 'C'};

/* Message Reader */

robotraconteurlite_status robotraconteurlite_message_reader_init(struct robotraconteurlite_message_reader* reader,
                                                                 struct robotraconteurlite_buffer_vec* buffer,
                                                                 robotraconteurlite_size_t offset,
                                                                 robotraconteurlite_size_t count)
{
    robotraconteurlite_size_t buffer_len = 0;
    robotraconteurlite_status rv = -1;
    robotraconteurlite_u8 magic_data[4];
    struct robotraconteurlite_array_uint8 magic_array;
    robotraconteurlite_u32 message_size = 0;

    magic_array.data = magic_data;
    magic_array.len = 4;

    rv = robotraconteurlite_buffer_vec_len(buffer, &buffer_len);
    if (FAILED(rv))
    {
        return rv;
    }

    if (buffer_len < (offset + count))
    {
        return ROBOTRACONTEURLITE_ERROR_OUT_OF_RANGE;
    }

    if (count < 10U)
    {
        return ROBOTRACONTEURLITE_ERROR_OUT_OF_RANGE;
    }

    rv = robotraconteurlite_buffer_vec_copy_to_uint8_array(buffer, offset, &magic_array, 0, 4);
    if (FAILED(rv))
    {
        return rv;
    }

    if (memcmp(magic_data, rrac_magic, 4) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }

    rv = robotraconteurlite_buffer_vec_copy_to_uint32(buffer, offset + 4U, &message_size);
    if (FAILED(rv))
    {
        return rv;
    }

    if (count < message_size)
    {
        return ROBOTRACONTEURLITE_ERROR_OUT_OF_RANGE;
    }

    reader->buffer = buffer;
    reader->buffer_offset = offset;
    reader->buffer_count = count;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_message_read_header_string(
    struct robotraconteurlite_buffer_vec* buffer, robotraconteurlite_size_t offset, robotraconteurlite_u16 str_len,
    struct robotraconteurlite_string* str)
{
    robotraconteurlite_status rv = -1;

    if (str_len == 0U)
    {
        str->len = 0;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    if ((str->data == NULL) && (str->len == 0U))
    {
        str->len = str_len;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    else
    {
        assert(str->data != NULL);
        if (str->len < str_len)
        {
            return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
        }

        rv = robotraconteurlite_buffer_vec_copy_to_string(buffer, offset, str, 0, str_len);
        if (FAILED(rv))
        {
            return rv;
        }

        str->len = str_len;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
}

static robotraconteurlite_status robotraconteurlite_message_read_uint32(struct robotraconteurlite_buffer_vec* buffer,
                                                                        robotraconteurlite_size_t* offset,
                                                                        robotraconteurlite_u32* val)
{
    robotraconteurlite_status rv = -1;
    rv = robotraconteurlite_buffer_vec_copy_to_uint32(buffer, *offset, val);
    if (FAILED(rv))
    {
        return rv;
    }
    *offset += 4U;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_message_read_uint16(struct robotraconteurlite_buffer_vec* buffer,
                                                                        robotraconteurlite_size_t* offset,
                                                                        robotraconteurlite_u16* val)
{
    robotraconteurlite_status rv = -1;
    rv = robotraconteurlite_buffer_vec_copy_to_uint16(buffer, *offset, val);
    if (FAILED(rv))
    {
        return rv;
    }
    *offset += 2U;
    return rv;
}

static robotraconteurlite_status robotraconteurlite_message_read_uint8(struct robotraconteurlite_buffer_vec* buffer,
                                                                       robotraconteurlite_size_t* offset,
                                                                       robotraconteurlite_byte* val)
{
    robotraconteurlite_status rv = -1;
    rv = robotraconteurlite_buffer_vec_copy_to_uint8(buffer, *offset, val);
    if (FAILED(rv))
    {
        return rv;
    }
    *offset += 1U;
    return rv;
}

static robotraconteurlite_status robotraconteurlite_message_read_uint_x(struct robotraconteurlite_buffer_vec* buffer,
                                                                        robotraconteurlite_size_t* offset,
                                                                        robotraconteurlite_u32* val)
{
    robotraconteurlite_status rv = 0;
    robotraconteurlite_u8 b = 0;
    rv = robotraconteurlite_message_read_uint8(buffer, offset, &b);
    if (FAILED(rv))
    {
        return rv;
    }

    if (b <= 252U)
    {
        *val = b;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    switch (b)
    {
    case 253: {
        robotraconteurlite_u16 val16 = 0;
        rv = robotraconteurlite_message_read_uint16(buffer, offset, &val16);
        if (FAILED(rv))
        {
            return rv;
        }
        *val = val16;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    case 254:
        return robotraconteurlite_message_read_uint32(buffer, offset, val);
    default:
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }
}

static robotraconteurlite_status robotraconteurlite_message_read_int_x(struct robotraconteurlite_buffer_vec* buffer,
                                                                       robotraconteurlite_size_t* offset,
                                                                       robotraconteurlite_i32* val)
{
    robotraconteurlite_status rv = 0;
    robotraconteurlite_i8 b = 0;
    rv = robotraconteurlite_buffer_vec_copy_to_int8(buffer, *offset, &b);
    if (FAILED(rv))
    {
        return rv;
    }
    *offset += 1U;

    if (b <= 124)
    {
        /*NOLINTNEXTLINE(bugprone-signed-char-misuse)*/
        *val = b;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    switch (b)
    {
    case 125: {
        robotraconteurlite_i16 val16 = 0;
        rv = robotraconteurlite_buffer_vec_copy_to_int16(buffer, *offset, &val16);
        if (FAILED(rv))
        {
            return rv;
        }
        *val = val16;
        *offset += 2U;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    case 126: {
        robotraconteurlite_i32 val32 = 0;
        rv = robotraconteurlite_buffer_vec_copy_to_int32(buffer, *offset, &val32);
        if (FAILED(rv))
        {
            return rv;
        }
        *val = val32;
        *offset += 4U;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    default:
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }
}

static robotraconteurlite_status robotraconteurlite_message_read_count(struct robotraconteurlite_buffer_vec* buffer,
                                                                       robotraconteurlite_u16 ver,
                                                                       robotraconteurlite_size_t* offset,
                                                                       robotraconteurlite_u32* val)
{
    switch (ver)
    {
    case 2:
        return robotraconteurlite_message_read_uint32(buffer, offset, val);
    case 4:
        return robotraconteurlite_message_read_uint_x(buffer, offset, val);
    default:
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }
}

static robotraconteurlite_status robotraconteurlite_message_read_count2(struct robotraconteurlite_buffer_vec* buffer,
                                                                        robotraconteurlite_u16 ver,
                                                                        robotraconteurlite_size_t* offset,
                                                                        robotraconteurlite_u32* val)
{
    switch (ver)
    {
    case 2: {
        robotraconteurlite_u16 val16 = 0;
        robotraconteurlite_status rv = robotraconteurlite_message_read_uint16(buffer, offset, &val16);
        if (FAILED(rv))
        {
            return rv;
        }
        *val = val16;
        return rv;
    }
    case 4:
        return robotraconteurlite_message_read_uint_x(buffer, offset, val);
    default:
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }
}

static robotraconteurlite_status robotraconteurlite_message_read_nodeid(struct robotraconteurlite_buffer_vec* buffer,
                                                                        robotraconteurlite_size_t* offset,
                                                                        struct robotraconteurlite_array_uint8* val)
{
    robotraconteurlite_status rv = -1;
    rv = robotraconteurlite_buffer_vec_copy_to_uint8_array(buffer, *offset, val, 0, 16);
    if (FAILED(rv))
    {
        return rv;
    }
    *offset += 16U;
    return rv;
}

struct robotraconteurlite_message_read_header_string_info
{
    robotraconteurlite_size_t offset;
    robotraconteurlite_size_t str_len;
};

static robotraconteurlite_status robotraconteurlite_message_read_header_string_with_len_prefix2_1(
    struct robotraconteurlite_buffer_vec* buffer, robotraconteurlite_size_t* offset,
    struct robotraconteurlite_message_read_header_string_info* str_info)
{
    robotraconteurlite_status rv = -1;
    robotraconteurlite_u16 str_len = 0;
    rv = robotraconteurlite_message_read_uint16(buffer, offset, &str_len);
    if (FAILED(rv))
    {
        return rv;
    }
    str_info->offset = *offset;
    str_info->str_len = str_len;
    *offset += str_len;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_message_read_header_string_with_len_prefix2_2(
    struct robotraconteurlite_buffer_vec* buffer, struct robotraconteurlite_message_read_header_string_info* str_info,
    struct robotraconteurlite_string* str)
{
    return robotraconteurlite_message_read_header_string(buffer, str_info->offset, str_info->str_len, str);
}

static robotraconteurlite_status robotraconteurlite_message_read_header_string_with_len_prefix4_1(
    struct robotraconteurlite_buffer_vec* buffer, robotraconteurlite_size_t* offset,
    struct robotraconteurlite_message_read_header_string_info* str_info)
{
    robotraconteurlite_status rv = -1;
    robotraconteurlite_u32 str_len = 0;
    rv = robotraconteurlite_message_read_uint_x(buffer, offset, &str_len);
    if (FAILED(rv))
    {
        return rv;
    }
    str_info->offset = *offset;
    str_info->str_len = str_len;
    *offset += str_len;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_message_read_header_string_with_len_prefix4_2(
    struct robotraconteurlite_buffer_vec* buffer, struct robotraconteurlite_message_read_header_string_info* str_info,
    struct robotraconteurlite_string* str)
{
    return robotraconteurlite_message_read_header_string(buffer, str_info->offset, str_info->str_len, str);
}

robotraconteurlite_status robotraconteurlite_message_reader_read_header(
    struct robotraconteurlite_message_reader* reader, struct robotraconteurlite_message_header* header)
{
    struct robotraconteurlite_message_buffer_info buffer_info;
    robotraconteurlite_status rv = -1;
    rv = robotraconteurlite_message_reader_read_header_ex(reader, header, &buffer_info);
    if (FAILED(rv))
    {
        return rv;
    }
    reader->message_version = header->message_version;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_message_reader_read_header4_ex(
    struct robotraconteurlite_message_reader* reader, struct robotraconteurlite_message_header* header,
    struct robotraconteurlite_message_buffer_info* buffer_info)
{
    robotraconteurlite_size_t o = reader->buffer_offset;
    robotraconteurlite_size_t start_o = o;
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_array_uint8 sender_nodeid_array;
    struct robotraconteurlite_array_uint8 recv_nodeid_array;
    struct robotraconteurlite_message_read_header_string_info sender_nodename_info;
    struct robotraconteurlite_message_read_header_string_info receiver_nodename_info;
    struct robotraconteurlite_message_read_header_string_info meta_info;

    struct robotraconteurlite_message_buffer_info temp_buffer_info;

    robotraconteurlite_u8 flags = 0;

    (void)memset(&sender_nodename_info, 0, sizeof(sender_nodename_info));
    (void)memset(&receiver_nodename_info, 0, sizeof(receiver_nodename_info));
    (void)memset(&meta_info, 0, sizeof(meta_info));

    temp_buffer_info.start_buffer_offset = reader->buffer_offset;

    sender_nodeid_array.data = header->sender_nodeid.data;
    sender_nodeid_array.len = 16;
    recv_nodeid_array.data = header->receiver_nodeid.data;
    recv_nodeid_array.len = 16;

    /* skip RRAC magic */
    o += 4U;

    rv = robotraconteurlite_message_read_uint32(reader->buffer, &o, &header->message_size);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_uint16(reader->buffer, &o, &header->message_version);
    if (FAILED(rv))
    {
        return rv;
    }

    if (header->message_version != 4U)
    {
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }

    rv = robotraconteurlite_message_read_uint_x(reader->buffer, &o, &header->header_size);
    if (FAILED(rv))
    {
        return rv;
    }

    temp_buffer_info.header_size = header->header_size;

    rv = robotraconteurlite_message_read_uint8(reader->buffer, &o, &flags);
    if (FAILED(rv))
    {
        return rv;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_ROUTING_INFO))
    {
        rv = robotraconteurlite_message_read_nodeid(reader->buffer, &o, &sender_nodeid_array);
        if (FAILED(rv))
        {
            return rv;
        }

        rv = robotraconteurlite_message_read_nodeid(reader->buffer, &o, &recv_nodeid_array);
        if (FAILED(rv))
        {
            return rv;
        }

        rv =
            robotraconteurlite_message_read_header_string_with_len_prefix4_1(reader->buffer, &o, &sender_nodename_info);
        if (FAILED(rv))
        {
            return rv;
        }

        rv = robotraconteurlite_message_read_header_string_with_len_prefix4_1(reader->buffer, &o,
                                                                              &receiver_nodename_info);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        (void)memset(header->sender_nodeid.data, 0, 16);
        (void)memset(header->receiver_nodeid.data, 0, 16);
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_ENDPOINT_INFO))
    {
        rv = robotraconteurlite_message_read_uint_x(reader->buffer, &o, &header->sender_endpoint);
        if (FAILED(rv))
        {
            return rv;
        }

        rv = robotraconteurlite_message_read_uint_x(reader->buffer, &o, &header->receiver_endpoint);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        header->sender_endpoint = 0;
        header->receiver_endpoint = 0;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_PRIORITY))
    {
        rv = robotraconteurlite_message_read_uint16(reader->buffer, &o, &header->priority);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        header->priority = 0;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_META_INFO))
    {
        rv = robotraconteurlite_message_read_header_string_with_len_prefix4_1(reader->buffer, &o, &meta_info);
        if (FAILED(rv))
        {
            return rv;
        }

        rv = robotraconteurlite_message_read_uint16(reader->buffer, &o, &header->message_id);
        if (FAILED(rv))
        {
            return rv;
        }

        rv = robotraconteurlite_message_read_uint16(reader->buffer, &o, &header->message_res_id);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        header->message_id = 0;
        header->message_res_id = 0;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_STRING_TABLE))
    {
        robotraconteurlite_u32 string_table_size = 0;
        rv = robotraconteurlite_message_read_uint_x(reader->buffer, &o, &string_table_size);
        if (FAILED(rv))
        {
            return rv;
        }
        o += string_table_size;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_MULTIPLE_ENTRIES))
    {
        temp_buffer_info.entry_count_offset = o;
        rv = robotraconteurlite_message_read_uint_x(reader->buffer, &o, &header->entry_count);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        header->entry_count = 1;
        temp_buffer_info.entry_count_offset = ROBOTRACONTEURLITE_SIZE_T_MAX;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_EXTENDED))
    {
        robotraconteurlite_u32 extended_size = 0;
        rv = robotraconteurlite_message_read_uint_x(reader->buffer, &o, &extended_size);
        if (FAILED(rv))
        {
            return rv;
        }
        o += extended_size;
    }

    if ((o - start_o) != header->header_size)
    {
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_ROUTING_INFO))
    {
        rv = robotraconteurlite_message_read_header_string_with_len_prefix4_2(reader->buffer, &sender_nodename_info,
                                                                              &header->sender_nodename);
        if (FAILED(rv))
        {
            return rv;
        }

        rv = robotraconteurlite_message_read_header_string_with_len_prefix4_2(reader->buffer, &receiver_nodename_info,
                                                                              &header->receiver_nodename);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        header->sender_nodename.data = NULL;
        header->sender_nodename.len = 0;
        header->receiver_nodename.data = NULL;
        header->receiver_nodename.len = 0;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_META_INFO))
    {
        rv = robotraconteurlite_message_read_header_string_with_len_prefix4_2(reader->buffer, &meta_info,
                                                                              &header->metadata);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        header->metadata.data = NULL;
        header->metadata.len = 0;
    }

    header->message_flags = flags;

    temp_buffer_info.entry_start_offset = o;

    *buffer_info = temp_buffer_info;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_message_reader_read_header2_ex(
    struct robotraconteurlite_message_reader* reader, struct robotraconteurlite_message_header* header,
    struct robotraconteurlite_message_buffer_info* buffer_info)
{
    robotraconteurlite_size_t o = reader->buffer_offset;
    robotraconteurlite_size_t start_o = o;
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_array_uint8 sender_nodeid_array;
    struct robotraconteurlite_array_uint8 recv_nodeid_array;
    struct robotraconteurlite_message_read_header_string_info sender_nodename_info;
    struct robotraconteurlite_message_read_header_string_info receiver_nodename_info;
    struct robotraconteurlite_message_read_header_string_info meta_info;

    struct robotraconteurlite_message_buffer_info temp_buffer_info;

    temp_buffer_info.start_buffer_offset = reader->buffer_offset;

    sender_nodeid_array.data = header->sender_nodeid.data;
    sender_nodeid_array.len = 16;
    recv_nodeid_array.data = header->receiver_nodeid.data;
    recv_nodeid_array.len = 16;

    /* skip RRAC magic */
    o += 4U;

    rv = robotraconteurlite_message_read_uint32(reader->buffer, &o, &header->message_size);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_uint16(reader->buffer, &o, &header->message_version);
    if (FAILED(rv))
    {
        return rv;
    }
    {
        robotraconteurlite_u16 header_size_16 = 0;
        rv = robotraconteurlite_message_read_uint16(reader->buffer, &o, &header_size_16);
        if (FAILED(rv))
        {
            return rv;
        }
        header->header_size = header_size_16;
    }

    temp_buffer_info.header_size = header->header_size;

    rv = robotraconteurlite_message_read_nodeid(reader->buffer, &o, &sender_nodeid_array);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_nodeid(reader->buffer, &o, &recv_nodeid_array);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_uint32(reader->buffer, &o, &header->sender_endpoint);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_uint32(reader->buffer, &o, &header->receiver_endpoint);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_header_string_with_len_prefix2_1(reader->buffer, &o, &sender_nodename_info);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_header_string_with_len_prefix2_1(reader->buffer, &o, &receiver_nodename_info);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_header_string_with_len_prefix2_1(reader->buffer, &o, &meta_info);
    if (FAILED(rv))
    {
        return rv;
    }

    temp_buffer_info.entry_count_offset = o;

    {
        robotraconteurlite_u16 entry_count_16 = 0;
        rv = robotraconteurlite_message_read_uint16(reader->buffer, &o, &entry_count_16);
        if (FAILED(rv))
        {
            return rv;
        }
        header->entry_count = entry_count_16;
    }

    rv = robotraconteurlite_message_read_uint16(reader->buffer, &o, &header->message_id);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_uint16(reader->buffer, &o, &header->message_res_id);
    if (FAILED(rv))
    {
        return rv;
    }

    if ((o - start_o) != header->header_size)
    {
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }

    rv = robotraconteurlite_message_read_header_string_with_len_prefix2_2(reader->buffer, &sender_nodename_info,
                                                                          &header->sender_nodename);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_header_string_with_len_prefix2_2(reader->buffer, &receiver_nodename_info,
                                                                          &header->receiver_nodename);
    if (FAILED(rv))
    {
        return rv;
    }

    rv =
        robotraconteurlite_message_read_header_string_with_len_prefix2_2(reader->buffer, &meta_info, &header->metadata);
    if (FAILED(rv))
    {
        return rv;
    }

    header->message_flags = ROBOTRACONTEURLITE_MESSAGE_FLAGS_VERSION2COMPAT;
    header->priority = 0;

    temp_buffer_info.entry_start_offset = o;

    *buffer_info = temp_buffer_info;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_message_reader_read_header_ex(
    struct robotraconteurlite_message_reader* reader, struct robotraconteurlite_message_header* header,
    struct robotraconteurlite_message_buffer_info* buffer_info)
{
    robotraconteurlite_size_t o = reader->buffer_offset;
    robotraconteurlite_status rv = -1;

    o += 8U;

    rv = robotraconteurlite_message_read_uint16(reader->buffer, &o, &header->message_version);
    if (FAILED(rv))
    {
        return rv;
    }

    switch (header->message_version)
    {
    case 2:
        return robotraconteurlite_message_reader_read_header2_ex(reader, header, buffer_info);
    case 4:
        return robotraconteurlite_message_reader_read_header4_ex(reader, header, buffer_info);
    default:
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }
}

robotraconteurlite_status robotraconteurlite_message_reader_begin_read_entries(
    struct robotraconteurlite_message_reader* reader, struct robotraconteurlite_messageentry_reader* entry_reader)
{
    robotraconteurlite_size_t o = reader->buffer_offset;
    robotraconteurlite_size_t o2 = 0;
    robotraconteurlite_status rv = -1;
    robotraconteurlite_u32 entry_size = 0;
    struct robotraconteurlite_message_header message_header;
    struct robotraconteurlite_message_buffer_info message_buffer_info;
    (void)memset(&message_header, 0, sizeof(message_header));

    rv = robotraconteurlite_message_reader_read_header_ex(reader, &message_header, &message_buffer_info);
    if (FAILED(rv))
    {
        return rv;
    }

    o += message_header.header_size;
    o2 = o;
    rv = robotraconteurlite_message_read_count(reader->buffer, reader->message_version, &o2, &entry_size);
    if (FAILED(rv))
    {
        return rv;
    }

    if ((o + entry_size) > (reader->buffer_count))
    {
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }

    entry_reader->buffer = reader->buffer;
    entry_reader->buffer_offset = reader->buffer_offset + o;
    entry_reader->buffer_count = entry_size;
    entry_reader->buffer_remaining = reader->buffer_count - o - entry_size;
    entry_reader->current_entry = 0;
    entry_reader->entry_count_offset = message_buffer_info.entry_count_offset;
    entry_reader->message_version = reader->message_version;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageentry_reader_move_next(
    struct robotraconteurlite_messageentry_reader* entry_reader)
{
    robotraconteurlite_size_t o = entry_reader->buffer_offset + entry_reader->buffer_count;
    robotraconteurlite_u32 entry_size = 0;
    robotraconteurlite_status rv = -1;

    robotraconteurlite_u32 entry_count = 0;
    robotraconteurlite_size_t o2 = entry_reader->entry_count_offset;

    if (o2 == ROBOTRACONTEURLITE_SIZE_T_MAX)
    {
        return ROBOTRACONTEURLITE_ERROR_NO_MORE;
    }
    rv = robotraconteurlite_message_read_count2(entry_reader->buffer, entry_reader->message_version, &o2, &entry_count);
    if (FAILED(rv))
    {
        return rv;
    }

    if (!((entry_reader->current_entry + 1U) < entry_count))
    {
        return ROBOTRACONTEURLITE_ERROR_NO_MORE;
    }

    o2 = o;
    rv = robotraconteurlite_message_read_count(entry_reader->buffer, entry_reader->message_version, &o2, &entry_size);
    if (FAILED(rv))
    {
        return rv;
    }

    if (entry_reader->buffer_remaining < entry_size)
    {
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }

    entry_reader->buffer_offset = o;
    entry_reader->buffer_count = entry_size;
    entry_reader->buffer_remaining -= entry_size;
    entry_reader->current_entry++;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageentry_reader_find_element(
    struct robotraconteurlite_messageentry_reader* entry_reader,
    const struct robotraconteurlite_const_string* element_name,
    struct robotraconteurlite_messageelement_reader* element_reader)
{
    char temp_str_data[16];
    robotraconteurlite_status rv = -1;

    assert(element_name != NULL);
    assert(element_name->data != NULL);

    rv = robotraconteurlite_messageentry_reader_begin_read_elements(entry_reader, element_reader);
    if (FAILED(rv))
    {
        if (NO_MORE(rv))
        {
            return ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_NOT_FOUND;
        }
        return rv;
    }
    while (1)
    {
        struct robotraconteurlite_messageelement_header element_header;
        struct robotraconteurlite_messageelement_buffer_info element_buffer_info;
        robotraconteurlite_size_t str_len = 0;
        robotraconteurlite_size_t str_offset = 0;
        (void)memset(&element_header, 0, sizeof(element_header));
        (void)memset(&element_buffer_info, 0, sizeof(element_buffer_info));

        rv = robotraconteurlite_messageelement_reader_read_header_ex(element_reader, &element_header,
                                                                     &element_buffer_info);

        if (FAILED(rv))
        {
            return rv;
        }
        str_len = element_header.element_name.len;

        if (str_len == element_name->len)
        {
            struct robotraconteurlite_string temp_str;
            temp_str.data = temp_str_data;
            temp_str.len = sizeof(temp_str_data);

            do
            {
                robotraconteurlite_size_t str_read = ((str_len <= temp_str.len) ? str_len : temp_str.len);
                rv = robotraconteurlite_buffer_vec_copy_to_string(
                    element_reader->buffer, element_buffer_info.element_name_str_offset + str_offset, &temp_str, 0,
                    str_read);
                if (FAILED(rv))
                {
                    return rv;
                }

                if (memcmp(temp_str.data, &element_name->data[str_offset], str_read) != 0)
                {
                    break;
                }

                str_len -= str_read;
                if (str_len == 0U)
                {
                    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
                }
                str_offset += str_read;

            } while (str_len > 0U);
        }

        rv = robotraconteurlite_messageelement_reader_move_next(element_reader);
        if (FAILED(rv))
        {
            if (NO_MORE(rv))
            {
                return ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_NOT_FOUND;
            }
            return rv;
        }
    }
}

robotraconteurlite_status robotraconteurlite_messageentry_reader_find_element_verify_scalar(
    struct robotraconteurlite_messageentry_reader* entry_reader,
    const struct robotraconteurlite_const_string* element_name,
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_u16 expected_data_type)
{
    struct robotraconteurlite_messageelement_header header;
    robotraconteurlite_status rv = -1;

    rv = robotraconteurlite_messageentry_reader_find_element(entry_reader, element_name, element_reader);
    if (FAILED(rv))
    {
        return rv;
    }

    (void)memset(&header, 0, sizeof(header));
    rv = robotraconteurlite_messageelement_reader_read_header(element_reader, &header);
    if (FAILED(rv))
    {
        return rv;
    }

    if (header.element_type != expected_data_type)
    {
        return ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_TYPE_MISMATCH;
    }

    if (header.data_count != 1U)
    {
        return ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_TYPE_MISMATCH;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageentry_reader_find_element_verify_array(
    struct robotraconteurlite_messageentry_reader* entry_reader,
    const struct robotraconteurlite_const_string* element_name,
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_u16 expected_data_type,
    robotraconteurlite_u32 expected_len, robotraconteurlite_u8 var_length)
{
    struct robotraconteurlite_messageelement_header header;
    robotraconteurlite_status rv = -1;

    rv = robotraconteurlite_messageentry_reader_find_element(entry_reader, element_name, element_reader);
    if (FAILED(rv))
    {
        return rv;
    }

    (void)memset(&header, 0, sizeof(header));
    rv = robotraconteurlite_messageelement_reader_read_header(element_reader, &header);
    if (FAILED(rv))
    {
        return rv;
    }

    if (header.element_type != expected_data_type)
    {
        return ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_TYPE_MISMATCH;
    }

    if (!var_length)
    {
        if (header.data_count != expected_len)
        {
            return ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_TYPE_MISMATCH;
        }
    }
    else
    {
        if (header.data_count > expected_len)
        {
            return ROBOTRACONTEURLITE_ERROR_OUT_OF_RANGE;
        }
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageentry_reader_find_element_verify_string(
    struct robotraconteurlite_messageentry_reader* entry_reader,
    const struct robotraconteurlite_const_string* element_name,
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_u32 expected_max_len)
{
    struct robotraconteurlite_messageelement_header header;
    robotraconteurlite_status rv = -1;

    rv = robotraconteurlite_messageentry_reader_find_element(entry_reader, element_name, element_reader);
    if (FAILED(rv))
    {
        return rv;
    }

    (void)memset(&header, 0, sizeof(header));
    rv = robotraconteurlite_messageelement_reader_read_header(element_reader, &header);
    if (FAILED(rv))
    {
        return rv;
    }

    if (header.element_type != ROBOTRACONTEURLITE_DATATYPE_STRING)
    {
        return ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_TYPE_MISMATCH;
    }

    if (header.data_count > expected_max_len)
    {
        return ROBOTRACONTEURLITE_ERROR_OUT_OF_RANGE;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageentry_reader_find_element_c_str(
    struct robotraconteurlite_messageentry_reader* entry_reader, const char* element_name,
    struct robotraconteurlite_messageelement_reader* element_reader)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageentry_reader_find_element(entry_reader, &element_name_str, element_reader);
}

robotraconteurlite_status robotraconteurlite_messageentry_reader_find_element_verify_scalar_c_str(
    struct robotraconteurlite_messageentry_reader* entry_reader, const char* element_name,
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_u16 expected_data_type)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageentry_reader_find_element_verify_scalar(entry_reader, &element_name_str,
                                                                             element_reader, expected_data_type);
}

robotraconteurlite_status robotraconteurlite_messageentry_reader_find_element_verify_array_c_str(
    struct robotraconteurlite_messageentry_reader* entry_reader, const char* element_name,
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_u16 expected_data_type,
    robotraconteurlite_u32 expected_len, robotraconteurlite_u8 var_length)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageentry_reader_find_element_verify_array(
        entry_reader, &element_name_str, element_reader, expected_data_type, expected_len, var_length);
}

robotraconteurlite_status robotraconteurlite_messageentry_reader_find_element_verify_string_c_str(
    struct robotraconteurlite_messageentry_reader* entry_reader, const char* element_name,
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_u32 expected_max_len)
{
    struct robotraconteurlite_const_string element_name_str;
    robotraconteurlite_string_from_c_str(element_name, &element_name_str);
    return robotraconteurlite_messageentry_reader_find_element_verify_string(entry_reader, &element_name_str,
                                                                             element_reader, expected_max_len);
}

robotraconteurlite_status robotraconteurlite_messageentry_reader_read_header(
    struct robotraconteurlite_messageentry_reader* entry_reader, struct robotraconteurlite_messageentry_header* header)
{
    struct robotraconteurlite_messageentry_buffer_info buffer_info;
    return robotraconteurlite_messageentry_reader_read_header_ex(entry_reader, header, &buffer_info);
}

robotraconteurlite_status robotraconteurlite_messageentry_reader_read_header2_ex(
    struct robotraconteurlite_messageentry_reader* entry_reader, struct robotraconteurlite_messageentry_header* header,
    struct robotraconteurlite_messageentry_buffer_info* buffer_info)
{
    robotraconteurlite_size_t o = entry_reader->buffer_offset;
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_message_read_header_string_info service_path_info;
    struct robotraconteurlite_message_read_header_string_info member_name_info;
    struct robotraconteurlite_message_read_header_string_info meta_info;

    struct robotraconteurlite_messageentry_buffer_info temp_buffer_info;

    (void)memset(&service_path_info, 0, sizeof(service_path_info));
    (void)memset(&member_name_info, 0, sizeof(member_name_info));
    (void)memset(&meta_info, 0, sizeof(meta_info));

    temp_buffer_info.start_buffer_offset = entry_reader->buffer_offset;
    temp_buffer_info.entry_size_offset = o;

    rv = robotraconteurlite_message_read_uint32(entry_reader->buffer, &o, &header->entry_size);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_uint16(entry_reader->buffer, &o, &header->entry_type);
    if (FAILED(rv))
    {
        return rv;
    }

    /* reserved */
    o += 2U;

    rv = robotraconteurlite_message_read_header_string_with_len_prefix2_1(entry_reader->buffer, &o, &service_path_info);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_header_string_with_len_prefix2_1(entry_reader->buffer, &o, &member_name_info);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_uint32(entry_reader->buffer, &o, &header->request_id);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_uint16(entry_reader->buffer, &o, &header->error);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_header_string_with_len_prefix2_1(entry_reader->buffer, &o, &meta_info);
    if (FAILED(rv))
    {
        return rv;
    }

    temp_buffer_info.element_count_offset = o;
    {
        robotraconteurlite_u16 element_count_16 = 0;
        rv = robotraconteurlite_message_read_uint16(entry_reader->buffer, &o, &element_count_16);

        if (FAILED(rv))
        {
            return rv;
        }
        header->element_count = element_count_16;
    }

    rv = robotraconteurlite_message_read_header_string_with_len_prefix2_2(entry_reader->buffer, &service_path_info,
                                                                          &header->service_path);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_header_string_with_len_prefix2_2(entry_reader->buffer, &member_name_info,
                                                                          &header->member_name);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_header_string_with_len_prefix2_2(entry_reader->buffer, &meta_info,
                                                                          &header->metadata);
    if (FAILED(rv))
    {
        return rv;
    }

    temp_buffer_info.header_size = o - entry_reader->buffer_offset;
    temp_buffer_info.element_start_offset = o;

    *buffer_info = temp_buffer_info;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageentry_reader_read_header4_ex(
    struct robotraconteurlite_messageentry_reader* entry_reader, struct robotraconteurlite_messageentry_header* header,
    struct robotraconteurlite_messageentry_buffer_info* buffer_info)
{
    robotraconteurlite_size_t o = entry_reader->buffer_offset;
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_message_read_header_string_info service_path_info;
    struct robotraconteurlite_message_read_header_string_info member_name_info;
    struct robotraconteurlite_message_read_header_string_info meta_info;

    struct robotraconteurlite_messageentry_buffer_info temp_buffer_info;
    robotraconteurlite_u8 flags = 0;

    (void)memset(&service_path_info, 0, sizeof(service_path_info));
    (void)memset(&member_name_info, 0, sizeof(member_name_info));
    (void)memset(&meta_info, 0, sizeof(meta_info));

    temp_buffer_info.start_buffer_offset = entry_reader->buffer_offset;
    temp_buffer_info.entry_size_offset = o;

    rv = robotraconteurlite_message_read_uint_x(entry_reader->buffer, &o, &header->entry_size);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_uint8(entry_reader->buffer, &o, &flags);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_uint16(entry_reader->buffer, &o, &header->entry_type);
    if (FAILED(rv))
    {
        return rv;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_SERVICE_PATH_STR))
    {
        rv = robotraconteurlite_message_read_header_string_with_len_prefix4_1(entry_reader->buffer, &o,
                                                                              &service_path_info);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_SERVICE_PATH_CODE))
    {
        rv = robotraconteurlite_message_read_uint_x(entry_reader->buffer, &o, &header->service_path_code);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        header->service_path_code = 0;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_MEMBER_NAME_STR))
    {
        rv = robotraconteurlite_message_read_header_string_with_len_prefix4_1(entry_reader->buffer, &o,
                                                                              &member_name_info);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_MEMBER_NAME_CODE))
    {
        rv = robotraconteurlite_message_read_uint_x(entry_reader->buffer, &o, &header->member_name_code);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        header->member_name_code = 0;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_REQUEST_ID))
    {
        rv = robotraconteurlite_message_read_uint_x(entry_reader->buffer, &o, &header->request_id);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        header->request_id = 0;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_ERROR))
    {
        rv = robotraconteurlite_message_read_uint16(entry_reader->buffer, &o, &header->error);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        header->error = 0;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_META_INFO))
    {
        rv = robotraconteurlite_message_read_header_string_with_len_prefix4_1(entry_reader->buffer, &o, &meta_info);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_EXTENDED))
    {
        robotraconteurlite_u32 extended_size = 0;
        rv = robotraconteurlite_message_read_uint_x(entry_reader->buffer, &o, &extended_size);
        if (FAILED(rv))
        {
            return rv;
        }
        o += extended_size;
    }

    temp_buffer_info.element_count_offset = o;
    rv = robotraconteurlite_message_read_uint_x(entry_reader->buffer, &o, &header->element_count);
    if (FAILED(rv))
    {
        return rv;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_SERVICE_PATH_STR))
    {
        rv = robotraconteurlite_message_read_header_string_with_len_prefix4_2(entry_reader->buffer, &service_path_info,
                                                                              &header->service_path);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        header->service_path.data = NULL;
        header->service_path.len = 0;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_MEMBER_NAME_STR))
    {
        rv = robotraconteurlite_message_read_header_string_with_len_prefix4_2(entry_reader->buffer, &member_name_info,
                                                                              &header->member_name);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        header->member_name.data = NULL;
        header->member_name.len = 0;
    }
    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_META_INFO))
    {
        rv = robotraconteurlite_message_read_header_string_with_len_prefix4_2(entry_reader->buffer, &meta_info,
                                                                              &header->metadata);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        header->metadata.data = NULL;
        header->metadata.len = 0;
    }

    temp_buffer_info.header_size = o - entry_reader->buffer_offset;
    temp_buffer_info.element_start_offset = o;

    *buffer_info = temp_buffer_info;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageentry_reader_read_header_ex(
    struct robotraconteurlite_messageentry_reader* entry_reader, struct robotraconteurlite_messageentry_header* header,
    struct robotraconteurlite_messageentry_buffer_info* buffer_info)
{
    switch (entry_reader->message_version)
    {
    case 2:
        return robotraconteurlite_messageentry_reader_read_header2_ex(entry_reader, header, buffer_info);
    case 4:
        return robotraconteurlite_messageentry_reader_read_header4_ex(entry_reader, header, buffer_info);
    default:
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }
}

robotraconteurlite_status robotraconteurlite_messageentry_reader_begin_read_elements(
    struct robotraconteurlite_messageentry_reader* entry_reader,
    struct robotraconteurlite_messageelement_reader* element_reader)
{
    robotraconteurlite_size_t o = 0;
    robotraconteurlite_size_t o2 = 0;
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_messageentry_header entry_header;
    struct robotraconteurlite_messageentry_buffer_info entry_buffer_info;
    robotraconteurlite_u32 element_size = 0;

    (void)memset(&entry_header, 0, sizeof(entry_header));
    (void)memset(&entry_buffer_info, 0, sizeof(entry_buffer_info));

    rv = robotraconteurlite_messageentry_reader_read_header_ex(entry_reader, &entry_header, &entry_buffer_info);
    if (FAILED(rv))
    {
        return rv;
    }

    if (entry_header.element_count == 0U)
    {
        return ROBOTRACONTEURLITE_ERROR_NO_MORE;
    }

    o = entry_buffer_info.element_start_offset;
    o2 = o;

    rv = robotraconteurlite_message_read_count(entry_reader->buffer, entry_reader->message_version, &o2, &element_size);
    if (FAILED(rv))
    {
        return rv;
    }

    if (((o - entry_reader->buffer_offset) + element_size) > entry_reader->buffer_count)
    {
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }

    element_reader->buffer = entry_reader->buffer;
    element_reader->buffer_offset = o;
    element_reader->buffer_count = element_size;
    element_reader->buffer_remaining = entry_reader->buffer_count - entry_buffer_info.header_size - element_size;
    element_reader->current_element = 0;
    element_reader->total_elements = entry_header.element_count;
    element_reader->message_version = entry_reader->message_version;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_move_next(
    struct robotraconteurlite_messageelement_reader* element_reader)
{
    robotraconteurlite_size_t o =
        (robotraconteurlite_size_t)(element_reader->buffer_offset + element_reader->buffer_count);
    robotraconteurlite_u32 element_size = 0;
    robotraconteurlite_status rv = -1;
    robotraconteurlite_size_t o2 = 0;

    robotraconteurlite_size_t element_count = element_reader->total_elements;

    if (!((element_reader->current_element + 1U) < element_count))
    {
        return ROBOTRACONTEURLITE_ERROR_NO_MORE;
    }

    o2 = o;
    rv = robotraconteurlite_message_read_count(element_reader->buffer, element_reader->message_version, &o2,
                                               &element_size);
    if (FAILED(rv))
    {
        return rv;
    }

    if (element_reader->buffer_remaining < element_size)
    {
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }

    element_reader->buffer_offset = o;
    element_reader->buffer_count = element_size;
    element_reader->buffer_remaining -= element_size;
    element_reader->current_element++;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_header(
    struct robotraconteurlite_messageelement_reader* element_reader,
    struct robotraconteurlite_messageelement_header* header)
{
    struct robotraconteurlite_messageelement_buffer_info buffer_info;
    return robotraconteurlite_messageelement_reader_read_header_ex(element_reader, header, &buffer_info);
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_header2_ex(
    struct robotraconteurlite_messageelement_reader* element_reader,
    struct robotraconteurlite_messageelement_header* header,
    struct robotraconteurlite_messageelement_buffer_info* buffer_info)
{
    robotraconteurlite_size_t o = element_reader->buffer_offset;
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_message_read_header_string_info elementname_info;
    struct robotraconteurlite_message_read_header_string_info elementtypename_info;
    struct robotraconteurlite_message_read_header_string_info meta_info;
    struct robotraconteurlite_messageelement_buffer_info temp_buffer_info;

    (void)memset(&elementname_info, 0, sizeof(elementname_info));
    (void)memset(&elementtypename_info, 0, sizeof(elementtypename_info));
    (void)memset(&meta_info, 0, sizeof(meta_info));

    temp_buffer_info.start_buffer_offset = element_reader->buffer_offset;
    temp_buffer_info.element_size_offset = o;

    rv = robotraconteurlite_message_read_uint32(element_reader->buffer, &o, &header->element_size);
    if (FAILED(rv))
    {
        return rv;
    }

    rv =
        robotraconteurlite_message_read_header_string_with_len_prefix2_1(element_reader->buffer, &o, &elementname_info);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_uint16(element_reader->buffer, &o, &header->element_type);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_header_string_with_len_prefix2_1(element_reader->buffer, &o,
                                                                          &elementtypename_info);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_header_string_with_len_prefix2_1(element_reader->buffer, &o, &meta_info);
    if (FAILED(rv))
    {
        return rv;
    }

    temp_buffer_info.element_count_offset = o;

    rv = robotraconteurlite_message_read_uint32(element_reader->buffer, &o, &header->data_count);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_header_string_with_len_prefix2_2(element_reader->buffer, &elementname_info,
                                                                          &header->element_name);
    if (FAILED(rv))
    {
        return rv;
    }
    temp_buffer_info.element_name_str_offset = elementname_info.offset;

    rv = robotraconteurlite_message_read_header_string_with_len_prefix2_2(element_reader->buffer, &elementtypename_info,
                                                                          &header->element_type_name);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_header_string_with_len_prefix2_2(element_reader->buffer, &meta_info,
                                                                          &header->metadata);
    if (FAILED(rv))
    {
        return rv;
    }

    temp_buffer_info.header_size = o - element_reader->buffer_offset;
    temp_buffer_info.data_start_offset = o;

    *buffer_info = temp_buffer_info;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_header4_ex(
    struct robotraconteurlite_messageelement_reader* element_reader,
    struct robotraconteurlite_messageelement_header* header,
    struct robotraconteurlite_messageelement_buffer_info* buffer_info)
{
    robotraconteurlite_size_t o = element_reader->buffer_offset;
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_message_read_header_string_info elementname_info;
    struct robotraconteurlite_message_read_header_string_info elementtypename_info;
    struct robotraconteurlite_message_read_header_string_info meta_info;

    struct robotraconteurlite_messageelement_buffer_info temp_buffer_info;

    robotraconteurlite_u8 flags = 0;

    (void)memset(&elementname_info, 0, sizeof(elementname_info));
    (void)memset(&elementtypename_info, 0, sizeof(elementtypename_info));
    (void)memset(&meta_info, 0, sizeof(meta_info));

    temp_buffer_info.start_buffer_offset = element_reader->buffer_offset;
    temp_buffer_info.element_size_offset = o;

    rv = robotraconteurlite_message_read_uint_x(element_reader->buffer, &o, &header->element_size);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_read_uint8(element_reader->buffer, &o, &flags);
    if (FAILED(rv))
    {
        return rv;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_NAME_STR))
    {
        rv = robotraconteurlite_message_read_header_string_with_len_prefix4_1(element_reader->buffer, &o,
                                                                              &elementname_info);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_NAME_CODE))
    {
        rv = robotraconteurlite_message_read_uint_x(element_reader->buffer, &o, &header->element_name_code);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        header->element_name_code = 0;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_NUMBER))
    {
        rv = robotraconteurlite_message_read_int_x(element_reader->buffer, &o, &header->element_number);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        header->element_number = 0;
    }

    rv = robotraconteurlite_message_read_uint16(element_reader->buffer, &o, &header->element_type);
    if (FAILED(rv))
    {
        return rv;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_TYPE_NAME_STR))
    {
        rv = robotraconteurlite_message_read_header_string_with_len_prefix4_1(element_reader->buffer, &o,
                                                                              &elementtypename_info);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_TYPE_NAME_CODE))
    {
        rv = robotraconteurlite_message_read_uint_x(element_reader->buffer, &o, &header->element_type_name_code);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        header->element_type_name_code = 0;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_META_INFO))
    {
        rv = robotraconteurlite_message_read_header_string_with_len_prefix4_1(element_reader->buffer, &o, &meta_info);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_EXTENDED))
    {
        robotraconteurlite_u32 extended_size = 0;
        rv = robotraconteurlite_message_read_uint_x(element_reader->buffer, &o, &extended_size);
        if (FAILED(rv))
        {
            return rv;
        }
        o += extended_size;
    }

    temp_buffer_info.element_count_offset = o;

    rv = robotraconteurlite_message_read_uint_x(element_reader->buffer, &o, &header->data_count);
    if (FAILED(rv))
    {
        return rv;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_NAME_STR))
    {
        rv = robotraconteurlite_message_read_header_string_with_len_prefix4_2(element_reader->buffer, &elementname_info,
                                                                              &header->element_name);
        if (FAILED(rv))
        {
            return rv;
        }
        temp_buffer_info.element_name_str_offset = elementname_info.offset;
    }
    else
    {
        temp_buffer_info.element_name_str_offset = ROBOTRACONTEURLITE_SIZE_T_MAX;

        header->element_name.data = NULL;
        header->element_name.len = 0;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_TYPE_NAME_STR))
    {
        rv = robotraconteurlite_message_read_header_string_with_len_prefix4_2(
            element_reader->buffer, &elementtypename_info, &header->element_type_name);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        header->element_type_name.data = NULL;
        header->element_type_name.len = 0;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_META_INFO))
    {
        rv = robotraconteurlite_message_read_header_string_with_len_prefix4_2(element_reader->buffer, &meta_info,
                                                                              &header->metadata);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        header->metadata.data = NULL;
        header->metadata.len = 0;
    }

    temp_buffer_info.header_size = o - element_reader->buffer_offset;
    temp_buffer_info.data_start_offset = o;

    *buffer_info = temp_buffer_info;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_header_ex(
    struct robotraconteurlite_messageelement_reader* element_reader,
    struct robotraconteurlite_messageelement_header* header,
    struct robotraconteurlite_messageelement_buffer_info* buffer_info)
{
    switch (element_reader->message_version)
    {
    case 2:
        return robotraconteurlite_messageelement_reader_read_header2_ex(element_reader, header, buffer_info);
    case 4:
        return robotraconteurlite_messageelement_reader_read_header4_ex(element_reader, header, buffer_info);
    default:
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_get_data_info(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_size_t* data_offset,
    robotraconteurlite_size_t* data_size, robotraconteurlite_u32* data_count, robotraconteurlite_u16 data_type,
    robotraconteurlite_size_t data_element_size)
{
    struct robotraconteurlite_messageelement_header element_header;
    struct robotraconteurlite_messageelement_buffer_info element_buffer_info;
    robotraconteurlite_status rv = -1;

    assert(element_reader != NULL);

    (void)memset(&element_header, 0, sizeof(element_header));
    (void)memset(&element_buffer_info, 0, sizeof(element_buffer_info));
    rv = robotraconteurlite_messageelement_reader_read_header_ex(element_reader, &element_header, &element_buffer_info);
    if (FAILED(rv))
    {
        return rv;
    }

    if (element_header.element_type != data_type)
    {
        return ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_TYPE_MISMATCH;
    }

    *data_offset = element_buffer_info.data_start_offset;
    *data_size = element_header.element_size - element_buffer_info.header_size;
    *data_count = element_header.data_count;

    if ((data_element_size != 0U) && ((data_element_size * element_header.data_count) != *data_size))
    {
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_ex(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_byte* dest_buf,
    robotraconteurlite_size_t* dest_len, robotraconteurlite_u16 dest_elem_type,
    robotraconteurlite_size_t dest_elem_size)
{
    robotraconteurlite_size_t data_offset = 0;
    robotraconteurlite_size_t data_size = 0;
    robotraconteurlite_u32 data_count = 0;
    robotraconteurlite_status rv = -1;

    assert(dest_buf != NULL);
    assert(dest_len != NULL);

    rv = robotraconteurlite_messageelement_reader_get_data_info(element_reader, &data_offset, &data_size, &data_count,
                                                                dest_elem_type, dest_elem_size);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_buffer_vec_copy_to_mem(element_reader->buffer, data_offset, dest_buf, *dest_len, 0,
                                                   dest_elem_size, data_count);

    if (FAILED(rv))
    {
        return rv;
    }

    *dest_len = data_count;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_read_data_scalar_ex(
    struct robotraconteurlite_messageelement_reader* element_reader, robotraconteurlite_byte* dest_scalar,
    robotraconteurlite_u16 dest_elem_type, robotraconteurlite_size_t dest_elem_size)
{
    robotraconteurlite_size_t data_offset = 0;
    robotraconteurlite_size_t data_size = 0;
    robotraconteurlite_u32 data_count = 0;
    robotraconteurlite_status rv = -1;

    struct robotraconteurlite_buffer dest_buf;
    struct robotraconteurlite_buffer_vec dest_vec;

    dest_buf.data = dest_scalar;
    dest_buf.len = 1;
    dest_vec.buffer_vec = &dest_buf;
    dest_vec.buffer_vec_cnt = 1;

    rv = robotraconteurlite_messageelement_reader_get_data_info(element_reader, &data_offset, &data_size, &data_count,
                                                                dest_elem_type, dest_elem_size);
    if (FAILED(rv))
    {
        return rv;
    }

    if (data_count != 1U)
    {
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }

    return robotraconteurlite_buffer_vec_copy_vec_ex(element_reader->buffer, data_offset, 1, dest_elem_size, &dest_vec,
                                                     0, dest_elem_size, 1);
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_begin_read_nested_elements(
    struct robotraconteurlite_messageelement_reader* element_reader,
    struct robotraconteurlite_messageelement_reader* nested_element_reader)
{
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_messageelement_header header;
    struct robotraconteurlite_messageelement_buffer_info buffer_info;
    robotraconteurlite_size_t o = 0;
    robotraconteurlite_u32 nested_element_size = 0;

    (void)memset(&header, 0, sizeof(header));
    (void)memset(&buffer_info, 0, sizeof(buffer_info));
    rv = robotraconteurlite_messageelement_reader_read_header_ex(element_reader, &header, &buffer_info);
    if (FAILED(rv))
    {
        return rv;
    }

    switch (header.element_type)
    {
    case ROBOTRACONTEURLITE_DATATYPE_STRUCTURE:
    case ROBOTRACONTEURLITE_DATATYPE_MAP_INT32:
    case ROBOTRACONTEURLITE_DATATYPE_MAP_STRING:
    case ROBOTRACONTEURLITE_DATATYPE_LIST:
    case ROBOTRACONTEURLITE_DATATYPE_MULTIDIMARRAY:
    case ROBOTRACONTEURLITE_DATATYPE_POD:
    case ROBOTRACONTEURLITE_DATATYPE_POD_ARRAY:
    case ROBOTRACONTEURLITE_DATATYPE_POD_MULTIDIMARRAY:
    case ROBOTRACONTEURLITE_DATATYPE_NAMEDARRAY_ARRAY:
    case ROBOTRACONTEURLITE_DATATYPE_NAMEDARRAY_MULTIDIMARRAY:
        break;
    default:
        return ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_TYPE_MISMATCH;
    }

    if (header.data_count == 0U)
    {
        return ROBOTRACONTEURLITE_ERROR_NO_MORE;
    }

    o = buffer_info.data_start_offset;
    rv = robotraconteurlite_message_read_count(element_reader->buffer, element_reader->message_version, &o,
                                               &nested_element_size);
    if (FAILED(rv))
    {
        return rv;
    }

    if ((element_reader->buffer_count - buffer_info.header_size) < nested_element_size)
    {
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }

    nested_element_reader->buffer = element_reader->buffer;
    nested_element_reader->buffer_offset = buffer_info.data_start_offset;
    nested_element_reader->buffer_count = nested_element_size;
    nested_element_reader->buffer_remaining =
        element_reader->buffer_count - buffer_info.header_size - nested_element_size;
    nested_element_reader->current_element = 0;
    nested_element_reader->total_elements = header.data_count;
    nested_element_reader->message_version = element_reader->message_version;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_find_nested_element(
    struct robotraconteurlite_messageelement_reader* element_reader,
    const struct robotraconteurlite_const_string* nested_element_name,
    struct robotraconteurlite_messageelement_reader* nested_element_reader)
{
    char temp_str_data[16];
    robotraconteurlite_status rv = -1;

    assert(nested_element_name != NULL);
    assert(nested_element_name->data != NULL);

    rv = robotraconteurlite_messageelement_reader_begin_read_nested_elements(element_reader, nested_element_reader);
    if (FAILED(rv))
    {
        if (NO_MORE(rv))
        {
            return ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_NOT_FOUND;
        }
        return rv;
    }
    while (1)
    {
        struct robotraconteurlite_messageelement_header element_header;
        struct robotraconteurlite_messageelement_buffer_info element_buffer_info;
        robotraconteurlite_size_t str_len = 0;
        robotraconteurlite_size_t str_offset = 0;
        (void)memset(&element_header, 0, sizeof(element_header));
        (void)memset(&element_buffer_info, 0, sizeof(element_buffer_info));

        rv = robotraconteurlite_messageelement_reader_read_header_ex(nested_element_reader, &element_header,
                                                                     &element_buffer_info);
        if (FAILED(rv))
        {
            return rv;
        }

        str_len = element_header.element_name.len;

        if (str_len == nested_element_name->len)
        {
            struct robotraconteurlite_string temp_str;
            temp_str.data = temp_str_data;
            temp_str.len = sizeof(temp_str_data);

            do
            {
                robotraconteurlite_size_t str_read = ((str_len <= temp_str.len) ? str_len : temp_str.len);
                rv = robotraconteurlite_buffer_vec_copy_to_string(
                    nested_element_reader->buffer, element_buffer_info.element_name_str_offset + str_offset, &temp_str,
                    0, str_read);
                if (FAILED(rv))
                {
                    return rv;
                }

                if (memcmp(temp_str.data, &nested_element_name->data[str_offset], str_read) != 0)
                {
                    break;
                }

                str_len -= str_read;
                if (str_len == 0U)
                {
                    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
                }
                str_offset += str_read;

            } while (str_len > 0U);
        }

        rv = robotraconteurlite_messageelement_reader_move_next(nested_element_reader);
        if (FAILED(rv))
        {
            if (NO_MORE(rv))
            {
                return ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_NOT_FOUND;
            }
            return rv;
        }
    }
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_find_nested_element_verify_scalar(
    struct robotraconteurlite_messageelement_reader* element_reader,
    const struct robotraconteurlite_const_string* nested_element_name,
    struct robotraconteurlite_messageelement_reader* nested_element_reader, robotraconteurlite_u16 expected_data_type)
{
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_messageelement_header header;
    rv = robotraconteurlite_messageelement_reader_find_nested_element(element_reader, nested_element_name,
                                                                      nested_element_reader);
    if (FAILED(rv))
    {
        return rv;
    }

    (void)memset(&header, 0, sizeof(header));
    rv = robotraconteurlite_messageelement_reader_read_header(nested_element_reader, &header);
    if (FAILED(rv))
    {
        return rv;
    }

    if (header.element_type != expected_data_type)
    {
        return ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_TYPE_MISMATCH;
    }

    if (header.data_count != 1U)
    {
        return ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_TYPE_MISMATCH;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_find_nested_element_verify_array(
    struct robotraconteurlite_messageelement_reader* element_reader,
    const struct robotraconteurlite_const_string* nested_element_name,
    struct robotraconteurlite_messageelement_reader* nested_element_reader, robotraconteurlite_u16 expected_data_type,
    robotraconteurlite_u32 expected_len, robotraconteurlite_u8 var_length)
{
    struct robotraconteurlite_messageelement_header header;
    robotraconteurlite_status rv = -1;

    rv = robotraconteurlite_messageelement_reader_find_nested_element(element_reader, nested_element_name,
                                                                      nested_element_reader);
    if (FAILED(rv))
    {
        return rv;
    }

    (void)memset(&header, 0, sizeof(header));
    rv = robotraconteurlite_messageelement_reader_read_header(nested_element_reader, &header);
    if (FAILED(rv))
    {
        return rv;
    }

    if (header.element_type != expected_data_type)
    {
        return ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_TYPE_MISMATCH;
    }

    if (!var_length)
    {
        if (header.data_count != expected_len)
        {
            return ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_TYPE_MISMATCH;
        }
    }
    else
    {
        if (header.data_count > expected_len)
        {
            return ROBOTRACONTEURLITE_ERROR_OUT_OF_RANGE;
        }
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_find_nested_element_verify_string(
    struct robotraconteurlite_messageelement_reader* element_reader,
    const struct robotraconteurlite_const_string* nested_element_name,
    struct robotraconteurlite_messageelement_reader* nested_element_reader, robotraconteurlite_u32 expected_max_len)
{
    struct robotraconteurlite_messageelement_header header;
    robotraconteurlite_status rv = -1;
    rv = robotraconteurlite_messageelement_reader_find_nested_element(element_reader, nested_element_name,
                                                                      nested_element_reader);
    if (FAILED(rv))
    {
        return rv;
    }

    (void)memset(&header, 0, sizeof(header));
    rv = robotraconteurlite_messageelement_reader_read_header(nested_element_reader, &header);
    if (FAILED(rv))
    {
        return rv;
    }

    if (header.element_type != ROBOTRACONTEURLITE_DATATYPE_STRING)
    {
        return ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_TYPE_MISMATCH;
    }

    if (header.data_count > expected_max_len)
    {
        return ROBOTRACONTEURLITE_ERROR_OUT_OF_RANGE;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_find_nested_element_c_str(
    struct robotraconteurlite_messageelement_reader* element_reader, const char* nested_element_name,
    struct robotraconteurlite_messageelement_reader* nested_element_reader)
{
    struct robotraconteurlite_const_string nested_element_name_str;
    robotraconteurlite_string_from_c_str(nested_element_name, &nested_element_name_str);
    return robotraconteurlite_messageelement_reader_find_nested_element(element_reader, &nested_element_name_str,
                                                                        nested_element_reader);
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_find_nested_element_verify_scalar_c_str(
    struct robotraconteurlite_messageelement_reader* element_reader, const char* nested_element_name,
    struct robotraconteurlite_messageelement_reader* nested_element_reader, robotraconteurlite_u16 expected_data_type)
{
    struct robotraconteurlite_const_string nested_element_name_str;
    robotraconteurlite_string_from_c_str(nested_element_name, &nested_element_name_str);
    return robotraconteurlite_messageelement_reader_find_nested_element_verify_scalar(
        element_reader, &nested_element_name_str, nested_element_reader, expected_data_type);
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_find_nested_element_verify_array_c_str(
    struct robotraconteurlite_messageelement_reader* element_reader, const char* nested_element_name,
    struct robotraconteurlite_messageelement_reader* nested_element_reader, robotraconteurlite_u16 expected_data_type,
    robotraconteurlite_u32 expected_len, robotraconteurlite_u8 var_length)
{
    struct robotraconteurlite_const_string nested_element_name_str;
    robotraconteurlite_string_from_c_str(nested_element_name, &nested_element_name_str);
    return robotraconteurlite_messageelement_reader_find_nested_element_verify_array(
        element_reader, &nested_element_name_str, nested_element_reader, expected_data_type, expected_len, var_length);
}

robotraconteurlite_status robotraconteurlite_messageelement_reader_find_nested_element_verify_string_c_str(
    struct robotraconteurlite_messageelement_reader* element_reader, const char* nested_element_name,
    struct robotraconteurlite_messageelement_reader* nested_element_reader, robotraconteurlite_u32 expected_max_len)
{
    struct robotraconteurlite_const_string nested_element_name_str;
    robotraconteurlite_string_from_c_str(nested_element_name, &nested_element_name_str);
    return robotraconteurlite_messageelement_reader_find_nested_element_verify_string(
        element_reader, &nested_element_name_str, nested_element_reader, expected_max_len);
}

/* Message Writer */

robotraconteurlite_status robotraconteurlite_message_writer_init(struct robotraconteurlite_message_writer* writer,
                                                                 struct robotraconteurlite_buffer_vec* buffer,
                                                                 robotraconteurlite_size_t offset,
                                                                 robotraconteurlite_size_t count,
                                                                 robotraconteurlite_u16 message_version)
{
    robotraconteurlite_size_t buffer_len = 0;
    robotraconteurlite_status rv = -1;

    assert(writer != NULL);
    assert(buffer != NULL);

    /*if (buffer->buffer_vec_cnt == 1 && (buffer->buffer_vec != NULL) && (buffer->buffer_vec->data == NULL) &&
    (buffer->buffer_vec_cnt == 0))
    {
        writer->dry_run = 1;
    }
    else
    {*/
    rv = robotraconteurlite_buffer_vec_len(buffer, &buffer_len);
    if (FAILED(rv))
    {
        return rv;
    }

    if (buffer_len < (offset + count))
    {
        return ROBOTRACONTEURLITE_ERROR_OUT_OF_RANGE;
    }
    /*writer->dry_run = 0;
}*/

    writer->buffer = buffer;
    writer->buffer_offset = offset;
    writer->buffer_count = count;
    writer->message_version = message_version;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_message_write_uint32(struct robotraconteurlite_buffer_vec* buffer,

                                                                         robotraconteurlite_size_t* offset,
                                                                         robotraconteurlite_u32 val)
{
    robotraconteurlite_status rv = robotraconteurlite_buffer_vec_copy_from_uint32(buffer, *offset, val);
    if (FAILED(rv))
    {
        return rv;
    }
    *offset += 4U;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_message_write_uint16(struct robotraconteurlite_buffer_vec* buffer,
                                                                         robotraconteurlite_size_t* offset,
                                                                         robotraconteurlite_u16 val)
{
    robotraconteurlite_status rv = robotraconteurlite_buffer_vec_copy_from_uint16(buffer, *offset, val);
    if (FAILED(rv))
    {
        return rv;
    }
    *offset += 2U;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_message_write_uint8(struct robotraconteurlite_buffer_vec* buffer,
                                                                        robotraconteurlite_size_t* offset,
                                                                        robotraconteurlite_u8 val)
{
    robotraconteurlite_status rv = robotraconteurlite_buffer_vec_copy_from_uint8(buffer, *offset, val);
    if (FAILED(rv))
    {
        return rv;
    }
    *offset += 1U;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_message_write_int32(struct robotraconteurlite_buffer_vec* buffer,

                                                                        robotraconteurlite_size_t* offset,
                                                                        robotraconteurlite_i32 val)
{
    robotraconteurlite_status rv = robotraconteurlite_buffer_vec_copy_from_int32(buffer, *offset, val);
    if (FAILED(rv))
    {
        return rv;
    }
    *offset += 4U;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_message_write_int16(struct robotraconteurlite_buffer_vec* buffer,
                                                                        robotraconteurlite_size_t* offset,
                                                                        robotraconteurlite_i16 val)
{
    robotraconteurlite_status rv = robotraconteurlite_buffer_vec_copy_from_int16(buffer, *offset, val);
    if (FAILED(rv))
    {
        return rv;
    }
    *offset += 2U;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_message_write_int8(struct robotraconteurlite_buffer_vec* buffer,
                                                                       robotraconteurlite_size_t* offset,
                                                                       robotraconteurlite_i8 val)
{
    robotraconteurlite_status rv = robotraconteurlite_buffer_vec_copy_from_int8(buffer, *offset, val);
    if (FAILED(rv))
    {
        return rv;
    }
    *offset += 1U;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_message_write_uint_x(struct robotraconteurlite_buffer_vec* buffer,
                                                                         robotraconteurlite_size_t* offset,
                                                                         robotraconteurlite_u32 val)
{
    robotraconteurlite_status rv = -1;
    if (val <= 252U)
    {
        return robotraconteurlite_message_write_uint8(buffer, offset, (robotraconteurlite_u8)val);
    }
    else if (val <= ROBOTRACONTEURLITE_UINT16_MAX)
    {
        rv = robotraconteurlite_message_write_uint8(buffer, offset, 253U);
        if (FAILED(rv))
        {
            return rv;
        }
        return robotraconteurlite_message_write_uint16(buffer, offset, (robotraconteurlite_u16)val);
    }
    else
    {
        rv = robotraconteurlite_message_write_uint8(buffer, offset, 254U);
        if (FAILED(rv))
        {
            return rv;
        }
        return robotraconteurlite_message_write_uint32(buffer, offset, val);
    }
}

static robotraconteurlite_status robotraconteurlite_message_uint_x_size(robotraconteurlite_u32 val,
                                                                        robotraconteurlite_size_t* size)
{
    if (val <= 252U)
    {
        *size = 1U;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    else if (val <= ROBOTRACONTEURLITE_UINT16_MAX)
    {
        *size = 3U;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    else
    {
        *size = 5U;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
}

static robotraconteurlite_status robotraconteurlite_message_len_plus_uint_x(robotraconteurlite_u32 data_len,
                                                                            robotraconteurlite_u32* data_len_out,
                                                                            robotraconteurlite_size_t* size)
{
    if (data_len <= 251U)
    {
        *size = 1U;
        *data_len_out = data_len + 1U;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    else if (data_len <= (ROBOTRACONTEURLITE_UINT16_MAX - 3U))
    {
        *size = 3U;
        *data_len_out = data_len + 3U;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    else if (data_len <= (ROBOTRACONTEURLITE_UINT32_MAX - 5U))
    {
        *size = 5U;
        *data_len_out = data_len + 5U;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    else
    {
        return ROBOTRACONTEURLITE_ERROR_OUT_OF_RANGE;
    }
}

static robotraconteurlite_status robotraconteurlite_message_write_padded_uint_x(
    struct robotraconteurlite_buffer_vec* buffer, robotraconteurlite_size_t* offset, robotraconteurlite_u32 count,
    robotraconteurlite_size_t* count_offset)
{
    robotraconteurlite_status rv = -1;
    rv = robotraconteurlite_message_write_uint8(buffer, offset, 254U);
    if (FAILED(rv))
    {
        return rv;
    }
    *count_offset = *offset;
    return robotraconteurlite_message_write_uint32(buffer, offset, count);
}

static robotraconteurlite_status robotraconteurlite_message_padded_uint_x_size(robotraconteurlite_u32 count,
                                                                               robotraconteurlite_size_t* size)
{
    ROBOTRACONTEURLITE_UNUSED(count);
    *size = 5U;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_message_write_padded_uint_x_val(
    struct robotraconteurlite_buffer_vec* buffer, robotraconteurlite_size_t count_offset, robotraconteurlite_u32 count)
{
    robotraconteurlite_status rv = robotraconteurlite_buffer_vec_copy_from_uint32(buffer, count_offset, count);
    return rv;
}

static robotraconteurlite_status robotraconteurlite_message_write_int_x(struct robotraconteurlite_buffer_vec* buffer,
                                                                        robotraconteurlite_size_t* offset,
                                                                        robotraconteurlite_i32 val)
{
    robotraconteurlite_status rv = -1;
    if ((val >= ROBOTRACONTEURLITE_INT8_MIN) && (val <= 124))
    {
        return robotraconteurlite_message_write_int8(buffer, offset, (robotraconteurlite_i8)val);
    }
    else if ((val >= ROBOTRACONTEURLITE_INT16_MIN) && (val <= ROBOTRACONTEURLITE_INT16_MAX))
    {
        rv = robotraconteurlite_message_write_int8(buffer, offset, 125);
        if (FAILED(rv))
        {
            return rv;
        }
        return robotraconteurlite_message_write_int16(buffer, offset, (robotraconteurlite_i16)val);
    }
    else
    {
        rv = robotraconteurlite_message_write_int8(buffer, offset, 126);
        if (FAILED(rv))
        {
            return rv;
        }
        return robotraconteurlite_message_write_int32(buffer, offset, val);
    }
}

static robotraconteurlite_status robotraconteurlite_message_int_x_size(robotraconteurlite_u32 val,
                                                                       robotraconteurlite_size_t* size)
{
    if (val <= 252U)
    {
        *size = 1U;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    else if (val <= ROBOTRACONTEURLITE_UINT16_MAX)
    {
        *size = 3U;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    else
    {
        *size = 5U;
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
}

static robotraconteurlite_status robotraconteurlite_message_header_string_with_len_prefix_size4(
    const struct robotraconteurlite_const_string* str, robotraconteurlite_size_t* size)
{
    robotraconteurlite_size_t s1 = 0;
    robotraconteurlite_status rv = robotraconteurlite_message_uint_x_size(str->len, &s1);
    if (FAILED(rv))
    {
        return rv;
    }

    s1 += str->len;
    *size = s1;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_message_write_header_string_with_len_prefix2(
    struct robotraconteurlite_buffer_vec* buffer, robotraconteurlite_size_t* offset,
    const struct robotraconteurlite_const_string* str)
{
    robotraconteurlite_status rv = -1;
    rv = robotraconteurlite_message_write_uint16(buffer, offset, str->len);
    if (FAILED(rv))
    {
        return rv;
    }

    if (str->len > 0U)
    {
        rv = robotraconteurlite_buffer_vec_copy_from_string(buffer, *offset, str, 0, str->len);
        if (FAILED(rv))
        {
            return rv;
        }

        *offset += str->len;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_message_write_header_string_with_len_prefix4(
    struct robotraconteurlite_buffer_vec* buffer, robotraconteurlite_size_t* offset,
    const struct robotraconteurlite_const_string* str)
{
    robotraconteurlite_status rv = -1;
    rv = robotraconteurlite_message_write_uint_x(buffer, offset, str->len);
    if (FAILED(rv))
    {
        return rv;
    }

    if (str->len > 0U)
    {
        rv = robotraconteurlite_buffer_vec_copy_from_string(buffer, *offset, str, 0, str->len);
        if (FAILED(rv))
        {
            return rv;
        }

        *offset += str->len;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_message_writer_write_header2_ex(
    struct robotraconteurlite_message_writer* writer, robotraconteurlite_size_t* offset,
    struct robotraconteurlite_message_const_header* header, struct robotraconteurlite_message_buffer_info* buffer_info)
{
    robotraconteurlite_size_t str1_len = header->sender_nodename.len;
    robotraconteurlite_size_t str2_len = header->receiver_nodename.len;
    robotraconteurlite_size_t str3_len = header->metadata.len;
    robotraconteurlite_size_t str4_len = str1_len + str2_len + str3_len;
    robotraconteurlite_u16 header_size = 0;
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_array_uint8 sender_nodeid_array;
    struct robotraconteurlite_array_uint8 recv_nodeid_array;

    if ((str1_len > ROBOTRACONTEURLITE_UINT16_MAX) || (str2_len > ROBOTRACONTEURLITE_UINT16_MAX) ||
        (str3_len > ROBOTRACONTEURLITE_UINT16_MAX) || ((str4_len + 64U) > ROBOTRACONTEURLITE_UINT16_MAX))
    {
        return ROBOTRACONTEURLITE_ERROR_OUT_OF_RANGE;
    }

    header_size = (robotraconteurlite_u16)(str4_len + 64U);

    buffer_info->start_buffer_offset = *offset;

    rv = robotraconteurlite_buffer_vec_copy_from_mem(writer->buffer, *offset,
                                                     (const robotraconteurlite_byte*)rrac_magic, 4, 0, 1, 4);
    if (FAILED(rv))
    {
        return rv;
    }

    *offset += 4U;

    rv = robotraconteurlite_message_write_uint32(writer->buffer, offset, 0);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_uint16(writer->buffer, offset, 2);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_uint16(writer->buffer, offset, header_size);
    if (FAILED(rv))
    {
        return rv;
    }

    sender_nodeid_array.data = header->sender_nodeid.data;
    sender_nodeid_array.len = 16;
    recv_nodeid_array.data = header->receiver_nodeid.data;
    recv_nodeid_array.len = 16;

    rv = robotraconteurlite_buffer_vec_copy_from_uint8_array(writer->buffer, *offset, &sender_nodeid_array, 0, 16);
    if (FAILED(rv))
    {
        return rv;
    }
    *offset += 16U;

    rv = robotraconteurlite_buffer_vec_copy_from_uint8_array(writer->buffer, *offset, &recv_nodeid_array, 0, 16);
    if (FAILED(rv))
    {
        return rv;
    }
    *offset += 16U;

    rv = robotraconteurlite_message_write_uint32(writer->buffer, offset, header->sender_endpoint);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_uint32(writer->buffer, offset, header->receiver_endpoint);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_header_string_with_len_prefix2(writer->buffer, offset,
                                                                         &header->sender_nodename);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_header_string_with_len_prefix2(writer->buffer, offset,
                                                                         &header->receiver_nodename);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_header_string_with_len_prefix2(writer->buffer, offset, &header->metadata);
    if (FAILED(rv))
    {
        return rv;
    }

    buffer_info->entry_count_offset = *offset;
    rv = robotraconteurlite_message_write_uint16(writer->buffer, offset, header->entry_count);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_uint16(writer->buffer, offset, header->message_id);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_uint16(writer->buffer, offset, header->message_res_id);
    if (FAILED(rv))
    {
        return rv;
    }
    buffer_info->entry_start_offset = *offset;
    header->header_size = header_size;
    buffer_info->header_size = header_size;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_message_writer_write_header4_ex(
    struct robotraconteurlite_message_writer* writer, robotraconteurlite_size_t* offset,
    struct robotraconteurlite_message_const_header* header, robotraconteurlite_u8 message_flags_mask,
    struct robotraconteurlite_message_buffer_info* buffer_info)
{
    robotraconteurlite_size_t header_size = 11U;
    robotraconteurlite_u8 flags = 0;
    robotraconteurlite_status rv = -1;

    if (FLAGS_CHECK(message_flags_mask, ROBOTRACONTEURLITE_MESSAGE_FLAGS_ROUTING_INFO))
    {
        if ((robotraconteurlite_nodeid_isany(&header->sender_nodeid) == 0U) ||
            (robotraconteurlite_nodeid_isany(&header->receiver_nodeid) == 0U) || (header->sender_nodename.len != 0U) ||
            (header->receiver_nodename.len != 0U))
        {
            robotraconteurlite_size_t s = 0;
            header_size += 32U;
            rv = robotraconteurlite_message_header_string_with_len_prefix_size4(&header->sender_nodename, &s);
            if (FAILED(rv))
            {
                return rv;
            }
            header_size += s;
            rv = robotraconteurlite_message_header_string_with_len_prefix_size4(&header->receiver_nodename, &s);
            if (FAILED(rv))
            {
                return rv;
            }
            header_size += s;
            FLAGS_SET(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_ROUTING_INFO);
        }
    }

    if (FLAGS_CHECK(message_flags_mask, ROBOTRACONTEURLITE_MESSAGE_FLAGS_ENDPOINT_INFO))
    {
        if ((header->sender_endpoint != 0U) || (header->receiver_endpoint != 0U))
        {
            robotraconteurlite_size_t s = 0;
            rv = robotraconteurlite_message_uint_x_size(header->sender_endpoint, &s);
            if (FAILED(rv))
            {
                return rv;
            }
            header_size += s;
            rv = robotraconteurlite_message_uint_x_size(header->receiver_endpoint, &s);
            if (FAILED(rv))
            {
                return rv;
            }
            header_size += s;
            FLAGS_SET(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_ENDPOINT_INFO);
        }
    }

    if (FLAGS_CHECK(message_flags_mask, ROBOTRACONTEURLITE_MESSAGE_FLAGS_PRIORITY))
    {
        if (header->priority != 0U)
        {
            FLAGS_SET(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_PRIORITY);
            header_size += 2U;
        }
    }

    if (FLAGS_CHECK(message_flags_mask, ROBOTRACONTEURLITE_MESSAGE_FLAGS_META_INFO))
    {
        if ((header->metadata.len != 0U) || (header->message_id != 0U) || (header->message_res_id != 0U))
        {
            robotraconteurlite_size_t s = 0;
            rv = robotraconteurlite_message_header_string_with_len_prefix_size4(&header->metadata, &s);
            if (FAILED(rv))
            {
                return rv;
            }
            header_size += s;
            header_size += 4U;
            FLAGS_SET(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_META_INFO);
        }
    }

    if (FLAGS_CHECK(message_flags_mask, ROBOTRACONTEURLITE_MESSAGE_FLAGS_MULTIPLE_ENTRIES))
    {
        if (header->entry_count != 1U)
        {
            robotraconteurlite_size_t s = 0;
            rv = robotraconteurlite_message_padded_uint_x_size(header->entry_count, &s);
            if (FAILED(rv))
            {
                return rv;
            }
            header_size += s;
            FLAGS_SET(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_MULTIPLE_ENTRIES);
        }
    }

    flags |= ((message_flags_mask & ROBOTRACONTEURLITE_MESSAGE_FLAGS_UNRELIABLE) & flags);
    {
        robotraconteurlite_size_t s = 0;
        robotraconteurlite_u32 header_size2 = 0;
        rv = robotraconteurlite_message_len_plus_uint_x(header_size, &header_size2, &s);
        if (FAILED(rv))
        {
            return rv;
        }
        header_size += s;
    }

    if (header_size > ROBOTRACONTEURLITE_UINT32_MAX)
    {
        return ROBOTRACONTEURLITE_ERROR_OUT_OF_RANGE;
    }

    buffer_info->start_buffer_offset = *offset;

    rv = robotraconteurlite_buffer_vec_copy_from_mem(writer->buffer, *offset,
                                                     (const robotraconteurlite_byte*)rrac_magic, 4, 0, 1, 4);
    if (FAILED(rv))
    {
        return rv;
    }

    *offset += 4U;

    rv = robotraconteurlite_message_write_uint32(writer->buffer, offset, 0);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_uint16(writer->buffer, offset, 4);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_uint_x(writer->buffer, offset, (robotraconteurlite_u32)header_size);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_uint8(writer->buffer, offset, flags);
    if (FAILED(rv))
    {
        return rv;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_ROUTING_INFO))
    {
        struct robotraconteurlite_array_uint8 sender_nodeid_array;
        struct robotraconteurlite_array_uint8 recv_nodeid_array;
        sender_nodeid_array.data = header->sender_nodeid.data;
        sender_nodeid_array.len = 16;
        recv_nodeid_array.data = header->receiver_nodeid.data;
        recv_nodeid_array.len = 16;

        rv = robotraconteurlite_buffer_vec_copy_from_uint8_array(writer->buffer, *offset, &sender_nodeid_array, 0, 16);
        if (FAILED(rv))
        {
            return rv;
        }
        *offset += 16U;

        rv = robotraconteurlite_buffer_vec_copy_from_uint8_array(writer->buffer, *offset, &recv_nodeid_array, 0, 16);
        if (FAILED(rv))
        {
            return rv;
        }
        *offset += 16U;

        rv = robotraconteurlite_message_write_header_string_with_len_prefix4(writer->buffer, offset,
                                                                             &header->sender_nodename);
        if (FAILED(rv))
        {
            return rv;
        }

        rv = robotraconteurlite_message_write_header_string_with_len_prefix4(writer->buffer, offset,
                                                                             &header->receiver_nodename);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_ENDPOINT_INFO))
    {
        rv = robotraconteurlite_message_write_uint_x(writer->buffer, offset, header->sender_endpoint);
        if (FAILED(rv))
        {
            return rv;
        }

        rv = robotraconteurlite_message_write_uint_x(writer->buffer, offset, header->receiver_endpoint);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_PRIORITY))
    {
        rv = robotraconteurlite_message_write_uint16(writer->buffer, offset, header->priority);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_META_INFO))
    {
        rv = robotraconteurlite_message_write_header_string_with_len_prefix4(writer->buffer, offset, &header->metadata);
        if (FAILED(rv))
        {
            return rv;
        }

        rv = robotraconteurlite_message_write_uint16(writer->buffer, offset, header->message_id);
        if (FAILED(rv))
        {
            return rv;
        }

        rv = robotraconteurlite_message_write_uint16(writer->buffer, offset, header->message_res_id);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGE_FLAGS_MULTIPLE_ENTRIES))
    {
        rv = robotraconteurlite_message_write_padded_uint_x(writer->buffer, offset, header->entry_count,
                                                            &buffer_info->entry_count_offset);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        buffer_info->entry_count_offset = ROBOTRACONTEURLITE_SIZE_T_MAX;
    }

    if ((*offset - buffer_info->start_buffer_offset) != header_size)
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }

    buffer_info->entry_start_offset = *offset;
    header->header_size = header_size;
    buffer_info->header_size = header_size;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_message_writer_begin_message(
    struct robotraconteurlite_message_writer* writer, struct robotraconteurlite_message_const_header* header,
    struct robotraconteurlite_messageentry_writer* entry_writer)
{
    return robotraconteurlite_message_writer_begin_message_ex(writer, header, entry_writer, 0xFFU);
}

robotraconteurlite_status robotraconteurlite_message_writer_begin_message_ex(
    struct robotraconteurlite_message_writer* writer, struct robotraconteurlite_message_const_header* header,
    struct robotraconteurlite_messageentry_writer* entry_writer, robotraconteurlite_u8 message_flags_mask)
{
    robotraconteurlite_status rv = -1;
    struct robotraconteurlite_message_buffer_info message_buffer_info;
    robotraconteurlite_size_t o = writer->buffer_offset;

    assert(header != NULL);
    assert(writer != NULL);
    assert(entry_writer != NULL);

    switch (writer->message_version)
    {
    case 2:
        rv = robotraconteurlite_message_writer_write_header2_ex(writer, &o, header, &message_buffer_info);
        break;
    case 4:
        rv = robotraconteurlite_message_writer_write_header4_ex(writer, &o, header, message_flags_mask,
                                                                &message_buffer_info);
        break;
    default:
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }
    if (FAILED(rv))
    {
        return rv;
    }

    entry_writer->buffer = writer->buffer;
    entry_writer->buffer_offset = writer->buffer_offset + message_buffer_info.header_size;
    entry_writer->buffer_count = writer->buffer_count - message_buffer_info.header_size;
    entry_writer->entries_written_count = 0;
    entry_writer->entries_written_size = 0;
    entry_writer->message_version = writer->message_version;
    writer->message_size_offset = message_buffer_info.start_buffer_offset + 4U;
    writer->entry_count_offset = message_buffer_info.entry_count_offset;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_message_writer_end_message(
    struct robotraconteurlite_message_writer* writer, struct robotraconteurlite_message_const_header* header,
    struct robotraconteurlite_messageentry_writer* entry_writer)
{
    robotraconteurlite_size_t message_size = 0;
    robotraconteurlite_status rv = -1;
    robotraconteurlite_u16 ver = writer->message_version;

    assert(writer != NULL);
    assert(header != NULL);
    assert(entry_writer != NULL);

    message_size = entry_writer->entries_written_size + header->header_size;

    rv = robotraconteurlite_buffer_vec_copy_from_uint32(writer->buffer, writer->message_size_offset, message_size);
    if (FAILED(rv))
    {
        return rv;
    }

    if (writer->entry_count_offset != ROBOTRACONTEURLITE_SIZE_T_MAX)
    {
        switch (ver)
        {
        case 2: {
            robotraconteurlite_size_t o = writer->entry_count_offset;
            rv = robotraconteurlite_message_write_uint16(writer->buffer, &o,
                                                         (robotraconteurlite_u16)entry_writer->entries_written_count);
            break;
        }
        case 4:
            rv = robotraconteurlite_message_write_padded_uint_x_val(
                writer->buffer, writer->entry_count_offset,
                (robotraconteurlite_u32)entry_writer->entries_written_count);
            break;
        default:
            return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
        }
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        if (entry_writer->entries_written_count != 1U)
        {
            return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
        }
    }

    header->message_size = message_size;
    header->entry_count = (robotraconteurlite_u32)entry_writer->entries_written_count;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageentry_writer_write_header2_ex(
    struct robotraconteurlite_messageentry_writer* entry_writer, robotraconteurlite_size_t* offset,
    struct robotraconteurlite_messageentry_const_header* header,
    struct robotraconteurlite_messageentry_buffer_info* buffer_info)
{
    robotraconteurlite_status rv = -1;
    buffer_info->entry_size_offset = *offset;
    rv = robotraconteurlite_message_write_uint32(entry_writer->buffer, offset, 0);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_uint16(entry_writer->buffer, offset, header->entry_type);
    if (FAILED(rv))
    {
        return rv;
    }

    /* pad */
    rv = robotraconteurlite_message_write_uint16(entry_writer->buffer, offset, 0);
    if (FAILED(rv))
    {
        return rv;
    }
    rv = robotraconteurlite_message_write_header_string_with_len_prefix2(entry_writer->buffer, offset,
                                                                         &header->service_path);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_header_string_with_len_prefix2(entry_writer->buffer, offset,
                                                                         &header->member_name);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_uint32(entry_writer->buffer, offset, header->request_id);

    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_uint16(entry_writer->buffer, offset, header->error);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_header_string_with_len_prefix2(entry_writer->buffer, offset,
                                                                         &header->metadata);
    if (FAILED(rv))
    {
        return rv;
    }

    buffer_info->element_count_offset = *offset;
    rv = robotraconteurlite_message_write_uint16(entry_writer->buffer, offset, 0);
    if (FAILED(rv))
    {
        return rv;
    }

    buffer_info->header_size = *offset - entry_writer->buffer_offset;
    buffer_info->element_start_offset = *offset;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageentry_writer_write_header4_ex(
    struct robotraconteurlite_messageentry_writer* entry_writer, robotraconteurlite_size_t* offset,
    struct robotraconteurlite_messageentry_const_header* header,
    struct robotraconteurlite_messageentry_buffer_info* buffer_info)
{
    robotraconteurlite_status rv = -1;
    robotraconteurlite_u8 flags = 0;

    if (header->service_path.len > 0U)
    {
        FLAGS_SET(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_SERVICE_PATH_STR);
    }

    if (header->service_path_code != 0U)
    {
        FLAGS_SET(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_SERVICE_PATH_CODE);
    }

    if (header->member_name.len > 0U)
    {
        FLAGS_SET(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_MEMBER_NAME_STR);
    }

    if (header->request_id != 0U)
    {
        FLAGS_SET(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_REQUEST_ID);
    }

    if (header->error != 0U)
    {
        FLAGS_SET(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_ERROR);
    }

    if (header->metadata.len > 0U)
    {
        FLAGS_SET(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_META_INFO);
    }

    rv = robotraconteurlite_message_write_padded_uint_x(entry_writer->buffer, offset, 0,
                                                        &buffer_info->entry_size_offset);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_uint8(entry_writer->buffer, offset, flags);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_uint16(entry_writer->buffer, offset, header->entry_type);
    if (FAILED(rv))
    {
        return rv;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_SERVICE_PATH_STR))
    {
        rv = robotraconteurlite_message_write_header_string_with_len_prefix4(entry_writer->buffer, offset,
                                                                             &header->service_path);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_SERVICE_PATH_CODE))
    {
        rv = robotraconteurlite_message_write_uint_x(entry_writer->buffer, offset, header->service_path_code);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_MEMBER_NAME_STR))
    {
        rv = robotraconteurlite_message_write_header_string_with_len_prefix4(entry_writer->buffer, offset,
                                                                             &header->member_name);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_MEMBER_NAME_CODE))
    {
        rv = robotraconteurlite_message_write_uint_x(entry_writer->buffer, offset, header->member_name_code);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_REQUEST_ID))
    {
        rv = robotraconteurlite_message_write_uint_x(entry_writer->buffer, offset, header->request_id);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_ERROR))
    {
        rv = robotraconteurlite_message_write_uint16(entry_writer->buffer, offset, header->error);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEENTRY_FLAGS_META_INFO))
    {
        rv = robotraconteurlite_message_write_header_string_with_len_prefix4(entry_writer->buffer, offset,
                                                                             &header->metadata);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    rv = robotraconteurlite_message_write_padded_uint_x(entry_writer->buffer, offset, 0,
                                                        &buffer_info->element_count_offset);

    if (FAILED(rv))
    {
        return rv;
    }

    buffer_info->header_size = *offset - entry_writer->buffer_offset;
    buffer_info->element_start_offset = *offset;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageentry_writer_begin_entry(
    struct robotraconteurlite_messageentry_writer* entry_writer,
    struct robotraconteurlite_messageentry_const_header* header,
    struct robotraconteurlite_messageelement_writer* element_writer)
{

    robotraconteurlite_status rv = -1;
    robotraconteurlite_size_t o = entry_writer->buffer_offset;
    struct robotraconteurlite_messageentry_buffer_info buffer_info;

    assert(entry_writer != NULL);
    assert(header != NULL);
    assert(element_writer != NULL);

    if (!(entry_writer->entries_written_count < ROBOTRACONTEURLITE_UINT16_MAX))
    {
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }
    switch (entry_writer->message_version)
    {
    case 2:
        rv = robotraconteurlite_messageentry_writer_write_header2_ex(entry_writer, &o, header, &buffer_info);
        break;
    case 4:
        rv = robotraconteurlite_messageentry_writer_write_header4_ex(entry_writer, &o, header, &buffer_info);
        break;
    default:
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }
    if (FAILED(rv))
    {
        return rv;
    }

    element_writer->buffer = entry_writer->buffer;
    element_writer->buffer_offset = entry_writer->buffer_offset + buffer_info.header_size;
    element_writer->buffer_count = entry_writer->buffer_count - buffer_info.header_size;
    element_writer->elements_written_count = 0;
    element_writer->elements_written_size = 0;
    element_writer->message_version = entry_writer->message_version;
    entry_writer->entry_size_offset = buffer_info.entry_size_offset;
    entry_writer->element_count_offset = buffer_info.element_count_offset;
    entry_writer->entry_header_size = buffer_info.header_size;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageentry_writer_end_entry(
    struct robotraconteurlite_messageentry_writer* entry_writer,
    struct robotraconteurlite_messageentry_const_header* header,
    struct robotraconteurlite_messageelement_writer* element_writer)
{

    robotraconteurlite_status rv = -1;
    robotraconteurlite_u32 entry_size = 0;

    assert(entry_writer != NULL);
    assert(header != NULL);
    assert(element_writer != NULL);

    entry_size = (robotraconteurlite_u32)(entry_writer->entry_header_size + element_writer->elements_written_size);

    switch (entry_writer->message_version)
    {
    case 2: {
        robotraconteurlite_size_t o = entry_writer->entry_size_offset;
        rv = robotraconteurlite_message_write_uint32(entry_writer->buffer, &o, entry_size);
        if (FAILED(rv))
        {
            return rv;
        }
        o = entry_writer->element_count_offset;
        rv = robotraconteurlite_message_write_uint16(entry_writer->buffer, &o,
                                                     (robotraconteurlite_u16)element_writer->elements_written_count);
        if (FAILED(rv))
        {
            return rv;
        }
        break;
    }
    case 4: {
        rv = robotraconteurlite_message_write_padded_uint_x_val(entry_writer->buffer, entry_writer->entry_size_offset,
                                                                entry_size);
        if (FAILED(rv))
        {
            return rv;
        }
        rv = robotraconteurlite_message_write_padded_uint_x_val(
            entry_writer->buffer, entry_writer->element_count_offset,
            (robotraconteurlite_u32)element_writer->elements_written_count);
        if (FAILED(rv))
        {
            return rv;
        }
        break;
    }
    default:
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }

    entry_writer->entries_written_count++;
    entry_writer->entries_written_size += entry_size;
    entry_writer->buffer_offset += entry_size;
    entry_writer->buffer_count -= entry_size;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_header4_ex(
    struct robotraconteurlite_messageelement_writer* element_writer, robotraconteurlite_size_t* offset,
    struct robotraconteurlite_messageelement_const_header* header,
    struct robotraconteurlite_messageelement_buffer_info* buffer_info)
{
    robotraconteurlite_status rv = -1;
    robotraconteurlite_u8 flags = 0;

    if (header->element_name.len > 0U)
    {
        FLAGS_SET(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_NAME_STR);
    }

    if (header->element_name_code != 0U)
    {
        FLAGS_SET(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_NAME_CODE);
    }

    if (header->element_number != 0)
    {
        FLAGS_SET(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_NUMBER);
    }

    if (header->element_type_name.len > 0U)
    {
        FLAGS_SET(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_TYPE_NAME_STR);
    }

    if (header->element_type_name_code != 0U)
    {
        FLAGS_SET(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_TYPE_NAME_CODE);
    }

    if (header->metadata.len > 0U)
    {
        FLAGS_SET(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_META_INFO);
    }

    buffer_info->start_buffer_offset = *offset;
    rv = robotraconteurlite_message_write_padded_uint_x(element_writer->buffer, offset, 0,
                                                        &buffer_info->element_size_offset);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_uint8(element_writer->buffer, offset, flags);
    if (FAILED(rv))
    {
        return rv;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_NAME_STR))
    {
        buffer_info->element_name_str_offset = *offset;
        rv = robotraconteurlite_message_write_header_string_with_len_prefix4(element_writer->buffer, offset,
                                                                             &header->element_name);
        if (FAILED(rv))
        {
            return rv;
        }
    }
    else
    {
        buffer_info->element_name_str_offset = ROBOTRACONTEURLITE_SIZE_T_MAX;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_NAME_CODE))
    {
        rv = robotraconteurlite_message_write_uint_x(element_writer->buffer, offset, header->element_name_code);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_NUMBER))
    {
        rv = robotraconteurlite_message_write_int_x(element_writer->buffer, offset, header->element_number);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    rv = robotraconteurlite_message_write_uint16(element_writer->buffer, offset, header->element_type);
    if (FAILED(rv))
    {
        return rv;
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_TYPE_NAME_STR))
    {
        rv = robotraconteurlite_message_write_header_string_with_len_prefix4(element_writer->buffer, offset,
                                                                             &header->element_type_name);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_TYPE_NAME_CODE))
    {
        rv = robotraconteurlite_message_write_uint_x(element_writer->buffer, offset, header->element_type_name_code);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_META_INFO))
    {
        rv = robotraconteurlite_message_write_header_string_with_len_prefix4(element_writer->buffer, offset,
                                                                             &header->metadata);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_padded_uint_x(element_writer->buffer, offset, 0,
                                                        &buffer_info->element_count_offset);
    if (FAILED(rv))
    {
        return rv;
    }

    buffer_info->header_size = *offset - element_writer->buffer_offset;
    buffer_info->data_start_offset = *offset;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_header2_ex(
    struct robotraconteurlite_messageelement_writer* element_writer, robotraconteurlite_size_t* offset,
    struct robotraconteurlite_messageelement_const_header* header,
    struct robotraconteurlite_messageelement_buffer_info* buffer_info)
{
    robotraconteurlite_status rv = -1;
    buffer_info->start_buffer_offset = *offset;
    buffer_info->element_size_offset = *offset;
    rv = robotraconteurlite_message_write_uint32(element_writer->buffer, offset, 0);
    if (FAILED(rv))
    {
        return rv;
    }

    buffer_info->element_name_str_offset = *offset;
    rv = robotraconteurlite_message_write_header_string_with_len_prefix2(element_writer->buffer, offset,
                                                                         &header->element_name);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_uint16(element_writer->buffer, offset, header->element_type);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_header_string_with_len_prefix2(element_writer->buffer, offset,
                                                                         &header->element_type_name);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_header_string_with_len_prefix2(element_writer->buffer, offset,
                                                                         &header->metadata);

    if (FAILED(rv))
    {
        return rv;
    }

    buffer_info->element_count_offset = *offset;
    rv = robotraconteurlite_message_write_uint32(element_writer->buffer, offset, 0);
    if (FAILED(rv))
    {
        return rv;
    }

    buffer_info->header_size = *offset - element_writer->buffer_offset;
    buffer_info->data_start_offset = *offset;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_begin_nested_element(
    struct robotraconteurlite_messageelement_writer* element_writer,
    struct robotraconteurlite_messageelement_const_header* header,
    struct robotraconteurlite_messageelement_writer* nested_element_writer)
{

    robotraconteurlite_status rv = -1;
    robotraconteurlite_size_t o = element_writer->buffer_offset;
    struct robotraconteurlite_messageelement_buffer_info buffer_info;

    assert(element_writer != NULL);
    assert(header != NULL);
    assert(nested_element_writer != NULL);

    if (!(element_writer->elements_written_count < ROBOTRACONTEURLITE_UINT32_MAX))
    {
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }

    switch (element_writer->message_version)
    {
    case 2:
        rv = robotraconteurlite_messageelement_writer_write_header2_ex(element_writer, &o, header, &buffer_info);
        break;
    case 4:
        rv = robotraconteurlite_messageelement_writer_write_header4_ex(element_writer, &o, header, &buffer_info);
        break;
    default:
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }

    if (FAILED(rv))
    {
        return rv;
    }

    nested_element_writer->buffer = element_writer->buffer;
    nested_element_writer->buffer_offset = buffer_info.data_start_offset;
    nested_element_writer->buffer_count = element_writer->buffer_count - buffer_info.header_size;
    nested_element_writer->elements_written_count = 0;
    nested_element_writer->elements_written_size = 0;
    nested_element_writer->message_version = element_writer->message_version;
    element_writer->element_size_offset = buffer_info.element_size_offset;
    element_writer->data_count_offset = buffer_info.element_count_offset;
    element_writer->element_header_size = buffer_info.header_size;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_end_nested_element(
    struct robotraconteurlite_messageelement_writer* element_writer,
    struct robotraconteurlite_messageelement_const_header* header,
    struct robotraconteurlite_messageelement_writer* nested_element_writer)
{
    robotraconteurlite_status rv = -1;
    robotraconteurlite_size_t element_size = 0;
    robotraconteurlite_size_t o = 0;

    assert(element_writer != NULL);
    assert(header != NULL);
    assert(nested_element_writer != NULL);

    element_size = element_writer->element_header_size + nested_element_writer->elements_written_size;

    switch (element_writer->message_version)
    {
    case 2: {
        o = element_writer->element_size_offset;
        rv = robotraconteurlite_message_write_uint32(element_writer->buffer, &o, element_size);
        if (FAILED(rv))
        {
            return rv;
        }

        o = element_writer->data_count_offset;
        rv = robotraconteurlite_message_write_uint32(
            element_writer->buffer, &o, (robotraconteurlite_u32)nested_element_writer->elements_written_count);

        if (FAILED(rv))
        {
            return rv;
        }
        break;
    }
    case 4: {
        rv = robotraconteurlite_message_write_padded_uint_x_val(element_writer->buffer,
                                                                element_writer->element_size_offset, element_size);
        if (FAILED(rv))
        {
            return rv;
        }

        rv = robotraconteurlite_message_write_padded_uint_x_val(
            element_writer->buffer, element_writer->data_count_offset,
            (robotraconteurlite_u32)nested_element_writer->elements_written_count);
        if (FAILED(rv))
        {
            return rv;
        }
        break;
    }
    default:
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }

    element_writer->elements_written_count++;
    element_writer->elements_written_size += element_size;
    element_writer->buffer_offset += element_size;
    element_writer->buffer_count -= element_size;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_data_header2_ex(
    struct robotraconteurlite_messageelement_writer* element_writer, robotraconteurlite_size_t* offset,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_size_t data_len,
    robotraconteurlite_u16 data_type, robotraconteurlite_size_t data_elem_size,
    struct robotraconteurlite_messageelement_buffer_info* buffer_info, robotraconteurlite_size_t* element_size)
{
    robotraconteurlite_status rv = -1;

    robotraconteurlite_size_t str1_len = 0;
    robotraconteurlite_size_t data_size = 0;
    robotraconteurlite_size_t elem_size = 0;

    if (element_name->len > ROBOTRACONTEURLITE_UINT16_MAX)
    {
        return ROBOTRACONTEURLITE_ERROR_OUT_OF_RANGE;
    }

    str1_len = element_name->len;
    data_size = data_len * data_elem_size;
    elem_size = 16U + str1_len + data_size;
    if (elem_size > ROBOTRACONTEURLITE_UINT32_MAX)
    {
        return ROBOTRACONTEURLITE_ERROR_OUT_OF_RANGE;
    }

    buffer_info->start_buffer_offset = *offset;
    buffer_info->element_size_offset = *offset;
    rv = robotraconteurlite_message_write_uint32(element_writer->buffer, offset, (robotraconteurlite_u32)elem_size);
    if (FAILED(rv))
    {
        return rv;
    }

    buffer_info->element_name_str_offset = *offset;
    rv = robotraconteurlite_message_write_header_string_with_len_prefix2(element_writer->buffer, offset, element_name);

    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_uint16(element_writer->buffer, offset, data_type);
    if (FAILED(rv))
    {
        return rv;
    }

    /* No element type name */
    rv = robotraconteurlite_message_write_uint16(element_writer->buffer, offset, 0);
    if (FAILED(rv))
    {
        return rv;
    }

    /* No metadata */

    rv = robotraconteurlite_message_write_uint16(element_writer->buffer, offset, 0);
    if (FAILED(rv))
    {
        return rv;
    }

    buffer_info->element_count_offset = *offset;
    rv = robotraconteurlite_message_write_uint32(element_writer->buffer, offset, data_len);
    if (FAILED(rv))
    {
        return rv;
    }

    buffer_info->data_start_offset = *offset;
    buffer_info->header_size = *offset - buffer_info->start_buffer_offset;
    *element_size = elem_size;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_data_header4_ex(
    struct robotraconteurlite_messageelement_writer* element_writer, robotraconteurlite_size_t* offset,
    const struct robotraconteurlite_const_string* element_name, robotraconteurlite_size_t data_len,
    robotraconteurlite_u16 data_type, robotraconteurlite_size_t data_elem_size,
    struct robotraconteurlite_messageelement_buffer_info* buffer_info, robotraconteurlite_size_t* element_size)
{
    robotraconteurlite_status rv = -1;

    robotraconteurlite_size_t data_size = 0;
    robotraconteurlite_size_t elem_size = 3;
    robotraconteurlite_u32 elem_size2 = 0;
    robotraconteurlite_u8 flags = 0;

    if (element_name->len != 0U)
    {
        robotraconteurlite_size_t s = 0;
        FLAGS_SET(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_NAME_STR);
        rv = robotraconteurlite_message_header_string_with_len_prefix_size4(element_name, &s);
        if (FAILED(rv))
        {
            return rv;
        }
        elem_size += s;
    }

    {
        robotraconteurlite_size_t s = 0;
        rv = robotraconteurlite_message_uint_x_size(data_len, &s);
        if (FAILED(rv))
        {
            return rv;
        }
        elem_size += s;
    }

    data_size = data_len * data_elem_size;
    elem_size += data_size;

    if (elem_size > ROBOTRACONTEURLITE_UINT32_MAX)
    {
        return ROBOTRACONTEURLITE_ERROR_OUT_OF_RANGE;
    }

    {
        robotraconteurlite_size_t s = 0;
        rv = robotraconteurlite_message_len_plus_uint_x(elem_size, &elem_size2, &s);
        if (FAILED(rv))
        {
            return rv;
        }
        elem_size += s;
    }

    if (elem_size > ROBOTRACONTEURLITE_UINT32_MAX)
    {
        return ROBOTRACONTEURLITE_ERROR_OUT_OF_RANGE;
    }

    buffer_info->start_buffer_offset = *offset;
    buffer_info->element_size_offset = ROBOTRACONTEURLITE_SIZE_T_MAX;
    rv = robotraconteurlite_message_write_uint_x(element_writer->buffer, offset, (robotraconteurlite_u32)elem_size);
    if (FAILED(rv))
    {
        return rv;
    }

    rv = robotraconteurlite_message_write_uint8(element_writer->buffer, offset, flags);
    if (FAILED(rv))
    {
        return rv;
    }

    buffer_info->element_name_str_offset = ROBOTRACONTEURLITE_SIZE_T_MAX;
    if (FLAGS_CHECK(flags, ROBOTRACONTEURLITE_MESSAGEELEMENT_FLAGS_ELEMENT_NAME_STR))
    {
        rv = robotraconteurlite_message_write_header_string_with_len_prefix4(element_writer->buffer, offset,
                                                                             element_name);
        if (FAILED(rv))
        {
            return rv;
        }
    }

    rv = robotraconteurlite_message_write_uint16(element_writer->buffer, offset, data_type);
    if (FAILED(rv))
    {
        return rv;
    }

    buffer_info->element_count_offset = ROBOTRACONTEURLITE_SIZE_T_MAX;
    rv = robotraconteurlite_message_write_uint_x(element_writer->buffer, offset, data_len);
    if (FAILED(rv))
    {
        return rv;
    }

    buffer_info->data_start_offset = *offset;
    buffer_info->header_size = *offset - buffer_info->start_buffer_offset;
    *element_size = elem_size;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_messageelement_writer_write_raw(
    struct robotraconteurlite_messageelement_writer* element_writer,
    const struct robotraconteurlite_const_string* element_name, const robotraconteurlite_byte* data_buf,
    robotraconteurlite_size_t data_len, robotraconteurlite_u16 data_type, robotraconteurlite_size_t data_elem_size)
{

    robotraconteurlite_status rv = -1;
    robotraconteurlite_size_t o = 0;
    struct robotraconteurlite_messageelement_buffer_info buffer_info;
    robotraconteurlite_size_t elem_size = 0;

    assert(element_writer != NULL);
    assert(element_name != NULL);
    assert(data_buf != NULL);

    o = element_writer->buffer_offset;

    if (!(element_writer->elements_written_count < ROBOTRACONTEURLITE_UINT32_MAX))
    {
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }

    switch (element_writer->message_version)
    {
    case 2:
        rv = robotraconteurlite_messageelement_writer_write_data_header2_ex(
            element_writer, &o, element_name, data_len, data_type, data_elem_size, &buffer_info, &elem_size);
        break;
    case 4:
        rv = robotraconteurlite_messageelement_writer_write_data_header4_ex(
            element_writer, &o, element_name, data_len, data_type, data_elem_size, &buffer_info, &elem_size);
        break;
    default:
        return ROBOTRACONTEURLITE_ERROR_PROTOCOL;
    }

    if (FAILED(rv))
    {
        return rv;
    }

    o = buffer_info.data_start_offset;

    rv = robotraconteurlite_buffer_vec_copy_from_mem(element_writer->buffer, o, data_buf, data_len, 0, data_elem_size,
                                                     data_len);

    if (FAILED(rv))
    {
        return rv;
    }

    element_writer->elements_written_count++;
    element_writer->elements_written_size += elem_size;
    element_writer->buffer_offset += elem_size;
    element_writer->buffer_count -= elem_size;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

void robotraconteurlite_message_header_shallow_copy_from_mutable(const struct robotraconteurlite_message_header* src,
                                                                 struct robotraconteurlite_message_const_header* dest)
{
    assert(src != NULL);
    assert(dest != NULL);
    assert(sizeof(struct robotraconteurlite_message_header) == sizeof(struct robotraconteurlite_message_const_header));

    /* TODO: element wise copy? */
    (void)memcpy(dest, src, sizeof(struct robotraconteurlite_message_const_header));
}

void robotraconteurlite_messageentry_header_shallow_copy_from_mutable(
    const struct robotraconteurlite_messageentry_header* src, struct robotraconteurlite_messageentry_const_header* dest)
{
    assert(src != NULL);
    assert(dest != NULL);
    assert(sizeof(struct robotraconteurlite_messageentry_header) ==
           sizeof(struct robotraconteurlite_messageentry_const_header));

    (void)memcpy(dest, src, sizeof(struct robotraconteurlite_messageentry_const_header));
}

void robotraconteurlite_messageelement_header_shallow_copy_from_mutable(
    const struct robotraconteurlite_messageelement_header* src,
    struct robotraconteurlite_messageelement_const_header* dest)
{
    assert(src != NULL);
    assert(dest != NULL);
    assert(sizeof(struct robotraconteurlite_messageelement_header) ==
           sizeof(struct robotraconteurlite_messageelement_const_header));

    (void)memcpy(dest, src, sizeof(struct robotraconteurlite_messageelement_const_header));
}
