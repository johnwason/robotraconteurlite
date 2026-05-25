
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "robotraconteurlite/message.h"

#define inline
#include <cmocka.h>

robotraconteurlite_byte message2[] = {
    82,  82,  65,  67,  94,  4,   0,   0,   2,   0,   107, 0,   216, 53,  196, 29,  51,  156, 71,  168, 132, 219, 240,
    10,  143, 239, 210, 250, 174, 120, 72,  27,  194, 190, 79,  38,  159, 239, 116, 187, 110, 138, 4,   63,  71,  88,
    55,  248, 113, 100, 49,  167, 11,  0,   115, 101, 110, 100, 101, 114, 95,  110, 111, 100, 101, 9,   0,   114, 101,
    99,  118, 95,  110, 111, 100, 101, 23,  0,   101, 120, 116, 101, 110, 100, 101, 100, 95,  100, 97,  116, 97,  10,
    98,  108, 97,  104, 32,  98,  108, 97,  104, 3,   0,   10,  0,   20,  0,   70,  0,   0,   0,   88,  4,   0,   0,
    25,  0,   109, 121, 46,  115, 101, 114, 118, 105, 99,  101, 46,  111, 98,  106, 101, 99,  116, 91,  37,  50,  56,
    97,  98,  99,  93,  9,   0,   109, 121, 95,  109, 101, 109, 98,  101, 114, 208, 9,   6,   0,   20,  0,   14,  0,
    109, 111, 114, 101, 10,  98,  108, 97,  104, 10,  98,  108, 97,  104, 0,   0,   151, 3,   0,   0,   98,  4,   0,
    0,   25,  0,   109, 121, 46,  115, 101, 114, 118, 105, 99,  101, 46,  111, 98,  106, 101, 99,  116, 91,  37,  49,
    50,  53,  52,  50,  93,  10,  0,   109, 121, 95,  109, 101, 109, 98,  101, 114, 50,  158, 150, 8,   0,   0,   0,
    0,   0,   4,   0,   59,  0,   0,   0,   3,   0,   101, 108, 49,  1,   0,   0,   0,   0,   0,   5,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   240, 63,  0,   0,   0,   0,   0,   0,   0,   64,  0,   0,   0,   0,   0,   0,   8,
    64,  0,   0,   0,   0,   0,   0,   16,  64,  0,   0,   0,   0,   0,   0,   20,  64,  37,  0,   0,   0,   3,   0,
    101, 108, 50,  11,  0,   0,   0,   0,   0,   18,  0,   0,   0,   115, 111, 109, 101, 32,  114, 97,  110, 100, 111,
    109, 32,  115, 116, 114, 105, 110, 103, 172, 2,   0,   0,   3,   0,   101, 108, 51,  101, 0,   16,  0,   115, 111,
    109, 101, 95,  115, 116, 114, 117, 99,  116, 95,  116, 121, 112, 101, 0,   0,   14,  0,   0,   0,   59,  0,   0,
    0,   11,  0,   115, 117, 98,  95,  100, 111, 117, 98,  108, 101, 115, 1,   0,   0,   0,   0,   0,   4,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   240, 63,  0,   0,   0,   0,   0,   0,   0,   64,  0,   0,   0,   0,   0,   0,
    8,   64,  0,   0,   0,   0,   0,   0,   16,  64,  43,  0,   0,   0,   11,  0,   115, 117, 98,  95,  115, 105, 110,
    103, 108, 101, 115, 2,   0,   0,   0,   0,   0,   4,   0,   0,   0,   0,   0,   128, 63,  0,   0,   0,   64,  0,
    0,   64,  64,  0,   0,   128, 64,  28,  0,   0,   0,   8,   0,   115, 117, 98,  95,  105, 110, 116, 56,  3,   0,
    0,   0,   0,   0,   4,   0,   0,   0,   1,   2,   3,   4,   29,  0,   0,   0,   9,   0,   115, 117, 98,  95,  117,
    105, 110, 116, 56,  4,   0,   0,   0,   0,   0,   4,   0,   0,   0,   1,   2,   3,   4,   33,  0,   0,   0,   9,
    0,   115, 117, 98,  95,  105, 110, 116, 49,  54,  5,   0,   0,   0,   0,   0,   4,   0,   0,   0,   1,   0,   2,
    0,   3,   0,   4,   0,   34,  0,   0,   0,   10,  0,   115, 117, 98,  95,  117, 105, 110, 116, 49,  54,  6,   0,
    0,   0,   0,   0,   4,   0,   0,   0,   1,   0,   2,   0,   3,   0,   4,   0,   41,  0,   0,   0,   9,   0,   115,
    117, 98,  95,  105, 110, 116, 51,  50,  7,   0,   0,   0,   0,   0,   4,   0,   0,   0,   1,   0,   0,   0,   2,
    0,   0,   0,   3,   0,   0,   0,   4,   0,   0,   0,   42,  0,   0,   0,   10,  0,   115, 117, 98,  95,  117, 105,
    110, 116, 51,  50,  8,   0,   0,   0,   0,   0,   4,   0,   0,   0,   1,   0,   0,   0,   2,   0,   0,   0,   3,
    0,   0,   0,   4,   0,   0,   0,   57,  0,   0,   0,   9,   0,   115, 117, 98,  95,  105, 110, 116, 54,  52,  9,
    0,   0,   0,   0,   0,   4,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,   2,   0,   0,   0,   0,   0,
    0,   0,   3,   0,   0,   0,   0,   0,   0,   0,   4,   0,   0,   0,   0,   0,   0,   0,   58,  0,   0,   0,   10,
    0,   115, 117, 98,  95,  117, 105, 110, 116, 54,  52,  10,  0,   0,   0,   0,   0,   4,   0,   0,   0,   1,   0,
    0,   0,   0,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,   4,
    0,   0,   0,   0,   0,   0,   0,   47,  0,   0,   0,   10,  0,   115, 117, 98,  95,  115, 116, 114, 105, 110, 103,
    11,  0,   0,   0,   0,   0,   21,  0,   0,   0,   97,  110, 111, 116, 104, 101, 114, 32,  114, 97,  110, 100, 111,
    109, 32,  115, 116, 114, 105, 110, 103, 91,  0,   0,   0,   11,  0,   115, 117, 98,  95,  99,  100, 111, 117, 98,
    108, 101, 12,  0,   0,   0,   0,   0,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   240, 63,  0,   0,   0,
    0,   0,   0,   36,  64,  0,   0,   0,   0,   0,   0,   0,   64,  0,   0,   0,   0,   0,   0,   52,  64,  0,   0,
    0,   0,   0,   0,   8,   64,  0,   0,   0,   0,   0,   0,   62,  64,  0,   0,   0,   0,   0,   0,   16,  64,  0,
    0,   0,   0,   0,   0,   68,  64,  59,  0,   0,   0,   11,  0,   115, 117, 98,  95,  99,  115, 105, 110, 103, 108,
    101, 13,  0,   0,   0,   0,   0,   4,   0,   0,   0,   0,   0,   128, 63,  0,   0,   32,  65,  0,   0,   0,   64,
    0,   0,   160, 65,  0,   0,   64,  64,  0,   0,   240, 65,  0,   0,   128, 64,  0,   0,   32,  66,  28,  0,   0,
    0,   8,   0,   115, 117, 98,  95,  98,  111, 111, 108, 14,  0,   0,   0,   0,   0,   4,   0,   0,   0,   1,   1,
    0,   1,   82,  0,   0,   0,   58,  0,   101, 108, 52,  95,  105, 115, 95,  97,  95,  118, 101, 114, 121, 95,  108,
    111, 110, 103, 95,  110, 97,  109, 101, 95,  105, 116, 95,  107, 101, 101, 112, 115, 95,  103, 111, 105, 110, 103,
    95,  97,  110, 100, 95,  103, 111, 105, 110, 103, 95,  97,  110, 100, 95,  103, 111, 105, 110, 103, 1,   0,   0,
    0,   0,   0,   1,   0,   0,   0,   92,  143, 194, 245, 40,  44,  69,  64,  22,  0,   0,   0,   1,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

robotraconteurlite_byte message4[] = {
    82,  82,  65,  67,  141, 3,   0,   0,   4,   0,   105, 83,  216, 53,  196, 29,  51,  156, 71,  168, 132, 219, 240,
    10,  143, 239, 210, 250, 174, 120, 72,  27,  194, 190, 79,  38,  159, 239, 116, 187, 110, 138, 4,   63,  11,  115,
    101, 110, 100, 101, 114, 95,  110, 111, 100, 101, 9,   114, 101, 99,  118, 95,  110, 111, 100, 101, 254, 71,  88,
    55,  248, 254, 113, 100, 49,  167, 23,  101, 120, 116, 101, 110, 100, 101, 100, 95,  100, 97,  116, 97,  10,  98,
    108, 97,  104, 32,  98,  108, 97,  104, 10,  0,   20,  0,   3,   63,  117, 88,  4,   25,  109, 121, 46,  115, 101,
    114, 118, 105, 99,  101, 46,  111, 98,  106, 101, 99,  116, 91,  37,  50,  56,  97,  98,  99,  93,  9,   109, 121,
    95,  109, 101, 109, 98,  101, 114, 254, 208, 9,   6,   0,   20,  0,   14,  109, 111, 114, 101, 10,  98,  108, 97,
    104, 10,  98,  108, 97,  104, 0,   253, 222, 2,   21,  98,  4,   25,  109, 121, 46,  115, 101, 114, 118, 105, 99,
    101, 46,  111, 98,  106, 101, 99,  116, 91,  37,  49,  50,  53,  52,  50,  93,  10,  109, 121, 95,  109, 101, 109,
    98,  101, 114, 50,  254, 158, 150, 8,   0,   4,   49,  1,   3,   101, 108, 49,  1,   0,   5,   0,   0,   0,   0,
    0,   0,   240, 63,  0,   0,   0,   0,   0,   0,   0,   64,  0,   0,   0,   0,   0,   0,   8,   64,  0,   0,   0,
    0,   0,   0,   16,  64,  0,   0,   0,   0,   0,   0,   20,  64,  27,  1,   3,   101, 108, 50,  11,  0,   18,  115,
    111, 109, 101, 32,  114, 97,  110, 100, 111, 109, 32,  115, 116, 114, 105, 110, 103, 253, 25,  2,   9,   3,   101,
    108, 51,  101, 0,   16,  115, 111, 109, 101, 95,  115, 116, 114, 117, 99,  116, 95,  116, 121, 112, 101, 14,  49,
    1,   11,  115, 117, 98,  95,  100, 111, 117, 98,  108, 101, 115, 1,   0,   4,   0,   0,   0,   0,   0,   0,   240,
    63,  0,   0,   0,   0,   0,   0,   0,   64,  0,   0,   0,   0,   0,   0,   8,   64,  0,   0,   0,   0,   0,   0,
    16,  64,  33,  1,   11,  115, 117, 98,  95,  115, 105, 110, 103, 108, 101, 115, 2,   0,   4,   0,   0,   128, 63,
    0,   0,   0,   64,  0,   0,   64,  64,  0,   0,   128, 64,  18,  1,   8,   115, 117, 98,  95,  105, 110, 116, 56,
    3,   0,   4,   1,   2,   3,   4,   19,  1,   9,   115, 117, 98,  95,  117, 105, 110, 116, 56,  4,   0,   4,   1,
    2,   3,   4,   23,  1,   9,   115, 117, 98,  95,  105, 110, 116, 49,  54,  5,   0,   4,   1,   0,   2,   0,   3,
    0,   4,   0,   24,  1,   10,  115, 117, 98,  95,  117, 105, 110, 116, 49,  54,  6,   0,   4,   1,   0,   2,   0,
    3,   0,   4,   0,   31,  1,   9,   115, 117, 98,  95,  105, 110, 116, 51,  50,  7,   0,   4,   1,   0,   0,   0,
    2,   0,   0,   0,   3,   0,   0,   0,   4,   0,   0,   0,   32,  1,   10,  115, 117, 98,  95,  117, 105, 110, 116,
    51,  50,  8,   0,   4,   1,   0,   0,   0,   2,   0,   0,   0,   3,   0,   0,   0,   4,   0,   0,   0,   47,  1,
    9,   115, 117, 98,  95,  105, 110, 116, 54,  52,  9,   0,   4,   1,   0,   0,   0,   0,   0,   0,   0,   2,   0,
    0,   0,   0,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,   4,   0,   0,   0,   0,   0,   0,   0,   48,
    1,   10,  115, 117, 98,  95,  117, 105, 110, 116, 54,  52,  10,  0,   4,   1,   0,   0,   0,   0,   0,   0,   0,
    2,   0,   0,   0,   0,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,   4,   0,   0,   0,   0,   0,   0,
    0,   37,  1,   10,  115, 117, 98,  95,  115, 116, 114, 105, 110, 103, 11,  0,   21,  97,  110, 111, 116, 104, 101,
    114, 32,  114, 97,  110, 100, 111, 109, 32,  115, 116, 114, 105, 110, 103, 81,  1,   11,  115, 117, 98,  95,  99,
    100, 111, 117, 98,  108, 101, 12,  0,   4,   0,   0,   0,   0,   0,   0,   240, 63,  0,   0,   0,   0,   0,   0,
    36,  64,  0,   0,   0,   0,   0,   0,   0,   64,  0,   0,   0,   0,   0,   0,   52,  64,  0,   0,   0,   0,   0,
    0,   8,   64,  0,   0,   0,   0,   0,   0,   62,  64,  0,   0,   0,   0,   0,   0,   16,  64,  0,   0,   0,   0,
    0,   0,   68,  64,  49,  1,   11,  115, 117, 98,  95,  99,  115, 105, 110, 103, 108, 101, 13,  0,   4,   0,   0,
    128, 63,  0,   0,   32,  65,  0,   0,   0,   64,  0,   0,   160, 65,  0,   0,   64,  64,  0,   0,   240, 65,  0,
    0,   128, 64,  0,   0,   32,  66,  18,  1,   8,   115, 117, 98,  95,  98,  111, 111, 108, 14,  0,   4,   1,   1,
    0,   1,   72,  1,   58,  101, 108, 52,  95,  105, 115, 95,  97,  95,  118, 101, 114, 121, 95,  108, 111, 110, 103,
    95,  110, 97,  109, 101, 95,  105, 116, 95,  107, 101, 101, 112, 115, 95,  103, 111, 105, 110, 103, 95,  97,  110,
    100, 95,  103, 111, 105, 110, 103, 95,  97,  110, 100, 95,  103, 111, 105, 110, 103, 1,   0,   1,   92,  143, 194,
    245, 40,  44,  69,  64,  7,   5,   1,   0,   0,   0,   0};

static int cmp_double(const robotraconteurlite_double* a, const robotraconteurlite_double* b,
                      robotraconteurlite_size_t len)
{
    robotraconteurlite_size_t i = 0;
    for (i = 0; i < len; i++)
    {
        if (a[i] != b[i])
        {
            return 1;
        }
    }
    return 0;
}

static int cmp_float(const robotraconteurlite_single* a, const robotraconteurlite_single* b,
                     robotraconteurlite_size_t len)
{
    robotraconteurlite_size_t i = 0;
    for (i = 0; i < len; i++)
    {
        if (fabs((robotraconteurlite_double)(a[i] - b[i])) > 1e-4)
        {
            return 1;
        }
    }
    return 0;
}

static int cmp_cdouble(const struct robotraconteurlite_cdouble* a, const struct robotraconteurlite_cdouble* b,
                       robotraconteurlite_size_t len)
{
    robotraconteurlite_size_t i = 0;
    for (i = 0; i < len; i++)
    {
        if (a[i].real != b[i].real || a[i].imag != b[i].imag)
        {
            return 1;
        }
    }
    return 0;
}

static int cmp_csingle(const struct robotraconteurlite_csingle* a, const struct robotraconteurlite_csingle* b,
                       robotraconteurlite_size_t len)
{
    robotraconteurlite_size_t i = 0;
    for (i = 0; i < len; i++)
    {
        if (fabs((robotraconteurlite_double)(a[i].real - b[i].real)) > 1e-4 ||
            fabs((robotraconteurlite_double)(a[i].imag - b[i].imag)) > 1e-4)
        {
            return 1;
        }
    }
    return 0;
}

void robotraconteurlite_message_run_reader_basictest(robotraconteurlite_byte* message_bytes,
                                                     robotraconteurlite_size_t message_bytes_len,
                                                     robotraconteurlite_u16 message_ver)
{
    struct robotraconteurlite_buffer buffer1;
    struct robotraconteurlite_buffer_vec buffer;

    struct robotraconteurlite_message_reader reader;

    struct robotraconteurlite_message_header header;

    char sender_nodename[256];
    char receiver_nodename[256];
    char extended[256];

    struct robotraconteurlite_message_header header2;

    const char* sender_nodename_expected_data = "sender_node";
    struct robotraconteurlite_const_string sender_nodename_expected;
    const char* receiver_nodename_expected_data = "recv_node";
    struct robotraconteurlite_const_string receiver_nodename_expected;
    const char* extended_expected_data = "extended_data\nblah blah";
    struct robotraconteurlite_const_string extended_expected;
    struct robotraconteurlite_messageentry_reader entry_reader;

    buffer1.data = message_bytes;
    buffer1.len = message_bytes_len;
    buffer.buffer_vec = &buffer1;
    buffer.buffer_vec_cnt = 1;

    robotraconteurlite_string_from_c_str(sender_nodename_expected_data, &sender_nodename_expected);
    robotraconteurlite_string_from_c_str(receiver_nodename_expected_data, &receiver_nodename_expected);
    robotraconteurlite_string_from_c_str(extended_expected_data, &extended_expected);

    assert_return_code(robotraconteurlite_message_reader_init(&reader, &buffer, 0, message_bytes_len), 0);

    /* Read message header */

    (void)memset(&header, 0, sizeof(header));
    assert_return_code(robotraconteurlite_message_reader_read_header(&reader, &header), 0);

    switch (message_ver)
    {
    case 2:
        assert_true(header.message_size == 1118);
        assert_true(header.message_version == 2);
        assert_true(header.header_size == 107);
        break;
    case 4:
        assert_true(header.message_size == 909 || header.message_size == 939);
        assert_true(header.message_version == 4);
        assert_true(header.header_size == 105 || header.header_size == 109);
        break;
    default:
        assert_true(0);
        break;
    }
    assert_true(header.sender_endpoint == 4164376647);
    assert_true(header.receiver_endpoint == 2805032049);
    assert_true(header.sender_nodename.len == 11);
    assert_true(header.receiver_nodename.len == 9);
    assert_true(header.metadata.len == 23);
    assert_true(header.entry_count == 3);
    assert_true(header.message_id == 10);
    assert_true(header.message_res_id == 20);

    header2.sender_nodename.data = sender_nodename;
    header2.sender_nodename.len = sizeof(sender_nodename);
    header2.receiver_nodename.data = receiver_nodename;
    header2.receiver_nodename.len = sizeof(receiver_nodename);
    header2.metadata.data = extended;
    header2.metadata.len = sizeof(extended);

    assert_return_code(robotraconteurlite_message_reader_read_header(&reader, &header2), 0);

    switch (message_ver)
    {
    case 2:
        assert_true(header.message_size == 1118);
        assert_true(header.message_version == 2);
        assert_true(header.header_size == 107);
        break;
    case 4:
        assert_true(header.message_size == 909 || header.message_size == 939);
        assert_true(header.message_version == 4);
        assert_true(header.header_size == 105 || header.header_size == 109);
        break;
    default:
        assert_true(0);
        break;
    }
    assert_true(header2.sender_endpoint == 4164376647);
    assert_true(header2.receiver_endpoint == 2805032049);
    assert_true(header2.sender_nodename.len == 11);
    assert_true(header2.receiver_nodename.len == 9);
    assert_true(header2.metadata.len == 23);
    assert_true(header2.entry_count == 3);
    assert_true(header2.message_id == 10);
    assert_true(header2.message_res_id == 20);

    assert_true(robotraconteurlite_string_cmp_mutable(&header2.sender_nodename, &sender_nodename_expected) == 0);
    assert_true(robotraconteurlite_string_cmp_mutable(&header2.receiver_nodename, &receiver_nodename_expected) == 0);
    assert_true(robotraconteurlite_string_cmp_mutable(&header2.metadata, &extended_expected) == 0);

    /* Read message entries */
    assert_return_code(robotraconteurlite_message_reader_begin_read_entries(&reader, &entry_reader), 0);

    {
        struct robotraconteurlite_messageentry_header entry_header;
        (void)memset(&entry_header, 0, sizeof(entry_header));
        assert_return_code(robotraconteurlite_messageentry_reader_read_header(&entry_reader, &entry_header), 0);

        switch (message_ver)
        {
        case 2:
            assert_true(entry_header.entry_size == 70);
            break;
        case 4:
            assert_true(entry_header.entry_size == 63 || entry_header.entry_size == 71);
            break;
        default:
            assert_true(0);
            break;
        }
        assert_true(entry_header.entry_type = ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PROPERTYGETRES);
        assert_true(entry_header.request_id == 395728);
        assert_true(entry_header.service_path.len == 25);
        assert_true(entry_header.member_name.len == 9);
        assert_true(entry_header.error == ROBOTRACONTEURLITE_MESSAGEERRORTYPE_NULLVALUE);
        assert_true(entry_header.metadata.len == 14);
        assert_true(entry_header.element_count == 0);
    }

    {
        const char* service_path_expected_data = "my.service.object[%28abc]";
        struct robotraconteurlite_const_string service_path_expected;
        const char* member_name_expected_data = "my_member";
        struct robotraconteurlite_const_string member_name_expected;
        const char* entry_extended_expected_data = "more\nblah\nblah";
        struct robotraconteurlite_const_string entry_extended_expected;

        char service_path_data[256];
        char member_name_data[256];
        char entry_extended_data[256];

        struct robotraconteurlite_messageentry_header entry_header2;

        entry_header2.service_path.data = service_path_data;
        entry_header2.service_path.len = sizeof(service_path_data);
        entry_header2.member_name.data = member_name_data;
        entry_header2.member_name.len = sizeof(member_name_data);
        entry_header2.metadata.data = entry_extended_data;
        entry_header2.metadata.len = sizeof(entry_extended_data),

        robotraconteurlite_string_from_c_str(service_path_expected_data, &service_path_expected);
        robotraconteurlite_string_from_c_str(member_name_expected_data, &member_name_expected);
        robotraconteurlite_string_from_c_str(entry_extended_expected_data, &entry_extended_expected);

        assert_return_code(robotraconteurlite_messageentry_reader_read_header(&entry_reader, &entry_header2), 0);

        assert_true(robotraconteurlite_string_cmp_mutable(&entry_header2.service_path, &service_path_expected) == 0);
        assert_true(robotraconteurlite_string_cmp_mutable(&entry_header2.member_name, &member_name_expected) == 0);
        assert_true(robotraconteurlite_string_cmp_mutable(&entry_header2.metadata, &entry_extended_expected) == 0);
        switch (message_ver)
        {
        case 2:
            assert_true(entry_header2.entry_size == 70);
            break;
        case 4:
            assert_true(entry_header2.entry_size == 63 || entry_header2.entry_size == 71);
            break;
        default:
            assert_true(0);
            break;
        }

        assert_true(entry_header2.entry_type = ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PROPERTYGETRES);
        assert_true(entry_header2.request_id == 395728);
        assert_true(entry_header2.service_path.len == 25);
        assert_true(entry_header2.member_name.len == 9);
        assert_true(entry_header2.error == ROBOTRACONTEURLITE_MESSAGEERRORTYPE_NULLVALUE);
        assert_true(entry_header2.metadata.len == 14);
        assert_true(entry_header2.element_count == 0);
    }
    {
        struct robotraconteurlite_messageentry_header entry_header3;
        assert_return_code(robotraconteurlite_messageentry_reader_move_next(&entry_reader), 0);
        (void)memset(&entry_header3, 0, sizeof(entry_header3));
        assert_return_code(robotraconteurlite_messageentry_reader_read_header(&entry_reader, &entry_header3), 0);
        switch (message_ver)
        {
        case 2:
            assert_true(entry_header3.entry_size == 919);
            break;
        case 4:
            assert_true(entry_header3.entry_size == 734 || entry_header3.entry_size == 746);
            break;
        default:
            assert_true(0);
            break;
        }
        assert_true(entry_header3.entry_type == 1122);
        assert_true(entry_header3.request_id == 562846);
        assert_true(entry_header3.service_path.len == 25);
        assert_true(entry_header3.member_name.len == 10);
        assert_true(entry_header3.metadata.len == 0);
        assert_true(entry_header3.element_count == 4);
    }

    {
        struct robotraconteurlite_messageelement_reader element_reader;
        struct robotraconteurlite_messageelement_header element_header;
        assert_return_code(robotraconteurlite_messageentry_reader_begin_read_elements(&entry_reader, &element_reader),
                           0);
        (void)memset(&element_header, 0, sizeof(element_header));
        assert_return_code(robotraconteurlite_messageelement_reader_read_header(&element_reader, &element_header), 0);
    }

    {
        struct robotraconteurlite_messageelement_reader element_reader2;
        const char* element2_name_data = "el2";
        struct robotraconteurlite_const_string element2_name;
        struct robotraconteurlite_messageelement_header element_header2;
        char element_name2_header_data[256];

        robotraconteurlite_string_from_c_str(element2_name_data, &element2_name);

        assert_return_code(
            robotraconteurlite_messageentry_reader_find_element(&entry_reader, &element2_name, &element_reader2), 0);

        (void)memset(&element_header2, 0, sizeof(element_header2));
        element_header2.element_name.data = element_name2_header_data;
        element_header2.element_name.len = sizeof(element_name2_header_data);
        assert_return_code(robotraconteurlite_messageelement_reader_read_header(&element_reader2, &element_header2), 0);
        assert_true(robotraconteurlite_string_cmp_mutable(&element_header2.element_name, &element2_name) == 0);
        switch (message_ver)
        {
        case 2:
            assert_true(element_header2.element_size == 37);
            break;
        case 4:
            assert_true(element_header2.element_size == 27 || element_header2.element_size == 26);
            break;
        default:
            assert_true(0);
            break;
        }

        {

            char element2_str_data_dat[256];
            struct robotraconteurlite_string element2_str_data;
            const char* element2_str_data_expected_dat = "some random string";
            struct robotraconteurlite_const_string element2_str_data_expected;
            element2_str_data.data = element2_str_data_dat;
            element2_str_data.len = sizeof(element2_str_data_dat);
            assert_return_code(
                robotraconteurlite_messageelement_reader_read_data_string(&element_reader2, &element2_str_data), 0);

            robotraconteurlite_string_from_c_str(element2_str_data_expected_dat, &element2_str_data_expected);
            assert_true(robotraconteurlite_string_cmp_mutable(&element2_str_data, &element2_str_data_expected) == 0);
        }
    }

    {
        struct robotraconteurlite_messageelement_reader element_reader3;
        const char* element3_name_data = "el3";
        struct robotraconteurlite_const_string element3_name;
        robotraconteurlite_string_from_c_str(element3_name_data, &element3_name);
        assert_return_code(
            robotraconteurlite_messageentry_reader_find_element(&entry_reader, &element3_name, &element_reader3), 0);

        {
            /* nested sub_doubles */
            struct robotraconteurlite_messageelement_reader nested_element1_reader;
            const char* nested_element1_name_data = "sub_doubles";
            struct robotraconteurlite_const_string nested_element1_name;
            robotraconteurlite_double nested_element1_expected_data[] = {1, 2, 3, 4};
            robotraconteurlite_double nested_element1_read_data_storage[4];
            struct robotraconteurlite_array_double nested_element1_read_data;
            robotraconteurlite_string_from_c_str(nested_element1_name_data, &nested_element1_name);
            nested_element1_read_data.data = nested_element1_read_data_storage;
            nested_element1_read_data.len =
                sizeof(nested_element1_read_data_storage) / sizeof(robotraconteurlite_double);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element(
                                   &element_reader3, &nested_element1_name, &nested_element1_reader),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element_verify_array(
                                   &element_reader3, &nested_element1_name, &nested_element1_reader,
                                   ROBOTRACONTEURLITE_DATATYPE_DOUBLE, 4, 0),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_read_data_double_array(
                                   &nested_element1_reader, &nested_element1_read_data),
                               0);
            assert_true(nested_element1_read_data.len == 4);
            assert_true(cmp_double(nested_element1_read_data_storage, nested_element1_expected_data, 4) == 0);
        }

        {
            /* nested sub_singles */
            struct robotraconteurlite_messageelement_reader nested_element2_reader;
            const char* nested_element2_name_data = "sub_singles";
            struct robotraconteurlite_const_string nested_element2_name;
            robotraconteurlite_single nested_element2_expected_data[] = {1, 2, 3, 4};
            robotraconteurlite_single nested_element2_read_data_storage[4];
            struct robotraconteurlite_array_single nested_element2_read_data;
            robotraconteurlite_string_from_c_str(nested_element2_name_data, &nested_element2_name);
            nested_element2_read_data.data = nested_element2_read_data_storage;
            nested_element2_read_data.len =
                sizeof(nested_element2_read_data_storage) / sizeof(robotraconteurlite_single);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element(
                                   &element_reader3, &nested_element2_name, &nested_element2_reader),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element_verify_array(
                                   &element_reader3, &nested_element2_name, &nested_element2_reader,
                                   ROBOTRACONTEURLITE_DATATYPE_SINGLE, 4, 0),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_read_data_single_array(
                                   &nested_element2_reader, &nested_element2_read_data),
                               0);
            assert_true(nested_element2_read_data.len == 4);
            assert_true(cmp_float(nested_element2_read_data_storage, nested_element2_expected_data, 4) == 0);
        }

        {
            /* nested sub_int8 */
            struct robotraconteurlite_messageelement_reader nested_element3_reader;
            const char* nested_element3_name_data = "sub_int8";
            struct robotraconteurlite_const_string nested_element3_name;
            robotraconteurlite_i8 nested_element3_expected_data[] = {1, 2, 3, 4};
            robotraconteurlite_i8 nested_element3_read_data_storage[4];
            struct robotraconteurlite_array_int8 nested_element3_read_data;
            robotraconteurlite_string_from_c_str(nested_element3_name_data, &nested_element3_name);
            nested_element3_read_data.data = nested_element3_read_data_storage;
            nested_element3_read_data.len = sizeof(nested_element3_read_data_storage) / sizeof(robotraconteurlite_i8);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element(
                                   &element_reader3, &nested_element3_name, &nested_element3_reader),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element_verify_array(
                                   &element_reader3, &nested_element3_name, &nested_element3_reader,
                                   ROBOTRACONTEURLITE_DATATYPE_INT8, 4, 0),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_read_data_int8_array(
                                   &nested_element3_reader, &nested_element3_read_data),
                               0);
            assert_true(nested_element3_read_data.len == 4);
            assert_true(memcmp(nested_element3_read_data_storage, nested_element3_expected_data,
                               sizeof(nested_element3_expected_data)) == 0);
        }

        {
            /* nested sub_uint8 */
            struct robotraconteurlite_messageelement_reader nested_element4_reader;
            const char* nested_element4_name_data = "sub_uint8";
            struct robotraconteurlite_const_string nested_element4_name;
            robotraconteurlite_u8 nested_element4_expected_data[] = {1, 2, 3, 4};
            robotraconteurlite_u8 nested_element4_read_data_storage[4];
            struct robotraconteurlite_array_uint8 nested_element4_read_data;
            robotraconteurlite_string_from_c_str(nested_element4_name_data, &nested_element4_name);
            nested_element4_read_data.data = nested_element4_read_data_storage;
            nested_element4_read_data.len = sizeof(nested_element4_read_data_storage) / sizeof(robotraconteurlite_u8);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element(
                                   &element_reader3, &nested_element4_name, &nested_element4_reader),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element_verify_array(
                                   &element_reader3, &nested_element4_name, &nested_element4_reader,
                                   ROBOTRACONTEURLITE_DATATYPE_UINT8, 4, 0),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_read_data_uint8_array(
                                   &nested_element4_reader, &nested_element4_read_data),
                               0);
            assert_true(nested_element4_read_data.len == 4);
            assert_true(memcmp(nested_element4_read_data_storage, nested_element4_expected_data,
                               sizeof(nested_element4_expected_data)) == 0);
        }

        {
            /* nested sub_int16 */
            struct robotraconteurlite_messageelement_reader nested_element5_reader;
            const char* nested_element5_name_data = "sub_int16";
            struct robotraconteurlite_const_string nested_element5_name;
            robotraconteurlite_i16 nested_element5_expected_data[] = {1, 2, 3, 4};
            robotraconteurlite_i16 nested_element5_read_data_storage[4];
            struct robotraconteurlite_array_int16 nested_element5_read_data;
            robotraconteurlite_string_from_c_str(nested_element5_name_data, &nested_element5_name);
            nested_element5_read_data.data = nested_element5_read_data_storage;
            nested_element5_read_data.len = sizeof(nested_element5_read_data_storage) / sizeof(robotraconteurlite_i16);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element(
                                   &element_reader3, &nested_element5_name, &nested_element5_reader),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element_verify_array(
                                   &element_reader3, &nested_element5_name, &nested_element5_reader,
                                   ROBOTRACONTEURLITE_DATATYPE_INT16, 4, 0),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_read_data_int16_array(
                                   &nested_element5_reader, &nested_element5_read_data),
                               0);
            assert_true(nested_element5_read_data.len == 4);
            assert_true(memcmp(nested_element5_read_data_storage, nested_element5_expected_data,
                               sizeof(nested_element5_expected_data)) == 0);
        }

        {
            /* nested sub_uint16 */
            struct robotraconteurlite_messageelement_reader nested_element6_reader;
            const char* nested_element6_name_data = "sub_uint16";
            struct robotraconteurlite_const_string nested_element6_name;
            robotraconteurlite_u16 nested_element6_expected_data[] = {1, 2, 3, 4};
            robotraconteurlite_u16 nested_element6_read_data_storage[4];
            struct robotraconteurlite_array_uint16 nested_element6_read_data;
            robotraconteurlite_string_from_c_str(nested_element6_name_data, &nested_element6_name);
            nested_element6_read_data.data = nested_element6_read_data_storage;
            nested_element6_read_data.len = sizeof(nested_element6_read_data_storage) / sizeof(robotraconteurlite_u16);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element(
                                   &element_reader3, &nested_element6_name, &nested_element6_reader),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element_verify_array(
                                   &element_reader3, &nested_element6_name, &nested_element6_reader,
                                   ROBOTRACONTEURLITE_DATATYPE_UINT16, 4, 0),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_read_data_uint16_array(
                                   &nested_element6_reader, &nested_element6_read_data),
                               0);
            assert_true(nested_element6_read_data.len == 4);
            assert_true(memcmp(nested_element6_read_data_storage, nested_element6_expected_data,
                               sizeof(nested_element6_expected_data)) == 0);
        }

        {
            /* nested sub_int32 */
            struct robotraconteurlite_messageelement_reader nested_element7_reader;
            const char* nested_element7_name_data = "sub_int32";
            struct robotraconteurlite_const_string nested_element7_name;
            robotraconteurlite_i32 nested_element7_expected_data[] = {1, 2, 3, 4};
            robotraconteurlite_i32 nested_element7_read_data_storage[4];
            struct robotraconteurlite_array_int32 nested_element7_read_data;
            robotraconteurlite_string_from_c_str(nested_element7_name_data, &nested_element7_name);
            nested_element7_read_data.data = nested_element7_read_data_storage;
            nested_element7_read_data.len = sizeof(nested_element7_read_data_storage) / sizeof(robotraconteurlite_i32);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element(
                                   &element_reader3, &nested_element7_name, &nested_element7_reader),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element_verify_array(
                                   &element_reader3, &nested_element7_name, &nested_element7_reader,
                                   ROBOTRACONTEURLITE_DATATYPE_INT32, 4, 0),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_read_data_int32_array(
                                   &nested_element7_reader, &nested_element7_read_data),
                               0);
            assert_true(nested_element7_read_data.len == 4);
            assert_true(memcmp(nested_element7_read_data_storage, nested_element7_expected_data,
                               sizeof(nested_element7_expected_data)) == 0);
        }

        {
            /* nested sub_uint32 */
            struct robotraconteurlite_messageelement_reader nested_element8_reader;
            const char* nested_element8_name_data = "sub_uint32";
            struct robotraconteurlite_const_string nested_element8_name;
            robotraconteurlite_u32 nested_element8_expected_data[] = {1, 2, 3, 4};
            robotraconteurlite_u32 nested_element8_read_data_storage[4];
            struct robotraconteurlite_array_uint32 nested_element8_read_data;
            robotraconteurlite_string_from_c_str(nested_element8_name_data, &nested_element8_name);
            nested_element8_read_data.data = nested_element8_read_data_storage;
            nested_element8_read_data.len = sizeof(nested_element8_read_data_storage) / sizeof(robotraconteurlite_u32);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element(
                                   &element_reader3, &nested_element8_name, &nested_element8_reader),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element_verify_array(
                                   &element_reader3, &nested_element8_name, &nested_element8_reader,
                                   ROBOTRACONTEURLITE_DATATYPE_UINT32, 4, 0),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_read_data_uint32_array(
                                   &nested_element8_reader, &nested_element8_read_data),
                               0);
            assert_true(nested_element8_read_data.len == 4);
            assert_true(memcmp(nested_element8_read_data_storage, nested_element8_expected_data,
                               sizeof(nested_element8_expected_data)) == 0);
        }

        {
            /* nested sub_int64 */
            struct robotraconteurlite_messageelement_reader nested_element9_reader;
            const char* nested_element9_name_data = "sub_int64";
            struct robotraconteurlite_const_string nested_element9_name;
            robotraconteurlite_i64 nested_element9_expected_data[] = {1, 2, 3, 4};
            robotraconteurlite_i64 nested_element9_read_data_storage[4];
            struct robotraconteurlite_array_int64 nested_element9_read_data;
            robotraconteurlite_string_from_c_str(nested_element9_name_data, &nested_element9_name);
            nested_element9_read_data.data = nested_element9_read_data_storage;
            nested_element9_read_data.len = sizeof(nested_element9_read_data_storage) / sizeof(robotraconteurlite_i64);

            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element(
                                   &element_reader3, &nested_element9_name, &nested_element9_reader),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element_verify_array(
                                   &element_reader3, &nested_element9_name, &nested_element9_reader,
                                   ROBOTRACONTEURLITE_DATATYPE_INT64, 4, 0),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_read_data_int64_array(
                                   &nested_element9_reader, &nested_element9_read_data),
                               0);
            assert_true(nested_element9_read_data.len == 4);
            assert_true(memcmp(nested_element9_read_data_storage, nested_element9_expected_data,
                               sizeof(nested_element9_expected_data)) == 0);
        }

        {
            /* nested sub_uint64 */
            struct robotraconteurlite_messageelement_reader nested_element10_reader;
            const char* nested_element10_name_data = "sub_uint64";
            struct robotraconteurlite_const_string nested_element10_name;
            robotraconteurlite_u64 nested_element10_expected_data[] = {1, 2, 3, 4};
            robotraconteurlite_u64 nested_element10_read_data_storage[4];
            struct robotraconteurlite_array_uint64 nested_element10_read_data;
            robotraconteurlite_string_from_c_str(nested_element10_name_data, &nested_element10_name);
            nested_element10_read_data.data = nested_element10_read_data_storage;
            nested_element10_read_data.len =
                sizeof(nested_element10_read_data_storage) / sizeof(robotraconteurlite_u64);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element(
                                   &element_reader3, &nested_element10_name, &nested_element10_reader),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element_verify_array(
                                   &element_reader3, &nested_element10_name, &nested_element10_reader,
                                   ROBOTRACONTEURLITE_DATATYPE_UINT64, 4, 0),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_read_data_uint64_array(
                                   &nested_element10_reader, &nested_element10_read_data),
                               0);
            assert_true(nested_element10_read_data.len == 4);
            assert_true(memcmp(nested_element10_read_data_storage, nested_element10_expected_data,
                               sizeof(nested_element10_expected_data)) == 0);
        }

        {
            /* nested sub_string */
            struct robotraconteurlite_messageelement_reader nested_element11_reader;
            const char* nested_element11_name_data = "sub_string";
            struct robotraconteurlite_const_string nested_element11_name;

            char nested_element11_expected_data[] = "another random string";
            char nested_element11_read_data_storage[22];
            struct robotraconteurlite_string nested_element11_read_data;
            robotraconteurlite_string_from_c_str(nested_element11_name_data, &nested_element11_name);
            nested_element11_read_data.data = nested_element11_read_data_storage;
            nested_element11_read_data.len = sizeof(nested_element11_read_data_storage) / sizeof(char);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element(
                                   &element_reader3, &nested_element11_name, &nested_element11_reader),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element_verify_string(
                                   &element_reader3, &nested_element11_name, &nested_element11_reader, 32),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_read_data_string(&nested_element11_reader,
                                                                                         &nested_element11_read_data),
                               0);
            assert_true(nested_element11_read_data.len == strlen(nested_element11_expected_data));
            assert_true(memcmp(nested_element11_read_data_storage, nested_element11_expected_data,
                               strlen(nested_element11_expected_data)) == 0);
        }

        {
            /* nested sub_cdoubles */
            struct robotraconteurlite_messageelement_reader nested_element12_reader;
            const char* nested_element12_name_data = "sub_cdouble";
            struct robotraconteurlite_const_string nested_element12_name;
            struct robotraconteurlite_cdouble nested_element12_expected_data[] = {{1, 10}, {2, 20}, {3, 30}, {4, 40}};
            struct robotraconteurlite_cdouble nested_element12_read_data_storage[40];
            struct robotraconteurlite_array_cdouble nested_element12_read_data;
            robotraconteurlite_string_from_c_str(nested_element12_name_data, &nested_element12_name);
            nested_element12_read_data.data = nested_element12_read_data_storage;
            nested_element12_read_data.len =
                sizeof(nested_element12_read_data_storage) / sizeof(struct robotraconteurlite_cdouble);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element(
                                   &element_reader3, &nested_element12_name, &nested_element12_reader),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element_verify_array(
                                   &element_reader3, &nested_element12_name, &nested_element12_reader,
                                   ROBOTRACONTEURLITE_DATATYPE_CDOUBLE, 4, 0),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_read_data_cdouble_array(
                                   &nested_element12_reader, &nested_element12_read_data),
                               0);
            assert_true(nested_element12_read_data.len == 4);
            assert_true(cmp_cdouble(nested_element12_read_data_storage, nested_element12_expected_data, 4) == 0);
        }

        {
            /* nested sub_csingles */
            struct robotraconteurlite_messageelement_reader nested_element13_reader;
            const char* nested_element13_name_data = "sub_csingle";
            struct robotraconteurlite_const_string nested_element13_name;
            struct robotraconteurlite_csingle nested_element13_expected_data[] = {{1, 10}, {2, 20}, {3, 30}, {4, 40}};
            struct robotraconteurlite_csingle nested_element13_read_data_storage[40];
            struct robotraconteurlite_array_csingle nested_element13_read_data;
            robotraconteurlite_string_from_c_str(nested_element13_name_data, &nested_element13_name);
            nested_element13_read_data.data = nested_element13_read_data_storage;
            nested_element13_read_data.len =
                sizeof(nested_element13_read_data_storage) / sizeof(struct robotraconteurlite_csingle);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element(
                                   &element_reader3, &nested_element13_name, &nested_element13_reader),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element_verify_array(
                                   &element_reader3, &nested_element13_name, &nested_element13_reader,
                                   ROBOTRACONTEURLITE_DATATYPE_CSINGLE, 4, 0),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_read_data_csingle_array(
                                   &nested_element13_reader, &nested_element13_read_data),
                               0);
            assert_true(nested_element13_read_data.len == 4);
            assert_true(cmp_csingle(nested_element13_read_data_storage, nested_element13_expected_data, 4) == 0);
        }

        {
            /* nested sub_bool */
            struct robotraconteurlite_messageelement_reader nested_element14_reader;
            const char* nested_element14_name_data = "sub_bool";
            robotraconteurlite_u8 nested_element14_expected_data[] = {1, 1, 0, 1};
            robotraconteurlite_u8 nested_element14_read_data_storage[4];
            struct robotraconteurlite_array_bool nested_element14_read_data;
            struct robotraconteurlite_const_string nested_element14_name;
            nested_element14_read_data.data = (struct robotraconteurlite_bool*)nested_element14_read_data_storage;
            nested_element14_read_data.len = sizeof(nested_element14_read_data_storage) / sizeof(robotraconteurlite_u8);
            robotraconteurlite_string_from_c_str(nested_element14_name_data, &nested_element14_name);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element(
                                   &element_reader3, &nested_element14_name, &nested_element14_reader),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_find_nested_element_verify_array(
                                   &element_reader3, &nested_element14_name, &nested_element14_reader,
                                   ROBOTRACONTEURLITE_DATATYPE_BOOL, 4, 0),
                               0);
            assert_return_code(robotraconteurlite_messageelement_reader_read_data_bool_array(
                                   &nested_element14_reader, &nested_element14_read_data),
                               0);
            assert_true(nested_element14_read_data.len == 4);
            assert_true(memcmp(nested_element14_read_data_storage, nested_element14_expected_data,
                               sizeof(nested_element14_expected_data)) == 0);
        }
    }

    {
        struct robotraconteurlite_messageelement_reader element_reader4;
        const char* element4_name_data = "el4_is_a_very_long_name_it_keeps_going_and_going_and_going";
        struct robotraconteurlite_const_string element4_name;
        robotraconteurlite_string_from_c_str(element4_name_data, &element4_name);
        assert_return_code(robotraconteurlite_messageentry_reader_find_element_verify_scalar(
                               &entry_reader, &element4_name, &element_reader4, ROBOTRACONTEURLITE_DATATYPE_DOUBLE),
                           0);

        {
            robotraconteurlite_double el4_array_data[16];
            struct robotraconteurlite_array_double el4_array;
            el4_array.data = el4_array_data;
            el4_array.len = sizeof(el4_array_data) / sizeof(robotraconteurlite_double);
            assert_return_code(
                robotraconteurlite_messageelement_reader_read_data_double_array(&element_reader4, &el4_array), 0);
        }

        {
            robotraconteurlite_double el4_scalar_data = 0.0;
            assert_return_code(
                robotraconteurlite_messageelement_reader_read_data_double(&element_reader4, &el4_scalar_data), 0);
        }

        assert_return_code(robotraconteurlite_messageentry_reader_move_next(&entry_reader), 0);
        {
            struct robotraconteurlite_messageentry_header entry_header4;
            (void)memset(&entry_header4, 0, sizeof(entry_header4));
            assert_return_code(robotraconteurlite_messageentry_reader_read_header(&entry_reader, &entry_header4), 0);
            switch (message_ver)
            {
            case 2:
                assert_true(entry_header4.entry_size == 22);
                break;
            case 4:
                assert_true(entry_header4.entry_size == 7 || entry_header4.entry_size == 13);
                break;
            default:
                assert_true(0);
                break;
            }
            assert_true(entry_header4.entry_type == 1);
            assert_true(entry_header4.request_id == 0);
            assert_true(entry_header4.service_path.len == 0);
            assert_true(entry_header4.member_name.len == 0);
            assert_true(entry_header4.metadata.len == 0);
            assert_true(entry_header4.element_count == 0);
        }

        assert_true(robotraconteurlite_messageentry_reader_find_element_verify_scalar(
                        &entry_reader, &element4_name, &element_reader4, ROBOTRACONTEURLITE_DATATYPE_SINGLE) < 0);
    }

    assert_true(robotraconteurlite_messageentry_reader_move_next(&entry_reader) == ROBOTRACONTEURLITE_ERROR_NO_MORE);
    {
        struct robotraconteurlite_messageelement_reader element_reader5;
        const char* element5_name_data = "el5";
        struct robotraconteurlite_const_string element5_name;
        robotraconteurlite_string_from_c_str(element5_name_data, &element5_name);
        assert_true(
            robotraconteurlite_messageentry_reader_find_element(&entry_reader, &element5_name, &element_reader5) ==
            ROBOTRACONTEURLITE_ERROR_MESSAGEELEMENT_NOT_FOUND);
    }

    assert_true(1);
}

