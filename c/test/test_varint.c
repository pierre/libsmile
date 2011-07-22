/*
 * Copyright 2011 Pierre-Alexandre Meyer
 *
 * Pierre-Alexandre Meyer licenses this file to you under the Apache License,
 * version 2.0 (the "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at:
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "smile_tests.h"
#include "../src/api.h"
#include "../src/smile_utils.h"

static int pass = 0;

#define ASSERT_EQUAL(expected, actual) \
    if (expected != actual) { \
        fprintf (stderr, "Test failure: expected %d, got %d\n", expected, actual); \
        exit(1); \
    } else { \
        pass++; \
    }

void test_number_encoding()
{
    u8 buf[3];
    buf[0] = 0x01;
    buf[1] = 0x7C;
    buf[2] = 0xA0;

    // 01 7c a0 should be 8080 (0x1F90)
    // 0x 0111 1100 1010 0000
    //     111 1100  010 0000
    //      11 1110 0010 0000
    ASSERT_EQUAL(8080, zz_decode(varint_decode_buffer(&buf)));
}

void test_zigzag()
{
    ASSERT_EQUAL(0, zz_decode(0))
    ASSERT_EQUAL(-1, zz_decode(1))
    ASSERT_EQUAL(1, zz_decode(2))
    ASSERT_EQUAL(-2, zz_decode(3))
    ASSERT_EQUAL(2147483647, zz_decode(4294967294))
    ASSERT_EQUAL(-2147483648, zz_decode(4294967295))
}

void test_varint()
{
    test_number_encoding();
    test_zigzag();
    printf("varint tests run: %d assertions passed\n", pass);
}
