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
#include "smile_backrefs.h"
#include "smile_utils.h"

/* By default, shared keys are enabled, but not values */
const struct smile_header DEFAULT_SMILE_HEADER = {0, 0, 1, 1, 0};

int smile_decode_key(u8** orig_data, struct content_handler* handler)
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

        // 2 bytes lookup
        char* decoded_key = lookup_short_shared_key(*orig_data);
        (*orig_data) += 2;
        handler->characters(decoded_key, 0, strlen(decoded_key));

        handler->end_key();
    } else if (*ip == 0x32) {
        // Long (not-yet-shared) Unicode name
        handler->start_key();
        (*orig_data)++;
        ip++;

        // 64 bytes or more
        int length = 0;
        while (**orig_data != SMILE_EOS) {
            length++;
            (*orig_data)++;
        }

        if (length < 64) {
            fprintf(stderr, "Long (not-yet-shared) Unicode name should be greater than 63\n");
            exit(1);
        } else {
            handler->characters(ip, 0, length);
        }

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

        // 1 byte lookup
        char* decoded_key = lookup_short_shared_key(*orig_data);
        (*orig_data)++;
        handler->characters(decoded_key, 0, strlen(decoded_key));

        handler->end_key();
    } else if (*ip >= 0x80 && *ip <= 0xBF) {
        // Short Ascii names
        // 5 LSB used to indicate lengths from 2 to 32 (bytes == chars)
        handler->start_key();
        length = (*ip & 0x1F) + 1;
        (*orig_data) += 1 + length;
        ip++;

        handler->characters(ip, 0, length);
        save_key_string(ip, length);

        handler->end_key();
    } else if (*ip >= 0xC0 && *ip <= 0xF7) {
        // Short Unicode names
        // 5 LSB used to indicate lengths from 1 to 32 (bytes == chars)
        handler->start_key();
        length = (*ip & 0x1F);
        (*orig_data) += length;
        ip++;

        handler->characters(ip, 0, length);
        save_key_string(ip, length);

        handler->end_key();
    } else if (*ip >= 0xF8 && *ip <= 0xFA) {
        // Reserved
        // TODO?
        handler->start_object();
        (*orig_data)++;
        return 0;
    } else if (*ip == 0xFB) {
        // END_OBJECT marker
        handler->end_object();
        (*orig_data)++;
        return 0;
    } else if (*ip >= 0xFC) {
        // Reserved
        (*orig_data)++;
    }

    return 1;
}

int smile_decode_value(u8** orig_data, struct content_handler* handler)
{
    int length = 0;
    u8* ip = *orig_data;

    if (*ip >= 0x01 && *ip <= 0x1f) {
        // Reference value
        handler->start_value();

        char* decoded_value = lookup_short_shared_value(*orig_data);
        (*orig_data)++;
        handler->characters(decoded_value, 0, strlen(decoded_value));

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
        save_value_string(ip, length);

        handler->end_value();
    // Small ASCII
    } else if (*ip >= 0x60 && *ip <= 0x7F) {
        // 5 LSB used to indicate lengths from 33 to 64 (bytes == chars)
        handler->start_value();
        length = (*ip & 0x1F) + 33;
        (*orig_data) += 1 + length;
        ip++;

        handler->characters(ip, 0, length);
        save_value_string(ip, length);

        handler->end_value();
    // Tiny Unicode
    } else if (*ip >= 0x80 && *ip <= 0x9F) {
        // 5 LSB used to indicate _byte_ lengths from 2 to 33
        handler->start_value();
        length = (*ip & 0x1F) + 1;
        (*orig_data) += 1 + length;
        ip++;

        handler->characters(ip, 0, length);
        save_value_string(ip, length);

        handler->end_value();
    // Small Unicode
    } else if (*ip >= 0xA0 && *ip <= 0xBF) {
        // 5 LSB used to indicate _byte_ lengths from 34 to 65
        handler->start_value();
        length = (*ip & 0x1F) + 33;
        (*orig_data) += 1 + length;
        ip++;

        handler->characters(ip, 0, length);
        save_value_string(ip, length);

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
        if (*ip >= 0xE0 && *ip < 0xE4) {
            handler->start_value();
            (*orig_data)++;
            ip++;

            // Long (variable length) ASCII text
            length = 0;
            // Smile EOS is either 0xFC or 0xFE?
            while (**orig_data != 0xFC) {
                length++;
                (*orig_data)++;
            }
            handler->characters(ip, 0, length);

            handler->end_value();
        } else if (*ip >= 0xE4 && *ip < 0xE8) {
            // Long (variable length) Unicode text
             handler->start_value();
            (*orig_data)++;
            ip++;

            // Long (variable length) ASCII text
            length = 0;
            // Smile EOS is either 0xFC or 0xFE?
            while (**orig_data != 0xFC) {
                length++;
                (*orig_data)++;
            }
            handler->characters(ip, 0, length);

            handler->end_value();
        } else if (*ip >= 0xE8 && *ip < 0xEC) {
            // Shared String reference, long
            handler->start_value();

            char* decoded_key = lookup_long_shared_key(*orig_data);
            (*orig_data)++;
            handler->characters(decoded_key, 0, strlen(decoded_key));

            handler->end_value();
        } else if (*ip >= 0xEC && *ip < 0xF8) {
            // Binary, 7-bit encoded
            (*orig_data)++;
        } else if (*ip == SMILE_START_OBJECT) {
            handler->start_object();
            (*orig_data)++;
        } else if (*ip == SMILE_END_OBJECT) {
            handler->end_object();
            (*orig_data)++;
        } else if  (*ip == SMILE_START_ARRAY) {
            handler->start_array();
            (*orig_data)++;
        } else if (*ip == SMILE_END_ARRAY) {
            handler->end_array();
            (*orig_data)++;
        } else {
                // TODO
        }
    }

    return 1;
}

struct smile_header smile_decode_header(u8* raw_header)
{
    struct smile_header header = DEFAULT_SMILE_HEADER;

    header.valid = (raw_header[0] == ':' && raw_header[1] == ')' && raw_header[2] == '\n');

    // Optional properties
    u8 options = raw_header[3];

    // 0x00 for current version
    header.version = (options & 0xF0);

    // Whether raw binary (unescaped 8-bit) values may be present in content
    header.raw_binary = (options & 0x04) >> 2;

    // Shared String key
    header.shared_key_names = (options & 0x01);
    // Shared String value
    header.shared_value_names = (options & 0x02) >> 1;

    return header;
}

void smile_decode(u8* orig_data, int nbytes, struct content_handler* handler)
{
    u8* ip = orig_data;
    while (nbytes--) {
        dprintf("Decoding key\n");
        if (smile_decode_key((u8**) &ip, handler)) {
            dprintf("Decoding value\n");
            smile_decode_value((u8**) &ip, handler);
        }
    }
}
