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

#define ASSERT_EQUAL(expected, actual) \
    if (expected != actual) { \
        fprintf (stderr, "Test failure: expected %d (%c), got %d (%c)\n", expected, expected, actual, actual); \
        exit(1); \
    } else { \
        pass++; \
    }

/*
 * {
 *   "eventDate": 1311225160270,
 *   "eventGranularity": "HOURLY",
 *   "int": 1242,
 *   "string": "hello",
 *   "negative": -1,
 *   "long": 123456789
 * }
 */
static u8 data[] = {58,41,10,3,-6,-120,101,118,101,110,116,68,97,116,101,37,1,24,82,99,66,18,-100,-113,101,118,101,110,116,71,114,97,110,117,108,97,114,105,116,121,69,72,79,85,82,76,89,-126,105,110,116,36,38,-76,-123,115,116,114,105,110,103,68,104,101,108,108,111,-121,110,101,103,97,116,105,118,101,-63,-125,108,111,110,103,36,1,107,60,104,-86,-5};

inline void noop()
{
}

static long expected_long = 0;
static char* expected_string = NULL;

inline void number(long actual)
{
    ASSERT_EQUAL(expected_long, actual)
}

inline void string(const u8* ch, int start, int length)
{
    //printf("%s (%s)\n", ch, expected_string);
    int i = 0;
    while (length--) {
        ASSERT_EQUAL(*(expected_string + i), *(ch+i))
        i++;
    }
}

struct content_handler handler = {
    .name = "TESTER",
    .start_object = noop,
    .end_object = noop,
    .start_array = noop,
    .end_array = noop,
    .start_key = noop,
    .end_key = noop,
    .start_value = noop,
    .end_value = noop,
    .null_value = noop,
    .false_value = noop,
    .true_value = noop,
    .number_value = number,
    .characters = string
};

void test_decode_smile()
{
    // Header + 1
    u8* ip = data + 5;

    expected_string = "eventDate";
    smile_decode_key((u8**) &ip, &handler);
    expected_long = 1311225160270;
    smile_decode_value((u8**) &ip, &handler);

    expected_string = "eventGranularity";
    smile_decode_key((u8**) &ip, &handler);
    expected_string = "HOURLY";
    smile_decode_value((u8**) &ip, &handler);

    expected_string = "int";
    smile_decode_key((u8**) &ip, &handler);
    expected_long = 1242;
    smile_decode_value((u8**) &ip, &handler);

    expected_string = "string";
    smile_decode_key((u8**) &ip, &handler);
    expected_string = "hello";
    smile_decode_value((u8**) &ip, &handler);

    expected_string = "negative";
    smile_decode_key((u8**) &ip, &handler);
    expected_long = -1;
    smile_decode_value((u8**) &ip, &handler);

    expected_string = "long";
    smile_decode_key((u8**) &ip, &handler);
    expected_long = 123456789;
    smile_decode_value((u8**) &ip, &handler);
}

void test_decode()
{
    test_decode_smile();
    printf("decode tests run: %d assertions passed\n", pass);
}
