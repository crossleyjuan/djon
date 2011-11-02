# -------------------------------------------------
# Project created by QtCreator 2010-08-19T11:35:54
# -------------------------------------------------
QT += xml
# QT -= gui
TARGET = djon-util
TEMPLATE = lib

linux-g++:LIBS += -lXss
macx-g++:LIBS += -L/usr/X11R6/lib -lXss -lX11 -framework IOKit -framework Carbon

INCLUDEPATH += . \
    C:\development\boost_1_47_0


DESTDIR = ../libs
DEFINES += DJONUTIL_LIBRARY
SOURCES += util.cpp \
    util/stringfunctions.cpp \
    util/fileutil.cpp \
    util/errorHandle.cpp \
    util/dtime.cpp \
    util/DateTime.cpp \
    util/version.cpp \
    util/calendar.cpp \
    util/fileinputstream.cpp \
    util/debuginputstream.cpp \
    util/outputstream.cpp \
    util/logger.cpp
HEADERS += djon-util_global.h \
    util.h \
    util/fileutil.h \
    util/errorHandle.h \
    util/debuginputstream.h \
    util/fileinputstream.h \
    util/inputstream.h \
    util/outputstream.h \
    util/dtime.h \
    util/DateTime.h \
    util/stringfunctions.h \
    util/version.h \
    util/calendar.h \
    config.h \
    util/logger.h
