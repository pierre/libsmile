include libsmile.mk

PROG    = unsmile

LIB_OBJS = $(CURDIR)/lib/decode.o $(CURDIR)/lib/block.o
LIB_H    = $(CURDIR)/lib/decode.h $(CURDIR)/api/smile.h
LIB_FILE = libsmile.a

all: $(PROG) $(LIB_FILE)

$(LIB_FILE): $(LIB_OBJS)
	$(LIBTOOL) -o $@ $(LIB_OBJS)

UNSMILE_LIBS = $(CURDIR)/tools/unsmile.c $(CURDIR)/tools/usage.o
.PHONY: unsmile
unsmile: $(LIB_FILE) $(UNSMILE_LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(UNSMILE_LIBS) $(LIB_OBJS)
.PHONY: check
check: unsmile
	@$(CURDIR)/test/test_data.sh

.PHONY: clean
clean:
	@find . \
		\( -name '*.[oas]' -o -name '.*.cmd' \
		-o -name '.*.d' -o -name '.*.tmp' \
		-o -name '.tmp_*.o.*' \) -type f -print | xargs rm -f

# Extensions

PHP_DIR = $(CURDIR)/php

libsmile-php: $(LIB_FILE)
	@cd $(PHP_DIR) && \
		phpize && \
		./configure --enable-libsmile && \
		$(MAKE)

install-php:
	cp -f $(PHP_DIR)/modules/libsmile.so `php-config --extension-dir`

clean-php:
	@cd $(PHP_DIR) && \
		phpize --clean
