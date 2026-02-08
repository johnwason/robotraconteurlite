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

#include "robotraconteurlite/poll.h"
#include <winsock2.h>
#include <ws2tcpip.h>

int robotraconteurlite_poll_impl(struct robotraconteurlite_pollfd* fds, int nfds, int timeout)
{
    int ret = -1;
    int last_err;
    WSAPOLLFD* pfd = (WSAPOLLFD*)fds;

    if (nfds == 0)
    {
        if (timeout > 0)
        {
            Sleep((DWORD)timeout);
        }
        return 0;
    }

    ret = WSAPoll(pfd, (ULONG)nfds, timeout);
    if (ret < 0)
    {
        last_err = WSAGetLastError();
        if (last_err == WSAEINTR)
        {
            return 0;
        }
        return ret;
    }
    return ret;
}
