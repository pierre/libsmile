topdir = .
include $(topdir)/libsmile.mk

############
# libsmile #
############

LIBSMILE_H     = $(LIB_DIR)/decode.h  $(API_DIR)/smile.h
LIBSMILE_OBJS  = $(LIB_DIR)/decode.o  $(LIB_DIR)/block.o
SLIBSMILE_OBJS = $(LIB_DIR)/decode.lo $(LIB_DIR)/block.lo
LIBSMILE       = $(CURDIR)/libsmile.la

all: $(UNSMILE) $(LIBSMILE)

$(LIBSMILE): $(SLIBSMILE_OBJS)
	$(LIBTOOL) --tag=CC --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SLIBSMILE_OBJS)

###########
# unsmile #
###########

UNSMILE      = unsmile
UNSMILE_OBJS = $(CURDIR)/tools/unsmile.o $(CURDIR)/tools/usage.o

$(UNSMILE): $(SLIBSMILE_OBJS) $(UNSMILE_OBJS)
	$(LIBTOOL) --tag=CC --mode=link $(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SLIBSMILE_OBJS) $(UNSMILE_OBJS)

check: unsmile
	@$(TEST_DIR)/test.sh

clean: clean-php
	@find . \
		\( -name '*.[oas]' -o -name '.*.cmd' \
		-o -name '.*.d' -o -name '.*.tmp' -o -name '*.lo' -o -name '.tmp_*.o.*' \
		-o -name $(UNSMILE) -o -name `basename $(LIBSMILE)` \) -type f -print | xargs rm -f && \
		rm -rf $(UNSMILE).dSYM $(CURDIR)/.libs


#################
# PHP Extension #
#################

PHP_LIBSMILE = $(PHP_DIR)/modules/libsmile.so

php: $(PHP_LIBSMILE)

$(PHP_LIBSMILE): $(LIBSMILE)
	@cd $(PHP_DIR) && \
		phpize && \
		./configure --enable-libsmile && \
		$(MAKE)

check-php:
	$(PHP) -c $(TEST_DIR) $(TEST_DIR)/test.php

install-php:
	@cp -f $(PHP_LIBSMILE) `php-config --extension-dir`

clean-php:
	@cd $(PHP_DIR) && \
		phpize --clean && \
		rm -f config.h.in~


##################
# Ruby Extension #
##################

RUBY_LIBSMILE = $(RUBY_DIR)/libsmile.bundle

ruby: $(RUBY_LIBSMILE)

$(RUBY_LIBSMILE): $(LIBSMILE)
	@cd $(RUBY_DIR) && \
		$(RUBY) extconf.rb &&
		$(MAKE)

check-ruby:
	$(RUBY) $(TEST_DIR)/test.rb

install-ruby:

clean-ruby:
	@cd $(RUBY_DIR) && \
		$(MAKE) clean && \
		rm -f Makefile
