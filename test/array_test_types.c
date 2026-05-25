

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "robotraconteurlite/array.h"
#define inline
#include <cmocka.h>

void robotraconteurlite_buffer_int8_test(void** state)
{
    robotraconteurlite_i8 a1[256];
    robotraconteurlite_i8 a2[256];
    robotraconteurlite_byte buf_data[256 * sizeof(robotraconteurlite_i8)];
    robotraconteurlite_size_t i = 0;
    struct robotraconteurlite_buffer buf;
    struct robotraconteurlite_buffer_vec buf_vec;
    struct robotraconteurlite_array_int8 int8s;
    struct robotraconteurlite_array_int8 int8s2;

    robotraconteurlite_i8 d1 = 10;
    robotraconteurlite_i8 d2 = 0;

    ROBOTRACONTEURLITE_UNUSED(state);

    for (i = 0; i < sizeof(a1) / sizeof(robotraconteurlite_i8); i++)
        a1[i] = (robotraconteurlite_i8)i;
    (void)memset(a2, 0, sizeof(a2));
    (void)memset(buf_data, 0, sizeof(buf_data));

    buf.data = buf_data;
    buf.len = sizeof(buf_data);

    buf_vec.buffer_vec = &buf;
    buf_vec.buffer_vec_cnt = 1;

    int8s.data = a1;
    int8s.len = sizeof(a1) / sizeof(robotraconteurlite_i8);

    assert_return_code(robotraconteurlite_buffer_vec_copy_from_int8_array(&buf_vec, 32, &int8s, 10, 15), 0);

    assert_true(memcmp((a1 + 10), (buf_data + 32), 15 * sizeof(robotraconteurlite_i8)) == 0);

    int8s2.data = a2;
    int8s2.len = sizeof(a2) / sizeof(robotraconteurlite_i8);

    assert_return_code(robotraconteurlite_buffer_vec_copy_to_int8_array(&buf_vec, 32, &int8s2, 12, 12), 0);

    assert_true(memcmp(a1 + 10, a2 + 12, 12 * sizeof(robotraconteurlite_i8)) == 0);

    robotraconteurlite_buffer_vec_copy_from_int8(&buf_vec, 128, d1);
    assert_true(memcmp(&d1, (buf_data + 128), sizeof(robotraconteurlite_i8)) == 0);
    robotraconteurlite_buffer_vec_copy_to_int8(&buf_vec, 32 + sizeof(robotraconteurlite_i8), &d2);
    assert_true(d2 == 11);
}

void robotraconteurlite_buffer_uint8_test(void** state)
{
    robotraconteurlite_u8 a1[256];
    robotraconteurlite_u8 a2[256];
    robotraconteurlite_byte buf_data[256 * sizeof(robotraconteurlite_u8)];
    robotraconteurlite_size_t i = 0;
    struct robotraconteurlite_buffer buf;
    struct robotraconteurlite_buffer_vec buf_vec;
    struct robotraconteurlite_array_uint8 uint8s;
    struct robotraconteurlite_array_uint8 uint8s2;

    robotraconteurlite_u8 d1 = 10;
    robotraconteurlite_u8 d2 = 0;

    ROBOTRACONTEURLITE_UNUSED(state);

    for (i = 0; i < sizeof(a1) / sizeof(robotraconteurlite_u8); i++)
        a1[i] = (robotraconteurlite_u8)i;
    (void)memset(a2, 0, sizeof(a2));
    (void)memset(buf_data, 0, sizeof(buf_data));

    buf.data = buf_data;
    buf.len = sizeof(buf_data);

    buf_vec.buffer_vec = &buf;
    buf_vec.buffer_vec_cnt = 1;

    uint8s.data = a1;
    uint8s.len = sizeof(a1) / sizeof(robotraconteurlite_u8);

    assert_return_code(robotraconteurlite_buffer_vec_copy_from_uint8_array(&buf_vec, 32, &uint8s, 10, 15), 0);

    assert_true(memcmp((a1 + 10), (buf_data + 32), 15 * sizeof(robotraconteurlite_u8)) == 0);

    uint8s2.data = a2;
    uint8s2.len = sizeof(a2) / sizeof(robotraconteurlite_u8);

    assert_return_code(robotraconteurlite_buffer_vec_copy_to_uint8_array(&buf_vec, 32, &uint8s2, 12, 12), 0);

    assert_true(memcmp(a1 + 10, a2 + 12, 12 * sizeof(robotraconteurlite_u8)) == 0);

    robotraconteurlite_buffer_vec_copy_from_uint8(&buf_vec, 128, d1);
    assert_true(memcmp(&d1, (buf_data + 128), sizeof(robotraconteurlite_u8)) == 0);
    robotraconteurlite_buffer_vec_copy_to_uint8(&buf_vec, 32 + sizeof(robotraconteurlite_u8), &d2);
    assert_true(d2 == 11);
}

