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

#ifndef ROBOTRACONTEURLITE_NODEID_H
#define ROBOTRACONTEURLITE_NODEID_H

#include "robotraconteurlite/config.h"
#include "robotraconteurlite/err.h"
#include "robotraconteurlite/util.h"

struct robotraconteurlite_const_string;

#ifdef __cplusplus
extern "C" {
#endif

#define ROBOTRACONTEURLITE_NODEID_SIZE 16

struct robotraconteurlite_nodeid
{
    robotraconteurlite_byte data[ROBOTRACONTEURLITE_NODEID_SIZE];
};

ROBOTRACONTEURLITE_API robotraconteurlite_u32
robotraconteurlite_nodeid_equal(const struct robotraconteurlite_nodeid* a, const struct robotraconteurlite_nodeid* b);

ROBOTRACONTEURLITE_API robotraconteurlite_u32
robotraconteurlite_nodeid_isany(const struct robotraconteurlite_nodeid* a);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_nodeid_newrandom(struct robotraconteurlite_nodeid* a);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_nodeid_reset(struct robotraconteurlite_nodeid* a);

ROBOTRACONTEURLITE_API robotraconteurlite_status
robotraconteurlite_nodeid_copy_to(const struct robotraconteurlite_nodeid* src, struct robotraconteurlite_nodeid* dst);

ROBOTRACONTEURLITE_API robotraconteurlite_status robotraconteurlite_nodeid_parse(
    const struct robotraconteurlite_const_string* src_str, struct robotraconteurlite_nodeid* dst);

#ifdef __cplusplus
}
#endif

#endif /* ROBOTRACONTEURLITE_NODEID_H */
