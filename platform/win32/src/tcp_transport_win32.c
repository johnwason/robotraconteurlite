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

#include "robotraconteurlite/tcp_transport.h"

#include <stdlib.h>
#include <string.h>

/* Linux socket includes */
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

#include <winsock2.h>

/* Macros defined by win32 headers */
#undef FAILED
#undef SUCCEEDED

#define FLAGS_CHECK_ALL ROBOTRACONTEURLITE_FLAGS_CHECK_ALL
#define FLAGS_CHECK ROBOTRACONTEURLITE_FLAGS_CHECK
#define FLAGS_SET ROBOTRACONTEURLITE_FLAGS_SET
#define FLAGS_CLEAR ROBOTRACONTEURLITE_FLAGS_CLEAR

#define FAILED ROBOTRACONTEURLITE_FAILED

robotraconteurlite_status robotraconteurlite_tcp_sha1(const robotraconteurlite_byte* data,
                                                      robotraconteurlite_size_t len,
                                                      struct robotraconteurlite_tcp_sha1_storage* storage)
{
    /* Use Windows API to generate SHA1 */
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    DWORD cbHash = 0;
    DWORD dwFlags = CRYPT_VERIFYCONTEXT;
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, dwFlags))
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }

    if (!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash))
    {
        CryptReleaseContext(hProv, 0);
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }

    if (!CryptHashData(hHash, data, (DWORD)len, 0))
    {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }

    cbHash = sizeof(storage->sha1_bytes);
    if (!CryptGetHashParam(hHash, HP_HASHVAL, storage->sha1_bytes, &cbHash, 0))
    {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_tcp_base64_encode(const robotraconteurlite_byte* binary_data,
                                                               robotraconteurlite_size_t binary_len, char* base64_data,
                                                               robotraconteurlite_size_t* base64_len)
{
    DWORD hashlen = 0;
    char base64_data2[29];
    robotraconteurlite_size_t base64_len2 = sizeof(base64_data2);
    CryptBinaryToString(binary_data, (DWORD)binary_len, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, NULL, &hashlen);

    if (hashlen > base64_len2)
    {
        return ROBOTRACONTEURLITE_ERROR_INVALID_PARAMETER;
    }

    if (!CryptBinaryToString(binary_data, (DWORD)binary_len, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, base64_data2,
                             &hashlen))
    {
        return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
    }

    if (base64_data2[hashlen - 1] == '\0')
    {
        hashlen--;
    }

    if (hashlen > *base64_len)
    {
        return ROBOTRACONTEURLITE_ERROR_INVALID_PARAMETER;
    }

    memcpy(base64_data, base64_data2, hashlen);

    *base64_len = hashlen;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_tcp_socket_recv_nonblocking(
    struct robotraconteurlite_connection_socket* sock, robotraconteurlite_byte* buffer, robotraconteurlite_size_t* pos,
    robotraconteurlite_size_t len, int* errno_out)
{
    robotraconteurlite_size_t pos1 = *pos;
    WSABUF wsaBuf;
    DWORD flags = 0;
    DWORD bytesReceived;

    FLAGS_CLEAR(sock->flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_RECEIVE_WOULD_BLOCK);

    if (sock == 0)
    {
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    while ((*pos - pos1) < len)
    {
        wsaBuf.buf = &buffer[*pos];
        wsaBuf.len = len - (*pos - pos1);
        int ret = WSARecv(sock->sock, &wsaBuf, 1, &bytesReceived, &flags, NULL, NULL);
        if (ret == SOCKET_ERROR)
        {
            int wsaError = WSAGetLastError();
            if (wsaError == WSAEWOULDBLOCK)
            {
                FLAGS_SET(sock->flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_RECEIVE_WOULD_BLOCK);
                return ROBOTRACONTEURLITE_ERROR_SUCCESS;
            }
            *errno_out = wsaError;
            return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
        }
        *pos += bytesReceived;
        if (bytesReceived == 0)
        {
            if (*pos == pos1)
            {
                return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
            }
            else
            {
                return ROBOTRACONTEURLITE_ERROR_SUCCESS;
            }
        }
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_tcp_socket_send_nonblocking(
    struct robotraconteurlite_connection_socket* sock, const robotraconteurlite_byte* buffer,
    robotraconteurlite_size_t* pos, robotraconteurlite_size_t len, int* errno_out)
{
    robotraconteurlite_size_t pos1 = *pos;
    WSABUF wsaBuf;
    DWORD bytesSent;
    DWORD flags = 0;

    FLAGS_CLEAR(sock->flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_SEND_WOULD_BLOCK);

    if (sock == 0)
    {
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    while ((*pos - pos1) < len)
    {
        wsaBuf.buf = &buffer[*pos];
        wsaBuf.len = len - (*pos - pos1);
        int ret = WSASend(sock->sock, &wsaBuf, 1, &bytesSent, flags, NULL, NULL);
        if (ret == SOCKET_ERROR)
        {
            int wsaError = WSAGetLastError();
            if (wsaError == WSAEWOULDBLOCK)
            {
                FLAGS_SET(sock->flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_SEND_WOULD_BLOCK);
                return ROBOTRACONTEURLITE_ERROR_SUCCESS;
            }
            *errno_out = wsaError;
            return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
        }
        *pos += bytesSent;
        if (bytesSent == 0)
        {
            if (*pos == pos1)
            {
                return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
            }
            else
            {
                return ROBOTRACONTEURLITE_ERROR_SUCCESS;
            }
        }
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_tcp_socket_begin_server(
    const struct sockaddr_storage* serv_addr, robotraconteurlite_size_t backlog,
    struct robotraconteurlite_connection_socket* sock_out, int* errno_out)
{
    /* Create socket */
    SOCKET sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
    u_long mode = 1;
    if (sock == INVALID_SOCKET)
    {
        *errno_out = WSAGetLastError();
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    /* Make socket non-blocking */
    if (ioctlsocket(sock, FIONBIO, &mode) != NO_ERROR)
    {
        *errno_out = WSAGetLastError();
        closesocket(sock);
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    /* Bind socket */
    if (bind(sock, (struct sockaddr*)serv_addr, sizeof(struct sockaddr_storage)) == SOCKET_ERROR)
    {
        *errno_out = WSAGetLastError();
        closesocket(sock);
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    /* Listen */
    if (listen(sock, (int)backlog) == SOCKET_ERROR)
    {
        *errno_out = WSAGetLastError();
        closesocket(sock);
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    sock_out->sock = sock;
    sock_out->flags = ROBOTRACONTEURLITE_SOCKET_FLAGS_ACTIVE;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_tcp_configure_socket(ROBOTRACONTEURLITE_SOCKET_HANDLE sock,
                                                                         int* errno_out)
{
    int flags = 1;
    u_long mode = 1;

    if (sock == 0)
    {
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    /* Make socket non-blocking */
    if (ioctlsocket(sock, FIONBIO, &mode) != NO_ERROR)
    {
        *errno_out = WSAGetLastError();
        closesocket(sock);
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    /* Set TCP no delay */
    if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&flags, sizeof(int)) == SOCKET_ERROR)
    {
        *errno_out = WSAGetLastError();
        closesocket(sock);
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_tcp_socket_accept(
    struct robotraconteurlite_connection_socket* acceptor_sock,
    struct robotraconteurlite_connection_socket* client_sock, int* errno_out)
{
    /* Accept connection */
    struct sockaddr_in cli_addr;
    int clilen = sizeof(cli_addr);
    FLAGS_CLEAR(acceptor_sock->flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_RECEIVE_WOULD_BLOCK);
    *errno_out = 0;
    SOCKET newsockfd = WSAAccept(acceptor_sock->sock, (struct sockaddr*)&cli_addr, &clilen, NULL, NULL);
    if (newsockfd == INVALID_SOCKET)
    {
        int wsaError = WSAGetLastError();
        if (wsaError == WSAEWOULDBLOCK)
        {
            FLAGS_SET(acceptor_sock->flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_RECEIVE_WOULD_BLOCK);
            return ROBOTRACONTEURLITE_ERROR_RETRY;
        }
        *errno_out = wsaError;
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    client_sock->sock = newsockfd;
    client_sock->flags = ROBOTRACONTEURLITE_SOCKET_FLAGS_ACTIVE;

    return robotraconteurlite_tcp_configure_socket(newsockfd, errno_out);
}

robotraconteurlite_status robotraconteurlite_tcp_socket_close(struct robotraconteurlite_connection_socket* sock)
{
    if (sock->sock == 0)
    {
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    closesocket(sock->sock);
    FLAGS_CLEAR(sock->flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_ACTIVE);
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_tcp_server_socket_close(struct robotraconteurlite_connection_socket* sock)
{
    if (sock->sock == 0)
    {
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }
    closesocket(sock->sock);
    FLAGS_CLEAR(sock->flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_ACTIVE);
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_u64 robotraconteurlite_be64toh(robotraconteurlite_u64 big_endian_64bits)
{
    /* Implement byte swap */
    robotraconteurlite_u64 ret = 0;
    robotraconteurlite_byte* ret_bytes = (robotraconteurlite_byte*)&ret;
    robotraconteurlite_byte* big_endian_bytes = (robotraconteurlite_byte*)&big_endian_64bits;
    ret_bytes[0] = big_endian_bytes[7];
    ret_bytes[1] = big_endian_bytes[6];
    ret_bytes[2] = big_endian_bytes[5];
    ret_bytes[3] = big_endian_bytes[4];
    ret_bytes[4] = big_endian_bytes[3];
    ret_bytes[5] = big_endian_bytes[2];
    ret_bytes[6] = big_endian_bytes[1];
    ret_bytes[7] = big_endian_bytes[0];
    return ret;
}

robotraconteurlite_status robotraconteurlite_tcp_socket_connect(struct robotraconteurlite_sockaddr_storage* addr,
                                                                struct robotraconteurlite_connection_socket* sock_out,
                                                                int* errno_out)
{
    ROBOTRACONTEURLITE_SOCKET_HANDLE sock = socket(AF_INET, SOCK_STREAM, 0);
    *errno_out = 0;
    robotraconteurlite_status rv = robotraconteurlite_tcp_configure_socket(sock, errno_out);
    if (FAILED(rv))
    {
        return rv;
    }

    if (connect(sock, (struct sockaddr*)addr, sizeof(struct sockaddr_storage)) == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAEWOULDBLOCK)
        {
            sock_out->sock = sock;
            sock_out->flags = ROBOTRACONTEURLITE_SOCKET_FLAGS_ACTIVE;
            return ROBOTRACONTEURLITE_ERROR_SUCCESS;
        }
        closesocket(sock);
        *errno_out = WSAGetLastError();
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }
    sock_out->sock = sock;
    sock_out->flags = ROBOTRACONTEURLITE_SOCKET_FLAGS_ACTIVE;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

static robotraconteurlite_status robotraconteurlite_poll_add_fd(ROBOTRACONTEURLITE_SOCKET_HANDLE sock,
                                                                short extra_events,
                                                                struct robotraconteurlite_pollfd* pollfds,
                                                                robotraconteurlite_size_t* pollfd_count,
                                                                robotraconteurlite_size_t max_pollfds)
{
    int i = (int)*pollfd_count;
    if (i >= (int)max_pollfds)
    {
        return ROBOTRACONTEURLITE_ERROR_INVALID_PARAMETER;
    }
    (*pollfd_count) = (*pollfd_count) + 1;

    pollfds[i].fd = sock;
    pollfds[i].events = extra_events;
    pollfds[i].revents = 0;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_poll_impl_add_fd(ROBOTRACONTEURLITE_SOCKET_HANDLE sock_handle,
                                                              robotraconteurlite_u16 sock_flags,
                                                              struct robotraconteurlite_pollfd* pollfds,
                                                              robotraconteurlite_size_t* pollfd_count,
                                                              robotraconteurlite_size_t max_pollfds)
{
    short extra_events = 0;
    if ((!FLAGS_CHECK(sock_flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_ACTIVE)) || (sock_handle == 0))
    {
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    if (FLAGS_CHECK(sock_flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_WANT_SEND))
    {
        extra_events |= POLLOUT;
    }

    if (FLAGS_CHECK(sock_flags, ROBOTRACONTEURLITE_SOCKET_FLAGS_WANT_RECEIVE))
    {
        extra_events |= POLLIN;
    }

    return robotraconteurlite_poll_add_fd(sock_handle, extra_events, pollfds, pollfd_count, max_pollfds);
}

robotraconteurlite_status robotraconteurlite_tcp_socket_is_connection_complete(ROBOTRACONTEURLITE_SOCKET_HANDLE sock,
                                                                               int* errno_out)
{

    WSAPOLLFD fds[1];
    int ret = 0;

    memset(fds, 0, sizeof(fds));

    fds[0].fd = sock;
    fds[0].events = POLLOUT;

    *errno_out = 0;

    ret = WSAPoll(fds, 1, 0);

    if (ret < 0)
    {
        *errno_out = WSAGetLastError();
        if (sock != 0)
        {
            (void)closesocket(sock);
        }
        return ROBOTRACONTEURLITE_ERROR_SYSTEM_ERROR;
    }

    if (ret == 0)
    {
        return ROBOTRACONTEURLITE_ERROR_RETRY;
    }

    /* cppcheck-suppress misra-config */
    if ((fds[0].revents & POLLOUT) != 0)
    {
        return ROBOTRACONTEURLITE_ERROR_SUCCESS;
    }

    /* cppcheck-suppress misra-config */
    if ((fds[0].revents & POLLERR) != 0)
    {
        if (sock != 0)
        {
            (void)closesocket(sock);
        }
        return ROBOTRACONTEURLITE_ERROR_CONNECTION_ERROR;
    }

    return ROBOTRACONTEURLITE_ERROR_INTERNAL_ERROR;
}
