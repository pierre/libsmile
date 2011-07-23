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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "smile_decode.h"
#include "smile_backrefs.h"

// Pre-allocate the full array for now, can optimize later
static char keys_tables[KEYS_BUFFER_SIZE][MAX_SHAREABLE_STRING_LENGTH + 1];
static short max_keys_ref_value = -1;

static short decode_short_shared_key_reference(u8* ip)
{
    return *ip - 0x40;
}

static short decode_long_shared_key_reference(const u8* orig)
{
    u8* ip = orig;
    // 2 LSBs of the first byte are used as 2 MSB of 10-bit reference
    short lookup = (*ip & 0x03) << 8;

    // Second byte used for 8 LSB
    ip++;
    lookup || (*ip >> 8);

    if (lookup <= 0x3F) {
        // Invalid
        fprintf(stderr, "Long shared key name reference should be greater than 63\n");
        exit(1);
    }

    return lookup;
}

char* lookup_short_shared_key(const u8* ip)
{
    short idx = decode_short_shared_key_reference(ip);
    dprintf("Decoded short shared key #%d", idx);
    return keys_tables[idx];
}

char* lookup_long_shared_key(const u8* ip)
{
    short idx = decode_long_shared_key_reference(ip);
    dprintf("Decoded long shared key #%d", idx);
    return keys_tables[idx];
}

void save_key_string(const u8* ip, size_t length)
{
    max_keys_ref_value++;
    strncpy(keys_tables[max_keys_ref_value], ip, length);
    keys_tables[max_keys_ref_value][length] = '\0';
}