void robotraconteurlite_message_reader_basictest(void** state)
{
    ROBOTRACONTEURLITE_UNUSED(state);
    robotraconteurlite_message_run_reader_basictest(message2, sizeof(message2), 2);
}

void robotraconteurlite_message4_reader_basictest(void** state)
{
    ROBOTRACONTEURLITE_UNUSED(state);
    robotraconteurlite_message_run_reader_basictest(message4, sizeof(message4), 4);
}

void robotraconteurlite_message_run_writer_basictest(robotraconteurlite_byte* buffer_bytes,
                                                     robotraconteurlite_size_t* buffer_bytes_len,
                                                     robotraconteurlite_u16 message_ver,
                                                     robotraconteurlite_u8 message_flags_mask)
{

    struct robotraconteurlite_buffer buffer1;
    struct robotraconteurlite_buffer_vec buffer;

    struct robotraconteurlite_message_writer writer;
    struct robotraconteurlite_messageentry_writer entry_writer;

    struct robotraconteurlite_message_const_header header;

    const char* sender_nodename_data = "sender_node";
    const char* receiver_nodename_data = "recv_node";
    const char* extended_data = "extended_data\nblah blah";

    robotraconteurlite_u8 sender_nodeid[] = {0xd8, 0x35, 0xc4, 0x1d, 0x33, 0x9c, 0x47, 0xa8,
                                             0x84, 0xdb, 0xf0, 0x0a, 0x8f, 0xef, 0xd2, 0xfa};
    robotraconteurlite_u8 receiver_nodeid[] = {0xae, 0x78, 0x48, 0x1b, 0xc2, 0xbe, 0x4f, 0x26,
                                               0x9f, 0xef, 0x74, 0xbb, 0x6e, 0x8a, 0x04, 0x3f};

    memset(&header, 0, sizeof(header));

    buffer1.data = buffer_bytes;
    buffer1.len = *buffer_bytes_len;
    buffer.buffer_vec = &buffer1;
    buffer.buffer_vec_cnt = 1;
    assert_return_code(robotraconteurlite_message_writer_init(&writer, &buffer, 0, *buffer_bytes_len, message_ver), 0);

    header.message_size = 0;
    header.message_version = 2;
    header.header_size = 0;
    memcpy(header.sender_nodeid.data, sender_nodeid, 16);
    memcpy(header.receiver_nodeid.data, receiver_nodeid, 16);
    header.sender_endpoint = 4164376647;
    header.receiver_endpoint = 2805032049;
    robotraconteurlite_string_from_c_str(sender_nodename_data, &header.sender_nodename);
    robotraconteurlite_string_from_c_str(receiver_nodename_data, &header.receiver_nodename);
    robotraconteurlite_string_from_c_str(extended_data, &header.metadata);
    header.entry_count = 0;
    header.message_id = 10;
    header.message_res_id = 20;

    assert_return_code(
        robotraconteurlite_message_writer_begin_message_ex(&writer, &header, &entry_writer, message_flags_mask), 0);

    {
        struct robotraconteurlite_messageentry_const_header entry_header;
        struct robotraconteurlite_messageelement_writer element_writer;
        const char* member_name = "my_member";
        const char* service_path = "my.service.object[%28abc]";
        const char* extended = "more\nblah\nblah";

        memset(&entry_header, 0, sizeof(entry_header));
        entry_header.entry_size = 0;
        entry_header.entry_type = ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_PROPERTYGETRES;
        robotraconteurlite_string_from_c_str(member_name, &entry_header.member_name);
        entry_header.error = ROBOTRACONTEURLITE_MESSAGEERRORTYPE_NULLVALUE;
        entry_header.request_id = 395728;
        robotraconteurlite_string_from_c_str(service_path, &entry_header.service_path);
        robotraconteurlite_string_from_c_str(extended, &entry_header.metadata);
        entry_header.element_count = 0;

        assert_return_code(
            robotraconteurlite_messageentry_writer_begin_entry(&entry_writer, &entry_header, &element_writer), 0);
        assert_return_code(
            robotraconteurlite_messageentry_writer_end_entry(&entry_writer, &entry_header, &element_writer), 0);
    }

    {
        struct robotraconteurlite_messageentry_const_header entry_header;
        struct robotraconteurlite_messageelement_writer element_writer;
        const char* member_name = "my_member2";
        const char* service_path = "my.service.object[%12542]";
        const char* extended = "";

        const char* el1_name_data = "el1";
        struct robotraconteurlite_const_string el1_name;
        const char* el2_name_data = "el2";
        struct robotraconteurlite_const_string el2_name;
        robotraconteurlite_double el1_data_d[] = {1, 2, 3, 4, 5};
        const char* el2_data_d = "some random string";

        struct robotraconteurlite_array_double el1_data;
        struct robotraconteurlite_const_string el2_data;

        memset(&entry_header, 0, sizeof(entry_header));
        entry_header.entry_size = 0;
        entry_header.entry_type = ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_FUNCTIONCALLRES;
        robotraconteurlite_string_from_c_str(member_name, &entry_header.member_name);
        entry_header.error = ROBOTRACONTEURLITE_MESSAGEERRORTYPE_NONE;
        entry_header.request_id = 562846;
        robotraconteurlite_string_from_c_str(service_path, &entry_header.service_path);
        robotraconteurlite_string_from_c_str(extended, &entry_header.metadata);
        entry_header.element_count = 0;

        assert_return_code(
            robotraconteurlite_messageentry_writer_begin_entry(&entry_writer, &entry_header, &element_writer), 0);

        robotraconteurlite_string_from_c_str(el1_name_data, &el1_name);
        el1_data.data = el1_data_d;
        el1_data.len = sizeof(el1_data_d) / sizeof(robotraconteurlite_double);
        assert_return_code(
            robotraconteurlite_messageelement_writer_write_double_array(&element_writer, &el1_name, &el1_data), 0);

        robotraconteurlite_string_from_c_str(el2_name_data, &el2_name);
        robotraconteurlite_string_from_c_str(el2_data_d, &el2_data);
        assert_return_code(
            robotraconteurlite_messageelement_writer_write_data_string(&element_writer, &el2_name, &el2_data), 0);

        {
            const char* element_name_data = "el3";
            const char* type_string_data = "some_struct_type";
            struct robotraconteurlite_messageelement_const_header element_header;
            struct robotraconteurlite_messageelement_writer nested_element_writer;
            (void)memset(&element_header, 0, sizeof(element_header));

            robotraconteurlite_string_from_c_str(element_name_data, &element_header.element_name);
            element_header.element_type = ROBOTRACONTEURLITE_DATATYPE_STRUCTURE;
            robotraconteurlite_string_from_c_str(type_string_data, &element_header.element_type_name);

            assert_return_code(robotraconteurlite_messageelement_writer_begin_nested_element(
                                   &element_writer, &element_header, &nested_element_writer),
                               0);
            {
                const char* sub_doubles_name_d = "sub_doubles";
                robotraconteurlite_double sub_doubles_data_d[] = {1, 2, 3, 4};
                struct robotraconteurlite_const_string sub_doubles_name;
                struct robotraconteurlite_array_double sub_doubles_data;

                robotraconteurlite_string_from_c_str(sub_doubles_name_d, &sub_doubles_name);
                sub_doubles_data.data = sub_doubles_data_d;
                sub_doubles_data.len = sizeof(sub_doubles_data_d) / sizeof(robotraconteurlite_double);

                assert_return_code(robotraconteurlite_messageelement_writer_write_double_array(
                                       &nested_element_writer, &sub_doubles_name, &sub_doubles_data),
                                   0);
            }
            {
                const char* sub_singles_name_d = "sub_singles";
                robotraconteurlite_single sub_singles_data_d[] = {1, 2, 3, 4};
                struct robotraconteurlite_const_string sub_singles_name;
                struct robotraconteurlite_array_single sub_singles_data;

                robotraconteurlite_string_from_c_str(sub_singles_name_d, &sub_singles_name);
                sub_singles_data.data = sub_singles_data_d;
                sub_singles_data.len = sizeof(sub_singles_data_d) / sizeof(robotraconteurlite_single);

                assert_return_code(robotraconteurlite_messageelement_writer_write_single_array(
                                       &nested_element_writer, &sub_singles_name, &sub_singles_data),
                                   0);
            }
            {
                const char* sub_int8s_name_d = "sub_int8";
                robotraconteurlite_i8 sub_int8s_data_d[] = {1, 2, 3, 4};
                struct robotraconteurlite_const_string sub_int8s_name;
                struct robotraconteurlite_array_int8 sub_int8s_data;

                robotraconteurlite_string_from_c_str(sub_int8s_name_d, &sub_int8s_name);
                sub_int8s_data.data = sub_int8s_data_d;
                sub_int8s_data.len = sizeof(sub_int8s_data_d) / sizeof(robotraconteurlite_i8);

                assert_return_code(robotraconteurlite_messageelement_writer_write_int8_array(
                                       &nested_element_writer, &sub_int8s_name, &sub_int8s_data),
                                   0);
            }
            {
                const char* sub_uint8s_name_d = "sub_uint8";
                robotraconteurlite_u8 sub_uint8s_data_d[] = {1, 2, 3, 4};
                struct robotraconteurlite_const_string sub_uint8s_name;
                struct robotraconteurlite_array_uint8 sub_uint8s_data;

                robotraconteurlite_string_from_c_str(sub_uint8s_name_d, &sub_uint8s_name);
                sub_uint8s_data.data = sub_uint8s_data_d;
                sub_uint8s_data.len = sizeof(sub_uint8s_data_d) / sizeof(robotraconteurlite_u8);

                assert_return_code(robotraconteurlite_messageelement_writer_write_uint8_array(
                                       &nested_element_writer, &sub_uint8s_name, &sub_uint8s_data),
                                   0);
            }
            {
                const char* sub_int16s_name_d = "sub_int16";
                robotraconteurlite_i16 sub_int16s_data_d[] = {1, 2, 3, 4};
                struct robotraconteurlite_const_string sub_int16s_name;
                struct robotraconteurlite_array_int16 sub_int16s_data;

                robotraconteurlite_string_from_c_str(sub_int16s_name_d, &sub_int16s_name);
                sub_int16s_data.data = sub_int16s_data_d;
                sub_int16s_data.len = sizeof(sub_int16s_data_d) / sizeof(robotraconteurlite_i16);

                assert_return_code(robotraconteurlite_messageelement_writer_write_int16_array(
                                       &nested_element_writer, &sub_int16s_name, &sub_int16s_data),
                                   0);
            }
            {
                const char* sub_uint16s_name_d = "sub_uint16";
                robotraconteurlite_u16 sub_uint16s_data_d[] = {1, 2, 3, 4};
                struct robotraconteurlite_const_string sub_uint16s_name;
                struct robotraconteurlite_array_uint16 sub_uint16s_data;

                robotraconteurlite_string_from_c_str(sub_uint16s_name_d, &sub_uint16s_name);
                sub_uint16s_data.data = sub_uint16s_data_d;
                sub_uint16s_data.len = sizeof(sub_uint16s_data_d) / sizeof(robotraconteurlite_u16);

                assert_return_code(robotraconteurlite_messageelement_writer_write_uint16_array(
                                       &nested_element_writer, &sub_uint16s_name, &sub_uint16s_data),
                                   0);
            }
            {
                const char* sub_int32s_name_d = "sub_int32";
                robotraconteurlite_i32 sub_int32s_data_d[] = {1, 2, 3, 4};
                struct robotraconteurlite_const_string sub_int32s_name;
                struct robotraconteurlite_array_int32 sub_int32s_data;

                robotraconteurlite_string_from_c_str(sub_int32s_name_d, &sub_int32s_name);
                sub_int32s_data.data = sub_int32s_data_d;
                sub_int32s_data.len = sizeof(sub_int32s_data_d) / sizeof(robotraconteurlite_i32);

                assert_return_code(robotraconteurlite_messageelement_writer_write_int32_array(
                                       &nested_element_writer, &sub_int32s_name, &sub_int32s_data),
                                   0);
            }
            {
                const char* sub_uint32s_name_d = "sub_uint32";
                robotraconteurlite_u32 sub_uint32s_data_d[] = {1, 2, 3, 4};
                struct robotraconteurlite_const_string sub_uint32s_name;
                struct robotraconteurlite_array_uint32 sub_uint32s_data;

                robotraconteurlite_string_from_c_str(sub_uint32s_name_d, &sub_uint32s_name);
                sub_uint32s_data.data = sub_uint32s_data_d;
                sub_uint32s_data.len = sizeof(sub_uint32s_data_d) / sizeof(robotraconteurlite_u32);

                assert_return_code(robotraconteurlite_messageelement_writer_write_uint32_array(
                                       &nested_element_writer, &sub_uint32s_name, &sub_uint32s_data),
                                   0);
            }
            {
                const char* sub_int64s_name_d = "sub_int64";
                robotraconteurlite_i64 sub_int64s_data_d[] = {1, 2, 3, 4};
                struct robotraconteurlite_const_string sub_int64s_name;
                struct robotraconteurlite_array_int64 sub_int64s_data;

                robotraconteurlite_string_from_c_str(sub_int64s_name_d, &sub_int64s_name);
                sub_int64s_data.data = sub_int64s_data_d;
                sub_int64s_data.len = sizeof(sub_int64s_data_d) / sizeof(robotraconteurlite_i64);

                assert_return_code(robotraconteurlite_messageelement_writer_write_int64_array(
                                       &nested_element_writer, &sub_int64s_name, &sub_int64s_data),
                                   0);
            }
            {
                const char* sub_uint64s_name_d = "sub_uint64";
                robotraconteurlite_u64 sub_uint64s_data_d[] = {1, 2, 3, 4};
                struct robotraconteurlite_const_string sub_uint64s_name;
                struct robotraconteurlite_array_uint64 sub_uint64s_data;

                robotraconteurlite_string_from_c_str(sub_uint64s_name_d, &sub_uint64s_name);
                sub_uint64s_data.data = sub_uint64s_data_d;
                sub_uint64s_data.len = sizeof(sub_uint64s_data_d) / sizeof(robotraconteurlite_u64);

                assert_return_code(robotraconteurlite_messageelement_writer_write_uint64_array(
                                       &nested_element_writer, &sub_uint64s_name, &sub_uint64s_data),
                                   0);
            }
            {
                const char* sub_strings_name_d = "sub_string";
                const char* sub_strings_data_d = "another random string";
                struct robotraconteurlite_const_string sub_strings_name;
                struct robotraconteurlite_const_string sub_strings_data;

                robotraconteurlite_string_from_c_str(sub_strings_name_d, &sub_strings_name);
                robotraconteurlite_string_from_c_str(sub_strings_data_d, &sub_strings_data);

                assert_return_code(robotraconteurlite_messageelement_writer_write_data_string(
                                       &nested_element_writer, &sub_strings_name, &sub_strings_data),
                                   0);
            }
            {
                const char* sub_cdoubles_name_d = "sub_cdouble";
                robotraconteurlite_double sub_cdoubles_data_d[] = {1, 10, 2, 20, 3, 30, 4, 40};
                struct robotraconteurlite_const_string sub_cdoubles_name;
                struct robotraconteurlite_array_cdouble sub_cdoubles_data;

                robotraconteurlite_string_from_c_str(sub_cdoubles_name_d, &sub_cdoubles_name);
                sub_cdoubles_data.data = (struct robotraconteurlite_cdouble*)sub_cdoubles_data_d;
                sub_cdoubles_data.len = ((robotraconteurlite_size_t)sizeof(sub_cdoubles_data_d)) /
                                        ((robotraconteurlite_size_t)sizeof(struct robotraconteurlite_cdouble));

                assert_return_code(robotraconteurlite_messageelement_writer_write_cdouble_array(
                                       &nested_element_writer, &sub_cdoubles_name, &sub_cdoubles_data),
                                   0);
            }
            {
                const char* sub_csingles_name_d = "sub_csingle";
                robotraconteurlite_single sub_csingles_data_d[] = {1, 10, 2, 20, 3, 30, 4, 40};
                struct robotraconteurlite_const_string sub_csingles_name;
                struct robotraconteurlite_array_csingle sub_csingles_data;

                robotraconteurlite_string_from_c_str(sub_csingles_name_d, &sub_csingles_name);
                sub_csingles_data.data = (struct robotraconteurlite_csingle*)sub_csingles_data_d;
                sub_csingles_data.len = ((robotraconteurlite_size_t)sizeof(sub_csingles_data_d)) /
                                        ((robotraconteurlite_size_t)sizeof(struct robotraconteurlite_csingle));

                assert_return_code(robotraconteurlite_messageelement_writer_write_csingle_array(
                                       &nested_element_writer, &sub_csingles_name, &sub_csingles_data),
                                   0);
            }
            {
                const char* sub_bools_name_d = "sub_bool";
                robotraconteurlite_u8 sub_bools_data_d[] = {1, 1, 0, 1};
                struct robotraconteurlite_const_string sub_bools_name;
                struct robotraconteurlite_array_bool sub_bools_data;

                robotraconteurlite_string_from_c_str(sub_bools_name_d, &sub_bools_name);
                sub_bools_data.data = (struct robotraconteurlite_bool*)sub_bools_data_d;
                sub_bools_data.len = sizeof(sub_bools_data_d) / sizeof(robotraconteurlite_u8);

                assert_return_code(robotraconteurlite_messageelement_writer_write_bool_array(
                                       &nested_element_writer, &sub_bools_name, &sub_bools_data),
                                   0);
            }
            assert_return_code(robotraconteurlite_messageelement_writer_end_nested_element(
                                   &element_writer, &element_header, &nested_element_writer),
                               0);
            {
                const char* el4_name_d = "el4_is_a_very_long_name_it_keeps_going_and_going_and_going";
                struct robotraconteurlite_const_string el4_name;
                robotraconteurlite_string_from_c_str(el4_name_d, &el4_name);
                assert_return_code(
                    robotraconteurlite_messageelement_writer_write_double(&element_writer, &el4_name, 42.345), 0);
            }
        }

        assert_return_code(
            robotraconteurlite_messageentry_writer_end_entry(&entry_writer, &entry_header, &element_writer), 0);
    }

    {
        struct robotraconteurlite_messageentry_const_header entry_header;
        struct robotraconteurlite_messageelement_writer element_writer;
        (void)memset(&entry_header, 0, sizeof(entry_header));
        entry_header.entry_type = ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_STREAMOP;

        assert_return_code(
            robotraconteurlite_messageentry_writer_begin_entry(&entry_writer, &entry_header, &element_writer), 0);
        assert_return_code(
            robotraconteurlite_messageentry_writer_end_entry(&entry_writer, &entry_header, &element_writer), 0);
    }

    assert_return_code(robotraconteurlite_message_writer_end_message(&writer, &header, &entry_writer), 0);

    *buffer_bytes_len = header.message_size;
}

