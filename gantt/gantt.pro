# -------------------------------------------------
# Project created by QtCreator 2009-05-30T21:11:25
# -------------------------------------------------
QT -= gui
TARGET = gantt
TEMPLATE = lib
DEFINES += GANTT_LIBRARY
SOURCES += gantt.cpp \
    ganttchart.cpp \
    task.cpp
HEADERS += gantt.h \
    gantt_global.h \
    ganttchart.h \
    ../project.h \
    task.h
FORMS += ganttchart.ui
