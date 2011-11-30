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

#ifndef _SMILE_BITS_H_
#define _SMILE_BITS_H_

/*
 * Clear the input bit buffer
 *
 * This does not impact our position in the input and output buffers
 */
#define CLEAR_BITS() \
    do { \
        /* Update the bit buffer */ \
        hold = 0; \
        /* Update the number of bits in the bit buffer */ \
        bits = 0; \
    } while (0)

/*
 * Load a byte of input into the bit buffer
 *
 * If there isn't enough bytes in the input buffer, return from smile_decode,
 * saving enough state to be able to resume
 *
 * TODO: cache state->read
 */
#define PULL_BYTE() \
    do { \
        /* Check we have a least a byte in the input buffer */ \
        if (have <= 0) { \
            goto out; \
        } \
        /* Update number of bytes left in the current input buffer */ \
        have--; \
        /* Update the number of bytes read */ \
        state->read++; \
        /* Update the bit buffer */ \
        hold += (unsigned long)(*next++) << bits; \
        /* Update the number of bits in the bit buffer */ \
        bits += 8; \
    } while (0)

/*
 * Load at least n bits of input into the bit buffer
 *
 * If there isn't enough bytes in the input buffer, return from smile_decode,
 * saving enough state to be able to resume
 */
#define PULL_BITS(n) \
    do { \
        while (bits < (unsigned)(n)) { \
            PULL_BYTE(); \
        } \
    } while (0)

/*
 * Remove n bits from the bit buffer
 *
 * The caller is responsible for calling PULL_BITS(n) first
 * This does not impact our position in the input and output buffers
 */
#define DROP_BITS(n) \
    do { \
        /* Sanity check */ \
        if (bits < (unsigned)(n)) { \
            ERROR("DROP_BITS: asked to drop more bits than available"); \
        } \
        hold >>= (n); \
        bits -= (unsigned)(n); \
    } while (0)

/*
 * Return the low n bits of the bit buffer (n < 16)
 *
 * The caller is responsible for calling PULL_BITS(n) first
 * This does not impact our position in the input and output buffers
 */
#define BITS(n) \
    ((unsigned)hold & ((1U << (n)) - 1))

/*
 * Return the low 8 bits of the bit buffer
 *
 * The caller is responsible for calling PULL_BYTE() first
 * This does not impact our position in the input and output buffers
 */
#define BYTE()  BITS(8)

#endif