void robotraconteurlite_message_writer_basictest(void** state)
{
    robotraconteurlite_byte buffer_bytes[5000];
    robotraconteurlite_size_t buffer_bytes_len = sizeof(buffer_bytes);

    ROBOTRACONTEURLITE_UNUSED(state);

    robotraconteurlite_message_run_writer_basictest(buffer_bytes, &buffer_bytes_len, 2, 0xFF);

    assert_true(buffer_bytes_len == sizeof(message2));
    assert_true(memcmp(buffer_bytes, message2, sizeof(message2)) == 0);
}

void robotraconteurlite_message4_writer_basictest(void** state)
{
    robotraconteurlite_byte buffer_bytes[5000];
    robotraconteurlite_size_t buffer_bytes_len = sizeof(buffer_bytes);

    ROBOTRACONTEURLITE_UNUSED(state);

    robotraconteurlite_message_run_writer_basictest(buffer_bytes, &buffer_bytes_len, 4, 0xFF);

    robotraconteurlite_message_run_reader_basictest(buffer_bytes, buffer_bytes_len, 4);
}

void robotraconteurlite_message4_writer_basictest_masked(void** state)
{
    robotraconteurlite_byte buffer_bytes[5000];
    robotraconteurlite_size_t buffer_bytes_len = sizeof(buffer_bytes);

    robotraconteurlite_u8 message_mask = ROBOTRACONTEURLITE_MESSAGE_FLAGS_MULTIPLE_ENTRIES;

    ROBOTRACONTEURLITE_UNUSED(state);

    robotraconteurlite_message_run_writer_basictest(buffer_bytes, &buffer_bytes_len, 4, message_mask);

    {
        struct robotraconteurlite_buffer buffer1;
        struct robotraconteurlite_buffer_vec buffer;

        struct robotraconteurlite_message_reader reader;

        struct robotraconteurlite_message_header header;

        buffer1.data = buffer_bytes;
        buffer1.len = buffer_bytes_len;
        buffer.buffer_vec = &buffer1;
        buffer.buffer_vec_cnt = 1;

        assert_return_code(robotraconteurlite_message_reader_init(&reader, &buffer, 0, buffer_bytes_len), 0);
        (void)memset(&header, 0, sizeof(header));
        assert_return_code(robotraconteurlite_message_reader_read_header(&reader, &header), 0);

        assert_true(header.sender_endpoint == 0);
        assert_true(header.receiver_endpoint == 0);
        assert_true(header.sender_nodename.len == 0);
        assert_true(header.receiver_nodename.len == 0);
        assert_true(header.metadata.len == 0);
        assert_true(header.entry_count == 3);
        assert_true(header.message_id == 0);
        assert_true(header.message_res_id == 0);
    }
}

