#include "robotraconteurlite/url.h"

#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <net/if.h>
#include <errno.h>

robotraconteurlite_u16 robotraconteurlite_ntohs(robotraconteurlite_u16 netshort) { return ntohs(netshort); }

robotraconteurlite_u16 robotraconteurlite_htons(robotraconteurlite_u16 hostshort) { return htons(hostshort); }

robotraconteurlite_i32 robotraconteurlite_inet_pton(const char* src, void* dest)
{
    return inet_pton(AF_INET, src, dest);
}

robotraconteurlite_i32 robotraconteurlite_inet_pton6(const char* src, void* dest)
{
    return inet_pton(AF_INET6, src, dest);
}

robotraconteurlite_status robotraconteurlite_url_fill_sockaddr(struct robotraconteurlite_addr* addr,
                                                               const robotraconteurlite_u8* ip_addr,
                                                               robotraconteurlite_u16 port)
{
    /* cppcheck-suppress misra-c2012-11.3 */
    struct sockaddr_in* ip4 = (struct sockaddr_in*)&addr->socket_addr;
    ip4->sin_family = AF_INET;
    (void)memcpy(&ip4->sin_addr, ip_addr, sizeof(struct in_addr));
    ip4->sin_port = port;

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_url_fill_sockaddr6(struct robotraconteurlite_addr* addr,
                                                                const robotraconteurlite_u8* ip_addr,
                                                                robotraconteurlite_u16 port,
                                                                robotraconteurlite_u32 scopeid)
{
    /* cppcheck-suppress misra-c2012-11.3 */
    struct sockaddr_in6* ip6 = (struct sockaddr_in6*)&addr->socket_addr;
    ip6->sin6_family = AF_INET6;
    (void)memcpy(&ip6->sin6_addr, ip_addr, sizeof(struct in6_addr));
    ip6->sin6_port = port;
    ip6->sin6_scope_id = scopeid;
    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_u32 robotraconteurlite_if_nametoindex(const char* ifname) { return if_nametoindex(ifname); }
