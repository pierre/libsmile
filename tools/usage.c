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

#include "usage.h"

void usage(const char *err)
{
    fprintf(stderr, "usage: %s\n", err);
    exit(1);
}

static void report(const char *prefix, const char *err, va_list params)
{
    fputs(prefix, stderr);
    vfprintf(stderr, err, params);
    fputs("\n", stderr);
}

int error(const char *err, ...)
{
    va_list params;

    va_start(params, err);
    report("error: ", err, params);
    va_end(params);
    return -1;
}

void die(const char *err, ...)
{
    va_list params;

    va_start(params, err);
    report("fatal: ", err, params);
    va_end(params);
    exit(1);
}

#ifdef DEBUG
void dprintf(const char *debug, ...)
{
    va_list params;

    va_start(params, debug);
    fputs("DEBUG: ", stdout);
    vfprintf(stdout, debug, params);
    fputs("\n", stdout);
    va_end(params);
}
#endif
