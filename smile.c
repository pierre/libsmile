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
            if (*ip == (char)0x00) {
                // nothing
            }
            else if (*ip == SMILE_START_OBJECT) {
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
                // Tiny Unicode, Small Unicode
                if (*ip >= (u8) 0x80 && *ip <= (u8) 0xBF) {
                    length = (*ip & 0x1F);
                    ip++;
                    putchar('"');
                    // humm, why +1?
                    while (length + 1) {
                        putchar(*ip);
                        ip++;
                        length--;
                    }
                    putchar('"');
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
