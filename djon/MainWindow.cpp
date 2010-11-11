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
#include "workingdetectionwindow.h"
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
#include "config.h"
#include "ganttscene.h"
#include "userpreferencescontroller.h"
#include "djonpreferences.h"
#include "releasenotesview.h"
#include "workingdetector.h"
#include "trackcontrolwindow.h"
#include "settings.h"
#include "taskclosefiltermodel.h"
#include "updatemanager.h"
#include <sstream>
#include <QGraphicsView>
#include "ganttscene.h"
#include "systrayicon.h"
#include "taskeditordelegate.h"
#include "closedtaskfilter.h"

std::vector<Project*>* _projects;

MainWindow::MainWindow() {
    qDebug("MainWindow::MainWindow()");
    widget.setupUi(this);
    _logWindow = NULL;
    _projects = NULL;
    _idleDetector = NULL;
    _workingDetector = NULL;
    _timeTracker = NULL;
    _taskHeader = NULL;
    _activeLog = NULL;
    _activeProject = NULL;
    _activeTask = NULL;
    _timeWindow = NULL;
    _taskModel = NULL;
    _trayIcon = NULL;
    _taskPopUpMenu = NULL;
    _updateManager = NULL;
    _recordButton = NULL;

    _userPreferencesController = new UserPreferencesController(_taskModel);

    checkReleaseNotes();
    initialize();
    createTaskLogWindow();
    createCurrentTimeWindow();

    _updateManager = new UpdateManager(this);
    _updateManager->startCheck();

    setupActions();

    widget.taskView->setColumnWidth(0, 250);
    widget.taskView->setColumnWidth(1, 30);
    widget.taskView->setColumnWidth(2, 70);
    widget.taskView->setColumnWidth(3, 70);
    widget.taskView->setColumnWidth(4, 70);
    widget.taskView->setMinimumWidth(420);
    widget.taskView->setItemDelegate(new TaskEditorDelegate(this));
//    _idleDetector = new IdleDetector(5*60);// 5*60
    _idleDetector = new IdleDetector();// 5*60
    connect(_idleDetector, SIGNAL(idleTimeOut()), this, SLOT(idleTimeOut()));

    _workingDetector = new WorkingDetector();
    connect(_workingDetector, SIGNAL(workingDetected(DateTime)), this, SLOT(workingDetected(DateTime)));

    _timeTracker = new TimeTracker();
    connect(_timeTracker, SIGNAL(timeChanged(Task*, DTime&, DTime&)), _timeWindow, SLOT(updateTime(Task*, DTime&, DTime&)));
    connect(_timeTracker, SIGNAL(timeChanged(Task*, DTime&, DTime&)), _taskModel, SLOT(timeChanged(Task*)));
    connect(_timeTracker, SIGNAL(timeStopped(Task*,TaskLog*)), this, SLOT(timeStopped(Task*, TaskLog*)));
    connect(_timeTracker, SIGNAL(trackerStarted(Task*,TaskLog*)), this, SLOT(trackerStarted(Task*,TaskLog*)));
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(aboutToQuit()));


    _trackWindow = new TrackControlWindow(_timeTracker, NULL);
    _trackWindow->setModel(_taskModel);
    _trackWindow->show();
    connect(_timeTracker, SIGNAL(timeChanged(Task*,DTime&,DTime&)), _trackWindow, SLOT(updateCurrentTime()));
    connect(_timeTracker, SIGNAL(trackerStarted(Task*,TaskLog*)), _trackWindow, SLOT(trackerStateChanged(Task*)));
    connect(_timeTracker, SIGNAL(timeStopped(Task*,TaskLog*)), _trackWindow, SLOT(trackerStateChanged(Task*)));

    createTray();
    setLastSelectedTask();
    restoreUserSessionState();

    _workingDetector->startDetection();

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

void MainWindow::setupTemplateMenu(QMenu* menu) {
    std::vector<Template*>* templates = readTemplates();
    _templateMapper = new QSignalMapper(this);
    connect(_templateMapper, SIGNAL(mapped(QString)), this, SLOT(applyTemplate(QString)));
    for (std::vector<Template*>::iterator iter = templates->begin(); iter != templates->end(); iter++) {
        Template* tpl = *iter;
        QAction* act = menu->addAction(tpl->description()->c_str());
        _templateMapper->setMapping(act, QString(tpl->name()->c_str()));
        connect(act, SIGNAL(triggered()), _templateMapper, SLOT(map()));
    }
}

