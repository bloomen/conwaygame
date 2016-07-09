ACLOCAL_AMFLAGS = -I m4

SUBDIRS = \
src/lib \
src/bin \
test

TESTS = \
test/unittest

EXTRA_DIST = src/bin
dist-hook:
	make clean -C src/bin
	rm -f src/bin/Makefile
