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

#ifndef PHP_LIBSMILE_H
#define PHP_LIBSMILE_H

extern zend_module_entry libsmile_module_entry;
#define phpext_libsmile_ptr &libsmile_module_entry

#ifdef PHP_WIN32
#define PHP_LIBSMILE_API __declspec(dllexport)
#else
#define PHP_LIBSMILE_API
#endif

#include <smile.h>

PHP_MINIT_FUNCTION(libsmile);
PHP_MSHUTDOWN_FUNCTION(libsmile);
PHP_MINFO_FUNCTION(libsmile);

PHP_FUNCTION(libsmile_decode);

#endif  /* PHP_LIBSMILE_H */
