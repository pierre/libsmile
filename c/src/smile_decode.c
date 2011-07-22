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

/* http://wiki.fasterxml.com/SmileFormatSpec */

#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "smile_decode.h"
#include "smile_utils.h"

void smile_decode_key(u8** orig_data, struct content_handler* handler)
{
    int length = 0;
    const u8* ip = *orig_data;

    if (*ip >= 0x01 && *ip <= 0x1F) {
        // Reserved for future use
    } else if (*ip == 0x20) {
        // Empty String
        handler->start_key();
        (*orig_data)++;
        handler->end_key();
    } else if (*ip >= 0x21 && *ip <= 0x2F) {
        // Reserved for future use
        (*orig_data)++;
    } else if (*ip >= 0x30 && *ip <= 0x33) {
        // "Long" shared key name reference
        handler->start_key();
        (*orig_data)++;
        handler->end_key();
    } else if (*ip == 0x32) {
        // Long (not-yet-shared) Unicode name
        handler->start_key();
        (*orig_data)++;
        handler->end_key();
    } else if (*ip >= 0x35 && *ip <= 0x39) {
        // Reserved for future use
        (*orig_data)++;
    } else if (*ip == 0x3A) {
        // Error
        fprintf(stderr, "Error decoding key\n");
        exit(1);
    } else if (*ip >= 0x3B && *ip <= 0x3F) {
        // Reserved for future use
    } else if (*ip >= 0x40 && *ip <= 0x7F) {
        // "Short" shared key name reference
        handler->start_key();
        (*orig_data)++;
        handler->end_key();
    } else if (*ip >= 0x80 && *ip <= 0xBF) {
        // Short Ascii names
        // 5 LSB used to indicate lengths from 2 to 32 (bytes == chars)
        handler->start_key();
        length = (*ip & 0x1F) + 1;
        (*orig_data) += 1 + length;
        ip++;

        handler->characters(ip, 0, length);
        handler->end_key();
    } else if (*ip >= 0xC0 && *ip <= 0xF7) {
        // Short Unicode names
        // 5 LSB used to indicate lengths from 1 to 32 (bytes == chars)
        handler->start_key();
        length = (*ip & 0x1F);
        (*orig_data) += length;
        ip++;

        handler->characters(ip, 0, length);
        handler->end_key();
    } else if (*ip >= 0xF8 && *ip <= 0xFA) {
        // Reserved
        (*orig_data)++;
    } else if (*ip == 0xFB) {
        // END_OBJECT marker
        handler->end_object();
        (*orig_data)++;
    } else if (*ip >= 0xFC) {
        // Reserved
        (*orig_data)++;
    }
}

void smile_decode_value(u8** orig_data, struct content_handler* handler)
{
    int length = 0;
    u8* ip = *orig_data;

    if (*ip >= 0x01 && *ip <= 0x1f) {
        // Reference value
        handler->start_value();
        length = (*ip & 0xF8);
        (*orig_data)++;
        handler->end_value();
    } else if (*ip >= 0x20 && *ip <= 0x23) {
        handler->start_value();
        if (*ip == 0x20) {
            // Empty String
        } else if (*ip == 0x21) {
            // null
            handler->null_value();
            handler->end_value();
        } else if (*ip == 0x22) {
            // false
            handler->false_value();
            handler->end_value();
        } else if (*ip == 0x21) {
            // true
            handler->true_value();
        }
        (*orig_data)++;
        handler->end_value();
    // Integral numbers
    } else if (*ip >= 0x24 && *ip <= 0x27) {
        length = (*ip & 0x03);
        (*orig_data)++;
        ip++;

        if (length == 0) {
            // 32-bit
            handler->start_value();
            handler->number_value(zzvarint_decode(orig_data));
            (*orig_data)++;
            handler->end_value();
        } else if (length == 1) {
            // 64-bit
            handler->start_value();
            handler->number_value(zzvarint_decode(orig_data));
            (*orig_data)++;
            handler->end_value();
        } else if (length == 2) {
            // BigInteger
            handler->start_value();
            (*orig_data)++;
            handler->end_value();
        } else {
            // Reserved for future use
            (*orig_data)++;
        }
    // Integral numbers
    // Floating point numbers
    } else if (*ip >= 0x28 && *ip <= 0x2B) {
        handler->start_value();
        (*orig_data)++;
        handler->end_value();
    // Reserved for future use
    } else if (*ip >= 0x2C && *ip <= 0x3F) {
        handler->start_value();
        (*orig_data)++;
        handler->end_value();
    // Tiny ASCII
    } else if (*ip >= 0x40 && *ip <= 0x5F) {
        // 5 LSB used to indicate lengths from 2 to 32 (bytes == chars)
        handler->start_value();
        length = (*ip & 0x1F) + 1;
        (*orig_data) += 1 + length;
        ip++;

        handler->characters(ip, 0, length);
        handler->end_value();
    // Small ASCII
    } else if (*ip >= 0x60 && *ip <= 0x7F) {
        // 5 LSB used to indicate lengths from 33 to 64 (bytes == chars)
        handler->start_value();
        length = (*ip & 0x1F) + 32;
        (*orig_data) += 1 + length;
        ip++;

        handler->characters(ip, 0, length);
        handler->end_value();
    // Tiny Unicode
    } else if (*ip >= 0x80 && *ip <= 0x9F) {
        // 5 LSB used to indicate _byte_ lengths from 2 to 33
        handler->start_value();
        length = (*ip & 0x1F) + 1;
        (*orig_data) += 1 + length;
        ip++;

        handler->characters(ip, 0, length);
        handler->end_value();
    // Small Unicode
    } else if (*ip >= 0xA0 && *ip <= 0xBF) {
        // 5 LSB used to indicate _byte_ lengths from 34 to 65
        handler->start_value();
        length = (*ip & 0x1F) + 33;
        (*orig_data) += 1 + length;
        ip++;

        handler->characters(ip, 0, length);
        handler->end_value();
    // Small integers
    } else if (*ip >= 0xC0 && *ip <= 0xDF) {
        handler->start_value();
        (*orig_data)++;
        ip++;
        handler->number_value(zz_decode(*ip & 0x1F));
        handler->end_value();
    // Misc; binary / text / structure markers
    } else {
        if (*ip == SMILE_START_OBJECT) {
            handler->start_object();
        } else if (*ip == SMILE_END_OBJECT) {
            handler->end_object();
        } else if  (*ip == SMILE_START_ARRAY) {
            handler->start_array();
        } else if (*ip == SMILE_END_ARRAY) {
            handler->end_array();
        } else {
                // TODO
        }
        (*orig_data)++;
    }
}

int smile_decode_header(u8* header)
{
    return (header[0] == ':' && header[1] == ')' && header[2] == '\n');
}

void smile_decode(u8* orig_data, int nbytes, struct content_handler* handler)
{
    u8* ip = orig_data;
    while (nbytes--) {
        smile_decode_key((u8**) &ip, handler);
        smile_decode_value((u8**) &ip, handler);
    }
}
