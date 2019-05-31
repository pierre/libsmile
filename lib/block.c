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
 * API inspired from the cramfs interface: http://kernel.org
 */

#include "decode.h"

int smile_decode_block(void *dst, int dstlen, void *src, int srclen)
{
    s_stream stream;
    stream.workspace = malloc(sizeof(struct decode_workspace));
    stream.msg = malloc(MAX_ERROR_MSG_SIZE);
    smile_decode_init(&stream);

    stream.next_in = src;
    stream.avail_in = srclen;

    stream.next_out = dst;
    stream.avail_out = dstlen;

    // Decode block
    int err = smile_decode(&stream);

    free(stream.workspace);

    if (err == -1) {
      return -EIO;
    }

    free(stream.msg);

    return stream.total_out;
}
