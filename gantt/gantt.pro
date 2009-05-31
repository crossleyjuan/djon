# -------------------------------------------------
# Project created by QtCreator 2009-05-30T21:11:25
# -------------------------------------------------
# QT -= gui
TARGET = gantt
TEMPLATE = app
DEPENDPATH += . \
    qtc-gdbmacros
INCLUDEPATH += .

# DEFINES += GANTT_LIBRARY
SOURCES += ganttchart.cpp \
    task.cpp \
    main.cpp \
    mainwindow.cpp
HEADERS += gantt_global.h \
    ganttchart.h \
    ../project.h \
    task.h \
    main.h \
    mainwindow.h
FORMS += ganttchart.ui \
    mainwindow.ui
