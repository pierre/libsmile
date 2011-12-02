SMILE_INCLUDE = -I../../include

EXTENSION = libsmile.so

all: libsmile-ruby

# Need a stupid subdir because extconf.rb writes a Makefile
libsmile-ruby:
	cd build && \
		swig $(SMILE_INCLUDE) -ruby libsmile.i && \
		ruby extconf.rb && \
		make

clean:
	pushd build && \
		make clean && \
		popd && \
		rm build/libsmile_wrap.c \
		rm -f build/Makefile
