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

#ifndef _SMILE_DECODE_H_
#define _SMILE_DECODE_H_

#include <smile.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "backrefs.h"
#include "bits.h"
#include "copy.h"
#include "debug.h"
#include "error.h"
#include "varint.h"


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
    ROOT,       /* Waiting for Root object */
    ARRAY,      /* In array context */
    VALUE,      /* Waiting for value */
    KEY,        /* Waiting for key */
    DONE,       /* Done -- remain here until reset */
    BAD,        /* Got a data error -- remain here until reset */
} decode_mode;

// The encoded byte length of a String value that can be referenced is 64 bytes or less
#define MAX_SHAREABLE_STRING_LENGTH 64
#define KEYS_BUFFER_SIZE 1024
#define VALUES_BUFFER_SIZE 1024

struct decode_state {
    decode_mode mode;
    unsigned long hold;         /* input bit accumulator */
    unsigned bits;              /* number of bits in "hold" */
    unsigned long read;         /* protected copy of input count */
    unsigned long total;        /* protected copy of output count */

    int nested_depth;           /* pointer to current nest level */
    bool in_array[30];          /* true if in array context */
    bool first_array_element[30];   /* true if the next token is the first value of an array (used for printing) */

    bool first_key[30];         /* true if the next token is the first key of an object (used for printing) */

    struct smile_header hdr;    /* smile header */

    /* caches for back references */
    char keys_tables[KEYS_BUFFER_SIZE][MAX_SHAREABLE_STRING_LENGTH + 1];
    short max_keys_ref_value;
    char values_tables[VALUES_BUFFER_SIZE][MAX_SHAREABLE_STRING_LENGTH + 1];
    short max_values_ref_value;
};

#ifndef MAX_WBITS
#  define MAX_WBITS   15 /* 32K window */
#endif

#define MAX_ERROR_MSG_SIZE 100

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
int smile_decode_init(s_stream *strm);
int smile_decode_reset(s_stream *strm);

#endif
