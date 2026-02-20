
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "robotraconteurlite/url.h"

#define inline
#include <cmocka.h>

#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <net/if.h>

static int compare_sockaddr(const struct robotraconteurlite_addr* addr, const char* addr_str, uint16_t port)
{
    struct sockaddr_in* ip4 = (struct sockaddr_in*)&addr->socket_addr;
    struct sockaddr_in expected;
    memset(&expected, 0, sizeof(struct sockaddr_in));
    if (inet_pton(AF_INET, addr_str, &expected.sin_addr) != 1)
    {
        return -2;
    }
    expected.sin_port = htons(port);
    expected.sin_family = AF_INET;

    if (memcmp(ip4, &expected, sizeof(struct sockaddr_in)) == 0)
    {
        return 0;
    }
    return -1;
}

static int compare_sockaddr6(const struct robotraconteurlite_addr* addr, const char* addr_str, uint16_t port,
                             const char* scopeid)
{
    struct sockaddr_in6* ip6 = (struct sockaddr_in6*)&addr->socket_addr;
    struct sockaddr_in6 expected;
    memset(&expected, 0, sizeof(struct sockaddr_in6));
    if (inet_pton(AF_INET6, addr_str, &expected.sin6_addr) != 1)
    {
        return -2;
    }
    expected.sin6_port = htons(port);
    expected.sin6_family = AF_INET6;
    {
        char* endptr = NULL;
        long scope_l = strtol(scopeid, &endptr, 10);
        if (scopeid != endptr)
        {
            expected.sin6_scope_id = (uint32_t)(scope_l);
        }
        else
        {
            /* TODO: raise error if returns 0? */
            expected.sin6_scope_id = if_nametoindex(scopeid);
        }
    }

    if (memcmp(ip6, &expected, sizeof(struct sockaddr_in)) == 0)
    {
        return 0;
    }
    return -1;
}

static int compare_nodeid(const struct robotraconteurlite_addr* addr, const char* expected_nodeid)
{
    if (strlen(expected_nodeid) == 0)
    {
        robotraconteurlite_size_t i = 0;
        for (i = 0; i < 16; i++)
        {
            if (addr->nodeid.data[i] != 0)
            {
                return 1;
            }
        }
        return 0;
    }
    else
    {
        struct robotraconteurlite_nodeid expected_nodeid_id;
        struct robotraconteurlite_const_string expected_nodeid_str;
        robotraconteurlite_string_from_c_str(expected_nodeid, &expected_nodeid_str);
        if (robotraconteurlite_nodeid_parse(&expected_nodeid_str, &expected_nodeid_id) != 0)
        {
            return -1;
        }

        return robotraconteurlite_nodeid_equal(&addr->nodeid, &expected_nodeid_id) ? 0 : 1;
    }
}

