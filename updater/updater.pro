#-------------------------------------------------
#
# Project created by QtCreator 2010-08-19T11:08:26
#
#-------------------------------------------------

QT       += core gui

TARGET = updater
TEMPLATE = app

INCLUDEPATH += . \
    ../includes

LIBS += ../libs/djon-util.dll

SOURCES += main.cpp \
    Local.cpp

HEADERS  += \
    Local.h