void robotraconteurlite_buffer_int16_test(void** state)
{
    robotraconteurlite_i16 a1[256];
    robotraconteurlite_i16 a2[256];
    robotraconteurlite_byte buf_data[256 * sizeof(robotraconteurlite_i16)];
    robotraconteurlite_size_t i = 0;
    struct robotraconteurlite_buffer buf;
    struct robotraconteurlite_buffer_vec buf_vec;
    struct robotraconteurlite_array_int16 int16s;
    struct robotraconteurlite_array_int16 int16s2;

    robotraconteurlite_i16 d1 = 10;
    robotraconteurlite_i16 d2 = 0;

    ROBOTRACONTEURLITE_UNUSED(state);

    for (i = 0; i < sizeof(a1) / sizeof(robotraconteurlite_i16); i++)
        a1[i] = (robotraconteurlite_i16)i;
    (void)memset(a2, 0, sizeof(a2));
    (void)memset(buf_data, 0, sizeof(buf_data));

    buf.data = buf_data;
    buf.len = sizeof(buf_data);

    buf_vec.buffer_vec = &buf;
    buf_vec.buffer_vec_cnt = 1;

    int16s.data = a1;
    int16s.len = sizeof(a1) / sizeof(robotraconteurlite_i16);

    assert_return_code(robotraconteurlite_buffer_vec_copy_from_int16_array(&buf_vec, 32, &int16s, 10, 15), 0);

    assert_true(memcmp((a1 + 10), (buf_data + 32), 15 * sizeof(robotraconteurlite_i16)) == 0);

    int16s2.data = a2;
    int16s2.len = sizeof(a2) / sizeof(robotraconteurlite_i16);

    assert_return_code(robotraconteurlite_buffer_vec_copy_to_int16_array(&buf_vec, 32, &int16s2, 12, 12), 0);

    assert_true(memcmp(a1 + 10, a2 + 12, 12 * sizeof(robotraconteurlite_i16)) == 0);

    robotraconteurlite_buffer_vec_copy_from_int16(&buf_vec, 128, d1);
    assert_true(memcmp(&d1, (buf_data + 128), sizeof(robotraconteurlite_i16)) == 0);
    robotraconteurlite_buffer_vec_copy_to_int16(&buf_vec, 32 + sizeof(robotraconteurlite_i16), &d2);
    assert_true(d2 == 11);
}

