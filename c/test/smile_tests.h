#ifndef _SMILE_TESTS_H_
#define _SMILE_TESTS_H_

#define ASSERT_EQUAL(expected, actual) \
    if (expected != actual) { \
        fprintf (stderr, "Test failure: expected %ld (%c), got %ld (%c)\n", (long) expected, (char) expected, (long) actual, (char) actual); \
        exit(1); \
    } else { \
        pass++; \
    }

#endif
