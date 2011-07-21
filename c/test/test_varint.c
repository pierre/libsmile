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

void test_varint()
{
    u8 buf[2];
    buf[0] = 0x02;
    buf[1] = 0xAC;

    // Standard example, see
    // http://code.google.com/apis/protocolbuffers/docs/encoding.html#varints
    ASSERT_EQUAL(300, varint_decode_buffer(buf));
}

void test_zigzag()
{
    ASSERT_EQUAL(0, ZZ_DECODE(0))
    ASSERT_EQUAL(-1, ZZ_DECODE(1))
    ASSERT_EQUAL(1, ZZ_DECODE(2))
    ASSERT_EQUAL(-2, ZZ_DECODE(3))
    ASSERT_EQUAL(2147483647, ZZ_DECODE(4294967294))
    ASSERT_EQUAL(-2147483648, ZZ_DECODE(4294967295))
}

int main()
{
    //test_varint();
    test_zigzag();

    printf("Tests passed: %d assertions run.\n", pass);
    exit(0);
}
