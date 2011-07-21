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

long zzvarint_decode(u8 **msg)
{
    return ZZ_DECODE(varint_decode(msg));
}

unsigned long varint_decode(u8 **msg)
{
    long x = 0;
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
