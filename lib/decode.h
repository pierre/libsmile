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

#ifndef _DECODE_H_
#define _DECODE_H_

#include <smile.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct smile_header {
    // Whether the header is valid
    bool valid;

    short version;
    bool raw_binary;
    bool shared_key_names;
    bool shared_value_names;
};


#define EIO     5   /* I/O error */
#define ENOMEM  12  /* Out of memory */

typedef enum {
    HEAD,       /* Waiting for magic header :) */
    STRUCTURE,  /* Waiting for next structure, either begin-array or begin-object */
    DONE,       /* Done -- remain here until reset */
    BAD,        /* Got a data error -- remain here until reset */
} decode_mode;

struct decode_state {
    decode_mode mode;
    unsigned long hold;         /* input bit accumulator */
    unsigned bits;              /* number of bits in "in" */
    unsigned long total;        /* protected copy of output count */
    struct smile_header hdr;    /* smile header */
};

#ifndef MAX_WBITS
#  define MAX_WBITS   15 /* 32K window */
#endif

struct decode_workspace {
    struct decode_state decode_state;
    unsigned char working_window[1 << MAX_WBITS];
};
#define WS(z) ((struct decode_workspace *)(z->workspace))

typedef struct s_stream_s {
    const unsigned char *next_in; /* next input byte */
    unsigned int avail_in;        /* number of bytes available at next_in */
    unsigned long total_in;       /* total nb of input bytes read so far */

    unsigned char *next_out;      /* next output byte should be put there */
    unsigned int avail_out;       /* remaining free space at next_out */
    unsigned long total_out;      /* total nb of bytes output so far */

    struct decode_state* state;   /* State maintained between calls */
    void *workspace;              /* memory allocated for this stream */

    char *msg;                    /* last error message, NULL if no error */

    unsigned long reserved;       /* reserved for future use */
} s_stream;

int smile_decode(s_stream *strm);
void smile_decode_init(s_stream *strm);

#endif
