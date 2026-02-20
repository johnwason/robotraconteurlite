#include "robotraconteurlite/url.h"

#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <net/if.h>
#include <errno.h>

static int is_alpha_numeric(char c)
{
    if ((c >= ((char)'A')) && (c <= ((char)'Z')))
    {
        return 1;
    }

    if ((c >= ((char)'a')) && (c <= ((char)'z')))
    {
        return 1;
    }

    if ((c >= ((char)'0')) && (c <= ((char)'9')))
    {
        return 1;
    }

    return 0;
}

static int is_numeric_char(char c)
{
    if ((c >= ((char)'0')) && (c <= ((char)'9')))
    {
        return 1;
    }

    return 0;
}

static int is_scheme_char(char c)
{
    if (is_alpha_numeric(c) != 0)
    {
        return 1;
    }

    switch (c)
    {
    case '.':
    case '+':
    case '-':
        return 1;
    default:
        break;
    }
    return 0;
}

static int is_hostname_char(char c)
{
    if (is_alpha_numeric(c) != 0)
    {
        return 1;
    }

    switch (c)
    {
    case '.':
    case '-':
        return 1;
    default:
        break;
    }
    return 0;
}

static int is_ipv6_char(char c)
{
    if (is_alpha_numeric(c) != 0)
    {
        return 1;
    }

    switch (c)
    {
    case '.':
    case '-':
    case '%':
    case '_':
    case ':':
        return 1;
    default:
        break;
    }
    return 0;
}

static int is_path_char(char c)
{
    if (is_alpha_numeric(c) != 0)
    {
        return 1;
    }

    switch (c)
    {
    case '.':
    case '-':
    case '_':
    case '~':
    case '!':
    case '$':
    case '&':
    case '\'':
    case '(':
    case ')':
    case '*':
    case '+':
    case ':':
    case ',':
    case ';':
    case '=':
    case '?':
    case '@':
    case '/':
        return 1;
    default:
        break;
    }
    return 0;
}

static robotraconteurlite_status parse_port(const struct robotraconteurlite_const_string* url,
                                            robotraconteurlite_size_t* i)
{
    switch (url->data[(*i)])
    {
    case '/':
    case '?':
        return 0;
    case ':': {
        robotraconteurlite_size_t slash_delim = 0;
        robotraconteurlite_size_t port_start = *i + 1;
        (*i)++;
        while ((*i) < url->len)
        {
            if ((url->data[(*i)] == ((char)'/')) || (url->data[(*i)] == ((char)'?')))
            {
                slash_delim = (*i);
                break;
            }
            if (!is_numeric_char(url->data[*i]))
            {
                return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
            }
            (*i)++;
        }
        if (slash_delim == 0U)
        {
            return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
        }

        {
            char temp_buf[80];
            char* endptr = NULL;
            long port_l = 0;
            if ((slash_delim - port_start) >= sizeof(temp_buf))
            {
                return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
            }
            (void)memset(temp_buf, 0, sizeof(temp_buf));
            (void)memcpy(temp_buf, &url->data[port_start], (slash_delim - port_start));
            errno = 0;
            port_l = strtol(temp_buf, &endptr, 10);
            if ((endptr == temp_buf) || (errno != 0))
            {
                return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
            }
            if (port_l > 0xFFFFF)
            {
                return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
            }
            return (uint16_t)port_l;
        }
    }
    default:
        return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
    }
}

robotraconteurlite_status robotraconteurlite_url_parse(const struct robotraconteurlite_const_string* url,
                                                       struct robotraconteurlite_addr* addr_out)
{
    robotraconteurlite_size_t i = 0;
    robotraconteurlite_size_t k = 0;

    (void)memset(addr_out, 0, sizeof(struct robotraconteurlite_addr));

    /* Find scheme */
    for (i = 0; i < url->len; i++)
    {
        /* search for semicolon */
        if (url->data[i] == ((char)':'))
        {
            /* can't be first character*/
            if (i == 0U)
            {
                return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
            }

            if (url->len < (i + 3U))
            {
                return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
            }

            if ((url->data[i + 1U] != ((char)'/')) || (url->data[i + 2U] != ((char)'/')))
            {
                return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
            }

            /* found scheme */
            addr_out->scheme.data = url->data;
            addr_out->scheme.len = i + 3U;

            k = i + 3U;

            break;
        }

        if (!is_scheme_char(url->data[i]))
        {
            return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
        }
    }

