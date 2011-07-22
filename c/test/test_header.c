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
#include "../src/smile_decode.h"
#include "../src/smile_utils.h"

static int pass = 0;

void test_encoded_shared_values_shared_names()
{
    u8 header[] = {0x3a, 0x29, 0x0a, 0x03};

    struct smile_header sheader = smile_decode_header(header);
    ASSERT_EQUAL(0, sheader.raw_binary)
    ASSERT_EQUAL(1, sheader.shared_key_names)
    ASSERT_EQUAL(1, sheader.shared_value_names)
}

void test_raw_no_share()
{
    u8 header[] = {0x3a, 0x29, 0x0a, 0x04};

    struct smile_header sheader = smile_decode_header(header);
    ASSERT_EQUAL(1, sheader.raw_binary)
    ASSERT_EQUAL(0, sheader.shared_key_names)
    ASSERT_EQUAL(0, sheader.shared_value_names)
}

void test_raw_shared_names()
{
    u8 header[] = {0x3a, 0x29, 0x0a, 0x05};

    struct smile_header sheader = smile_decode_header(header);
    ASSERT_EQUAL(1, sheader.raw_binary)
    ASSERT_EQUAL(1, sheader.shared_key_names)
    ASSERT_EQUAL(0, sheader.shared_value_names)
}

void test_raw_shared_values_shared_names()
{
    u8 header[] = {0x3a, 0x29, 0x0a, 0x07};

    struct smile_header sheader = smile_decode_header(header);
    ASSERT_EQUAL(1, sheader.raw_binary)
    ASSERT_EQUAL(1, sheader.shared_key_names)
    ASSERT_EQUAL(1, sheader.shared_value_names)
}

void test_header()
{
    test_encoded_shared_values_shared_names();
    test_raw_no_share();
    test_raw_shared_names();
    test_raw_shared_values_shared_names();
    printf("header tests run: %d assertions passed\n", pass);
}