void robotraconteurlite_buffer_uint16_test(void** state)
{
    robotraconteurlite_u16 a1[256];
    robotraconteurlite_u16 a2[256];
    robotraconteurlite_byte buf_data[256 * sizeof(robotraconteurlite_u16)];
    robotraconteurlite_size_t i = 0;
    struct robotraconteurlite_buffer buf;
    struct robotraconteurlite_buffer_vec buf_vec;
    struct robotraconteurlite_array_uint16 uint16s;
    struct robotraconteurlite_array_uint16 uint16s2;

    robotraconteurlite_u16 d1 = 10;
    robotraconteurlite_u16 d2 = 0;

    ROBOTRACONTEURLITE_UNUSED(state);

    for (i = 0; i < sizeof(a1) / sizeof(robotraconteurlite_u16); i++)
        a1[i] = (robotraconteurlite_u16)i;
    (void)memset(a2, 0, sizeof(a2));
    (void)memset(buf_data, 0, sizeof(buf_data));

    buf.data = buf_data;
    buf.len = sizeof(buf_data);

    buf_vec.buffer_vec = &buf;
    buf_vec.buffer_vec_cnt = 1;

    uint16s.data = a1;
    uint16s.len = sizeof(a1) / sizeof(robotraconteurlite_u16);

    assert_return_code(robotraconteurlite_buffer_vec_copy_from_uint16_array(&buf_vec, 32, &uint16s, 10, 15), 0);

    assert_true(memcmp((a1 + 10), (buf_data + 32), 15 * sizeof(robotraconteurlite_u16)) == 0);

    uint16s2.data = a2;
    uint16s2.len = sizeof(a2) / sizeof(robotraconteurlite_u16);

    assert_return_code(robotraconteurlite_buffer_vec_copy_to_uint16_array(&buf_vec, 32, &uint16s2, 12, 12), 0);

    assert_true(memcmp(a1 + 10, a2 + 12, 12 * sizeof(robotraconteurlite_u16)) == 0);

    robotraconteurlite_buffer_vec_copy_from_uint16(&buf_vec, 128, d1);
    assert_true(memcmp(&d1, (buf_data + 128), sizeof(robotraconteurlite_u16)) == 0);
    robotraconteurlite_buffer_vec_copy_to_uint16(&buf_vec, 32 + sizeof(robotraconteurlite_u16), &d2);
    assert_true(d2 == 11);
}

void robotraconteurlite_buffer_int32_test(void** state)
{
    robotraconteurlite_i32 a1[256];
    robotraconteurlite_i32 a2[256];
    robotraconteurlite_byte buf_data[256 * sizeof(robotraconteurlite_i32)];
    robotraconteurlite_size_t i = 0;
    struct robotraconteurlite_buffer buf;
    struct robotraconteurlite_buffer_vec buf_vec;
    struct robotraconteurlite_array_int32 int32s;
    struct robotraconteurlite_array_int32 int32s2;

    robotraconteurlite_i32 d1 = 10;
    robotraconteurlite_i32 d2 = 0;

    ROBOTRACONTEURLITE_UNUSED(state);

    for (i = 0; i < sizeof(a1) / sizeof(robotraconteurlite_i32); i++)
        a1[i] = (robotraconteurlite_i32)i;
    (void)memset(a2, 0, sizeof(a2));
    (void)memset(buf_data, 0, sizeof(buf_data));

    buf.data = buf_data;
    buf.len = sizeof(buf_data);

    buf_vec.buffer_vec = &buf;
    buf_vec.buffer_vec_cnt = 1;

    int32s.data = a1;
    int32s.len = sizeof(a1) / sizeof(robotraconteurlite_i32);

    assert_return_code(robotraconteurlite_buffer_vec_copy_from_int32_array(&buf_vec, 32, &int32s, 10, 15), 0);

    assert_true(memcmp((a1 + 10), (buf_data + 32), 15 * sizeof(robotraconteurlite_i32)) == 0);

    int32s2.data = a2;
    int32s2.len = sizeof(a2) / sizeof(robotraconteurlite_i32);

    assert_return_code(robotraconteurlite_buffer_vec_copy_to_int32_array(&buf_vec, 32, &int32s2, 12, 12), 0);

    assert_true(memcmp(a1 + 10, a2 + 12, 12 * sizeof(robotraconteurlite_i32)) == 0);

    robotraconteurlite_buffer_vec_copy_from_int32(&buf_vec, 128, d1);
    assert_true(memcmp(&d1, (buf_data + 128), sizeof(robotraconteurlite_i32)) == 0);
    robotraconteurlite_buffer_vec_copy_to_int32(&buf_vec, 32 + sizeof(robotraconteurlite_i32), &d2);
    assert_true(d2 == 11);
}

