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

#ifndef ROBOTRACONTEURLITE_ERROR_H
#define ROBOTRACONTEURLITE_ERROR_H

#include "robotraconteurlite/config.h"

/* robotraconteurlite_error */

#define ROBOTRACONTEURLITE_ERROR_SUCCESS 0
#define ROBOTRACONTEURLITE_ERROR_GENERIC_ERROR (-1)
#define ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT (-2)
#define ROBOTRACONTEURLITE_ERROR_INVALID_OPERATION (-3)
#define ROBOTRACONTEURLITE_ERROR_OUT_OF_RANGE (-4)
#define ROBOTRACONTEURLITE_ERROR_INVALID_VERSION (-5)
#define ROBOTRACONTEURLITE_ERROR_DATA_ERROR (-6)
#define ROBOTRACONTEURLITE_ERROR_KEY_NOT_FOUND (-7)
#define ROBOTRACONTEURLITE_ERROR_NOT_IMPLEMENTED (-8)
#define ROBOTRACONTEURLITE_ERROR_NO_MORE (-9)
#define ROBOTRACONTEURLITE_ERROR_BUSY (-10)
#define ROBOTRACONTEURLITE_ERROR_NO_VALUE (-11)
#define ROBOTRACONTEURLITE_ERROR_TIMEOUT (-12)
#define ROBOTRACONTEURLITE_ERROR_PROTOCOL (-13)
#define ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_NOT_FOUND (-14)
#define ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_TYPE_MISMATCH (-15)
#define ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR (-16)
#define ROBOTRACONTEURLITE_ERROR_RETRY (-17)
#define ROBOTRACONTEURLITE_ERROR_CONSUMED (-18)
#define ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR (-19)
#define ROBOTRACONTEURLITE_ERROR_UNHANDLED_EVENT (-20)
#define ROBOTRACONTEURLITE_ERROR_REQUEST_REMOTE_ERROR (-21)
#define ROBOTRACONTEURLITE_ERROR_INVALID_PARAMETER (-22)
#define ROBOTRACONTEURLITE_ERROR_SYSTEM_ERROR (-23)
#define ROBOTRACONTEURLITE_ERROR_MEMBER_NOT_FOUND (-24)
#define ROBOTRACONTEURLITE_ERROR_IN_PROGRESS (-25)

#ifdef __cplusplus
extern "C" {
#endif

static robotraconteurlite_i32 robotraconteurlite_is_success(robotraconteurlite_status status)
{
    return status >= ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

#ifdef __cplusplus
}
#endif

#endif /* ROBOTRACONTEURLITE_ERROR_H */
