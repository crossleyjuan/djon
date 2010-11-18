# #####################################################################
# Automatically generated by qmake (2.01a) Sat Jul 17 18:40:05 2010
# #####################################################################
TEMPLATE = app
TARGET = 
DEPENDPATH += . \
    data \
    view
DESTDIR = ../libs
INCLUDEPATH += . \
    data \
    view \
    ../djon-util \
    ../includes

include(../qtsingleapplication-2.6_1-opensource/src/qtsingleapplication.pri)
QT += xml \
    webkit
LIBS += -L../libs
LIBS += -ldjon-util
CONFIG += static

# LIBS += -lRpcrt4
# Input
HEADERS += data.h \
    globaldefs.h \
    MainWindow.h \
    taskdelegate.h \
    taskdialog.h \
    taskitem.h \
    tasklogmodel.h \
    tasklogwindow.h \
    template.h \
    data/project.h \
    data/task.h \
    view/TaskModel.h \
    view/taskheaderview.h \
    idledetector.h \
    timetracker.h \
    tasklogdelegate.h \
    currenttime.h \
    view/projectwizard.h \
    view/projectinfo.h \
    view/projectsummary.h \
    view/taskcombobox.h \
    treecombobox.h \
    idletaskwindow.h \
    view/exportdialog.h \
    data/exportutility.h \
    view/dialogsettings.h \
    view/projectdialog.h \
    import/allneticimporthandler.h \
    import/import.h \
    import/xmlerrorhandler.h \
    updatemanager.h \
    view/ganttscene.h \
    view/ganttview.h \
    view/userpreferencescontroller.h \
    data/djonpreferences.h \
    data/duration.h \
    view/durationcontrol.h \
    view/releasenotesview.h \
    view/systrayicon.h \
    workingdetector.h \
    view/workingdetectionwindow.h \
    view/trackcontrolwindow.h \
    view/treecombobox.h \
    data/settings.h \
    preferences.h \
    view/taskclosefiltermodel.h \
    view/taskeditordelegate.h \
    view/tasktreeview.h \
    view/abstracttaskfilter.h \
    view/closedtaskfilter.h \
    view/logworkreportview.h \
    view/logworkreportscence.h
FORMS += MainWindow.ui \
    taskdialog.ui \
    tasklogwindow.ui \
    currenttime.ui \
    view/projectwizard.ui \
    view/projectinfo.ui \
    view/projectsummary.ui \
    idletaskwindow.ui \
    view/exportdialog.ui \
    view/dialogsettings.ui \
    view/projectdialog.ui \
    MainWindow.ui \
    view/releasenotesview.ui \
    view/workingdetectionwindow.ui \
    view/trackcontrolwindow.ui \
    view/treecombobox.ui
SOURCES += data.cpp \
    main.cpp \
    MainWindow.cpp \
    taskdelegate.cpp \
    taskdialog.cpp \
    taskitem.cpp \
    tasklogmodel.cpp \
    tasklogwindow.cpp \
    template.cpp \
    data/project.cpp \
    data/task.cpp \
    view/TaskModel.cpp \
    view/taskheaderview.cpp \
    idledetector.cpp \
    timetracker.cpp \
    tasklogdelegate.cpp \
    currenttime.cpp \
    view/projectwizard.cpp \
    view/projectinfo.cpp \
    view/projectsummary.cpp \
    view/taskcombobox.cpp \
    idletaskwindow.cpp \
    view/exportdialog.cpp \
    data/exportutility.cpp \
    view/dialogsettings.cpp \
    view/projectdialog.cpp \
    import/allneticimporthandler.cpp \
    import/import.cpp \
    import/xmlerrorhandler.cpp \
    updatemanager.cpp \
    view/ganttscene.cpp \
    view/ganttview.cpp \
    view/userpreferencescontroller.cpp \
    data/djonpreferences.cpp \
    data/duration.cpp \
    view/durationcontrol.cpp \
    view/releasenotesview.cpp \
    view/systrayicon.cpp \
    workingdetector.cpp \
    view/workingdetectionwindow.cpp \
    view/trackcontrolwindow.cpp \
    view/treecombobox.cpp \
    data/settings.cpp \
    preferences.cpp \
    view/taskclosefiltermodel.cpp \
    view/taskeditordelegate.cpp \
    view/tasktreeview.cpp \
    view/closedtaskfilter.cpp \
    view/logworkreportview.cpp \
    view/logworkreportscence.cpp
OTHER_FILES += djon.rc
RESOURCES += djon_rs.qrc
RC_FILE = djon.rc
