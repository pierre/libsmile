#ifndef _SMILE_H_
#define _SMILE_H_

typedef struct smile_stream_t smile_stream;

int smile_parse(smile_stream *stream);

smile_stream* smile_stream_create(const char* string);
int smile_stream_destroy(smile_stream* stream);

#endif /* !_SMILE_H_ */