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

#include <stdio.h>

#define SMILE_EOS = 0xFF;

typedef struct {
    size_t location;
    size_t size;
    unsigned char *buffer;
} smile_stream;

// returns SMILE_EOS at end of stream
static int ss_getc(unsigned char* c, smile_stream *s) {
    *c = s->buffer[s->location++];
    return 0;
}


static int consume_header(smile_stream *s) {
    unsigned char c;
    int rv;
    if ((rv = ss_getc(&c, s)) || c != ':') return rv;
    if ((rv = ss_getc(&c, s)) || c != ')') return rv;
    if ((rv = ss_getc(&c, s)) || c != '\n') return rv;
    return rv;
}

int smile_parse(smile_stream *stream) {
    int rv;
    if (!(rv = consume_header(stream))) return rv;
}

int main()
{    
    unsigned char ary[4];
    ary[0] = ':';
    ary[1] = ')';
    ary[2] = '\n';
    ary[3] = 0;
    
    smile_stream c;
    c.buffer = ary;
    c.location = 0;
    
    if (!consume_header(&c)) {
        printf("success\n");
    }
    else {
        printf("failure\n");
    }
}