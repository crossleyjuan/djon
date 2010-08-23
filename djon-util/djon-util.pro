#-------------------------------------------------
#
# Project created by QtCreator 2010-08-19T11:35:54
#
#-------------------------------------------------

QT       += xml

QT       -= gui

TARGET = djon-util
TEMPLATE = lib

DEFINES += DJONUTIL_LIBRARY

INCLUDEPATH += ../includes

SOURCES += \
    util.cpp \
    util/stringfunctions.cpp \
    util/fileutil.cpp \
    util/errorHandle.cpp \
    util/dtime.cpp \
    util/DateTime.cpp

HEADERS +=\
        djon-util_global.h \
    util.h \
    util/fileutil.h \
    util/errorHandle.h \
    util/dtime.h \
    util/DateTime.h \
    util/stringfunctions.h