void MainWindow::setupActions() {
    qDebug("MainWindow::setupActions()");
    QToolBar* bar = new QToolBar("Options");
    addToolBar(bar);

    QMenuBar* menuBar = new QMenuBar();
    _taskPopUpMenu = new QMenu(widget.taskView);

    setMenuBar(menuBar);

    QMenu* prjMenu = menuBar->addMenu(tr("&Project"));
    QMenu* prjView = menuBar->addMenu("&View");
    QMenu* trcMenu = menuBar->addMenu(tr("&Tracker"));
    QMenu* optMenu = menuBar->addMenu(tr("&Options"));
    QMenu* helpMenu = menuBar->addMenu(tr("&Help"));

    QAction* newProject = bar->addAction(QIcon(":/img/new-project.png"), tr("Create Project"));
    QAction* openProject = bar->addAction(QIcon(":/img/open-project.png"), tr("Open Existing Project"));
    bar->addSeparator();
    QAction* newTask = bar->addAction(QIcon(":/img/new-task.png"), tr("Create SubTask"));
    QAction* editTask = bar->addAction(QIcon(":/img/edit-task.png"), tr("Edit Task"));
    QAction* deleteTask = bar->addAction(QIcon(":/img/delete-task.png"), tr("Delete Task"));
    //    QAction* completeTask = bar->addAction(QIcon(":/img/complete-task.png"), tr("Complete Task"));
    bar->addSeparator();
    _recordButton = bar->addAction(QIcon(":/img/start.png"), tr("Start New Record"));
    QAction* stop = bar->addAction(QIcon(":/img/stop.png"), tr("Stop Current Record"));

    trcMenu->addAction(_recordButton);
    trcMenu->addAction(stop);
    prjMenu->addAction(newProject);
    prjMenu->addAction(openProject);
    QAction* editProject = prjMenu->addAction(QIcon(":/img/edit-project.png"), tr("Edit Project Information"));
    QAction* closeProject = prjMenu->addAction(QIcon(":/img/close-project.png"), tr("Close Project"));

#ifdef WINDOWS
    QAction* checkUpdate = helpMenu->addAction(QIcon(":/img/update.png"), tr("Check Updates"));
#endif
    QAction* releaseNotes = helpMenu->addAction(QIcon(":/img/release-notes.png"), tr("Release Notes"));

    prjMenu->addSeparator();
    prjMenu->addAction(newTask);
    prjMenu->addAction(editTask);
    prjMenu->addAction(deleteTask);
    QMenu* applyTemplate = prjMenu->addMenu("Apply Template");
    setupTemplateMenu(applyTemplate);
    _taskPopUpMenu->addAction(newTask);
    _taskPopUpMenu->addAction(editTask);
    _taskPopUpMenu->addAction(deleteTask);
    _taskPopUpMenu->addMenu(applyTemplate);
    _taskPopUpMenu->addSeparator();
    _taskPopUpMenu->addAction(newProject);
    _taskPopUpMenu->addAction(editProject);
    _taskPopUpMenu->addAction(closeProject);
    //    prjMenu->addAction(completeTask);
    prjMenu->addSeparator();

    QAction* import = prjMenu->addAction(QIcon(":/img/import-project.png"), tr("Import Projects"));
    QAction* expAction = prjMenu->addAction(QIcon(":/img/export-project.png"), tr("Export project information"));
    prjMenu->addSeparator();
    QAction* quit = prjMenu->addAction(QIcon(":/img/exit.png"), tr("Quit"));

    _filterClosedAction = prjView->addAction(QIcon(":/img/filter-closed.png"), "Only show &open tasks");
    _filterClosedAction->setCheckable(true);

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
    connect(_filterClosedAction, SIGNAL(triggered()), this, SLOT(filterClosedTasks()));

    //    connect(completeTask, SIGNAL(triggered()), this, SLOT(completeTask()));
    connect(_recordButton, SIGNAL(triggered()), this, SLOT(startRecord()));
    connect(stop, SIGNAL(triggered()), this, SLOT(stopRecord()));
    connect(expAction, SIGNAL(triggered()), this, SLOT(exportProjects()));
#ifdef WINDOWS
    connect(checkUpdate, SIGNAL(triggered()), _updateManager, SLOT(check()));
#endif
    connect(releaseNotes, SIGNAL(triggered()), this, SLOT(showReleaseNotes()));
    connect(settings, SIGNAL(triggered()), this, SLOT(settings()));

    connect(widget.taskView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(taskContextMenuRequested(QPoint)));
    widget.taskView->setContextMenuPolicy(Qt::CustomContextMenu);
}

