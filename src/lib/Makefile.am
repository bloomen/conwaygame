# the library names to build
lib_LTLIBRARIES = libconwaygame.la

libconwaygame_la_LDFLAGS = -version-info 0:0:0 -no-undefined

# where to install the headers on the system
libconwaygame_ladir = $(includedir)

# the list of header files that belong to the library (to be installed later)
libconwaygame_la_HEADERS = \
conwaygame/rules.h

EXTRA_DIST = \
conwaygame/expects.h \
conwaygame/cxxpool.h

# the sources to add to the library and to add to the source distribution
libconwaygame_la_SOURCES = \
rules.cpp
