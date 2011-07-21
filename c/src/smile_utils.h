#ifndef _SMILE_UTILS_H_
#define _SMILE_UTILS_H_

#include "api.h"

#define ZZ_DECODE(n) ((n >> 1) ^ (-(n & 1)))

long varint_decode(u8**);
long varint_decode_buffer(u8*);

#endif
