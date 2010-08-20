/*
 * File:   MainWindow.cpp
 * Author: cross
 *
 * Created on July 9, 2010, 8:50 AM
 */

#include <QtGui>
#include "MainWindow.h"
#include "view/TaskModel.h"
#include "globaldefs.h"
#include "tasklogwindow.h"
#include "taskheaderview.h"
#include "timetracker.h"
#include "currenttime.h"
#include "taskdialog.h"
#include "util.h"
#include "view/projectwizard.h"
#include "idletaskwindow.h"
#include "view/dialogsettings.h"
#include "exportdialog.h"
#include "view/projectdialog.h"
#include "import/import.h"
#ifdef WINDOWS
#include "updatemanager.h"
#endif
#include <sstream>

MainWindow::MainWindow() {
    qDebug("MainWindow::MainWindow()");
    widget.setupUi(this);
    _activeProject = NULL;
    _activeTask = NULL;
    _activeLog = NULL;
    _taskHeader = NULL;
    _projects = NULL;

    initialize();
    createTaskLogWindow();
    createCurrentTimeWindow();

    setupActions();
    setWindowState(Qt::WindowMaximized);

    widget.taskView->setColumnWidth(0, 250);
    widget.taskView->setColumnWidth(1, 70);
    widget.taskView->setColumnWidth(2, 70);
//    _idleDetector = new IdleDetector(5*60);// 5*60
    _idleDetector = new IdleDetector();// 5*60
    connect(_idleDetector, SIGNAL(idleTimeOut()), this, SLOT(idleTimeOut()));

    _timeTracker = new TimeTracker();
    connect(_timeTracker, SIGNAL(timeChanged(DTime&)), _timeWindow, SLOT(updateTime(DTime&)));
    connect(_timeTracker, SIGNAL(timeStopped(Task*,TaskLog*)), this, SLOT(timeStopped(Task*, TaskLog*)));
    connect(qApp, SIGNAL(aboutToQuit()), _timeTracker, SLOT(stopRecord()));

    createTray();

#ifdef WINDOWS
    _updateManager = new UpdateManager(this);
    _updateManager->startCheck(10);
#endif
}

void MainWindow::createTaskLogWindow() {
    qDebug("MainWindow::createTaskLogWindow()");
    _logWindow = new TaskLogWindow();
    _logWindow->setAllowedAreas(Qt::BottomDockWidgetArea);

    addDockWidget(Qt::BottomDockWidgetArea, _logWindow);
}

TaskDelegate* MainWindow::createTaskDelegate() {
    qDebug("MainWindow::createTaskDelegate()");
    TaskDelegate* delegate = new TaskDelegate(_projects);
    return delegate;
}

MainWindow::~MainWindow() {
}

void MainWindow::selectTaskChanged(QModelIndex current, QModelIndex previous) {
    qDebug("MainWindow::selectTaskChanged");
    TaskModel* model = (TaskModel*)current.model();
    Task* task = model->task(current);
    if (task != NULL) {
        setActiveTask(task);
    } else {
        Project* project = model->project(current);
        setActiveTask(NULL);
        _activeProject = project;
    }
}

