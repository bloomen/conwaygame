######################################################################
# Automatically generated by qmake (3.0) Sat Jul 9 01:41:02 2016
######################################################################

TEMPLATE = app
TARGET = conwaygame
INCLUDEPATH += . ../lib

QT += widgets

# Input
HEADERS += conwaygame_gui/game.h \
           conwaygame_gui/image.h
SOURCES += main.cpp \
           game.cpp \
           image.cpp

target.path = $$CONWAYGAME_PREFIX
INSTALLS += target

QMAKE_CLEAN += conwaygame

QMAKE_CXXFLAGS += -std=c++0x

LIBS += ../lib/.libs/libconwaygame.a