    /* confirm scheme was found */
    if (addr_out->scheme.len == 0U)
    {
        return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
    }

    /* Find host or IP address */
    if (url->data[k] == ((char)'['))
    {
        /* cppcheck-suppress misra-c2012-11.3 */
        struct sockaddr_in6* ip6 = (struct sockaddr_in6*)&addr_out->socket_addr;
        robotraconteurlite_size_t ipv6_end = 0;
        robotraconteurlite_size_t scope_delim = 0;
        /* IPv6 address */

        /* Find closing bracket */
        for (i = k + 1U; i < url->len; i++)
        {
            if (url->data[i] == ((char)']'))
            {
                ipv6_end = i;
                break;
            }

            if (url->data[i] == ((char)'%'))
            {
                scope_delim = i;
            }

            if (!is_ipv6_char(url->data[i]))
            {
                return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
            }
        }

        /* check if closing bracket was found */
        if (ipv6_end == 0U)
        {
            return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
        }

        if ((ipv6_end + 1U) >= url->len)
        {
            return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
        }

        {
            char temp_buf[80];
            if ((ipv6_end - (k + 1U)) > sizeof(temp_buf))
            {
                return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
            }
            (void)memset(temp_buf, 0, sizeof(temp_buf));
            if (scope_delim == 0U)
            {
                (void)memcpy(temp_buf, &url->data[k + 1U], (ipv6_end - k - 1U));
            }
            else
            {
                (void)memcpy(temp_buf, &url->data[k + 1U], (scope_delim - k - 1U));
            }

            /* false positive*/
            /* cppcheck-suppress [misra-c2012-17.3,misra-config]*/
            if (inet_pton(AF_INET6, temp_buf, &ip6->sin6_addr) != 1)
            {
                return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
            }
            ip6->sin6_family = AF_INET6;

            if (scope_delim != 0U)
            {
                if ((ipv6_end - scope_delim) < 2U)
                {
                    return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
                }
                if ((ipv6_end - scope_delim - 1U) >= sizeof(temp_buf))
                {
                    return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
                }
                (void)memset(temp_buf, 0, sizeof(temp_buf));
                (void)memcpy(temp_buf, &url->data[scope_delim + 1U], (ipv6_end - scope_delim - 1U));
                {
                    char* endptr = NULL;
                    long scope_l = 0;
                    errno = 0;
                    scope_l = strtol(temp_buf, &endptr, 10);
                    if ((temp_buf != endptr) && (errno == 0))
                    {
                        ip6->sin6_scope_id = (uint32_t)(scope_l);
                    }
                    else
                    {
                        /* TODO: raise error if returns 0? */
                        ip6->sin6_scope_id = if_nametoindex(temp_buf);
                    }
                }
            }
        }

        i++;
        {
            robotraconteurlite_status port_r = parse_port(url, &i);
            if (port_r < 0)
            {
                return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
            }
            ip6->sin6_port = htons((uint16_t)port_r);
        }

        addr_out->http_host.data = &url->data[k];
        addr_out->http_host.len = i - k;

        addr_out->flags |= ROBOTRACONTEURLITE_ADDR_FLAGS_SOCKADDR_VALID;

        k = i;
    }
    else
    {
        robotraconteurlite_size_t host_end = 0;
        /* cppcheck-suppress misra-c2012-11.3 */
        struct sockaddr_in* ip = (struct sockaddr_in*)&addr_out->socket_addr;
        for (i = k; i < url->len; i++)
        {
            if ((url->data[i] == ((char)':')) || (url->data[i] == ((char)'/')) || (url->data[i] == ((char)'?')))
            {
                host_end = i;
                break;
            }

            if (!is_hostname_char(url->data[i]))
            {
                return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
            }
        }

        if (host_end == 0U)
        {
            return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
        }

        {
            char temp_buf[120];
            if ((host_end - k) > sizeof(temp_buf))
            {
                return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
            }
            (void)memset(temp_buf, 0, sizeof(temp_buf));
            (void)memcpy(temp_buf, &url->data[k], (host_end - k));

            /* false positive*/
            /* cppcheck-suppress [misra-c2012-17.3,misra-config]*/
            if (inet_pton(AF_INET, temp_buf, &ip->sin_addr) == 1)
            {
                ip->sin_family = AF_INET;
                addr_out->flags |= ROBOTRACONTEURLITE_ADDR_FLAGS_SOCKADDR_VALID;
            }
        }

        {
            robotraconteurlite_status port_r = parse_port(url, &i);
            if (port_r < 0)
            {
                return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
            }
            ip->sin_port = htons((uint16_t)port_r);
        }

        addr_out->http_host.data = &url->data[k];
        addr_out->http_host.len = i - k;

        k = i;
    }

