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

#include "smile_utils.h"

inline long zz_decode(unsigned long n)
{
    return ((n >> 1) ^ (-(n & 1)));
}

long zzvarint_decode(u8 **msg)
{
    return zz_decode(varint_decode(msg));
}

unsigned long varint_decode(u8 **msg)
{
    unsigned long x = 0;
    while(!(**msg & 0x80)) {
        x <<= 7;
        x |= **msg;
        (*msg)++;
    }
    // last byte only has 6 payload bits
    x <<= 6;
    x |= (**msg & 0x3F);
    return x;
}

// For testing
unsigned long varint_decode_buffer(u8* input)
{
    u8* msg = input;
    varint_decode(&msg);
}

// Big enough?
u8 quoted[1024];

int quote(const u8* before, int length, u8** after)
{
    int i = 0;
    int j = 0;
    for (i = 0; i < length; i++) {
        switch (before[i]) {
            case '\n':
                // printf("\\n") will print 0x5c 0x6e (and not 0x5c 0x0a)
                quoted[j] = '\\';
                j++;
                quoted[j] = 'n';
                j++;
                break;
            case '"':
                quoted[j] = '\\';
                j++;
                // Fall through.
            default:
                quoted[j] = before[i];
                j++;
        }
    }

    *after = quoted;
    return j;
}