void robotraconteurlite_buffer_uint32_test(void** state)
{
    robotraconteurlite_u32 a1[256];
    robotraconteurlite_u32 a2[256];
    robotraconteurlite_byte buf_data[256 * sizeof(robotraconteurlite_u32)];
    robotraconteurlite_size_t i = 0;
    struct robotraconteurlite_buffer buf;
    struct robotraconteurlite_buffer_vec buf_vec;
    struct robotraconteurlite_array_uint32 uint32s;
    struct robotraconteurlite_array_uint32 uint32s2;

    robotraconteurlite_u32 d1 = 10;
    robotraconteurlite_u32 d2 = 0;

    ROBOTRACONTEURLITE_UNUSED(state);

    for (i = 0; i < sizeof(a1) / sizeof(robotraconteurlite_u32); i++)
        a1[i] = (robotraconteurlite_u32)i;
    (void)memset(a2, 0, sizeof(a2));
    (void)memset(buf_data, 0, sizeof(buf_data));

    buf.data = buf_data;
    buf.len = sizeof(buf_data);

    buf_vec.buffer_vec = &buf;
    buf_vec.buffer_vec_cnt = 1;

    uint32s.data = a1;
    uint32s.len = sizeof(a1) / sizeof(robotraconteurlite_u32);

    assert_return_code(robotraconteurlite_buffer_vec_copy_from_uint32_array(&buf_vec, 32, &uint32s, 10, 15), 0);

    assert_true(memcmp((a1 + 10), (buf_data + 32), 15 * sizeof(robotraconteurlite_u32)) == 0);

    uint32s2.data = a2;
    uint32s2.len = sizeof(a2) / sizeof(robotraconteurlite_u32);

    assert_return_code(robotraconteurlite_buffer_vec_copy_to_uint32_array(&buf_vec, 32, &uint32s2, 12, 12), 0);

    assert_true(memcmp(a1 + 10, a2 + 12, 12 * sizeof(robotraconteurlite_u32)) == 0);

    robotraconteurlite_buffer_vec_copy_from_uint32(&buf_vec, 128, d1);
    assert_true(memcmp(&d1, (buf_data + 128), sizeof(robotraconteurlite_u32)) == 0);
    robotraconteurlite_buffer_vec_copy_to_uint32(&buf_vec, 32 + sizeof(robotraconteurlite_u32), &d2);
    assert_true(d2 == 11);
}

void robotraconteurlite_buffer_int64_test(void** state)
{
    robotraconteurlite_i64 a1[256];
    robotraconteurlite_i64 a2[256];
    robotraconteurlite_byte buf_data[256 * sizeof(robotraconteurlite_i64)];
    robotraconteurlite_size_t i = 0;
    struct robotraconteurlite_buffer buf;
    struct robotraconteurlite_buffer_vec buf_vec;
    struct robotraconteurlite_array_int64 int64s;
    struct robotraconteurlite_array_int64 int64s2;

    robotraconteurlite_i64 d1 = 10;
    robotraconteurlite_i64 d2 = 0;

    ROBOTRACONTEURLITE_UNUSED(state);

    for (i = 0; i < sizeof(a1) / sizeof(robotraconteurlite_i64); i++)
        a1[i] = (robotraconteurlite_i64)i;
    (void)memset(a2, 0, sizeof(a2));
    (void)memset(buf_data, 0, sizeof(buf_data));

    buf.data = buf_data;
    buf.len = sizeof(buf_data);

    buf_vec.buffer_vec = &buf;
    buf_vec.buffer_vec_cnt = 1;

    int64s.data = a1;
    int64s.len = sizeof(a1) / sizeof(robotraconteurlite_i64);

    assert_return_code(robotraconteurlite_buffer_vec_copy_from_int64_array(&buf_vec, 32, &int64s, 10, 15), 0);

    assert_true(memcmp((a1 + 10), (buf_data + 32), 15 * sizeof(robotraconteurlite_i64)) == 0);

    int64s2.data = a2;
    int64s2.len = sizeof(a2) / sizeof(robotraconteurlite_i64);

    assert_return_code(robotraconteurlite_buffer_vec_copy_to_int64_array(&buf_vec, 32, &int64s2, 12, 12), 0);

    assert_true(memcmp(a1 + 10, a2 + 12, 12 * sizeof(robotraconteurlite_i64)) == 0);

    robotraconteurlite_buffer_vec_copy_from_int64(&buf_vec, 128, d1);
    assert_true(memcmp(&d1, (buf_data + 128), sizeof(robotraconteurlite_i64)) == 0);
    robotraconteurlite_buffer_vec_copy_to_int64(&buf_vec, 32 + sizeof(robotraconteurlite_i64), &d2);
    assert_true(d2 == 11);
}

