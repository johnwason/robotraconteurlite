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

#ifndef ROBOTRACONTEURLITE_CONFIG_H
#define ROBOTRACONTEURLITE_CONFIG_H

#ifdef ROBOTRACONTEURLITE_USE_LOCAL_CONFIG
#include "robotraconteurlite_config_local.h"
#endif

#if defined(_WIN32) && !defined(ROBOTRACONTEURLITE_NO_CMAKE_EXPORT)
#include "robotraconteurlite_export.h"
#endif

#ifndef ROBOTRACONTEURLITE_API
#define ROBOTRACONTEURLITE_API
#endif

#define ROBOTRACONTEURLITE_NODE_VERSION "0.18.0"

#define ROBOTRACONTEURLITE_UNUSED(x) (void)(x)

#ifdef _WIN32
#ifdef _WIN64
#define ROBOTRACONTEURLITE_SOCKET_HANDLE unsigned long long
#else
#define ROBOTRACONTEURLITE_SOCKET_HANDLE unsigned long
#endif
#else
#define ROBOTRACONTEURLITE_SOCKET_HANDLE int
#endif

#ifndef ROBOTRACONTEURLITE_NO_STDINT
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <float.h>
typedef int8_t robotraconteurlite_i8;
typedef uint8_t robotraconteurlite_u8;
typedef int16_t robotraconteurlite_i16;
typedef uint16_t robotraconteurlite_u16;
typedef int32_t robotraconteurlite_i32;
typedef uint32_t robotraconteurlite_u32;
typedef int64_t robotraconteurlite_i64;
typedef uint64_t robotraconteurlite_u64;
typedef float robotraconteurlite_single;
typedef double robotraconteurlite_double;
typedef size_t robotraconteurlite_size_t;
typedef uint8_t robotraconteurlite_byte;
#define ROBOTRACONTEURLITE_INT8_MIN INT8_MIN
#define ROBOTRACONTEURLITE_INT8_MAX INT8_MAX
#define ROBOTRACONTEURLITE_UINT8_MIN 0
#define ROBOTRACONTEURLITE_UINT8_MAX UINT8_MAX
#define ROBOTRACONTEURLITE_INT16_MIN INT16_MIN
#define ROBOTRACONTEURLITE_INT16_MAX INT16_MAX
#define ROBOTRACONTEURLITE_UINT16_MIN 0
#define ROBOTRACONTEURLITE_UINT16_MAX UINT16_MAX
#define ROBOTRACONTEURLITE_INT32_MIN INT32_MIN
#define ROBOTRACONTEURLITE_INT32_MAX INT32_MAX
#define ROBOTRACONTEURLITE_UINT32_MIN 0
#define ROBOTRACONTEURLITE_UINT32_MAX UINT32_MAX
#define ROBOTRACONTEURLITE_INT64_MIN INT64_MIN
#define ROBOTRACONTEURLITE_INT64_MAX INT64_MAX
#define ROBOTRACONTEURLITE_UINT64_MIN 0
#define ROBOTRACONTEURLITE_UINT64_MAX UINT64_MAX
#define ROBOTRACONTEURLITE_SINGLE_MIN FLT_MIN
#define ROBOTRACONTEURLITE_SINGLE_MAX FLT_MAX
#define ROBOTRACONTEURLITE_DOUBLE_MIN DBL_MIN
#define ROBOTRACONTEURLITE_DOUBLE_MAX DBL_MAX
#define ROBOTRACONTEURLITE_SIZE_T_MAX SIZE_MAX
#define ROBOTRACONTEURLITE_SIZE_T_MIN 0
#endif

#ifndef ROBOTRACONTEURLITE_TRANSPORT_STORAGE_SIZE
#define ROBOTRACONTEURLITE_TRANSPORT_STORAGE_SIZE 128
#endif

#ifndef ROBOTRACONTEURLITE_MESSAGE_STR_MAX_SIZE
#define ROBOTRACONTEURLITE_MESSAGE_STR_MAX_SIZE 128
#endif

#ifndef ROBOTRACONTEURLITE_SOCKADDR_STORAGE_SIZE
#define ROBOTRACONTEURLITE_SOCKADDR_STORAGE_SIZE 128
#endif

#endif /* ROBOTRACONTEURLITE_CONFIG_H */