void robotraconteurlite_url_parse_test(void** state)
{
    {
        const char* url1 = "rr+tcp://192.168.1.123:113/?service=test_service";
        const char* url2 = "rr+tcp://192.168.1.123/?nodename=test_node&service=test_service";
        const char* url3 = "rr+ws://a.test.domain.com/?nodename=test_node&service=test_service";
        const char* url4 = "rr+ws://a.test.domain.com:22222/"
                           "?nodename=test_node&nodeid=540b0a8a-c32d-46f7-8771-6270684fe86b&service=test_service";
        const char* url5 = "rr+tcp://[217e:02f2:c684:13c6:88c3:e400:9603:2f5e]:127/?service=test_service";
        const char* url6 =
            "rr+tcp://[fe80::88c3:e400:9603:2f5e%eth0]?nodename=test_node.with.dots&service=test_service";
        const char* url7 = "rr+tcp://[fe80::12dA:a716:92E4:c30e%10]:223/?service=test_service";
        const char* url8 = "rr+tcp://[fe80::f6e8:f648:315d:6c39]?service=test_service";
        const char* url9 = "rr+tcp://[::1]?service=test_service";
        const char* url10 = "rr+tcp://[::1]?service=test_service";
        const char* url11 = "rr+ws://a.test.domain.com/with/file/path?nodename=test_node&service=test_service";
        const char* url12 = "rr+ws://a.test.domain.com:22222/with/file/path2/"
                            "?nodename=test_node&nodeid=540b0a8a-c32d-46f7-8771-6270684fe86b&service=test_service";

        {
            struct robotraconteurlite_addr addr1;
            memset(&addr1, 0, sizeof(addr1));
            assert_return_code(robotraconteurlite_url_parse_cstr(url1, &addr1), 0);

            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr1.scheme, "rr+tcp://"), 0);
            assert_return_code(compare_sockaddr(&addr1, "192.168.1.123", 113), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr1.nodename, ""), 0);
            assert_return_code(compare_nodeid(&addr1, ""), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr1.service_name, "test_service"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr1.http_host, "192.168.1.123:113"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr1.http_path, "/?service=test_service"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr1.source_url, url1), 0);
            assert_int_equal(addr1.flags, ROBOTRACONTEURLITE_ADDR_FLAGS_SOCKADDR_VALID);
        }

        {
            struct robotraconteurlite_addr addr2;
            memset(&addr2, 0, sizeof(addr2));
            assert_return_code(robotraconteurlite_url_parse_cstr(url2, &addr2), 0);

            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr2.scheme, "rr+tcp://"), 0);
            assert_return_code(compare_sockaddr(&addr2, "192.168.1.123", 0), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr2.scheme, "rr+tcp://"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr2.nodename, "test_node"), 0);
            assert_return_code(compare_nodeid(&addr2, ""), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr2.service_name, "test_service"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr2.http_host, "192.168.1.123"), 0);
            assert_return_code(
                robotraconteurlite_string_cmp_c_str(&addr2.http_path, "/?nodename=test_node&service=test_service"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr2.source_url, url2), 0);
            assert_int_equal(addr2.flags, ROBOTRACONTEURLITE_ADDR_FLAGS_SOCKADDR_VALID);
        }

        {
            struct robotraconteurlite_addr addr3;
            memset(&addr3, 0, sizeof(addr3));
            assert_return_code(robotraconteurlite_url_parse_cstr(url3, &addr3), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr3.scheme, "rr+ws://"), 0);
            /*assert_return_code(compare_sockaddr(&addr3, "a.test.domain.com", 0), 0);*/
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr3.nodename, "test_node"), 0);
            assert_return_code(compare_nodeid(&addr3, ""), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr3.service_name, "test_service"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr3.http_host, "a.test.domain.com"), 0);
            assert_return_code(
                robotraconteurlite_string_cmp_c_str(&addr3.http_path, "/?nodename=test_node&service=test_service"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr3.source_url, url3), 0);
            assert_int_equal(addr3.flags, 0);
        }

        {
            struct robotraconteurlite_addr addr4;
            memset(&addr4, 0, sizeof(addr4));
            assert_return_code(robotraconteurlite_url_parse_cstr(url4, &addr4), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr4.scheme, "rr+ws://"), 0);
            /*assert_return_code(compare_sockaddr(&addr4, "a.test.domain.com", 22222), 0);*/
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr4.nodename, "test_node"), 0);
            assert_return_code(compare_nodeid(&addr4, "540b0a8a-c32d-46f7-8771-6270684fe86b"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr4.service_name, "test_service"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr4.http_host, "a.test.domain.com:22222"), 0);
            assert_return_code(
                robotraconteurlite_string_cmp_c_str(
                    &addr4.http_path,
                    "/?nodename=test_node&nodeid=540b0a8a-c32d-46f7-8771-6270684fe86b&service=test_service"),
                0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr4.source_url, url4), 0);
            assert_int_equal(addr4.flags, 0);
        }

        {
            struct robotraconteurlite_addr addr5;
            memset(&addr5, 0, sizeof(addr5));
            assert_return_code(robotraconteurlite_url_parse_cstr(url5, &addr5), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr5.scheme, "rr+tcp://"), 0);
            assert_return_code(compare_sockaddr6(&addr5, "217e:02f2:c684:13c6:88c3:e400:9603:2f5e", 127, ""), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr5.nodename, ""), 0);
            assert_return_code(compare_nodeid(&addr5, ""), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr5.service_name, "test_service"), 0);
            assert_return_code(
                robotraconteurlite_string_cmp_c_str(&addr5.http_host, "[217e:02f2:c684:13c6:88c3:e400:9603:2f5e]:127"),
                0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr5.http_path, "/?service=test_service"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr5.source_url, url5), 0);
            assert_int_equal(addr5.flags, ROBOTRACONTEURLITE_ADDR_FLAGS_SOCKADDR_VALID);
        }

        {
            struct robotraconteurlite_addr addr6;
            memset(&addr6, 0, sizeof(addr6));
            assert_return_code(robotraconteurlite_url_parse_cstr(url6, &addr6), 0);
            assert_return_code(compare_sockaddr6(&addr6, "fe80::88c3:e400:9603:2f5e", 0, "eth0"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr6.scheme, "rr+tcp://"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr6.nodename, "test_node.with.dots"), 0);
            assert_return_code(compare_nodeid(&addr6, ""), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr6.service_name, "test_service"), 0);
            assert_return_code(
                robotraconteurlite_string_cmp_c_str(&addr6.http_host, "[fe80::88c3:e400:9603:2f5e%eth0]"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(
                                   &addr6.http_path, "?nodename=test_node.with.dots&service=test_service"),
                               0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr6.source_url, url6), 0);
            assert_int_equal(addr6.flags, ROBOTRACONTEURLITE_ADDR_FLAGS_SOCKADDR_VALID);
        }

        {
            struct robotraconteurlite_addr addr6;
            memset(&addr6, 0, sizeof(addr6));
            assert_return_code(robotraconteurlite_url_parse_cstr(url6, &addr6), 0);
            assert_return_code(compare_sockaddr6(&addr6, "fe80::88c3:e400:9603:2f5e", 0, "eth0"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr6.scheme, "rr+tcp://"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr6.nodename, "test_node.with.dots"), 0);
            assert_return_code(compare_nodeid(&addr6, ""), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr6.service_name, "test_service"), 0);
            assert_return_code(
                robotraconteurlite_string_cmp_c_str(&addr6.http_host, "[fe80::88c3:e400:9603:2f5e%eth0]"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(
                                   &addr6.http_path, "?nodename=test_node.with.dots&service=test_service"),
                               0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr6.source_url, url6), 0);
            assert_int_equal(addr6.flags, ROBOTRACONTEURLITE_ADDR_FLAGS_SOCKADDR_VALID);
        }

        {
            struct robotraconteurlite_addr addr7;
            memset(&addr7, 0, sizeof(addr7));
            assert_return_code(robotraconteurlite_url_parse_cstr(url7, &addr7), 0);
            assert_return_code(compare_sockaddr6(&addr7, "fe80::12dA:a716:92E4:c30e", 223, "10"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr7.scheme, "rr+tcp://"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr7.nodename, ""), 0);
            assert_return_code(compare_nodeid(&addr7, ""), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr7.service_name, "test_service"), 0);
            assert_return_code(
                robotraconteurlite_string_cmp_c_str(&addr7.http_host, "[fe80::12dA:a716:92E4:c30e%10]:223"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr7.http_path, "/?service=test_service"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr7.source_url, url7), 0);
            assert_int_equal(addr7.flags, ROBOTRACONTEURLITE_ADDR_FLAGS_SOCKADDR_VALID);
        }

        {
            struct robotraconteurlite_addr addr8;
            memset(&addr8, 0, sizeof(addr8));
            assert_return_code(robotraconteurlite_url_parse_cstr(url8, &addr8), 0);
            assert_return_code(compare_sockaddr6(&addr8, "fe80::f6e8:f648:315d:6c39", 0, ""), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr8.scheme, "rr+tcp://"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr8.nodename, ""), 0);
            assert_return_code(compare_nodeid(&addr8, ""), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr8.service_name, "test_service"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr8.http_host, "[fe80::f6e8:f648:315d:6c39]"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr8.http_path, "?service=test_service"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr8.source_url, url8), 0);
            assert_int_equal(addr8.flags, ROBOTRACONTEURLITE_ADDR_FLAGS_SOCKADDR_VALID);
        }

        {
            struct robotraconteurlite_addr addr9;
            memset(&addr9, 0, sizeof(addr9));
            assert_return_code(robotraconteurlite_url_parse_cstr(url9, &addr9), 0);
            assert_return_code(compare_sockaddr6(&addr9, "::1", 0, ""), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr9.scheme, "rr+tcp://"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr9.nodename, ""), 0);
            assert_return_code(compare_nodeid(&addr9, ""), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr9.service_name, "test_service"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr9.http_host, "[::1]"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr9.http_path, "?service=test_service"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr9.source_url, url9), 0);
            assert_int_equal(addr9.flags, ROBOTRACONTEURLITE_ADDR_FLAGS_SOCKADDR_VALID);
        }

        {
            struct robotraconteurlite_addr addr10;
            memset(&addr10, 0, sizeof(addr10));
            assert_return_code(robotraconteurlite_url_parse_cstr(url10, &addr10), 0);
            assert_return_code(compare_sockaddr6(&addr10, "::1", 0, ""), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr10.scheme, "rr+tcp://"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr10.nodename, ""), 0);
            assert_return_code(compare_nodeid(&addr10, ""), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr10.service_name, "test_service"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr10.http_host, "[::1]"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr10.http_path, "?service=test_service"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr10.source_url, url10), 0);
            assert_int_equal(addr10.flags, ROBOTRACONTEURLITE_ADDR_FLAGS_SOCKADDR_VALID);
        }

        {
            struct robotraconteurlite_addr addr11;
            memset(&addr11, 0, sizeof(addr11));
            assert_return_code(robotraconteurlite_url_parse_cstr(url11, &addr11), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr11.scheme, "rr+ws://"), 0);
            /*assert_return_code(compare_sockaddr(&addr11, "a.test.domain.com", 0), 0);*/
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr11.nodename, "test_node"), 0);
            assert_return_code(compare_nodeid(&addr11, ""), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr11.service_name, "test_service"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr11.http_host, "a.test.domain.com"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(
                                   &addr11.http_path, "/with/file/path?nodename=test_node&service=test_service"),
                               0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr11.source_url, url11), 0);
            assert_int_equal(addr11.flags, 0);
        }
        {
            struct robotraconteurlite_addr addr12;
            memset(&addr12, 0, sizeof(addr12));
            assert_return_code(robotraconteurlite_url_parse_cstr(url12, &addr12), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr12.scheme, "rr+ws://"), 0);
            /*assert_return_code(compare_sockaddr(&addr12, "a.test.domain.com", 22222), 0);*/
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr12.nodename, "test_node"), 0);
            assert_return_code(compare_nodeid(&addr12, "540b0a8a-c32d-46f7-8771-6270684fe86b"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr12.service_name, "test_service"), 0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr12.http_host, "a.test.domain.com:22222"), 0);
            assert_return_code(
                robotraconteurlite_string_cmp_c_str(
                    &addr12.http_path,
                    "/with/file/path2/"
                    "?nodename=test_node&nodeid=540b0a8a-c32d-46f7-8771-6270684fe86b&service=test_service"),
                0);
            assert_return_code(robotraconteurlite_string_cmp_c_str(&addr12.source_url, url12), 0);
            assert_int_equal(addr12.flags, 0);
        }
    }
}

int main(void)
{
    const struct CMUnitTest tests[] = {cmocka_unit_test(robotraconteurlite_url_parse_test)};
    return cmocka_run_group_tests(tests, NULL, NULL);
}
