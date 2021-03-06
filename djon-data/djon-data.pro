#-------------------------------------------------
#
# Project created by QtCreator 2011-01-09T08:51:35
#
#-------------------------------------------------

QT       = core gui

TARGET = djon-data
TEMPLATE = lib
CONFIG += staticlib

DESTDIR = ../libs

LIBS += -L../libs
LIBS += -ldjon-util

INCLUDEPATH += . \
    data \
    ../djon-util \
    C:\development\boost_1_47_0

SOURCES += \
    template.cpp \
    data.cpp \
    data/djonpreferences.cpp \
    data/duration.cpp \
    data/project.cpp \
    data/settings.cpp \
    data/task.cpp \
    data/projectreader.cpp \
    data/projectwriter.cpp \
    data/workspace.cpp \
    data/workspacereader.cpp \
    data/workspacewriter.cpp

HEADERS += \
    template.h \
    data.h \
    data/djonpreferences.h \
    data/duration.h \
    data/project.h \
    data/projectreader.h \
    data/projectwriter.h \
    data/settings.h \
    data/task.h \
    data/workspace.h \
    data/workspacereader.h \
    data/workspacewriter.h
