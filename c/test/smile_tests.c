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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    clock_t start, end;

    printf("-------------------------------------------------------\n");
    printf(" T E S T S\n");
    printf("-------------------------------------------------------\n");
    printf("Running TestSuite\n");

    start = clock();

    test_varint();
    test_zigzag();
    test_decode();

    end = clock();
    printf("Tests run: %d, Time elapsed: %f sec\n", 3, ((double) (end - start)) / CLOCKS_PER_SEC);
    exit(0);
}
