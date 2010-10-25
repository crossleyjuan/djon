#-------------------------------------------------
#
# Project created by QtCreator 2010-08-19T11:08:26
#
#-------------------------------------------------

QT       += core gui

TARGET = updater
TEMPLATE = app
DESTDIR = ../libs

INCLUDEPATH += . \
    ../includes

LIBS += ../libs/djon-util.dll

SOURCES += main.cpp \
    Local.cpp

HEADERS  += \
    Local.h

OTHER_FILES += \
    djon.rc \
    update.exe.manifest
RC_FILE = djon.rc
