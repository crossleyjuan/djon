# -------------------------------------------------
# Project created by QtCreator 2009-05-30T21:11:25
# -------------------------------------------------
QT -= gui
TARGET = gantt
TEMPLATE = lib

# DEPENDPATH += . \
# qtc-gdbmacros
INCLUDEPATH += .
DEFINES += GANTT_LIBRARY
SOURCES += ganttchart.cpp \
    main.cpp \
    mainwindow.cpp \
    gantttask.cpp
HEADERS += gantt_global.h \
    ganttchart.h \
    main.h \
    mainwindow.h \
    gantttask.h
FORMS += ganttchart.ui \
    mainwindow.ui
