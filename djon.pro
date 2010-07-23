# #####################################################################
# Automatically generated by qmake (2.01a) Sat Jul 17 18:40:05 2010
# #####################################################################
TEMPLATE = app
TARGET = 
DEPENDPATH += . \
    data \
    util \
    view
INCLUDEPATH += . \
    data \
    util \
    view
LIBS += -lXss

# LIBS += -lRpcrt4
# Input
HEADERS += config.h \
    data.h \
    globaldefs.h \
    MainWindow.h \
    taskdelegate.h \
    taskdialog.h \
    taskitem.h \
    tasklogmodel.h \
    tasklogwindow.h \
    template.h \
    util.h \
    data/datautil.h \
    data/project.h \
    data/task.h \
    util/DateTime.h \
    util/fileutil.h \
    util/stringfunctions.h \
    view/TaskModel.h \
    view/taskheaderview.h \
    idledetector.h \
    timetracker.h \
    tasklogdelegate.h \
    util/dtime.h \
    currenttime.h \
    util/errorHandle.h \
    view/projectwizard.h \
    view/projectinfo.h \
    view/projectsummary.h \
    view/taskcombobox.h \
    treecombobox.h
FORMS += MainWindow.ui \
    taskdialog.ui \
    tasklogwindow.ui \
    currenttime.ui \
    view/projectwizard.ui \
    view/projectinfo.ui \
    view/projectsummary.ui
SOURCES += data.cpp \
    main.cpp \
    MainWindow.cpp \
    taskdelegate.cpp \
    taskdialog.cpp \
    taskitem.cpp \
    tasklogmodel.cpp \
    tasklogwindow.cpp \
    template.cpp \
    util.cpp \
    data/datautil.cpp \
    data/project.cpp \
    data/task.cpp \
    util/DateTime.cpp \
    util/fileutil.cpp \
    util/stringfunctions.cpp \
    view/TaskModel.cpp \
    view/taskheaderview.cpp \
    idledetector.cpp \
    timetracker.cpp \
    tasklogdelegate.cpp \
    util/dtime.cpp \
    currenttime.cpp \
    util/errorHandle.cpp \
    view/projectwizard.cpp \
    view/projectinfo.cpp \
    view/projectsummary.cpp \
    view/taskcombobox.cpp \
    treecombobox.cpp
OTHER_FILES += 
RESOURCES += djon_rs.qrc