void MainWindow::idleTimeOut() {
    qDebug("MainWindow::idleTimeOut()");
    // This will enforce the new TaskLog

    _updateManager->pause();

    _timeTracker->startRecordLap();
    IdleTaskWindow* w = new IdleTaskWindow(_projects, _timeTracker);
    connect(w, SIGNAL(currentTaskChanged(Task*)), _timeWindow, SLOT(setActiveTask(Task*)));
    w->exec();
    if (_timeTracker->status() == RUNNING) {
        _idleDetector->start();
    } else {
        _workingDetector->startDetection();
    }

    _updateManager->resume();
}

void MainWindow::startRecord() {
    qDebug("MainWindow::startRecord()");
    if (_activeTask != NULL) {
        if (_timeTracker->status() == RUNNING) {
            _timeTracker->stopRecord();
        }
        _timeTracker->startRecord(_activeTask);
    }
}

void MainWindow::stopRecord() {
    qDebug("MainWindow::stopRecord()");
    _timeTracker->stopRecord();
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
        QModelIndex index = _taskModel->index(_activeProject, _activeTask);
        if (index.isValid()) {
            widget.taskView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
            _trackWindow->setCurrentTask(_activeTask);
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
    Project* currentProject = _activeProject;
    TaskDialog* dialog = new TaskDialog(currentProject, new std::string(taskId.toStdString()), this);
    if (dialog->exec() == QDialog::Accepted) {
        if (errorOcurred()) {
            showErrorMessage(lastErrorCode(), lastErrorDescription(), this);
            return;
        }
        Task* createdTask = currentProject->task(taskId.toStdString());

        reloadProjects();
        setActiveTask(createdTask);
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
            connect(widget.taskView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(selectTaskChanged(QModelIndex,QModelIndex)));
            connect(_taskModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), widget.taskView, SLOT(reset()));
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
    widget.taskView->setAnimated(false);
    if (_taskModel != NULL) {
        _taskModel->setProjects(*_projects);
        widget.ganttView->refresh();
    } else {
        _taskModel = new TaskModel(WITH_TIMES, *_projects);
        widget.taskView->setModel(_taskModel);
        widget.ganttView->setModel(_taskModel);

        widget.taskView->setAlternatingRowColors(true);
        widget.taskView->setSelectionBehavior(QAbstractItemView::SelectRows);
    }

    widget.ganttView->scrollToday();
    connect(widget.taskView, SIGNAL(collapsed(QModelIndex)), widget.ganttView, SLOT(collapse(QModelIndex)));
    connect(widget.taskView, SIGNAL(expanded(QModelIndex)), widget.ganttView, SLOT(expand(QModelIndex)));
    connect(widget.taskView, SIGNAL(collapsed(QModelIndex)), _userPreferencesController, SLOT(collapsed(QModelIndex)));
    connect(widget.taskView, SIGNAL(expanded(QModelIndex)), _userPreferencesController, SLOT(expanded(QModelIndex)));

    connect(widget.taskView->verticalScrollBar(), SIGNAL(valueChanged(int)), widget.ganttView->verticalScrollBar(), SLOT(setValue(int)));
    connect(widget.ganttView->verticalScrollBar(), SIGNAL(valueChanged(int)), widget.taskView->verticalScrollBar(), SLOT(setValue(int)));
    connect(widget.taskView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(selectTaskChanged(QModelIndex,QModelIndex)));

    widget.taskView->expandAll();
    refreshCollapsedState();
    widget.taskView->setAnimated(true);
}

void MainWindow::timeStopped(Task* task, TaskLog* taskLog) {
    qDebug("MainWindow::timeStopped");
    _idleDetector->stop();
    _trayIcon->trackerStopped();
    _recordButton->setIcon(QIcon(":/img/start.png"));
    _workingDetector->startDetection();
}

void MainWindow::deleteTask() {
    qDebug("MainWindow::deleteTask()");
    if (_activeTask != NULL) {
        std::stringstream ss;
        ss << "This action will delete the Task \"" << *_activeTask->shortDescription();
        ss << "\" and all its subtasks, are you sure?";
        std::string message = ss.str();
        int res = QMessageBox::question(this, tr("djon"), QString(message.c_str()), QMessageBox::Yes, QMessageBox::No);
        if (res == QMessageBox::Yes) {
            // Check if the selected task is being tracked
            if (_timeTracker->status() == RUNNING) {
                Task* trackedTask = _timeTracker->task();
                if (trackedTask == _activeTask) {
                    _timeTracker->stopRecord();
                }
            }

            Task* tsk = _activeTask;
            _taskModel->removeTask(_activeTask);

            Project* project = tsk->project();
            project->removeTask(tsk);
            //_taskModel->reset();

            _activeTask = NULL;

        }
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
        if (getSettings()->closeToTray()) {
            if (getSettings()->showSysTrayWarning()) {
                QMessageBox::information(this, tr("d-jon"),
                                         tr("The program will keep running in the "
                                            "system tray. To terminate the program, "
                                            "choose <b>Quit</b> in the context menu "
                                            "of the system tray entry."));
                getSettings()->setShowSysTrayWarning(false);
                getSettings()->save();
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
    _trayIcon = new SysTrayIcon(this);
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
//    QModelIndex index = widget.taskView->indexAt(pos);
//    Task* task = _taskModel->task(index);
//    Project* project = _taskModel->project(index);
//
    _taskPopUpMenu->popup(QCursor::pos());
}

void MainWindow::settings() {
    qDebug("MainWindow::settings");
    int previousUpdateChek = getSettings()->checkUpdate();
    DialogSettings settings;
    int res = settings.exec();
    if (res == QDialog::Accepted) {
        if (_activeTask != NULL) {
            _logWindow->refresh(_activeTask);
        }
        _idleDetector->refreshIdleMaxSecs();
        if (previousUpdateChek != getSettings()->checkUpdate()) {
            _updateManager->startCheck();
        }
        _trackWindow->refreshSettings();
    }
}

void MainWindow::restoreWindowState() {
    qDebug("MainWindow::restoreWindowState");
    show();
    activateWindow();
    qApp->activeWindow();
    raise();
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
    QString selectedFileName = QFileDialog::getOpenFileName(this,
                                                            tr("Open Project"),
                                                            tr(getSettings()->lastProjectDir().c_str()),
                                                            tr("djon files (*.djon)"));
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
    if (_activeProject != NULL) {
        string* projectFileName = _activeProject->projectFileName();
        removeProject(projectFileName->c_str());
        _projects = loadProjects();
        if (errorOcurred()) {
            showErrorMessage(lastErrorDescription(), this);
            return;
        }
        reloadProjects();
    } else {
        QMessageBox box;
        box.setText("You don't have an active project, select one project and try again.");
        box.setWindowTitle("d-jon");
        box.exec();
        return;
    }
}

void MainWindow::collapse(const QModelIndex& index) {

}

void MainWindow::expand(const QModelIndex& index) {

}

void MainWindow::refreshCollapsedState() {
    vector<Element*>* elements = collapsedElements();
    Project* lastProject = NULL;
    for (vector<Element*>::iterator iter = elements->begin(); iter != elements->end(); iter++) {
        Element* elem = *iter;
        string* projName = elem->project();
        // Search for the project and leave it in lastProject variable
        if ((lastProject == NULL) || (lastProject->name()->compare(*projName) != 0)) {
            lastProject = searchProject(*_projects, *projName);
        }
        Task* collapsedTask = NULL;
        if (lastProject != NULL) {
            if (elem->task() != NULL) {
                collapsedTask = lastProject->task(*elem->task());
            }
            QModelIndex index = _taskModel->index(lastProject, collapsedTask);
            if (index.isValid()) {
                widget.taskView->collapse(index);
            }
        }
    }
}

void MainWindow::setLastSelectedTask() {
    widget.taskView->setAnimated(false);
    Element* element = lastTrackedTaskId();
    if (element != NULL) {
        Project* project = searchProject(*_projects, *element->project());
        if (project != NULL) {
            Task* task = project->task(*element->task());
            QModelIndex index = _taskModel->index(project, task);
            if (index.isValid()) {
                widget.taskView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
                _trackWindow->setCurrentTask(task);
            }
        }
    }
    widget.taskView->setAnimated(true);
}

void MainWindow::aboutToQuit() {
    _timeTracker->stopRecord();
    saveUserSessionState();
}

void MainWindow::saveUserSessionState() {
    int wState = windowState();
    std::stringstream state;
    state << wState << "++";
    if (wState != Qt::WindowMaximized) {
        QRect rect = geometry();
        state << rect.left() << "++";
        state << rect.top() << "++";
        state << rect.width() << "++";
        state << rect.height() << "++";
    }
    if (isVisible()) {
        state << "1";
    } else {
        state << "0";
    }
    std::string sstate = state.str();
    writePreference("last-window-state", sstate);

    _trackWindow->geometry();
    std::stringstream trackState;
    QRect rect = _trackWindow->geometry();
    trackState << rect.left() << "++";
    trackState << rect.top() << "++";
    trackState << rect.width() << "++";
    trackState << rect.height();

    std::string strackState = trackState.str();
    writePreference("last-track-window-state", strackState);

    std::stringstream ssFilters;
    ssFilters << _filterClosedAction->isChecked();
    writePreference("filters", ssFilters.str());
}

void MainWindow::restoreUserSessionState() {
    std::string state(readPreference("last-window-state", ""));

    if (state.length() > 0) {
        std::vector<string*>* values = split(state, "++");
        Qt::WindowState wState = static_cast<Qt::WindowState>(atoi(values->at(0)->c_str()));
        setWindowState(wState);
        string* visible = new string("1");
        if (windowState() != Qt::WindowMaximized) {
            int left = atoi(values->at(1)->c_str());
            int top = atoi(values->at(2)->c_str());
            int width = atoi(values->at(3)->c_str());
            int height = atoi(values->at(4)->c_str());
            setGeometry(left, top, width, height);
            if (values->size() > 5) {
                visible = values->at(5);
            }
        } else {
            if (values->size() > 1) {
                visible = values->at(1);
            }
        }
        if (visible->compare("0")) {
            hide();
        }
    } else {
        setWindowState(Qt::WindowMaximized);;
    }

    std::string trackState(readPreference("last-track-window-state", ""));
    if (trackState.length() > 0) {
        std::vector<string*>* values = split(trackState, "++");
        int left = atoi(values->at(0)->c_str());
        int top = atoi(values->at(1)->c_str());
        int width = atoi(values->at(2)->c_str());
        int height = atoi(values->at(3)->c_str());
        _trackWindow->setGeometry(left, top, width, height);
    } else {
        _trackWindow->showIn(BOTTOM_RIGHT_CORNER);
    }

    std::string filters = readPreference("filters", "0");
    if (filters.compare("1") == 0) {
        _filterClosedAction->setChecked(true);
        filterClosedTasks();
    }
}

void MainWindow::checkReleaseNotes() {
    std::string relVersion = getSettings()->lastReleaseNotes();
    if ((relVersion.length() == 0) || (relVersion.compare(VERSION) != 0)) {
        showReleaseNotes();
        getSettings()->setLastReleaseNotes(string(VERSION));
        getSettings()->save();
    }
}

void MainWindow::showReleaseNotes() {
    ReleaseNotesView* view = new ReleaseNotesView();
    view->exec();
}

void MainWindow::workingDetected(const DateTime since) {
    WorkingDetectionWindow* w = new WorkingDetectionWindow(_projects, _workingDetector, _timeTracker, since, this);
    connect(w, SIGNAL(currentTaskChanged(Task*)), this, SLOT(setActiveTask(Task*)));
    w->exec();
}

void MainWindow::trackerStarted(Task* task, TaskLog* taskLog) {
    _activeTask = task;
    _activeLog = taskLog;
    _taskModel->setTrackedTask(task);
    _timeWindow->setActiveTask(task);
    _idleDetector->start();
    _logWindow->refresh(task);
    _userPreferencesController->setLastTrackedTask(_taskModel->index(task->project(), task));
    _trayIcon->trackerStarted();
    _recordButton->setIcon(QIcon(":/img/play_running.png"));
    _workingDetector->stopDetection();
}

void MainWindow::filterClosedTasks() {
    if (_filterClosedAction->isChecked()) {
        _taskModel->addFilter(CLOSED_FILTER);
    } else {
        _taskModel->removeFilter(CLOSED_FILTER);
    }
    widget.taskView->setAnimated(false);
    widget.taskView->expandAll();
    refreshCollapsedState();
    widget.taskView->setAnimated(true);
}

void MainWindow::applyTemplate(QString templateName) {
    qDebug("MainWindow::applyTemplate: %s", templateName.toStdString().c_str());

    Template* tpl = readTemplate(templateName.toStdString());
    if (_activeTask != NULL) {
        Task* currentTask = _activeTask;
        currentTask->setTemplateName(tpl->name());
        updateTask(currentTask);
        currentTask->processTemplate();
        reloadProjects();
        setActiveTask(currentTask);
    }
//    QObject* wid = QObject::sender();
//    qDebug("pressed: %s", wid->name());
}
