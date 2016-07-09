AM_CXXFLAGS = -I$(top_srcdir)/src/lib -I/usr/include/qt5 @AM_CXXFLAGS@
bin_PROGRAMS = conwaygame

EXTRA_DIST =

conwaygame_SOURCES = main.cpp

conwaygame_LDADD = $(top_builddir)/src/lib/.libs/libconwaygame.a -lQt5Gui -lQt5Core $(AM_LDFLAGS)
