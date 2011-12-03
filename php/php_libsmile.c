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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_libsmile.h"

/* {{{ libsmile_functions[]
 *
 * Every user visible function must have an entry in libsmile_functions[].
 */
zend_function_entry libsmile_functions[] = {
    PHP_FE(libsmile_decode, NULL)
    {NULL, NULL, NULL}    /* Must be the last line in libsmile_functions[] */
};
/* }}} */

/* {{{ libsmile_module_entry
 */
zend_module_entry libsmile_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "libsmile",
    libsmile_functions,
    PHP_MINIT(libsmile),
    PHP_MSHUTDOWN(libsmile),
    NULL,
    NULL,
    PHP_MINFO(libsmile),
#if ZEND_MODULE_API_NO >= 20010901
    "0.2",
#endif
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_LIBSMILE
ZEND_GET_MODULE(libsmile)
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(libsmile)
{
    smile_decode_block_init();
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(libsmile)
{
    smile_decode_block_exit();
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(libsmile)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "libsmile support", "enabled");
    php_info_print_table_end();
}
/* }}} */


#define BUFFER_SIZE 65536

PHP_FUNCTION(libsmile_decode)
{
    zval *zinput;

    char *input;
    int len;
    char result[BUFFER_SIZE];

    int num_args = ZEND_NUM_ARGS();

    if (num_args != 1 || !zend_parse_parameters(num_args TSRMLS_CC, "z", &zinput) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    if (Z_TYPE_P(zinput) != IS_STRING) {
        convert_to_string(zinput);
    }

    input = Z_STRVAL_P(zinput);
    len = Z_STRLEN_P(zinput);

    smile_decode_block(result, BUFFER_SIZE, input, len);
    RETVAL_STRINGL(result, strlen(result), 1);
}
