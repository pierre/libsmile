include $(CURDIR)/../libsmile.mk

EXTENSION = libsmile.so

all: libsmile-ruby

# Need a stupid subdir because extconf.rb writes a Makefile
libsmile-ruby:
	cd build && \
		swig $(SMILE_INCLUDE) -ruby libsmile.i && \
		$(RUBY) extconf.rb && \
		$(MAKE)

clean:
	pushd build && \
		$(MAKE) clean && \
		popd && \
		rm build/libsmile_wrap.c \
		rm -f build/Makefile