void robotraconteurlite_message4_writer_basictest_single_entry(void** state)
{
    robotraconteurlite_byte buffer_bytes[5000];
    robotraconteurlite_size_t buffer_bytes_len = sizeof(buffer_bytes);

    ROBOTRACONTEURLITE_UNUSED(state);

    {
        robotraconteurlite_u8 message_flags_mask = 0;

        struct robotraconteurlite_buffer buffer1;
        struct robotraconteurlite_buffer_vec buffer;

        struct robotraconteurlite_message_writer writer;
        struct robotraconteurlite_messageentry_writer entry_writer;

        struct robotraconteurlite_message_const_header header;

        buffer1.data = buffer_bytes;
        buffer1.len = buffer_bytes_len;
        buffer.buffer_vec = &buffer1;
        buffer.buffer_vec_cnt = 1;
        assert_return_code(robotraconteurlite_message_writer_init(&writer, &buffer, 0, buffer_bytes_len, 4), 0);

        memset(&header, 0, sizeof(header));

        assert_return_code(
            robotraconteurlite_message_writer_begin_message_ex(&writer, &header, &entry_writer, message_flags_mask), 0);

        {
            struct robotraconteurlite_messageentry_const_header entry_header;
            struct robotraconteurlite_messageelement_writer element_writer;
            (void)memset(&entry_header, 0, sizeof(entry_header));
            entry_header.entry_type = ROBOTRACONTEURLITE_MESSAGEENTRYTYPE_STREAMOP;

            assert_return_code(
                robotraconteurlite_messageentry_writer_begin_entry(&entry_writer, &entry_header, &element_writer), 0);
            assert_return_code(
                robotraconteurlite_messageentry_writer_end_entry(&entry_writer, &entry_header, &element_writer), 0);
        }

        assert_return_code(robotraconteurlite_message_writer_end_message(&writer, &header, &entry_writer), 0);

        buffer_bytes_len = header.message_size;
    }
    {
        struct robotraconteurlite_buffer buffer1;
        struct robotraconteurlite_buffer_vec buffer;

        struct robotraconteurlite_message_reader reader;

        struct robotraconteurlite_message_header header;

        buffer1.data = buffer_bytes;
        buffer1.len = buffer_bytes_len;
        buffer.buffer_vec = &buffer1;
        buffer.buffer_vec_cnt = 1;

        assert_return_code(robotraconteurlite_message_reader_init(&reader, &buffer, 0, buffer_bytes_len), 0);
        (void)memset(&header, 0, sizeof(header));
        assert_return_code(robotraconteurlite_message_reader_read_header(&reader, &header), 0);

        assert_true(header.sender_endpoint == 0);
        assert_true(header.receiver_endpoint == 0);
        assert_true(header.sender_nodename.len == 0);
        assert_true(header.receiver_nodename.len == 0);
        assert_true(header.metadata.len == 0);
        assert_true(header.entry_count == 1);
        assert_true(header.message_id == 0);
        assert_true(header.message_res_id == 0);
    }
}

int main(void)
{
    const struct CMUnitTest tests[] = {cmocka_unit_test(robotraconteurlite_message_reader_basictest),
                                       cmocka_unit_test(robotraconteurlite_message4_reader_basictest),
                                       cmocka_unit_test(robotraconteurlite_message_writer_basictest),
                                       cmocka_unit_test(robotraconteurlite_message4_writer_basictest),
                                       cmocka_unit_test(robotraconteurlite_message4_writer_basictest_masked),
                                       cmocka_unit_test(robotraconteurlite_message4_writer_basictest_single_entry)};
    return cmocka_run_group_tests(tests, NULL, NULL);
}
