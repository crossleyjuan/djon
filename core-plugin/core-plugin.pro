#-------------------------------------------------
#
# Project created by QtCreator 2011-07-28T17:48:57
#
#-------------------------------------------------

QT       += gui

# TARGET = core-plugin
TARGET        = $$qtLibraryTarget(core-plugin)
TEMPLATE = lib
CONFIG       += plugin

DESTDIR = ../libs
DEFINES += COREPLUGIN_LIBRARY

INCLUDEPATH += . \
    ../djon-plugins \
    ../djon-util \
    ../djon-data

LIBS += -L../libs
LIBS += -ldjon-util
LIBS += -ldjon-data
LIBS += -ldjon-plugins

SOURCES += coreplugin.cpp \
    dialog.cpp

HEADERS += coreplugin.h\
        core-plugin_global.h \
    dialog.h

FORMS += \
    dialog.ui
