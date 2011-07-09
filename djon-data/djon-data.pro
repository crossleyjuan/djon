#-------------------------------------------------
#
# Project created by QtCreator 2011-01-09T08:51:35
#
#-------------------------------------------------

QT       = core gui

TARGET = djon-data
TEMPLATE = lib
# CONFIG += staticlib

DESTDIR = ../libs

LIBS += -L../libs
LIBS += -ldjon-util

INCLUDEPATH += . \
    data \
    ../djon-util

SOURCES += \
    template.cpp \
    data.cpp \
    data/task.cpp \
    data/project.cpp \
    data/duration.cpp \
    data/settings.cpp \
    data/djonpreferences.cpp \
    data/projectwriter.cpp \
    data/outputstream.cpp \
    data/inputstream.cpp \
    data/projectreader.cpp \
    data/workspace.cpp

HEADERS += \
    template.h \
    data.h \
    data/task.h \
    data/project.h \
    data/duration.h \
    data/settings.h \
    data/djonpreferences.h \
    data/projectwriter.h \
    data/outputstream.h \
    data/inputstream.h \
    data/projectreader.h \
    data/workspace.h
