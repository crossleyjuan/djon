# -------------------------------------------------
# Project created by QtCreator 2010-08-19T11:35:54
# -------------------------------------------------
QT += xml
# QT -= gui
TARGET = djon-util
TEMPLATE = lib

linux-g++:LIBS += -lXss
macx-g++:LIBS += -L/usr/X11R6/lib -lXss -lX11 -framework IOKit -framework Carbon

DESTDIR = ../libs
DEFINES += DJONUTIL_LIBRARY
INCLUDEPATH += ../includes
SOURCES += util.cpp \
    util/stringfunctions.cpp \
    util/fileutil.cpp \
    util/errorHandle.cpp \
    util/dtime.cpp \
    util/DateTime.cpp \
    util/version.cpp \
    util/calendar.cpp
HEADERS += djon-util_global.h \
    util.h \
    util/fileutil.h \
    util/errorHandle.h \
    util/dtime.h \
    util/DateTime.h \
    util/stringfunctions.h \
    util/version.h \
    util/calendar.h
