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

#include "robotraconteurlite/nodeid.h"
#include <string.h>
#include <stdlib.h>
#include "robotraconteurlite/array.h"
#include <errno.h>
#include <stdio.h>

robotraconteurlite_u32 robotraconteurlite_nodeid_equal(const struct robotraconteurlite_nodeid* a,
                                                       const struct robotraconteurlite_nodeid* b)
{
    return memcmp(a->data, b->data, sizeof(a->data)) == 0;
}

robotraconteurlite_u32 robotraconteurlite_nodeid_isany(const struct robotraconteurlite_nodeid* a)
{
    robotraconteurlite_u8 zero_uuid[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    return memcmp(a->data, zero_uuid, sizeof(a->data)) == 0;
}

robotraconteurlite_status robotraconteurlite_nodeid_reset(struct robotraconteurlite_nodeid* a)
{
    (void)memset(a->data, 0, sizeof(a->data));
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_nodeid_copy_to(const struct robotraconteurlite_nodeid* src,
                                                            struct robotraconteurlite_nodeid* dst)
{
    (void)memcpy(dst->data, src->data, sizeof(dst->data));
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_nodeid_parse(const struct robotraconteurlite_const_string* src_str,
                                                          struct robotraconteurlite_nodeid* dst)
{
    char buf[33];
    robotraconteurlite_size_t i = 0;

    (void)memset(buf, 0, sizeof(buf));

    if (src_str->len == 32U)
    {
        (void)memcpy(buf, src_str->data, 32);
    }
    else if ((src_str->len == 36U) || (src_str->len == 38U))
    {
        const char* s = src_str->data;

        if (src_str->len == 38U)
        {
            if ((s[0] != '{') || (s[37] != '}'))
            {
                return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
            }
            s = &s[1];
        }
        if ((s[8] != '-') || (s[13] != '-') || (s[18] != '-') || (s[23] != '-'))
        {
            return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
        }
        (void)memcpy(buf, s, 8);
        (void)memcpy(&buf[8], &s[9], 4);
        (void)memcpy(&buf[12], &s[14], 4);
        (void)memcpy(&buf[16], &s[19], 4);
        (void)memcpy(&buf[20], &s[24], 12);
    }
    else
    {
        return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
    }

    for (i = 0; i < 16U; i++)
    {
        char hex[3];
        unsigned long int val = 0;
        hex[0] = buf[i * 2U];
        hex[1] = buf[(i * 2U) + 1U];
        hex[2] = 0;
        errno = 0;
        val = strtoul(hex, NULL, 16);
        /* False positive misra violation */
        /* cppcheck-suppress misra-c2012-22.10 */
        if ((errno != 0) && (val > 255U))
        {
            return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
        }
        dst->data[i] = (robotraconteurlite_u8)val;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_nodeid_to_str(
    struct robotraconteurlite_nodeid* src, struct robotraconteurlite_string* dst_str)
{

    char tmp_buf[37];
    memset(tmp_buf, 0, sizeof(tmp_buf));

    if (dst_str->len != 36)
    {
        return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
    }

    sprintf(tmp_buf, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        src->data[0],  src->data[1],  src->data[2],  src->data[3],
        src->data[4],  src->data[5],  src->data[6],  src->data[7],
        src->data[8],  src->data[9],  src->data[10], src->data[11],
        src->data[12], src->data[13], src->data[14], src->data[15]
    );

    memcpy(dst_str->data, tmp_buf, 36);

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}