/* http://wiki.fasterxml.com/JacksonBinaryFormatSpec */
/*
Constant byte #0: 0x3A (ASCII ':')
Constant byte #1: 0x29 (ASCII ')')
Constant byte #2: 0x0A (ASCII linefeed, '\n')
Variable byte #3, consisting of bits:
Bits 4-7 (4 MSB): 4-bit version number; 0x00 for current version (note: it is possible that some bits may be reused if necessary)
Bits 3: Reserved
Bit 2 (mask 0x04) Whether raw binary (unescaped 8-bit) values may be present in content
Bit 1 (mask 0x02): Whether shared String value checking was enabled during encoding -- if header missing, default value of "false" must be assumed for decoding (meaning parser need not store decoded String values for back referencing)
Bit 0 (mask 0x01): Whether shared property name checking was enabled during encoding -- if header missing, default value of "true" must be assumed for decoding (meaning parser MUST store seen property names for possible back references)
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "smile.h"

#define SUCCESS 0

// Smile Tokens
#define SMILE_EOS 0xFF
#define SMILE_EMPTY_STRING 0x20
#define SMILE_NULL 0x21
#define SMILE_TRUE 0x22
#define SMILE_FALSE 0x23

#define SMILE_START_OBJECT 0xFA
#define SMILE_END_OBJECT 0xFB

// buffer bits

struct smile_stream_t {
    size_t location;
    size_t size;
    unsigned char *buffer;
};

smile_stream* smile_stream_create(const char* string) {
    smile_stream *s = malloc(sizeof(smile_stream));
    s->location = 0;
    s->size = strlen(string);
    s->buffer = (unsigned char*)strdup(string);
    return s;
}

int smile_stream_destroy(smile_stream* stream) {
    return SUCCESS;
}



// returns SMILE_EOS at end of stream
static int ss_getc(unsigned char* c, smile_stream *s) {
    *c = s->buffer[s->location++];
    return SUCCESS;
}

// Parser bits

typedef struct {
    unsigned char flags;
} parser_state;


static int consume_next_value(parser_state *ps, smile_stream *s);


static int consume_object(parser_state *ps, smile_stream *s) {
    
    return consume_next_value(ps, s);
}

static int consume_next_value(parser_state *ps, smile_stream *s) {
    unsigned char c;
    int rv;
    if ((rv = ss_getc(&c, s))) return rv;
    printf("TOKEN TYPE %x\n", c);
    switch (c) {
        case SMILE_EOS:
            printf("end of stream\n");
            return SUCCESS;
        case SMILE_START_OBJECT:
            printf("start object\n");
            return consume_object(ps, s);
        case SMILE_NULL:
            printf("null\n");
            return consume_next_value(ps, s);
        case SMILE_EMPTY_STRING:
            printf("empty string\n");
            return consume_next_value(ps, s);
        case SMILE_TRUE:
            printf("true\n");
            return consume_next_value(ps, s);
        case SMILE_FALSE:
            printf("true\n");
            return consume_next_value(ps, s);
        default:
            printf("DEFAULT OOPS\n");
            return 1;
    }
}

static int consume_header(parser_state *ps, smile_stream *s) {
    unsigned char c;
    int rv;
    if ((rv = ss_getc(&c, s)) || c != ':') return rv;
    if ((rv = ss_getc(&c, s)) || c != ')') return rv;
    if ((rv = ss_getc(&c, s)) || c != '\n') return rv;
    if ((rv = ss_getc(&c, s))) return rv;
    ps->flags = c;
    
    return consume_next_value(ps, s);
}

int smile_parse(smile_stream *stream) {
    int rv;
    parser_state ps;
    if ((rv = consume_header(&ps, stream))) return rv;
    
    return SUCCESS;
}


// test bits

int main()
{    
    // HEADER START_OBJECT EOS
    smile_stream *c2 = smile_stream_create(":)\nX\xFA\xFF");
    
    if (smile_parse(c2)) {
        printf("failure\n");
    }
    else {
        printf("success\n");
    }
}