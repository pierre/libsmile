%module libsmile
%include "typemaps.i"

%{
#include <smile.h>
%}

int smile_decode_block(void *OUTPUT, int dstlen, void *INPUT, int srclen);
int smile_decode_block_init(void);
void smile_decode_block_exit(void);

