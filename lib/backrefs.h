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

#ifndef _SMILE_BACKREFS_H_
#define _SMILE_BACKREFS_H_

/*
 * Save the next l bytes from the input stream as a shared key reference
 *
 * TODO: respect reference length of 1024 (10 bits)
 * TODO: cache state variables to avoid lookups
 */
#define SAVE_KEY_STRING(l) \
    do { \
        state->max_keys_ref_value++; \
        strncpy(state->keys_tables[state->max_keys_ref_value], next, l); \
        state->keys_tables[state->max_keys_ref_value][l] = '\0'; \
    } while(0)

/*
 * Save the next l bytes from the input stream as a shared value reference
 *
 * TODO: respect reference length of 1024 (10 bits)
 * TODO: cache state variables to avoid lookups
 */
#define SAVE_VALUE_STRING(l) \
    do { \
        state->max_values_ref_value++; \
        strncpy(state->values_tables[state->max_values_ref_value], next, l); \
        state->values_tables[state->max_values_ref_value][l] = '\0'; \
    } while(0)

/*
 * Handle the following bytes in the input buffer as a key String:
 *  - save the key in reference buffer if needed
 *  - copy the key to the output buffer
 *
 * This assumes the length of the string has already been computed
 * and stored in smile_key_length
 */
#define COPY_KEY_STRING() \
    do { \
        if (state->hdr.shared_key_names) { \
            SAVE_KEY_STRING(smile_key_length); \
        } \
        COPY("\""); \
        COPY_BUFFER(smile_key_length); \
        COPY("\":"); \
    } while(0)

/*
 * Handle the following bytes in the input buffer as a value String:
 *  - save the value in reference buffer if needed
 *  - copy the value to the output buffer
 *
 * This assumes the length of the string has already been computed
 * and stored in smile_value_length
 */
#define COPY_VALUE_STRING() \
    do { \
        if (state->hdr.shared_value_names) { \
            SAVE_VALUE_STRING(smile_value_length); \
        } \
        COPY("\""); \
        COPY_BUFFER(smile_value_length); \
        COPY("\""); \
    } while(0)

/*
 * Copy a shared key String to the output buffer
 */
#define COPY_SHARED_KEY_STRING() \
    do { \
        if (!state->hdr.shared_key_names) { \
            ERROR("Cannot lookup shared key, sharing disabled!"); \
        } \
        COPY("\""); \
        COPY(state->keys_tables[BITS(8) - 0x40]); \
        COPY("\":"); \
    } while(0)

/*
 * Copy a shared value String to the output buffer
 */
#define COPY_SHARED_VALUE_STRING() \
    do { \
        if (!state->hdr.shared_value_names) { \
            ERROR("Cannot lookup shared value, sharing disabled!"); \
        } \
        COPY("\""); \
        COPY(state->values_tables[BITS(10) - 1]); \
        COPY("\""); \
    } while(0)

#endif