void robotraconteurlite_buffer_uint64_test(void** state)
{
    robotraconteurlite_u64 a1[256];
    robotraconteurlite_u64 a2[256];
    robotraconteurlite_byte buf_data[256 * sizeof(robotraconteurlite_u64)];
    robotraconteurlite_size_t i = 0;
    struct robotraconteurlite_buffer buf;
    struct robotraconteurlite_buffer_vec buf_vec;
    struct robotraconteurlite_array_uint64 uint64s;
    struct robotraconteurlite_array_uint64 uint64s2;

    robotraconteurlite_u64 d1 = 10;
    robotraconteurlite_u64 d2 = 0;

    ROBOTRACONTEURLITE_UNUSED(state);

    for (i = 0; i < sizeof(a1) / sizeof(robotraconteurlite_u64); i++)
        a1[i] = (robotraconteurlite_u64)i;
    (void)memset(a2, 0, sizeof(a2));
    (void)memset(buf_data, 0, sizeof(buf_data));

    buf.data = buf_data;
    buf.len = sizeof(buf_data);

    buf_vec.buffer_vec = &buf;
    buf_vec.buffer_vec_cnt = 1;

    uint64s.data = a1;
    uint64s.len = sizeof(a1) / sizeof(robotraconteurlite_u64);

    assert_return_code(robotraconteurlite_buffer_vec_copy_from_uint64_array(&buf_vec, 32, &uint64s, 10, 15), 0);

    assert_true(memcmp((a1 + 10), (buf_data + 32), 15 * sizeof(robotraconteurlite_u64)) == 0);

    uint64s2.data = a2;
    uint64s2.len = sizeof(a2) / sizeof(robotraconteurlite_u64);

    assert_return_code(robotraconteurlite_buffer_vec_copy_to_uint64_array(&buf_vec, 32, &uint64s2, 12, 12), 0);

    assert_true(memcmp(a1 + 10, a2 + 12, 12 * sizeof(robotraconteurlite_u64)) == 0);

    robotraconteurlite_buffer_vec_copy_from_uint64(&buf_vec, 128, d1);
    assert_true(memcmp(&d1, (buf_data + 128), sizeof(robotraconteurlite_u64)) == 0);
    robotraconteurlite_buffer_vec_copy_to_uint64(&buf_vec, 32 + sizeof(robotraconteurlite_u64), &d2);
    assert_true(d2 == 11);
}

#define robotraconteurlite_array_float robotraconteurlite_array_single
#define robotraconteurlite_buffer_vec_copy_from_float_array robotraconteurlite_buffer_vec_copy_from_single_array
#define robotraconteurlite_buffer_vec_copy_to_float_array robotraconteurlite_buffer_vec_copy_to_single_array
#define robotraconteurlite_buffer_vec_copy_from_float robotraconteurlite_buffer_vec_copy_from_single
#define robotraconteurlite_buffer_vec_copy_to_float robotraconteurlite_buffer_vec_copy_to_single

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

