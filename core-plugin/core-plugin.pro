#-------------------------------------------------
#
# Project created by QtCreator 2011-07-28T17:48:57
#
#-------------------------------------------------

QT       += gui sql

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

LIBS += -lodbc32
LIBS += -L../libs
LIBS += -ldjon-util
LIBS += -ldjon-data
LIBS += -ldjon-plugins

SOURCES += coreplugin.cpp \
    dialog.cpp \
    itemdelegator.cpp \
    datamanager.cpp \
    taskitemdelegator.cpp \
    fileutil.cpp \
    pluginsettings.cpp

HEADERS += coreplugin.h\
        core-plugin_global.h \
    dialog.h \
    itemdelegator.h \
    datamanager.h \
    taskitemdelegator.h \
    fileutil.h \
    core-defs.h \
    pluginsettings.h

FORMS += \
    dialog.ui \
    pluginsettings.ui