void MainWindow::setupActions() {
    qDebug("MainWindow::setupActions()");
    QToolBar* bar = new QToolBar("Options");
    addToolBar(bar);

    QMenuBar* menuBar = new QMenuBar();
    _taskPopUpMenu = new QMenu(widget.taskView);

    setMenuBar(menuBar);

    QMenu* prjMenu = menuBar->addMenu(tr("Project"));
    QMenu* trcMenu = menuBar->addMenu(tr("Tracker"));
    QMenu* optMenu = menuBar->addMenu(tr("Options"));

    QAction* newProject = bar->addAction(QIcon(":/img/new-project.png"), tr("Create Project"));
    QAction* openProject = bar->addAction(QIcon(":/img/open-project.png"), tr("Open Existing Project"));
    bar->addSeparator();
    QAction* newTask = bar->addAction(QIcon(":/img/new-task.png"), tr("Create SubTask"));
    QAction* editTask = bar->addAction(QIcon(":/img/edit-task.png"), tr("Edit Task"));
    QAction* deleteTask = bar->addAction(QIcon(":/img/delete-task.png"), tr("Delete Task"));
    //    QAction* completeTask = bar->addAction(QIcon(":/img/complete-task.png"), tr("Complete Task"));
    bar->addSeparator();
    QAction* record = bar->addAction(QIcon(":/img/start.png"), tr("Start New Record"));
    QAction* stop = bar->addAction(QIcon(":/img/stop.png"), tr("Stop Current Record"));

    trcMenu->addAction(record);
    trcMenu->addAction(stop);
    prjMenu->addAction(newProject);
    prjMenu->addAction(openProject);
    QAction* editProject = prjMenu->addAction(QIcon(":/img/edit-project.png"), tr("Edit Project Information"));
    QAction* closeProject = prjMenu->addAction(QIcon(":/img/close-project.png"), tr("Close Project"));

    prjMenu->addSeparator();
    prjMenu->addAction(newTask);
    prjMenu->addAction(editTask);
    prjMenu->addAction(deleteTask);
    _taskPopUpMenu->addAction(newTask);
    _taskPopUpMenu->addAction(editTask);
    _taskPopUpMenu->addAction(deleteTask);
    //    prjMenu->addAction(completeTask);
    prjMenu->addSeparator();
    QAction* import = prjMenu->addAction(QIcon(":/img/import-project.png"), tr("Import Projects"));
    QAction* expAction = prjMenu->addAction(QIcon(":/img/export-project.png"), tr("Export project information"));
    prjMenu->addSeparator();
    QAction* quit = prjMenu->addAction(QIcon(":/img/exit.png"), tr("Quit"));

    QAction* settings = optMenu->addAction(QIcon(":/img/settings.png"), tr("Settings"));

    connect(newProject, SIGNAL(triggered()), this, SLOT(createNewProject()));
    connect(openProject, SIGNAL(triggered()), this, SLOT(openProject()));
    connect(editProject, SIGNAL(triggered()), this, SLOT(editProject()));
    connect(closeProject, SIGNAL(triggered()), this, SLOT(closeProject()));
    connect(import, SIGNAL(triggered()), this, SLOT(importProjects()));
    connect(newTask, SIGNAL(triggered()), this, SLOT(createNewTask()));
    connect(editTask, SIGNAL(triggered()), this, SLOT(editNewTask()));
    connect(deleteTask, SIGNAL(triggered()), this, SLOT(deleteTask()));
    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    //    connect(completeTask, SIGNAL(triggered()), this, SLOT(completeTask()));
    connect(record, SIGNAL(triggered()), this, SLOT(startRecord()));
    connect(stop, SIGNAL(triggered()), this, SLOT(stopRecord()));
    connect(expAction, SIGNAL(triggered()), this, SLOT(exportProjects()));

    connect(settings, SIGNAL(triggered()), this, SLOT(settings()));

    connect(widget.taskView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(taskContextMenuRequested(QPoint)));
    widget.taskView->setContextMenuPolicy(Qt::CustomContextMenu);
}

void MainWindow::idleTimeOut() {
    qDebug("MainWindow::idleTimeOut()");
    // This will enforce the new TaskLog
    _timeTracker->startRecord(_activeTask);
    IdleTaskWindow* w = new IdleTaskWindow(_projects, _timeTracker);
    connect(w, SIGNAL(currentTaskChanged(Task*)), _timeWindow, SLOT(setActiveTask(Task*)));
    w->exec();
    if (_timeTracker->status() == RUNNING) {
        _idleDetector->start();
    }
}

void MainWindow::startRecord() {
    qDebug("MainWindow::startRecord()");
    if (_activeTask != NULL) {
        if (_timeTracker->status() == RUNNING) {
            _timeTracker->stopRecord();
        }
        _taskModel->setTrackedTask(_activeTask);
        _timeWindow->setActiveTask(_activeTask);
        _timeTracker->startRecord(_activeTask);
        _idleDetector->start();
        _logWindow->refresh(_activeTask);
    }
}

