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

/* http://wiki.fasterxml.com/JacksonBinaryFormatSpec */

#include <errno.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "smile.h"

int indent = 0;
void decode_key(u8** orig_data)
{
    int length = 0;
    const u8* ip = *orig_data;

    if (*ip >= 0x01 && *ip <= 0x1F) {
        // Reserved for future use
    } else if (*ip == 0x20) {
        // Empty String
        putchar('"');
        putchar('"');
    } else if (*ip >= 0x21 && *ip <= 0x2F) {
        // Reserved for future use
         (*orig_data)++;
    } else if (*ip >= 0x30 && *ip <= 0x33) {
        // "Long" shared key name reference
         (*orig_data)++;
    } else if (*ip == 0x32) {
        // Long (not-yet-shared) Unicode name
         (*orig_data)++;
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
    } else if (*ip >= 0x80 && *ip <= 0xBF) {
        // Short Ascii names
        // 5 LSB used to indicate lengths from 2 to 32 (bytes == chars)
        length = (*ip & 0x1F) + 1;
        (*orig_data) += 1 + length;
        ip++;

        PRINT_STRING_KEY(ip, length)
        PRINT_EOK
    } else if (*ip >= 0xC0 && *ip <= 0xF7) {
        // Short Unicode names
        // 5 LSB used to indicate lengths from 1 to 32 (bytes == chars)
        length = (*ip & 0x1F);
        (*orig_data) += length;
        ip++;

        PRINT(ip, length)
    } else if (*ip >= 0xF8 && *ip <= 0xFA) {
        // Reserved
        (*orig_data)++;
    } else if (*ip == 0xFB) {
        // END_OBJECT marker
        putchar('}');
        (*orig_data)++;
    } else if (*ip >= 0xFC) {
        // Reserved
        (*orig_data)++;
    }
}

void decode_value(u8** orig_data)
{
    int length = 0;
    u8* ip = *orig_data;

    if (*ip >= 0x01 && *ip <= 0x1f) {
        // Reference value
        length = (*ip & 0xF8);
        (*orig_data)++;
    } else if (*ip >= 0x20 && *ip <= 0x23) {
        if (*ip == 0x20) {
            // Empty String
            putchar('"');
            putchar('"');
        } else if (*ip == 0x21) {
            // null
            putchar('(');
            putchar('n');
            putchar('u');
            putchar('l');
            putchar('l');
            putchar(')');
        } else if (*ip == 0x22) {
            // false
            putchar('f');
            putchar('a');
            putchar('l');
            putchar('s');
            putchar('e');
        } else if (*ip == 0x21) {
            // true
            putchar('t');
            putchar('r');
            putchar('u');
            putchar('e');
        }
        (*orig_data)++;
    // Integral numbers
    } else if (*ip >= 0x24 && *ip <= 0x27) {
        length = (*ip & 0x03);
        (*orig_data)++;
        ip++;

        if (length == 0) {
            // 32-bit
            PRINT_INT_VALUE((int) **orig_data)
            (*orig_data) += 4;
        } else if (length == 1) {
            // 64-bit
            PRINT_LONG_VALUE((long) **orig_data)
            (*orig_data) += 8;
        } else if (length == 2) {
            // BigInteger
            (*orig_data)++;
        } else {
            // Reserved for future use
            (*orig_data)++;
        }
    // Floating point numbers
    } else if (*ip >= 0x28 && *ip <= 0x2B) {
        (*orig_data)++;
    // Reserved for future use
    } else if (*ip >= 0x2C && *ip <= 0x3F) {
        (*orig_data)++;
    // Tiny ASCII
    } else if (*ip >= 0x40 && *ip <= 0x5F) {
        // 5 LSB used to indicate lengths from 2 to 32 (bytes == chars)
        length = (*ip & 0x1F) + 1;
        (*orig_data) += 1 + length;
        ip++;

        PRINT_STRING_VALUE(ip, length)
    // Small ASCII
    } else if (*ip >= 0x60 && *ip <= 0x7F) {
        // 5 LSB used to indicate lengths from 33 to 64 (bytes == chars)
        length = (*ip & 0x1F) + 32;
        (*orig_data) += 1 + length;
        ip++;

        PRINT_STRING_VALUE(ip, length)
    // Tiny Unicode
    } else if (*ip >= 0x80 && *ip <= 0x9F) {
        // 5 LSB used to indicate _byte_ lengths from 2 to 33
        length = (*ip & 0x1F) + 1;
        (*orig_data) += 1 + length;
        ip++;

        PRINT_STRING_VALUE(ip, length)
        PRINT_EOK
    // Small Unicode
    } else if (*ip >= 0xA0 && *ip <= 0xBF) {
        // 5 LSB used to indicate _byte_ lengths from 34 to 65
        length = (*ip & 0x1F) + 33;
        (*orig_data) += 1 + length;
        ip++;

        PRINT_STRING_VALUE(ip, length)
        PRINT_EOK
    // Small integers
    } else if (*ip >= 0xC0 && *ip <= 0xDF) {
        (*orig_data)++;
    // Misc; binary / text / structure markers
    } else {
        if (*ip == SMILE_START_OBJECT) {
            // New object
            indent += 2;
            putchar('{');
        } else if (*ip == SMILE_END_OBJECT) {
            putchar('}');
        } else if  (*ip == SMILE_START_ARRAY) {
            putchar('[');
        } else if (*ip == SMILE_END_ARRAY) {
            putchar(']');
        } else {
                // TODO
        }
        (*orig_data)++;
    }
}

// Usage: gcc -Wall smile.c -o smile && ./smile data.file
// Decodes raw smile
int main(int argc, char *argv[])
{
    int fd, rc = 0;
    struct stat mystat;
    const char* fname = argv[1];
    size_t nbytes;
    ssize_t bytes_read;
    u8 buf[BUFFER_SIZE];
    char header[4];
    u8* ip;
    u8* eob;

    nbytes = sizeof(buf);

    rc = stat(fname, &mystat);
    fd = open(fname, O_RDONLY);

    if (fd == -1) {
        fprintf (stderr, "%s: not a regular file.\n", fname);
        exit(1);
    }


    bytes_read = read(fd, header, 4);
    if (bytes_read != 4) {
          fprintf(stderr, "%s: unable to read header: \n", fname);
          perror ("");
          goto exit;
    } else {
        if (!(header[0] == ':' && header[1] == ')' && header[2] == '\n')) {
            fprintf(stderr, "%s: bad header: %s\n", fname, header);
            goto exit;
        } else {
            printf("Got header: %s", header);
        }
    }

    while (1) {
        // Read block by block
        bytes_read = read(fd, buf, nbytes);
        if (bytes_read == -1) {
          fprintf(stderr, "%s: read error: \n", fname);
          perror ("");
          goto exit;
        } else if (bytes_read == 0) {
            break;
        }

        fprintf(stderr, "Read %lu bytes\n", (unsigned long int) bytes_read);
        ip = buf;
        eob = buf + nbytes;
        while (ip < eob) {
            decode_key((u8**) &ip);
            decode_value((u8**) &ip);
            putchar(',\n');
        }
    }

exit:
    close(fd);
    puts("Done!");
    exit(0);
}
