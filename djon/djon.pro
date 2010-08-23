# #####################################################################
# Automatically generated by qmake (2.01a) Sat Jul 17 18:40:05 2010
# #####################################################################
TEMPLATE = app
TARGET = 
DEPENDPATH += . \
    data \
    view
INCLUDEPATH += . \
    data \
    view \
    ../includes
# LIBS += -lXss
QT += xml
LIBS += ../libs/djon-util.dll

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
    updatemanager.h
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
    MainWindow.ui
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
    treecombobox.cpp \
    idletaskwindow.cpp \
    view/exportdialog.cpp \
    data/exportutility.cpp \
    view/dialogsettings.cpp \
    view/projectdialog.cpp \
    import/allneticimporthandler.cpp \
    import/import.cpp \
    import/xmlerrorhandler.cpp \
    updatemanager.cpp
OTHER_FILES += djon.rc
RESOURCES += djon_rs.qrc
RC_FILE = djon.rc
