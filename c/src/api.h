#ifndef _SMILE_API_H_
#define _SMILE_API_H_

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

    // Receive notification of null value data
    void (*null_value) ();
    // Receive notification of false value data
    void (*false_value) ();
    // Receive notification of true value data
    void (*true_value) ();
    // Receive notification of number value data
    void (*number_value) (long);

    // Receive notification of key or value data (others)
    void (*characters) (const u8*, int, int);
};

#endif
