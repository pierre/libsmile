#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "api.h"
#include "printer.h"
#include "smile.h"

#define INDENT printf("%-*s", indent, " ");

static int indent = 0;

inline void print_start_object()
{
    INDENT
    putchar('{');
    putchar('\n');
    indent += 2;
}

inline void print_end_object()
{
    putchar('}');
}

inline void print_start_array()
{
    INDENT
    printf("bleh");
    putchar('[');
    putchar('\n');
    indent += 2;
}

inline void print_end_array()
{
    putchar(']');
}

inline void print_start_key()
{
    INDENT
    putchar('"');
}

inline void print_end_key()
{
    putchar('"');
    putchar(':');
    putchar(' ');
}

inline void print_start_value()
{
    putchar('"');
}

inline void print_end_value()
{
    putchar('"');
    putchar(',');
    putchar('\n');
}

void print_value(const u8* ch, int start, int length)
{
    const u8* ip = ch + start;
    while (length--) {
        putchar(*ip++);
    }
}

struct content_handler printer = {
    .name = "PRINTER",
    .start_object = print_start_object,
    .end_object = print_end_object,
    .start_array = print_start_array,
    .end_array = print_end_array,
    .start_key = print_start_key,
    .end_key = print_end_key,
    .start_value = print_start_value,
    .end_value = print_end_value,
    .characters = print_value
};

// Decodes raw smile
int main(int argc, char *argv[])
{
    int fd, rc = 0;
    struct stat mystat;
    const char* fname = argv[1];
    size_t nbytes;
    ssize_t bytes_read;
    u8 buf[BUFFER_SIZE];
    u8 header[4];

    nbytes = sizeof(buf);

    rc = stat(fname, &mystat);
    fd = open(fname, O_RDONLY);

    if (fd == -1) {
        fprintf (stderr, "%s: not a regular file.\n", fname);
        exit(1);
    }


    bytes_read = read(fd, header, 4);
    if (bytes_read != 4) {
          fprintf(stderr, "%s: unable to read header: \n", fname);
          perror ("");
          goto exit;
    } else {
        if (!smile_decode_header(header)) {
            fprintf(stderr, "%s: bad header: %s\n", fname, header);
            goto exit;
        } else {
            printf("Got header: %s", header);
        }
    }

    while (1) {
        // Read block by block
        bytes_read = read(fd, buf, nbytes);
        if (bytes_read == -1) {
          fprintf(stderr, "%s: read error: \n", fname);
          perror ("");
          goto exit;
        } else if (bytes_read == 0) {
            break;
        }

        fprintf(stderr, "Read %lu bytes\n", (unsigned long int) bytes_read);
        smile_decode(buf, nbytes, &printer);
   }

exit:
    close(fd);
    puts("Done!");
    exit(0);
}
