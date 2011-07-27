#ifndef _SMILE_BACKREFS_H_
#define _SMILE_BACKREFS_H_

// The encoded byte length of a String value that can be referenced is 64 bytes or less
#define MAX_SHAREABLE_STRING_LENGTH 64
#define KEYS_BUFFER_SIZE 1024
#define VALUES_BUFFER_SIZE 1024

char* lookup_short_shared_key(const u8*);
char* lookup_long_shared_key(const u8*);
void save_key_string(const u8*, size_t);

char* lookup_short_shared_value(const u8*);
char* lookup_long_shared_value(const u8*);
void save_value_string(const u8*, size_t);

#endif
