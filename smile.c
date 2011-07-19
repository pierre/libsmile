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
    const u8* ip;
    u8* eob;
    int length = 0;

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
            if (*ip >= (u8) 0x01 && *ip <= (u8) 0x1f) {
                // Reference value
                length = (*ip & 0xF8);
            } else if (*ip >= (u8) 0x20 && *ip <= (u8) 0x23) {
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
            // Integral numbers
            } else if (*ip >= (u8) 0x24 && *ip <= (u8) 0x27) {
                length = (*ip & 0x03);
                if (length == 0) {
                    // 32-bit
                } else if (length == 1) {
                    // 64-bit
                } else if (length == 2) {
                    // BigInteger
                } else {
                    // Reserved for future use
                }
            // Floating point numbers
            } else if (*ip >= (u8) 0x28 && *ip <= (u8) 0x2B) {
            // Reserved for future use
            } else if (*ip >= (u8) 0x2C && *ip <= (u8) 0x3F) {
            // Tiny ASCII
            } else if (*ip >= (u8) 0x40 && *ip <= (u8) 0x5F) {
                // 5 LSB used to indicate lengths from 1 to 32 (bytes == chars)
                length = (*ip & 0x1F);
                PRINT(ip, length)
            // Small ASCII
            } else if (*ip >= (u8) 0x60 && *ip <= (u8) 0x7F) {
                // 5 LSB used to indicate lengths from 33 to 64 (bytes == chars)
                length = (*ip & 0x1F) + 32;
                PRINT(ip, length)
            // Tiny Unicode
            } else if (*ip >= (u8) 0x80 && *ip <= (u8) 0x9F) {
                // 5 LSB used to indicate _byte_ lengths from 2 to 33
                length = (*ip & 0x1F) + 1;
                ip++;
                putchar('"');
                PRINT(ip, length)
                putchar('"');
                PRINT_EOK
            // Small Unicode
            } else if (*ip >= (u8) 0xA0 && *ip <= (u8) 0xBF) {
                // 5 LSB used to indicate _byte_ lengths from 34 to 65
                length = (*ip & 0x1F) + 33;
                ip++;
                putchar('"');
                PRINT(ip, length)
                putchar('"');
                PRINT_EOK
            // Small integers
            } else if (*ip >= (u8) 0xC0 && *ip <= (u8) 0xDF) {
            // Misc; binary / text / structure markers
            } else {
                if (*ip == SMILE_START_OBJECT) {
                    putchar('{');
                    putchar('\n');
                } else if (*ip == SMILE_END_OBJECT) {
                    putchar('}');
                } else if  (*ip == SMILE_START_ARRAY) {
                    putchar('[');
                    putchar('\n');
                } else if (*ip == SMILE_END_ARRAY) {
                    putchar(']');
                } else {
                        // TODO
                }
            }
            ip++;
        }
    }

exit:
    close(fd);
    puts("Done!");
    exit(0);
}