void MainWindow::stopRecord() {
    qDebug("MainWindow::stopRecord()");
    _timeTracker->stopRecord();
    _logWindow->refresh(_timeTracker->task());
    _idleDetector->stop();
    _taskModel->setTrackedTask(NULL);
}

void MainWindow::setActiveTask(Task* task) {
    qDebug("MainWindow::setActiveTask");
    _activeTask = task;
    if (_activeTask == NULL) {
        _activeProject = NULL;
    } else {
        _activeProject = task->project();
        if (_activeTask != NULL) {
            _logWindow->refresh(_activeTask);
        }
    }
}

void MainWindow::setActiveTaskLog(Task* task, TaskLog* taskLog) {

}

void MainWindow::createCurrentTimeWindow() {
    qDebug("MainWindow::createCurrentTimeWindow()");
    _timeWindow = new CurrentTime(_projects);
    //    _timeWindow->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::BottomRightCorner);

    addDockWidget(Qt::BottomDockWidgetArea, _timeWindow);
}

void MainWindow::createNewTask() {
    qDebug("MainWindow::createNewTask()");
    if (_activeProject == NULL) {
        QMessageBox box;
        box.setText("You don't have an active project, you should create a project first.");
        box.setWindowTitle("d-Jon");
        box.exec();
        return;
    }
    string id;
    QString taskId;
    if (_activeTask != NULL) {
        taskId = QString(_activeTask->nextChildId()->c_str());
    } else {
        taskId = QString(_activeProject->nextChildId()->c_str());
    }
    TaskDialog* dialog = new TaskDialog(_activeProject, new std::string(taskId.toStdString()), this);
    if (dialog->exec() == QDialog::Accepted) {
        if (errorOcurred()) {
            showErrorMessage(lastErrorCode(), lastErrorDescription(), this);
            return;
        }
        reloadTasks();
    }
}

void MainWindow::editNewTask() {
    qDebug("MainWindow::editNewTask()");
    if (_activeTask == NULL) {
        QMessageBox box;
        box.setText("You don't have an active task, select one task and then use edit option.");
        box.setWindowTitle("d-Jon");
        box.exec();
        return;
    }
    TaskDialog* dialog = new TaskDialog(_activeProject, _activeTask, this);
    if (dialog->exec() == QDialog::Accepted) {
        reloadTasks();
    }
}

int MainWindow::createNewProject() {
    qDebug("MainWindow::createNewProject()");
    ProjectWizard* wizard = new ProjectWizard();
    int res = 1;
    if (wizard->exec() == QWizard::Accepted) {
        res = createProject(wizard->project());
        if (res != 0) {
            QMessageBox box;
            box.setWindowTitle(tr("d-Jon"));
            string errorDescription = string("An error ocurred creating the project file. Error: ");
            errorDescription += lastErrorDescription();
            box.setText(tr(errorDescription.c_str()));
            box.exec();
        } else {
            _projects = loadProjects();
            if (errorOcurred()) {
                showErrorMessage(lastErrorCode(), lastErrorDescription(), this);
                return 1;
            }
            if (_projects->size() == 1) {
                _activeProject = _projects->at(0);
            }
            _taskModel = new TaskModel(WITH_TIMES, *_projects);
            widget.taskView->setModel(_taskModel);
            widget.taskView->expandAll();
            connect(widget.taskView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(selectTaskChanged(QModelIndex,QModelIndex)));
        }
    }
    delete(wizard);
    return res;
}

void MainWindow::reloadProjects() {
    qDebug("MainWindow::reloadProjects()");
    if (_projects->size() == 1) {
        _activeProject = _projects->at(0);
    }
    reloadTasks();
}

