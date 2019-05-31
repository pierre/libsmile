LIBSMILE_VERSION = 0.2

MAKEFLAGS += -rR --no-print-directory
MAKE      += -r

# Avoid funny character set dependencies
unexport LC_ALL
LC_COLLATE=C
LC_NUMERIC=C
export LC_COLLATE LC_NUMERIC

UNAME := $(shell uname)

BINDIR   = /usr/bin
SBINDIR  = /sbin
LIBDIR   = /usr/lib
DATADIR  = /usr/share
MANDIR   = /usr/man
INCDIR   = /usr/include

NASM     = nasm
NASMOPT  = -Ox
AR       = ar
NM       = nm
RANLIB   = ranlib
OBJDUMP  = objdump
OBJCOPY  = objcopy
STRIP    = strip
CC       = gcc
LD       = ld
ifeq ($(UNAME), Darwin)
LIBTOOL  = glibtool
CC       = gcc-9
else
LIBTOOL  = libtool
endif

SMILE_INCLUDE = -I$(CURDIR)/include
CFLAGS   += -std=c99 -pedantic -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -I. $(SMILE_INCLUDE) -g
LDFLAGS  += -g

PHP      = php
RUBY     = ruby

API_DIR   = $(CURDIR)/api
LIB_DIR   = $(CURDIR)/lib
TEST_DIR  = $(CURDIR)/test
PHP_DIR   = $(CURDIR)/php
RUBY_DIR  = $(CURDIR)/ruby

# Common stanza to make gcc generate .*.d dependency files
MAKEDEPS = -Wp,-MT,$@,-MD,$(dir $@).$(notdir $@).d

# Dependencies that exclude system headers; use whenever we use
# header files from the platform.
UMAKEDEPS = -Wp,-MT,$@,-MMD,$(dir $@).$(notdir $@).d

# DEBUG=1 make
ifdef DEBUG
  CFLAGS += -DDEBUG
endif

%.o: %.c
	$(CC) $(UMAKEDEPS) $(CFLAGS) -c $< -o $@

%.lo: %.c
	$(LIBTOOL) --tag=CC --mode=compile $(CC) $(UMAKEDEPS) $(CFLAGS) -c $< -o $@
