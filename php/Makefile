PHP_INCLUDE_PATH = $(HOME)/brew/Cellar/php/5.3.8/include/php
# Output of `php-config --includes`
PHP_INCLUDES = -I$(PHP_INCLUDE_PATH) -I$(PHP_INCLUDE_PATH)/main -I$(PHP_INCLUDE_PATH)/TSRM -I$(PHP_INCLUDE_PATH)/Zend -I$(PHP_INCLUDE_PATH)/ext -I$(PHP_INCLUDE_PATH)/ext/date/lib

SMILE_INCLUDE = -I../include

EXTENSION = libsmile.so

all: libsmile-php

libsmile-php:
	swig $(SMILE_INCLUDE) -php libsmile.i
	$(CC) -fPIC $(PHP_INCLUDES) $(SMILE_INCLUDE) -c libsmile_wrap.c
	$(CC) -shared ../libsmile.a libsmile_wrap.o -o $(EXTENSION)

install: libsmile-php
	cp -f $(EXTENSION) `php-config --extension-dir`
