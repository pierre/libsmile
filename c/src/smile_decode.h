#ifndef _SMILE_DECODE_H_
#define _SMILE_DECODE_H_

#include "api.h"

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
