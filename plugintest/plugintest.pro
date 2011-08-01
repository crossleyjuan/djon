#-------------------------------------------------
#
# Project created by QtCreator 2011-08-01T14:18:57
#
#-------------------------------------------------

QT       += core gui

TARGET = plugintest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


DESTDIR = ../libs

INCLUDEPATH += . \
    data \
    view \
    ../djon-util \
    ../djon-plugins \
    ../djon-data \
    ../core-plugin

LIBS += -L../libs
LIBS += -ldjon-util
LIBS += -ldjon-data
LIBS += -ldjon-plugins
LIBS += -lcore-plugin
CONFIG += static