void MainWindow::reloadTasks() {
    qDebug("MainWindow::reloadTasks()");
    _taskModel = new TaskModel(WITH_TIMES, *_projects);
    widget.taskView->setModel(_taskModel);
    TaskModel* model2 = new TaskModel(ONLY_TASKS, *_projects);
    widget.ganttView->setModel(model2);
    widget.ganttView->setIndentation(0);
    if (_taskHeader == NULL) {
        _taskHeader = new TaskHeaderView(_projects, Qt::Horizontal, widget.ganttView);
    } else {
        _taskHeader->setProjects(_projects);
    }
    widget.ganttView->setHeader(_taskHeader);
//    widget.ganttView->setItemsExpandable(false);;
    TaskDelegate* delegate = createTaskDelegate();
    widget.ganttView->setItemDelegate(delegate);
    widget.ganttView->setColumnHidden(1, true);

    connect(widget.taskView, SIGNAL(collapsed(QModelIndex)), widget.ganttView, SLOT(collapse(QModelIndex)));
    connect(widget.taskView, SIGNAL(expanded(QModelIndex)), widget.ganttView, SLOT(expand(QModelIndex)));
    connect(widget.taskView->verticalScrollBar(), SIGNAL(valueChanged(int)), widget.ganttView->verticalScrollBar(), SLOT(setValue(int)));
    connect(widget.ganttView->verticalScrollBar(), SIGNAL(valueChanged(int)), widget.taskView->verticalScrollBar(), SLOT(setValue(int)));

    widget.taskView->setAlternatingRowColors(true);
    widget.taskView->expandAll();
    widget.ganttView->expandAll();
    widget.taskView->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(widget.taskView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(selectTaskChanged(QModelIndex,QModelIndex)));
}

void MainWindow::timeStopped(Task* task, TaskLog* taskLog) {
    qDebug("MainWindow::timeStopped");
    _idleDetector->stop();
}

void MainWindow::deleteTask() {
    qDebug("MainWindow::deleteTask()");
    if (_activeTask != NULL) {
        // Check if the selected task is being tracked
        if (_timeTracker->status() == RUNNING) {
            Task* trackedTask = _timeTracker->task();
            if (trackedTask == _activeTask) {
                _timeTracker->stopRecord();
            }
        }

        Project* project = _activeTask->project();
        project->removeTask(_activeTask);
        //_taskModel->reset();

        _activeTask = NULL;

        reloadTasks();
    }
}

void MainWindow::completeTask() {
}

void MainWindow::exportProjects() {
    qDebug("MainWindow::exportProjects()");
    ExportDialog dialog(*_projects, this);
    dialog.exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug("MainWindow::closeEvent");
    if (_trayIcon->isVisible()) {
        int closeToSysTray = atoi(readConfValue("close-to-systray", "1"));
        if (closeToSysTray) {
            int warning = atoi(readConfValue("show-systray-warning", "1"));
            if (warning) {
                QMessageBox::information(this, tr("d-jon"),
                                         tr("The program will keep running in the "
                                            "system tray. To terminate the program, "
                                            "choose <b>Quit</b> in the context menu "
                                            "of the system tray entry."));
                writeConfValue("show-systray-warning", "0");
            }
            hide();
            event->ignore();
        } else {
            qApp->quit();
        }
    }
}

void MainWindow::createTray() {
    qDebug("MainWindow::createTray");
    _trayIcon = new QSystemTrayIcon(QIcon(":/img/djon.png"), this);
    connect(_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(restoreWindowState()));
    QMenu* mnu = new QMenu(this);
    QAction* quit = mnu->addAction(QIcon(":/img/quit.png"), tr("Quit"));
    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    _trayIcon->setContextMenu(mnu);
    _trayIcon->show();
    qApp->setQuitOnLastWindowClosed(false);
}


void MainWindow::taskContextMenuRequested(QPoint pos) {
    qDebug("MainWindow::taskContextMenuRequested");
    _taskPopUpMenu->popup(QCursor::pos());
}

void MainWindow::settings() {
    qDebug("MainWindow::settings");
    DialogSettings settings;
    int res = settings.exec();
    if (res == QDialog::Accepted) {
        _idleDetector->refreshIdleMaxSecs();
    }
}

void MainWindow::restoreWindowState() {
    qDebug("MainWindow::restoreWindowState");
    if (isHidden()) {
        showMaximized();
    } else {
        show();
    }
}

