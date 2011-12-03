topdir = .
include $(topdir)/libsmile.mk

PROG    = unsmile

SLIB_OBJS = $(CURDIR)/lib/decode.lo $(CURDIR)/lib/block.lo
LIB_OBJS  = $(CURDIR)/lib/decode.o $(CURDIR)/lib/block.o
LIB_H     = $(CURDIR)/lib/decode.h $(CURDIR)/api/smile.h
LIB_FILE  = libsmile.la

all: $(PROG) $(LIB_FILE)

$(LIB_FILE): $(SLIB_OBJS)
	$(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SLIB_OBJS)

UNSMILE_LIBS = $(CURDIR)/tools/unsmile.c $(CURDIR)/tools/usage.o
.PHONY: unsmile
unsmile: $(LIB_OBJS) $(UNSMILE_LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(UNSMILE_LIBS) $(LIB_OBJS)

.PHONY: check
check: unsmile
	@$(TEST_DIR)/test.sh

.PHONY: clean
clean:
	@find . \
		\( -name '*.[oas]' -o -name '.*.cmd' \
		-o -name '.*.d' -o -name '.*.tmp' -o -name '*.lo' \
		-o -name '.tmp_*.o.*' \) -type f -print | xargs rm -f

# Extensions

libsmile-php: $(LIB_FILE)
	@cd $(PHP_DIR) && \
		phpize && \
		./configure --enable-libsmile && \
		$(MAKE)

test-php:
	$(PHP) -c $(TEST_DIR) $(TEST_DIR)/test.php

install-php:
	cp -f $(PHP_DIR)/modules/libsmile.so `php-config --extension-dir`

clean-php:
	@cd $(PHP_DIR) && \
		phpize --clean
