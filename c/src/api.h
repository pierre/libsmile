#ifndef _API_H_
#define _API_H_

typedef unsigned char u8;

struct content_handler {
    const char *name;

    // Receive notification of the start of an object
    void (*start_object) ();
    // Receive notification of the end of an object
    void (*end_object) ();
    // Receive notification of the start of an array
    void (*start_array) ();
    // Receive notification of the end of an array
    void (*end_array) ();
    // Receive notification of the start of a key
    void (*start_key) ();
    // Receive notification of the end of a key
    void (*end_key) ();
    // Receive notification of the start of a value
    void (*start_value) ();
    // Receive notification of the end of a value
    void (*end_value) ();
    // Receive notification of key or value data
    void (*characters) (const u8*, int, int);
};

#endif