void MainWindow::editProject() {
    qDebug("MainWindow::editProject");
    ProjectDialog* dialog = new ProjectDialog(_activeProject, this);
    int res = dialog->exec();
    if (res == QDialog::Accepted) {
        updateProject(dialog->project());
    }
}

void MainWindow::importProjects() {
    qDebug("MainWindow::importProjects");
    QString selectedFileName = QFileDialog::getOpenFileName(this, tr("Import Projects"), tr(""), tr("XML Files (*.xml)"));
    if (selectedFileName.size() > 0){
        Template* tem = readTemplates()->at(0);
        string* status = tem->statusList()->at(0);
        vector<Project*>* imported = import(tem, status, new string(selectedFileName.toStdString()), ALLNETIC_FILE);

        if (imported) {
            for (vector<Project*>::iterator iter = imported->begin(); iter != imported->end(); iter++) {
                Project* proj = *iter;
                _projects->push_back(proj);
                createProject(proj);
                vector<Task*>* tasks = proj->tasks();
                for (vector<Task*>::iterator iterTask = tasks->begin(); iterTask != tasks->end(); iterTask++ ) {
                    Task* task = *iterTask;
                    createTask(task);
                    vector<TaskLog*>* logs = task->logs();
                    for (vector<TaskLog*>::iterator iterTaskLog = logs->begin(); iterTaskLog != logs->end(); iterTaskLog++) {
                        TaskLog* log = *iterTaskLog;
                        createTaskLog(task, log);
                    }
                }
            }
            reloadProjects();
        } else if (errorOcurred()) {
            showErrorMessage(lastErrorCode(), lastErrorDescription(), this);
            return;
        }
    }
}

void MainWindow::showErrorMessage(int errorCode, const char* errorMessage, QWidget* parent) {
    qDebug("MainWindow::showErrorMessage(int errorCode, const char* errorMessage, QWidget* parent)");
    std::stringstream ss;
    ss << errorMessage;
    if (errorCode > 0) {
        ss << "\nError Code: " << errorCode;
    }
    std::string msg = ss.str();
    QMessageBox::critical(this, QString("d-jon Error"), QString(msg.c_str()));
    clearError();
}

void MainWindow::showErrorMessage(const char* errorMessage, QWidget* parent) {
    qDebug("MainWindow::showErrorMessage(const char* errorMessage, QWidget* parent)");
    showErrorMessage(-1, errorMessage, parent);
}

void MainWindow::initialize() {
    qDebug("MainWindow::initialize()");
    _projects = loadProjects();
    if (errorOcurred()) {
        showErrorMessage(lastErrorDescription(), this);
        exit(EXIT_FAILURE);
        return;
    }
    if (_projects->size() == 0) {
        ProjectWizard* wizard = new ProjectWizard();
        if (wizard->exec() == QWizard::Accepted) {
            int res = createProject(wizard->project());
            if (res != 0) {
                exit(0);
            }
            _projects = loadProjects();
            if (errorOcurred()) {
                showErrorMessage(lastErrorDescription(), this);
            }
        }
    }
    reloadProjects();
}

void MainWindow::openProject() {
    qDebug("MainWindow::openProject()");
    QString selectedFileName = QFileDialog::getOpenFileName(this, tr("Open Project"), tr(readConfValue("last-project-dir")), tr("djon files (*.djon)"));
    if (selectedFileName.size() > 0){
        QFile file(selectedFileName);
        string fileName = file.fileName().toStdString();
        fileName = fileName.substr(fileName.find_last_of("/") + 1);
        fileName = fileName.substr(0, fileName.length() - 5);
        addProject(fileName.c_str());
        _projects = loadProjects();
        reloadProjects();
    }
}

void MainWindow::closeProject() {
    qDebug("MainWindow::closeProject()");
    string* projectFileName = _activeProject->projectFileName();
    removeProject(projectFileName->c_str());
    _projects = loadProjects();
    if (errorOcurred()) {
        showErrorMessage(lastErrorDescription(), this);
        return;
    }
    reloadProjects();
}
