AM_CXXFLAGS = -I$(top_srcdir)/src/lib @AM_CXXFLAGS@
check_PROGRAMS = unittest

EXTRA_DIST =

unittest_SOURCES = \
test_rules.cpp

unittest_LDADD = $(top_builddir)/src/lib/.libs/libconwaygame.a -lunittest $(AM_LDFLAGS)
