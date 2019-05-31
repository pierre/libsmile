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

#ifndef _SMILE_ERROR_H_
#define _SMILE_ERROR_H_

#define ENOSYS      38  /* Function not implemented */
#define EPROTO      71  /* Protocol error */
#define EILSEQ      84  /* Illegal byte sequence */


/*
 * Report an error and exit
 *
 * This forces a return from smile_decode, but since the state is
 * set to BAD, clients are expected to reset the decoder
 *
 * TODO: don't print the message, store it into strm->msg
 */
#define ERROR_REPORT(prefix, msg, error) \
    do { \
        /* TODO Should go in strm->msg */ \
        state->mode = BAD; \
        ret = error; \
        goto out; \
    } while (0)

#define ERROR(s)            ERROR_REPORT("ERROR", s, -EPROTO)
#define RESERVED(s)         ERROR_REPORT("RESERVED", s, -EILSEQ)
#define NOT_IMPLEMENTED(s)  ERROR_REPORT("NOT IMPLEMENTED", s, -ENOSYS)

#endif