void robotraconteurlite_buffer_double_test(void** state)
{
    robotraconteurlite_double a1[256];
    robotraconteurlite_double a2[256];
    robotraconteurlite_byte buf_data[256 * sizeof(robotraconteurlite_double)];
    robotraconteurlite_size_t i = 0;
    struct robotraconteurlite_buffer buf;
    struct robotraconteurlite_buffer_vec buf_vec;
    struct robotraconteurlite_array_double doubles;
    struct robotraconteurlite_array_double doubles2;

    robotraconteurlite_double d1 = 10.23;
    robotraconteurlite_double d2 = 0.0;

    ROBOTRACONTEURLITE_UNUSED(state);

    for (i = 0; i < sizeof(a1) / sizeof(robotraconteurlite_double); i++)
        a1[i] = (robotraconteurlite_double)i;
    (void)memset(a2, 0, sizeof(a2));
    (void)memset(buf_data, 0, sizeof(buf_data));

    buf.data = buf_data;
    buf.len = sizeof(buf_data);

    buf_vec.buffer_vec = &buf;
    buf_vec.buffer_vec_cnt = 1;

    doubles.data = a1;
    doubles.len = sizeof(a1) / sizeof(robotraconteurlite_double);

    assert_return_code(robotraconteurlite_buffer_vec_copy_from_double_array(&buf_vec, 32, &doubles, 10, 15), 0);

    assert_true(cmp_double(a1 + 10, (robotraconteurlite_double*)(buf_data + 32), 15) == 0);

    doubles2.data = a2;
    doubles2.len = sizeof(a2) / sizeof(robotraconteurlite_double);

    assert_return_code(robotraconteurlite_buffer_vec_copy_to_double_array(&buf_vec, 32, &doubles2, 12, 12), 0);

    assert_true(cmp_double(a1 + 10, a2 + 12, 12) == 0);

    robotraconteurlite_buffer_vec_copy_from_double(&buf_vec, 128, d1);
    assert_true(cmp_double(&d1, (robotraconteurlite_double*)(buf_data + 128), 1) == 0);
    robotraconteurlite_buffer_vec_copy_to_double(&buf_vec, 32 + sizeof(robotraconteurlite_double), &d2);
    assert_true(d2 == 11.0);
}

