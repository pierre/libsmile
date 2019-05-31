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
#include <fcntl.h>
#include <getopt.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wchar.h>

#include "usage.h"
#include <smile.h>

#define BUFFER_SIZE 32768

static const char unsmile_usage[] = "unsmile [-p|--pretty] <file>";

int main(int argc, char **argv)
{
    int opt, fd = -1, status = 0, pretty = 0, ret = 0, idx = 0, utf8 = 1;
    struct stat st;
    const char* path;
    char src[BUFFER_SIZE], dst[BUFFER_SIZE];
    ssize_t bytes_read, bytes_decoded;
    wchar_t bytes_out[BUFFER_SIZE];
    static const struct option opts[] =
    {
        { "pretty", no_argument, NULL, 'p' },
        { NULL,     no_argument, NULL, '\0'}
    };

    while ((opt = getopt_long(argc, argv, "p", opts, NULL)) != -1) {
        switch (opt) {
            case 'p':
                pretty = 1;
        }
    }

    if (argc != 2) {
        usage(unsmile_usage);
    }
    path = argv[1];

    if (!setlocale(LC_CTYPE, "en_US.UTF-8")) {
        die("Can't set the specified locale! Check LANG, LC_CTYPE, LC_ALL");
    }

    status = stat(path, &st);
    if (status < 0) {
        die("open(\"%s\"): %s", path, strerror(errno));
    } else if (S_ISDIR(st.st_mode)) {
        die("open(\"%s\"): Is a directory", path);
    }

    fd = open(path, O_RDONLY);
    if (fd == -1) {
        die("open(\"%s\"): %s", path, strerror(errno));
    }

    // Read block by block
    for (;;) {
        bytes_read = read(fd, src, BUFFER_SIZE);
        if (bytes_read == -1) {
            error("read(\"%s\"): %s", path, strerror(errno));
            ret = -1;
            goto exit;
        } else if (bytes_read == 0) {
            goto exit;
        }

        dprintf("read(\"%s\"): %zd bytes", path, bytes_read);
        bytes_decoded = smile_decode_block(dst, BUFFER_SIZE, src, bytes_read);

        // Convert to wide characters
        memset(bytes_out, '\0', BUFFER_SIZE);
        mbstowcs(bytes_out, dst, bytes_decoded);

        /* 0 queries the current mode */
        if (fwide(stdout, 0) == 0) {
            /* stdout has no specific char mode yet, attempt to set to wide */
            if (fwide(stdout, 1) <= 0) {
                /* a value greater than zero switches to wide character mode */
                utf8 = 0;
            }
        }

        if (!utf8) {
            for (idx = 0; idx < bytes_decoded; idx++) {
                putchar(dst[idx]);
            }
        } else {
            wprintf(L"%ls", bytes_out);
        }
    }

exit:
    close(fd);
    exit(ret);
}
