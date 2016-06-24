AM_CXXFLAGS = -I$(top_srcdir)/src/lib -I/usr/include/qt4 @AM_CXXFLAGS@
bin_PROGRAMS = conwaygame

EXTRA_DIST =

conwaygame_SOURCES = main.cpp

conwaygame_LDADD = $(top_builddir)/src/lib/.libs/libconwaygame.a -lQtGui $(AM_LDFLAGS)
