#ifndef _SMILE_DECODE_H_
#define _SMILE_DECODE_H_

#include "api.h"
#include <string.h>

#ifdef DEBUG
#define dprintf   printf
#else
#define dprintf(fmt, ...)   ((void)(0))
#endif

#define DEBUG_BYTE(byte) printf("0x%02x ", (u8) byte);

#define SUCCESS 0

#define BUFFER_SIZE 1024

// Smile Tokens
#define SMILE_EOS (u8) 0xFE
#define SMILE_EMPTY_STRING (u8) 0x20
#define SMILE_NULL (u8) 0x21
#define SMILE_TRUE (u8) 0x22
#define SMILE_FALSE (u8) 0x23

#define SMILE_START_ARRAY (u8) 0xF8
#define SMILE_END_ARRAY (u8) 0xF9
#define SMILE_START_OBJECT (u8) 0xFA
#define SMILE_END_OBJECT (u8) 0xFB

/**
 * NOT_AVAILABLE can be returned if {@link JsonParser}
 * implementation can not currently return the requested
 * token (usually next one), or even if any will be
 * available, but that may be able to determine this in
 * future. This is the case with non-blocking parsers --
 * they can not block to wait for more data to parse and
 * must return something.
 */
#define JSON_TOKEN_NOT_AVAILABLE 0x0

/**
 * START_OBJECT is returned when encountering '{'
 * which signals starting of an Object value.
 */
#define JSON_TOKEN_START_OBJECT 0x1

/**
 * START_OBJECT is returned when encountering '}'
 * which signals ending of an Object value
 */
#define JSON_TOKEN_END_OBJECT 0x2
/**
 * START_OBJECT is returned when encountering '['
 * which signals starting of an Array value
 */
#define JSON_TOKEN_START_ARRAY 0x3

/**
 * START_OBJECT is returned when encountering ']'
 * which signals ending of an Array value
 */
#define JSON_TOKEN_END_ARRAY 0x4

/**
 * FIELD_NAME is returned when a String token is encountered
 * as a field name (same lexical value, different function)
 */
#define JSON_TOKEN_FIELD_NAME 0x5

/**
 * Placeholder token returned when the input source has a concept
 * of embedded Object that are not accessible as usual structure
 * (of starting with {@link #START_OBJECT}, having values, ending with
 * {@link #END_OBJECT}), but as "raw" objects.
 *<p>
 * Note: this token is never returned by regular JSON readers, but
 * only by readers that expose other kinds of source (like
 * {@link JsonNode}-based JSON trees, Maps, Lists and such).
 *
 * @since 1.1
 */
#define JSON_TOKEN_VALUE_EMBEDDED_OBJECT 0x6

/**
 * VALUE_STRING is returned when a String token is encountered
 * in value context (array element, field value, or root-level
 * stand-alone value)
 */
#define JSON_TOKEN_VALUE_STRING 0x7

/**
 * VALUE_NUMBER_INT is returned when an integer numeric token is
 * encountered in value context: that is, a number that does
 * not have floating point or exponent marker in it (consists
 * only of an optional sign, followed by one or more digits)
 */
#define JSON_TOKEN_VALUE_NUMBER_INT 0x8

/**
 * VALUE_NUMBER_INT is returned when a numeric token other
 * that is not an integer is encountered: that is, a number that does
 * have floating point or exponent marker in it, in addition
 * to one or more digits.
 */
#define JSON_TOKEN_VALUE_NUMBER_FLOAT 0x9

/**
 * VALUE_TRUE is returned when encountering literal "true" in
 * value context
 */
#define JSON_TOKEN_VALUE_TRUE 0xA

/**
 * VALUE_FALSE is returned when encountering literal "false" in
 * value context
 */
#define JSON_TOKEN_VALUE_FALSE 0xB

/**
 * VALUE_NULL is returned when encountering literal "null" in
 * value context
 */
#define JSON_TOKEN_VALUE_NULL 0xC

#define CONTEXT_UNKNOWN 0x0
#define CONTEXT_ROOT 0x1
#define CONTEXT_ARRAY 0x2
#define CONTEXT_OBJECT 0x3

struct parser_context {
    struct parser_context* parent;
    struct parser_context* child;

    // Field name
    char name[64];
    // Offset in bytes
    int location;

    // Context type (object, array, ...)
    int contextType;
    // Token type (key, value string, value int, ...)
    int jsonToken;
};


struct smile_header {
    // Whether the header is valid
    int valid;

    int version;
    int raw_binary;
    int shared_key_names;
    int shared_value_names;
};

struct smile_header smile_decode_header(u8*);
int smile_decode_key(u8**, struct content_handler*);
int smile_decode_value(u8**, struct content_handler*);
void smile_decode(u8*, int, struct content_handler*);

#endif /* !_SMILE_DECODE_H_ */
