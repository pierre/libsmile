AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

SUBDIRS = doc src test

TESTS = test/smile_tests

pkg:
	debuild

.PHONY: pkg
