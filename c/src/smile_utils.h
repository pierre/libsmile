#ifndef _SMILE_UTILS_H_
#define _SMILE_UTILS_H_

#include "api.h"

inline long zz_decode(unsigned long);
long zzvarint_decode(u8**);
unsigned long varint_decode(u8**);
unsigned long varint_decode_buffer(u8*);

#endif