    addr_out->http_path.data = &url->data[k];
    addr_out->http_path.len = url->len - k;

    /* check remaining path for invalid characters*/
    for (i = k; i < url->len; i++)
    {
        if (!is_path_char(url->data[i]))
        {
            return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
        }
    }

    /* Read connection info from query string */

    /* Find '?' start of query string*/
    {
        robotraconteurlite_size_t query_start = 0;
        for (i = k; i < (url->len - 1U); i++)
        {
            if (url->data[i] == ((char)'?'))
            {
                query_start = i + 1U;
                break;
            }
        }
        if (query_start > 0U)
        {
            robotraconteurlite_size_t param_start = query_start;
            robotraconteurlite_size_t param_eq = 0;
            /*robotraconteurlite_size_t param_end = url->len;*/

            for (i = query_start; i < (url->len + 1U); i++)
            {
                if ((i >= url->len) || (url->data[i] == ((char)'&')))
                {
                    /* TODO: query param found */
                    if (param_eq == 0U)
                    {
                        /* Did not find an equal sign */
                        return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
                    }

                    /* cppcheck-suppress [misra-c2012-21.14,misra-c2012-21.16] */
                    if (((param_eq - param_start) == 7U) && (memcmp(&url->data[param_start], "service", 7) == 0) &&
                        ((i - param_eq) > 1U))
                    {
                        /* Found service */
                        addr_out->service_name.data = &url->data[param_eq + 1U];
                        addr_out->service_name.len = i - param_eq - 1U;
                    }

                    /* cppcheck-suppress [misra-c2012-21.14,misra-c2012-21.16] */
                    if (((param_eq - param_start) == 8U) && (memcmp(&url->data[param_start], "nodename", 8) == 0) &&
                        ((i - param_eq) > 1U))
                    {
                        /* Found nodename */
                        addr_out->nodename.data = &url->data[param_eq + 1U];
                        addr_out->nodename.len = i - param_eq - 1U;
                    }

                    /* cppcheck-suppress [misra-c2012-21.14,misra-c2012-21.16] */
                    if (((param_eq - param_start) == 6U) && (memcmp(&url->data[param_start], "nodeid", 6) == 0) &&
                        ((i - param_eq) > 1U))
                    {
                        /* Found nodeid */
                        struct robotraconteurlite_const_string nodeid_str;
                        nodeid_str.data = &url->data[param_eq + 1U];
                        nodeid_str.len = i - param_eq - 1U;
                        if (robotraconteurlite_nodeid_parse(&nodeid_str, &addr_out->nodeid) != 0)
                        {
                            return ROBOTRACONTEURLITE_ERROR_INVALID_ARGUMENT;
                        }
                    }

                    param_start = i + 1U;
                    param_eq = 0;
                    continue;
                }

                if (url->data[i] == ((char)'='))
                {
                    param_eq = i;
                }
            }
        }
    }

    (void)robotraconteurlite_string_shallow_copy_to(url, &addr_out->source_url);

    return ROBOTRACONTEURLITE_ERROR_SUCCESS;
}

robotraconteurlite_status robotraconteurlite_url_parse_cstr(const char* url, struct robotraconteurlite_addr* addr_out)
{
    struct robotraconteurlite_const_string url_str;
    robotraconteurlite_string_from_c_str(url, &url_str);
    return robotraconteurlite_url_parse(&url_str, addr_out);
}
