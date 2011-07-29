#-------------------------------------------------
#
# Project created by QtCreator 2011-07-28T14:18:52
#
#-------------------------------------------------

QT       += gui

TARGET = djon-plugins
TEMPLATE = lib

DEFINES += DJONPLUGINS_LIBRARY

DESTDIR = ../libs

LIBS += -L../libs
LIBS += -ldjon-util
LIBS += -ldjon-data
# CONFIG += static

INCLUDEPATH += . \
    ../djon-data \
    ../djon-util


SOURCES += \
    pluginmanager.cpp
#    plugins/WindowPlugin.cpp

HEADERS +=\
        djon-plugins_global.h \
    pluginmanager.h \
    plugins/WindowPlugin.h
