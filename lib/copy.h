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

#ifndef _SMILE_COPY_H_
#define _SMILE_COPY_H_

/*
 * Copy a (properly quoted) string into the output buffer
 *
 * If there isn't enough bytes in the output buffer, return from smile_decode,
 * saving enough state to be able to resume
 *
 * TODO: cache state->total variable to avoid lookups
 */
#define COPY_STRING(s) \
    do { \
        copy_string_length = strlen(s); \
        /* Check we have enough space in the output buffer */ \
        if (left < copy_string_length) { \
            goto out; \
        } \
        /* Cast here is safe enough (dump copy) */ \
        strncat((char*) put, (char*) (s), copy_string_length); \
        /* Update pointer to output buffer */ \
        put += copy_string_length; \
        /* Update total number of bytes written */ \
        state->total += copy_string_length; \
        /* Update number of bytes left in the current output buffer */ \
        left -= copy_string_length; \
    } while (0)

/*
 * Copy a signed int into the output buffer
 *
 * If there isn't enough bytes in the output buffer, return from smile_decode,
 * saving enough state to be able to resume
 */
#define COPY_INT(n) \
    do { \
        memset(copy_nb_buf, '\0', 21); \
        sprintf(copy_nb_buf, "%d", (int)(n)); \
        COPY_STRING(copy_nb_buf); \
    } while (0)

/*
 * Copy a signed long into the output buffer
 *
 * This will handle integers up to 20 digits (enough for 64 bits or 2^64 − 1)
 *
 * If there isn't enough bytes in the output buffer, return from smile_decode,
 * saving enough state to be able to resume
 */
#define COPY_LONG(n) \
    do { \
        memset(copy_nb_buf, '\0', 21); \
        sprintf(copy_nb_buf, "%ld", (long)(n)); \
        COPY_STRING(copy_nb_buf); \
    } while (0)

/*
 * Copy data from input stream into the output buffer
 *
 * If there isn't enough bytes in the output buffer, return from smile_decode,
 * saving enough state to be able to resume
 *
 * TODO: cache state->total variable to avoid lookups
 * TODO: make it actually re-entrant
 */
#define COPY_BUFFER(l) \
    do { \
        /* A bit more complex than strncpy(put, next, l); as we need to quote characters... */ \
        quote_idx = l; \
        do { \
            switch (*next) { \
                case '\n': \
                    /* printf("\\n") will print 0x5c 0x6e (and not 0x5c 0x0a) */ \
                    *put++ = '\\'; \
                    *put++ = 'n'; \
                    left -= 2; \
                    next++; \
                    break; \
                case '"': \
                    *put++ = '\\'; \
                    left--; \
                    /* Fall through */ \
                default: \
                    *put++ = *next++; \
            } \
        } while (--quote_idx != 0); \
        /* Update total number of bytes written */ \
        state->total += l; \
        /* Update number of bytes left in the current output buffer */ \
        left -= l; \
        /* Update number of bytes left in the current input buffer */ \
        have -= l; \
        /* Update the number of bytes read */ \
        state->read += l; \
    } while (0)

#define COPY_VARIABLE_LENGTH_STRING() \
    do { \
        LOOK_FOR_STRING_LENGTH(); \
        COPY_STRING("\""); \
        COPY_BUFFER(smile_value_length); \
        COPY_STRING("\""); \
        /* Drop end-of-string marker */ \
        next++; \
        have--; \
        /* Update the number of bytes read */ \
        state->read++; \
    } while(0)

#define LOOK_FOR_STRING_LENGTH() \
    do { \
        smile_value_length = 0; \
        while (*(next + smile_value_length) != 0xFC) { \
            smile_value_length++; \
        } \
    } while(0)
#endif
