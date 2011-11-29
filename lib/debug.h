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

#ifndef _SMILE_DEBUG_H_
#define _SMILE_DEBUG_H_

#ifdef DEBUG

/* printf pattern to output a byte in binary format */
#define BYTE_TO_BINARY_PATTERN "%d%d%d%d %d%d%d%d"

/* printf arguments to output individual bits of a byte */
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? 1 : 0), \
  ((byte) & 0x40 ? 1 : 0), \
  ((byte) & 0x20 ? 1 : 0), \
  ((byte) & 0x10 ? 1 : 0), \
  ((byte) & 0x08 ? 1 : 0), \
  ((byte) & 0x04 ? 1 : 0), \
  ((byte) & 0x02 ? 1 : 0), \
  ((byte) & 0x01 ? 1 : 0)

/*
 * Print the content of the current bit buffer
 *
 * This assumes that 16 bits have been loaded into the bit buffer
 */
#define DEBUG_BITS() \
  printf("DEBUG: [0x%lx] "BYTE_TO_BINARY_PATTERN" [0x%lx] "BYTE_TO_BINARY_PATTERN"\n", BITS(8), BYTE_TO_BINARY(BITS(8)), BITS(16)>>8, BYTE_TO_BINARY(BITS(16)>>8));

/*
 * Print the content of the next byte in the input buffer
 */
#define DEBUG_STREAM_BYTE() \
    printf("DEBUG: [%d] %c (0x%lx) (%d|%d)\n", state->mode, *next, *next, have, left)

/*
 * Print the content of the output buffer
 */
#define DEBUG_OUTPUT() \
    do { \
        debug_bytes_output = 0; \
        while (debug_bytes_output < strm->state->total) { \
          putchar(*(strm->next_out + debug_bytes_output)); \
          debug_bytes_output++; \
        } \
    } while(0);

/*
 * Pretty print the content of the Smile header
 */
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

#define BYTE_TO_BINARY_PATTERN
#define BYTE_TO_BINARY(byte)
#define DEBUG_BITS()
#define DEBUG_STREAM_BYTE()
#define DEBUG_OUTPUT()
#define DEBUG_HEADER()

#endif

#endif