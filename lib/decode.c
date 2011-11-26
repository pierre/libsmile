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
 *
 * State machine model inspired from zlib implementation: http://zlib.net/
 */

#include "decode.h"

#ifdef DEBUG
#define DEBUG_STREAM_BYTES(n) \
    do { \
        debug_bits = 0; \
        while (debug_bits < n) { \
            printf("DEBUG: [%d] %c (%d/%d)\n", (strm->state)->mode, *(strm->next_in + debug_bits), strm->total_in + debug_bits, strm->avail_in); \
            debug_bits++; \
        }; \
    } while (0)

#define DEBUG_HEADER() \
    do { \
        if (strm->state->hdr.valid) { \
            fputs("DEBUG: valid header, ", stdout); \
        } else { \
            fputs("DEBUG: invalid header, ", stdout); \
        } \
        printf("version [0x%u], ", strm->state->hdr.version); \
        if (strm->state->hdr.raw_binary) { \
            fputs("raw binary values present, ", stdout); \
        } else { \
            fputs("raw binary values absent, ", stdout); \
        } \
        if (strm->state->hdr.shared_key_names) { \
            fputs("shared key names, ", stdout); \
        } else { \
            fputs("non-shared key names, ", stdout); \
        } \
        if (strm->state->hdr.shared_value_names) { \
            fputs("shared values\n", stdout); \
        } else { \
            fputs("non-shared values\n", stdout); \
        } \
    } while (0)
#else
#define DEBUG_STREAM(n)
#define DEBUG_HEADER()
#endif

/* Load registers with state for speed */
#define LOAD() \
    do { \
        put = strm->next_out; \
        left = strm->avail_out; \
        next = strm->next_in; \
        have = strm->avail_in; \
        hold = state->hold; \
        bits = state->bits; \
    } while (0)

/* Restore state from registers */
#define RESTORE() \
    do { \
        strm->next_out = put; \
        strm->avail_out = left; \
        strm->next_in = next; \
        strm->avail_in = have; \
        state->hold = hold; \
        state->bits = bits; \
    } while (0)

/* Clear the input bit accumulator */
#define INITBITS() \
    do { \
        hold = 0; \
        bits = 0; \
    } while (0)

/* Get a byte of input into the bit accumulator, or return
   if there is no input available. */
#define PULLBYTE() \
    do { \
        if (have == 0) goto out; \
        have--; \
        hold += (unsigned long)(*next++) << bits; \
        bits += 8; \
    } while (0)

/* Assure that there are at least n bits in the bit accumulator.  If there is
   not enough available input to do that, then return */
#define NEEDBITS(n) \
    do { \
        while (bits < (unsigned)(n)) \
            PULLBYTE(); \
    } while (0)

/* Return the low n bits of the bit accumulator (n < 16) */
#define BITS(n) \
    ((unsigned)hold & ((1U << (n)) - 1))

/* Remove n bits from the bit accumulator */
#define DROPBITS(n) \
    do { \
        hold >>= (n); \
        bits -= (unsigned)(n); \
    } while (0)

/* Remove zero to seven bits as needed to go to a byte boundary */
#define BYTEBITS() \
    do { \
        hold >>= bits & 7; \
        bits -= bits & 7; \
    } while (0)

/* Reverse the bytes in a 32-bit value */
#define REVERSE(q) \
    ((((q) >> 24) & 0xff) + (((q) >> 8) & 0xff00) + \
     (((q) & 0xff00) << 8) + (((q) & 0xff) << 24))

int smile_decode(s_stream *strm)
{
    unsigned char *curr;
    struct decode_state *state;
    const unsigned char *next;  /* next input */
    unsigned char *put;         /* next output */
    unsigned int have, left;        /* available input and output */
    unsigned long hold;         /* bit buffer */
    unsigned int bits;              /* bits in bit buffer */
#ifdef DEBUG
    unsigned int debug_bits;
#endif
    unsigned int in, out;           /* save starting available input and output */
    unsigned int copy;              /* number of stored or match bytes to copy */
    unsigned char *from;        /* where to copy match bytes from */
    unsigned int len;               /* length to copy for repeats, bits to drop */
    int ret = 0;                    /* return code */

    if (strm == NULL || //strm->state == NULL ||
        (strm->next_in == NULL && strm->avail_in != 0)) {
        return -1;
    }

    state = (struct decode_state*)strm->state;
    LOAD();
    in = have;
    out = left;
    ret = 0;

    for (;;) {
        switch(state->mode) {
        case HEAD:
            NEEDBITS(32);
            if ((hold & 0xff) != ':' ||
                ((hold >> 8) & 0xff) != ')'||
                ((hold >> 16) & 0xff) != '\n') {
                strm->msg = (char *)"incorrect header";
                state->mode = BAD;
                break;
            }

            // Header verified
            state->mode = DONE;
            DROPBITS(24);

            state->hdr.valid = true;
            // 0x00 for current version
            state->hdr.version = (BITS(8) & 0xF0);
            // Whether raw binary (unescaped 8-bit) values may be present in content
            state->hdr.raw_binary = (BITS(8) & 0x04) >> 2;
            // Shared String key
            state->hdr.shared_key_names = (BITS(8) & 0x01);
            // Shared String value
            state->hdr.shared_value_names = (BITS(8) & 0x02) >> 1;
            DEBUG_HEADER();

            INITBITS();
            break;
        case BAD:
            ret = -1;
            goto out;
        DONE:
            goto out;
        default:
            strm->msg = (char *)"shouldn't get here";
            state->mode = BAD;
            goto out;
        }
    }

out:
    RESTORE();

    in -= strm->avail_in;
    out -= strm->avail_out;
    strm->total_in += in;
    strm->total_out += out;
    state->total += out;

    return ret;
}

void smile_decode_init(s_stream *strm)
{
    strm->state = &WS(strm)->decode_state;

    // Default Smile header: shared keys are enabled, but not values
    strm->state->hdr.raw_binary = true;
    strm->state->hdr.shared_key_names = true;
}
