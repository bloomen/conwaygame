TEMPLATE = app
TARGET = conwaygame

QT += widgets opengl

HEADERS += src/game.h \
           src/image.h \
           src/rules.h \
           src/expects.h \
           src/memory.h

SOURCES += src/main.cpp \
           src/game.cpp \
           src/image.cpp \
           src/rules.cpp

QMAKE_CXXFLAGS += -std=c++11
