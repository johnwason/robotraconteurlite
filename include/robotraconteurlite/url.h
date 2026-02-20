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

#ifndef ROBOTRACONTEURLITE_URL_H
#define ROBOTRACONTEURLITE_URL_H

#include "robotraconteurlite/config.h"
#include "robotraconteurlite/array.h"
#include "robotraconteurlite/nodeid.h"

struct robotraconteurlite_sockaddr_storage
{
    robotraconteurlite_byte _storage[ROBOTRACONTEURLITE_SOCKADDR_STORAGE_SIZE];
};

/* robotraconteurlite_addr_flags */
#define ROBOTRACONTEURLITE_ADDR_FLAGS_NULL 0x0U
#define ROBOTRACONTEURLITE_ADDR_FLAGS_SOCKADDR_VALID 0x1U

struct robotraconteurlite_addr
{
    struct robotraconteurlite_const_string scheme;
    struct robotraconteurlite_sockaddr_storage socket_addr;
    struct robotraconteurlite_nodeid nodeid;
    struct robotraconteurlite_const_string nodename;
    struct robotraconteurlite_const_string service_name;
    robotraconteurlite_u32 flags;
    struct robotraconteurlite_const_string http_host;
    struct robotraconteurlite_const_string http_path;
    struct robotraconteurlite_const_string source_url;
};

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_url_parse(
    const struct robotraconteurlite_const_string* url, struct robotraconteurlite_addr* addr_out);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_url_parse_cstr(const char* url, struct robotraconteurlite_addr* addr_out);

#endif /* ROBOTRACONTEURLITE_URL_H */
