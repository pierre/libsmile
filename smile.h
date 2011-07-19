#ifndef _SMILE_H_
#define _SMILE_H_

typedef unsigned char u8;

#define DEBUG_BYTE(byte) printf("0x%02x ", (u8) byte);

#define SUCCESS 0

#define BUFFER_SIZE 1024

// Smile Tokens
#define SMILE_EOS (u8) 0xFF
#define SMILE_EMPTY_STRING (u8) 0x20
#define SMILE_NULL (u8) 0x21
#define SMILE_TRUE (u8) 0x22
#define SMILE_FALSE (u8) 0x23

#define SMILE_START_ARRAY (u8) 0xF8
#define SMILE_END_ARRAY (u8) 0xF9
#define SMILE_START_OBJECT (u8) 0xFA
#define SMILE_END_OBJECT (u8) 0xFB

#endif /* !_SMILE_H_ */
