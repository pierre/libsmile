LIBSMILE_VERSION = 0.0.1-SNAPSHOT

MAKEFLAGS += -rR --no-print-directory

# Avoid funny character set dependencies
unexport LC_ALL
LC_COLLATE=C
LC_NUMERIC=C
export LC_COLLATE LC_NUMERIC

AR      = ar
CC      = gcc
CFLAGS  = -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -I. -I$(CURDIR)/include
LDFLAGS = -g

PROG    = unsmile

LIB_OBJS = $(CURDIR)/lib/decode.o $(CURDIR)/lib/block.o
LIB_H    = $(CURDIR)/lib/decode.h $(CURDIR)/api/smile.h
LIB_FILE = libsmile.a

all: $(PROG) $(LIB_FILE)

$(LIB_FILE): $(LIB_OBJS)
	$(AR) rcs $@ $(LIB_OBJS)

UNSMILE_LIBS = $(CURDIR)/tools/unsmile.c $(CURDIR)/tools/usage.o
.PHONY: unsmile
unsmile: $(LIB_FILE) $(UNSMILE_LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(UNSMILE_LIBS) $(LIB_OBJS)

.PHONY: clean
clean:
	@find . \
		\( -name '*.[oas]' -o -name '.*.cmd' \
		-o -name '.*.d' -o -name '.*.tmp' \
		-o -name '.tmp_*.o.*' \) -type f -print | xargs rm -f
