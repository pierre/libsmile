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

#ifndef _SMILE_VARINT_H_
#define _SMILE_VARINT_H_

/*
 * ZigZag decode an unsigned long
 */
#define ZZ_DECODE(n) (((n) >> 1) ^ (-((n) & 1)))

/*
 * Decode an unsigned varint into smile_zzvarint_decode
 *
 * TODO: cache state->read
 */
#define VARINT_DECODE() \
    do { \
        smile_zzvarint_decode = 0; \
        /* Check we have enough bytes in the input buffer */ \
        if (have <= 0) { \
            goto out; \
        } \
        while(!(*next & 0x80) && have > 0) { \
            smile_zzvarint_decode <<= 7; \
            smile_zzvarint_decode |= *next; \
            /* Update pointer to input buffer */ \
            next++; \
            /* Update number of bytes left in the current input buffer */ \
            have--; \
            /* Update the number of bytes read */ \
            state->read++; \
        } \
        /* Check we actually reached the end of the varint */ \
        if (!(*next & 0x80)) { \
            goto out; \
        } \
        /* Last byte only has 6 payload bits */ \
        smile_zzvarint_decode <<= 6; \
        smile_zzvarint_decode |= (*next & 0x3F); \
        /* Update pointer to input buffer */ \
        next++; \
        /* Update number of bytes left in the current input buffer */ \
        have--; \
        /* Update the number of bytes read */ \
        state->read++; \
    } while (0);

/*
 * Decode a signed varint (int) into the output buffer
 *
 * If there isn't enough bytes in the output buffer, return from smile_decode,
 * saving enough state to be able to resume
 *
 * TODO: make it actually re-entrant
 */
#define ZZVARINT_DECODE_INT() \
    do { \
        VARINT_DECODE(); \
        COPY_INT(ZZ_DECODE(smile_zzvarint_decode)); \
    } while (0);

/*
 * Decode a signed varint (long) into the output buffer
 *
 * If there isn't enough bytes in the output buffer, return from smile_decode,
 * saving enough state to be able to resume
 *
 * TODO: make it actually re-entrant
 */
#define ZZVARINT_DECODE_LONG() \
    do { \
        VARINT_DECODE(); \
        COPY_LONG(ZZ_DECODE(smile_zzvarint_decode)); \
    } while (0);

#endif