static int cmp_single(const robotraconteurlite_single* a, const robotraconteurlite_single* b,
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

void robotraconteurlite_buffer_single_test(void** state)
{
    robotraconteurlite_single a1[256];
    robotraconteurlite_single a2[256];
    robotraconteurlite_byte buf_data[256 * sizeof(robotraconteurlite_single)];
    robotraconteurlite_size_t i = 0;
    struct robotraconteurlite_buffer buf;
    struct robotraconteurlite_buffer_vec buf_vec;
    struct robotraconteurlite_array_single singles;
    struct robotraconteurlite_array_single singles2;

    robotraconteurlite_single d1 = 10.23f;
    robotraconteurlite_single d2 = 0.0;

    ROBOTRACONTEURLITE_UNUSED(state);

    for (i = 0; i < sizeof(a1) / sizeof(robotraconteurlite_single); i++)
        a1[i] = (robotraconteurlite_single)i;
    (void)memset(a2, 0, sizeof(a2));
    (void)memset(buf_data, 0, sizeof(buf_data));

    buf.data = buf_data;
    buf.len = sizeof(buf_data);

    buf_vec.buffer_vec = &buf;
    buf_vec.buffer_vec_cnt = 1;

    singles.data = a1;
    singles.len = sizeof(a1) / sizeof(robotraconteurlite_single);

    assert_return_code(robotraconteurlite_buffer_vec_copy_from_single_array(&buf_vec, 32, &singles, 10, 15), 0);

    assert_true(cmp_single(a1 + 10, (robotraconteurlite_single*)(buf_data + 32), 15) == 0);

    singles2.data = a2;
    singles2.len = sizeof(a2) / sizeof(robotraconteurlite_single);

    assert_return_code(robotraconteurlite_buffer_vec_copy_to_single_array(&buf_vec, 32, &singles2, 12, 12), 0);

    assert_true(cmp_single(a1 + 10, a2 + 12, 12) == 0);

    robotraconteurlite_buffer_vec_copy_from_single(&buf_vec, 128, d1);
    assert_true(cmp_single(&d1, (robotraconteurlite_single*)(buf_data + 128), 1) == 0);
    robotraconteurlite_buffer_vec_copy_to_single(&buf_vec, 32 + sizeof(robotraconteurlite_single), &d2);
    assert_true(d2 == 11.0);
}

static int cmp_csingle(const struct robotraconteurlite_csingle* a, const struct robotraconteurlite_csingle* b,
                       robotraconteurlite_size_t len)
{
    robotraconteurlite_size_t i = 0;
    for (i = 0; i < len; i++)
    {
        if (fabs((double)(a[i].real - b[i].real)) > 1e-4 || fabs((double)(a[i].imag - b[i].imag)) > 1e-4)
        {
            return 1;
        }
    }
    return 0;
}

void robotraconteurlite_buffer_csingle_test(void** state)
{
    struct robotraconteurlite_csingle a1[256];
    struct robotraconteurlite_csingle a2[256];
    robotraconteurlite_byte buf_data[256 * sizeof(struct robotraconteurlite_csingle)];
    robotraconteurlite_size_t i = 0;
    struct robotraconteurlite_buffer buf;
    struct robotraconteurlite_buffer_vec buf_vec;
    struct robotraconteurlite_array_csingle csingles;
    struct robotraconteurlite_array_csingle csingles2;

    struct robotraconteurlite_csingle d1;
    struct robotraconteurlite_csingle d2;
    memset(&d1, 0, sizeof(d1));
    memset(&d2, 0, sizeof(d2));

    d1.real = 10.23f;
    d1.imag = 11.34f;

    ROBOTRACONTEURLITE_UNUSED(state);

    for (i = 0; i < sizeof(a1) / sizeof(struct robotraconteurlite_csingle); i++)
    {
        a1[i].real = (robotraconteurlite_single)i;
        a1[i].imag = (robotraconteurlite_single)(i + 1);
    }
    (void)memset(a2, 0, sizeof(a2));
    (void)memset(buf_data, 0, sizeof(buf_data));

    buf.data = buf_data;
    buf.len = sizeof(buf_data);

    buf_vec.buffer_vec = &buf;
    buf_vec.buffer_vec_cnt = 1;

    csingles.data = a1;
    csingles.len = sizeof(a1) / sizeof(struct robotraconteurlite_csingle);

    assert_return_code(robotraconteurlite_buffer_vec_copy_from_csingle_array(&buf_vec, 32, &csingles, 10, 15), 0);

    assert_true(cmp_csingle(a1 + 10, (struct robotraconteurlite_csingle*)(buf_data + 32), 15) == 0);

    csingles2.data = a2;
    csingles2.len = sizeof(a2) / sizeof(struct robotraconteurlite_csingle);

    assert_return_code(robotraconteurlite_buffer_vec_copy_to_csingle_array(&buf_vec, 32, &csingles2, 12, 12), 0);

    assert_true(cmp_csingle(a1 + 10, a2 + 12, 12) == 0);

    robotraconteurlite_buffer_vec_copy_from_csingle(&buf_vec, 128, d1);
    assert_true(cmp_csingle(&d1, (struct robotraconteurlite_csingle*)(buf_data + 128), 1) == 0);
    robotraconteurlite_buffer_vec_copy_to_csingle(&buf_vec, 32 + sizeof(struct robotraconteurlite_csingle), &d2);
    assert_true(d2.real == 11.0);
    assert_true(d2.imag == 12.0);
}

static int cmp_cdouble(const struct robotraconteurlite_cdouble* a, const struct robotraconteurlite_cdouble* b,
                       robotraconteurlite_size_t len)
{
    robotraconteurlite_size_t i = 0;
    for (i = 0; i < len; i++)
    {
        if (fabs((double)(a[i].real - b[i].real)) > 1e-4 || fabs((double)(a[i].imag - b[i].imag)) > 1e-4)
        {
            return 1;
        }
    }
    return 0;
}

void robotraconteurlite_buffer_cdouble_test(void** state)
{
    struct robotraconteurlite_cdouble a1[256];
    struct robotraconteurlite_cdouble a2[256];
    robotraconteurlite_byte buf_data[256 * sizeof(struct robotraconteurlite_cdouble)];
    robotraconteurlite_size_t i = 0;
    struct robotraconteurlite_buffer buf;
    struct robotraconteurlite_buffer_vec buf_vec;
    struct robotraconteurlite_array_cdouble cdoubles;
    struct robotraconteurlite_array_cdouble cdoubles2;

    struct robotraconteurlite_cdouble d1;
    struct robotraconteurlite_cdouble d2;
    memset(&d1, 0, sizeof(d1));
    memset(&d2, 0, sizeof(d2));

    d1.real = 10.23;
    d1.imag = 11.34;

    ROBOTRACONTEURLITE_UNUSED(state);

    for (i = 0; i < sizeof(a1) / sizeof(struct robotraconteurlite_cdouble); i++)
    {
        a1[i].real = (robotraconteurlite_single)i;
        a1[i].imag = (robotraconteurlite_single)(i + 1);
    }
    (void)memset(a2, 0, sizeof(a2));
    (void)memset(buf_data, 0, sizeof(buf_data));

    buf.data = buf_data;
    buf.len = sizeof(buf_data);

    buf_vec.buffer_vec = &buf;
    buf_vec.buffer_vec_cnt = 1;

    cdoubles.data = a1;
    cdoubles.len = sizeof(a1) / sizeof(struct robotraconteurlite_cdouble);

    assert_return_code(robotraconteurlite_buffer_vec_copy_from_cdouble_array(&buf_vec, 32, &cdoubles, 10, 15), 0);

    assert_true(cmp_cdouble(a1 + 10, (struct robotraconteurlite_cdouble*)(buf_data + 32), 15) == 0);

    cdoubles2.data = a2;
    cdoubles2.len = sizeof(a2) / sizeof(struct robotraconteurlite_cdouble);

    assert_return_code(robotraconteurlite_buffer_vec_copy_to_cdouble_array(&buf_vec, 32, &cdoubles2, 12, 12), 0);

    assert_true(cmp_cdouble(a1 + 10, a2 + 12, 12) == 0);

    robotraconteurlite_buffer_vec_copy_from_cdouble(&buf_vec, 128, d1);
    assert_true(cmp_cdouble(&d1, (struct robotraconteurlite_cdouble*)(buf_data + 128), 1) == 0);
    robotraconteurlite_buffer_vec_copy_to_cdouble(&buf_vec, 32 + sizeof(struct robotraconteurlite_cdouble), &d2);
    assert_true(d2.real == 11.0);
    assert_true(d2.imag == 12.0);
}

int main(void)
{
    const struct CMUnitTest tests[] = {cmocka_unit_test(robotraconteurlite_buffer_int8_test),
                                       cmocka_unit_test(robotraconteurlite_buffer_uint8_test),
                                       cmocka_unit_test(robotraconteurlite_buffer_int16_test),
                                       cmocka_unit_test(robotraconteurlite_buffer_uint16_test),
                                       cmocka_unit_test(robotraconteurlite_buffer_int32_test),
                                       cmocka_unit_test(robotraconteurlite_buffer_uint32_test),
                                       cmocka_unit_test(robotraconteurlite_buffer_int64_test),
                                       cmocka_unit_test(robotraconteurlite_buffer_uint64_test),
                                       cmocka_unit_test(robotraconteurlite_buffer_double_test),
                                       cmocka_unit_test(robotraconteurlite_buffer_single_test),
                                       cmocka_unit_test(robotraconteurlite_buffer_csingle_test),
                                       cmocka_unit_test(robotraconteurlite_buffer_cdouble_test)};
    return cmocka_run_group_tests(tests, NULL, NULL);
}
