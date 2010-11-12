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

// Smile Stream
#define SMILE_EOS = 0xFF;

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
    return 0;
}

// returns SMILE_EOS at end of stream
static int ss_getc(unsigned char* c, smile_stream *s) {
    *c = s->buffer[s->location++];
    return 0;
}



// Parser

typedef struct {
    unsigned char flags;
} parser_state;

static int consume_header(parser_state *ps, smile_stream *s) {
    unsigned char c;
    int rv;
    if ((rv = ss_getc(&c, s)) || c != ':') return rv;
    if ((rv = ss_getc(&c, s)) || c != ')') return rv;
    if ((rv = ss_getc(&c, s)) || c != '\n') return rv;
    if ((rv = ss_getc(&c, s))) return rv;
    
    ps->flags = c;
    return rv;
}

int smile_parse(smile_stream *stream) {
    int rv;
    parser_state ps;
    if (!(rv = consume_header(&ps, stream))) {
        printf("noooooo\n");
        return rv;
    }
    
    return 0;
}

int main()
{    
    smile_stream *c = smile_stream_create(":)\na");
    
    if (!smile_parse(c)) {
        printf("failure\n");
    }
    else {
        printf("success\n");
    }
}