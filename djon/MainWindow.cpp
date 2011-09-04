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
//#include "currenttime.h"
#include "taskdialog.h"
#include "util.h"
#include "view/projectwizard.h"
#include "idletaskwindow.h"
#include "view/dialogsettings.h"
#include "exportdialog.h"
#include "view/projectdialog.h"
#include "import/import.h"
#include "config.h"
#include "userpreferencescontroller.h"
#include "releasenotesview.h"
#include "workingdetector.h"
#include "trackcontrolwindow.h"
#include "taskclosefiltermodel.h"
#include "updatemanager.h"
#include <sstream>
#include <QGraphicsView>
#include "systrayicon.h"
#include "taskeditordelegate.h"
#include "closedtaskfilter.h"
#include "pluginmanager.h"

// Views
#include "logview.h"
#include "ganttview.h"
#include "timeview.h"

//std::vector<Project*>* _projects;

MainWindow::MainWindow() {
    qDebug("MainWindow::MainWindow()");
    widget.setupUi(this);
    _logWindow = NULL;
//    _projects = NULL;
    _idleDetector = NULL;
    _workingDetector = NULL;
    _timeTracker = NULL;
    _taskHeader = NULL;
    _activeLog = NULL;
    _activeProject = NULL;
    _activeTask = NULL;
//    _timeWindow = NULL;
    _taskModel = NULL;
    _trayIcon = NULL;
    _taskPopUpMenu = NULL;
    _updateManager = NULL;
    _recordButton = NULL;
    _currentView = NULL;

    _userPreferencesController = new UserPreferencesController(_taskModel);

    // Objects creation
    _updateManager = new UpdateManager(this);
    _timeTracker = new TimeTracker();
    _idleDetector = new IdleDetector();// 5*60
    _workingDetector = new WorkingDetector();
    _trackWindow = new TrackControlWindow(_timeTracker, NULL);

    checkReleaseNotes();


    // Plugins
    PluginManager::loadPlugins();

    setupActions();

    initialize();

    reloadProjects();

    createTaskLogWindow();
//    createCurrentTimeWindow();

    _updateManager->startCheck();

    widget.taskView->setColumnWidth(0, 250);
    widget.taskView->setColumnWidth(1, 30);
    widget.taskView->setColumnWidth(2, 70);
    widget.taskView->setColumnWidth(3, 70);
    widget.taskView->setColumnWidth(4, 70);
    widget.taskView->setMinimumWidth(250);
    widget.taskView->setMaximumWidth(490 + widget.taskView->verticalScrollBar()->width() + 10);
    widget.taskView->setItemDelegate(new TaskEditorDelegate(this));

    connect(_idleDetector, SIGNAL(idleTimeOut()), this, SLOT(idleTimeOut()));

    connect(_workingDetector, SIGNAL(workingDetected(DateTime)), this, SLOT(workingDetected(DateTime)));

//    connect(_timeTracker, SIGNAL(timeChanged(Task*, DTime&, DTime&)), _timeWindow, SLOT(updateTime(Task*, DTime&, DTime&)));
    connect(_timeTracker, SIGNAL(timeStopped(Task*,TaskLog*)), this, SLOT(timeStopped(Task*, TaskLog*)));
    connect(_timeTracker, SIGNAL(trackerStarted(Task*,TaskLog*)), this, SLOT(trackerStarted(Task*,TaskLog*)));
    connect(_timeTracker, SIGNAL(trackerStarted(Task*,TaskLog*)), _taskModel, SLOT(setTrackedTask(Task*)));

    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(aboutToQuit()));

    _trackWindow->setModel(_taskModel);
    _trackWindow->show();
    _trackWindow->refreshSettings();
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
    _logWindow = new TaskLogWindow(_timeTracker);
    _logWindow->setAllowedAreas(Qt::BottomDockWidgetArea);
    connect(_logWindow, SIGNAL(timeChanged(Task*)), _taskModel, SLOT(timeChanged(Task*)));

    addDockWidget(Qt::BottomDockWidgetArea, _logWindow);
}

TaskDelegate* MainWindow::createTaskDelegate() {
    qDebug("MainWindow::createTaskDelegate()");
    TaskDelegate* delegate = new TaskDelegate(_workspace->projects());
    return delegate;
}

MainWindow::~MainWindow() {
    delete(_workspace);

    releaseCalendars();
}

void MainWindow::selectTaskChanged(QModelIndex current, QModelIndex previous) {
    qDebug("MainWindow::selectTaskChanged");
    TaskModel* model = NULL;
    if (current.isValid()) {
        model = (TaskModel*)current.model();
    }
    if (model == NULL) {
        setActiveTask(NULL);
        _activeProject = NULL;
        return;
    }
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

void MainWindow::setupWorkspacesMenu() {
    _menuWorkspaces->clear();
    _workspaceMapper = new QSignalMapper(this);
    connect(_workspaceMapper, SIGNAL(mapped(QString)), this, SLOT(loadWorkspaceSlot(QString)));
    std::vector<std::string> recent = getSettings()->recentWorkspaces();
    std::string workspace;
    foreach (workspace, recent) {
        QAction* action = _menuWorkspaces->addAction(QString(workspace.c_str()));
        _workspaceMapper->setMapping(action, QString(workspace.c_str()));
        connect(action, SIGNAL(triggered()), _workspaceMapper, SLOT(map()));
    }
    _menuWorkspaces->addSeparator();
    QAction* createAction = _menuWorkspaces->addAction(QString("Create Workspace"));
    connect(createAction, SIGNAL(triggered()), this, SLOT(createWorkspace()));
    QAction* openAction = _menuWorkspaces->addAction(QString("Open Workspace"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openWorkspace()));
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
    _stopButton = bar->addAction(QIcon(":/img/stop.png"), tr("Stop Current Record"));
    _stopButton->setEnabled(false);

    trcMenu->addAction(_recordButton);
    trcMenu->addAction(_stopButton);
    prjMenu->addAction(newProject);
    prjMenu->addAction(openProject);
    QAction* editProject = prjMenu->addAction(QIcon(":/img/edit-project.png"), tr("Edit Project Information"));
    QAction* closeProject = prjMenu->addAction(QIcon(":/img/close-project.png"), tr("Close Project"));

    QAction* checkUpdate = helpMenu->addAction(QIcon(":/img/update.png"), tr("Check Updates"));
    QAction* releaseNotes = helpMenu->addAction(QIcon(":/img/release-notes.png"), tr("Release Notes"));

    prjMenu->addSeparator();
    prjMenu->addAction(newTask);
    prjMenu->addAction(editTask);
    prjMenu->addAction(deleteTask);
    QMenu* applyTemplate = prjMenu->addMenu("Apply Template");
    setupTemplateMenu(applyTemplate);

    prjMenu->addSeparator();
    _menuWorkspaces = prjMenu->addMenu("Workspaces");
    setupWorkspacesMenu();
    //***********************************************************
    // Popup actions should be registered in showPopup function
    QAction* newTaskInline = _taskPopUpMenu->addAction(QIcon(":/img/new-task.png"), tr("Create SubTask"));
    connect(newTaskInline, SIGNAL(triggered()), this, SLOT(createNewInlineTask()));
    _taskPopUpMenu->addAction(editTask);
    _taskPopUpMenu->addAction(deleteTask);
    _taskPopUpMenu->addMenu(applyTemplate);
    _taskPopUpMenu->addSeparator();
    _taskPopUpMenu->addAction(newProject);
    _taskPopUpMenu->addAction(editProject);
    _taskPopUpMenu->addAction(closeProject);
    //***********************************************************

    //    prjMenu->addAction(completeTask);
    prjMenu->addSeparator();

    QAction* import = prjMenu->addAction(QIcon(":/img/import-project.png"), tr("Import Projects"));
    QAction* expAction = prjMenu->addAction(QIcon(":/img/export-project.png"), tr("Export project information"));
    prjMenu->addSeparator();
    QAction* quit = prjMenu->addAction(QIcon(":/img/exit.png"), tr("Quit"));

    _filterClosedAction = prjView->addAction("Only show &open tasks");
    _filterClosedAction->setCheckable(true);

    /***** Views ******/
    QMenu* views = prjView->addMenu(QIcon(":/img/views.png"), tr("Views"));
    QActionGroup* group = new QActionGroup(this);
    _ganttViewAction = views->addAction(tr("Gantt View"));
    _ganttViewAction->setCheckable(true);
    _ganttViewAction->setActionGroup(group);
    _ganttViewAction->setData(0);
    _logViewAction = views->addAction(tr("Log View"));
    _logViewAction->setCheckable(true);
    _logViewAction->setActionGroup(group);
    _logViewAction->setData(1);
    _timeViewAction = views->addAction(tr("Time by Date View"));
    _timeViewAction->setCheckable(true);
    _timeViewAction->setActionGroup(group);
    _timeViewAction->setData(2);
    connect(group, SIGNAL(triggered(QAction*)), this, SLOT(onMenuChangeView(QAction*)));
    /*****************/

    /***** Options menu ******/
    QAction* settings = optMenu->addAction(QIcon(":/img/settings.png"), tr("Settings"));
    QMenu* pluginsMenu = optMenu->addMenu(tr("Plugins"));

    std::vector<WindowPlugin*> plugins = PluginManager::plugins();
    for (std::vector<WindowPlugin*>::iterator iterPlugins = plugins.begin(); iterPlugins != plugins.end(); iterPlugins++) {
        WindowPlugin* plugin = *iterPlugins;
        QIcon pluginIcon = plugin->pluginIcon();
        QAction* pluginAction = pluginsMenu->addAction(pluginIcon, QString(plugin->pluginMenuText()));
        pluginAction->connect(pluginAction, SIGNAL(triggered()), plugin, SLOT(showDefaultScreen()));
    }
    /*****************/

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
    connect(_stopButton, SIGNAL(triggered()), this, SLOT(stopRecord()));
    connect(expAction, SIGNAL(triggered()), this, SLOT(exportProjects()));
    connect(checkUpdate, SIGNAL(triggered()), _updateManager, SLOT(check()));
    connect(releaseNotes, SIGNAL(triggered()), this, SLOT(showReleaseNotes()));
    connect(settings, SIGNAL(triggered()), this, SLOT(settings()));

    connect(widget.taskView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(taskContextMenuRequested(QPoint)));
    widget.taskView->installEventFilter(this);
    widget.taskView->setContextMenuPolicy(Qt::CustomContextMenu);
}

void MainWindow::idleTimeOut() {
    qDebug("MainWindow::idleTimeOut()");
    // This is to enforce the idle detector to be stopped while the action is being shown.
    _idleDetector->stop();
    _workingDetector->stopDetection();
    _updateManager->pause();
    _timeTracker->startRecordLap(_idleDetector->idleSince());
    IdleTaskWindow* w = new IdleTaskWindow(_workspace->projects(), _timeTracker);
    connect(w, SIGNAL(currentTaskChanged(Task*)), _taskModel, SLOT(setTrackedTask(Task*)));
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
    widget.taskView->selectionModel()->clearSelection();
    if (_activeTask == NULL) {
        _activeProject = NULL;
    } else {
        _activeProject = task->project();
        if (_activeTask != NULL) {
            _logWindow->refresh(_activeTask);
        }
        QModelIndex index = _taskModel->index(_activeProject, _activeTask);
        if (index.isValid()) {
            widget.taskView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
            if (_timeTracker->status() != RUNNING) {
                _trackWindow->setCurrentTask(task);
            }
        }
    }
}

void MainWindow::setActiveTaskLog(Task* task, TaskLog* taskLog) {

}

//void MainWindow::createCurrentTimeWindow() {
//    qDebug("MainWindow::createCurrentTimeWindow()");
//    _timeWindow = new CurrentTime(_projects);
//    //    _timeWindow->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::BottomRightCorner);

//    addDockWidget(Qt::BottomDockWidgetArea, _timeWindow);
//}

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
    if ((_activeTask != NULL) && (_activeProject->type() != Project::TODO)) {
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

        _taskModel->addTask(createdTask);

        setActiveTask(createdTask);
    }
}

void MainWindow::createNewInlineTask() {
    qDebug("MainWindow::createNewInlineTask()");
    if (_activeProject == NULL) {
        QMessageBox box;
        box.setText("You don't have an active project, you should create a project first.");
        box.setWindowTitle("d-Jon");
        box.exec();
        return;
    }

    if (_activeTask != NULL) {
        qDebug("creating task as sibling of %s", _activeTask->shortDescription()->c_str());
    }
    Task* newtsk = _activeProject->createTask((_activeTask == NULL) ? NULL: _activeTask->parent());
    saveProject(_activeProject);
    _taskModel->addTask(newtsk);
    setActiveTask(newtsk);
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
        res = saveProject(wizard->project());
        if (res != 0) {
            QMessageBox box;
            box.setWindowTitle(tr("d-Jon"));
            string errorDescription = string("An error ocurred creating the project file. Error: ");
            errorDescription += lastErrorDescription();
            box.setText(tr(errorDescription.c_str()));
            box.exec();
        } else {
            addProject(wizard->project()->projectFileName()->c_str());
            _workspace->addProject(wizard->project());
            saveWorkspace(_workspace);
            if (errorOcurred()) {
                showErrorMessage(lastErrorCode(), lastErrorDescription(), this);
                return 1;
            }
            if (_workspace->projects()->size() == 1) {
                _activeProject = _workspace->projects()->at(0);
            }
            _taskModel = new TaskModel(WITH_TIMES, *_workspace->projects());
            widget.taskView->setModel(_taskModel);
            connect(widget.taskView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(selectTaskChanged(QModelIndex,QModelIndex)));
            connect(_taskModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), widget.taskView, SLOT(reset()));

            refreshCollapsedState();
        }
    }
    delete(wizard);
    return res;
}

void MainWindow::reloadProjects() {
    qDebug("MainWindow::reloadProjects()");
    if (_workspace->projects()->size() == 1) {
        _activeProject = _workspace->projects()->at(0);
    }
    reloadTasks();
}

void MainWindow::reloadTasks() {
    qDebug("MainWindow::reloadTasks()");
    widget.taskView->setAnimated(false);
    if (_taskModel != NULL) {
        _taskModel->setProjects(*_workspace->projects());
        ((AbstractViewer*)_currentView)->refresh();
    } else {
        _taskModel = new TaskModel(WITH_TIMES, *_workspace->projects());
        widget.taskView->setModel(_taskModel);
        changeCurrentView(Log_View);

        widget.taskView->setAlternatingRowColors(true);
        widget.taskView->setSelectionBehavior(QAbstractItemView::SelectRows);
    }

    connect(_timeTracker, SIGNAL(timeChanged(Task*, DTime&, DTime&)), _taskModel, SLOT(timeChanged(Task*)));
    _currentView->scrollToday();
    connect(widget.taskView, SIGNAL(collapsed(QModelIndex)), _userPreferencesController, SLOT(collapsed(QModelIndex)));
    connect(widget.taskView, SIGNAL(expanded(QModelIndex)), _userPreferencesController, SLOT(expanded(QModelIndex)));

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
    _stopButton->setEnabled(false);
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
            saveProject(project);
        }
    }
}

void MainWindow::completeTask() {
}

void MainWindow::exportProjects() {
    qDebug("MainWindow::exportProjects()");
    ExportDialog dialog(*_workspace->projects(), this);
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
    QModelIndex index = widget.taskView->indexAt(pos);
    _activeTask = _taskModel->task(index);
    _activeProject = _taskModel->project(index);
    showPopup();
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
        saveProject(dialog->project());
    }
}

void MainWindow::importProjects() {
    qDebug("MainWindow::importProjects");
    QString selectedFileName = QFileDialog::getOpenFileName(this, tr("Import Projects"), tr(""), tr("XML Files (*.xml)"));
    if (selectedFileName.size() > 0){
        Template* tem = readTemplates()->at(0);
        string status = tem->statusList().at(0);
        vector<Project*>* imported = import(tem, &status, new string(selectedFileName.toStdString()), ALLNETIC_FILE);

        if (imported) {
            for (vector<Project*>::iterator iter = imported->begin(); iter != imported->end(); iter++) {
                Project* proj = *iter;
                _workspace->addProject(proj);
                saveProject(proj);
                saveWorkspace(_workspace);
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
    std::string lastWorkspace = getSettings()->lastWorkspace();
    if (lastWorkspace.length() == 0) {
        // if the last workspace does not exist then create a new default
        // this will support backward compatibility
        std::vector<string> openProjects = getSettings()->openProjects();
        std::string* homeDir = getHomeDir();
        std::string wkFileName = *homeDir + "/.djon/default.dwk";
        _workspace = new Workspace(wkFileName);
        for (std::vector<string>::iterator iterOP = openProjects.begin(); iterOP != openProjects.end(); iterOP++) {
            std::string projectFileName = *iterOP;
            _workspace->addProject(loadProject(projectFileName));
        }
        delete(homeDir);
        getSettings()->setLastWorkspace(wkFileName);
        getSettings()->save();
        saveWorkspace(_workspace);
    } else {
        _workspace = loadWorkspace(lastWorkspace);
    }
    if (lastErrorCode() > 0) {
        showErrorMessage(lastErrorCode(), lastErrorDescription(), this);
        clearError();
    }
    getSettings()->addRecentWorkspace(_workspace->fileName());
    getSettings()->save();
    if (_workspace->projects()->size() == 0) {
        ProjectWizard* wizard = new ProjectWizard();
        if (wizard->exec() == QWizard::Accepted) {
            int res = saveProject(wizard->project());
            if (res != 0) {
                exit(0);
            }
            addProject(wizard->project()->projectFileName()->c_str());
            std::vector<Project*>* projects = loadProjects();
            if (lastErrorCode() > 0) {
                showErrorMessage(lastErrorCode(), lastErrorDescription(), this);
                clearError();
                projects = new std::vector<Project*>();
            }
            _workspace->clear();
            for (std::vector<Project*>::iterator iter = projects->begin(); iter != projects->end(); iter++) {
                _workspace->addProject(*iter);
            }
            saveWorkspace(_workspace);
            delete(projects);
            if (errorOcurred()) {
                showErrorMessage(lastErrorDescription(), this);
            }
        }
    }

    // Initialize the plugins
    std::vector<WindowPlugin*> plugins = PluginManager::plugins();
    for (std::vector<WindowPlugin*>::iterator iterPlugins = plugins.begin(); iterPlugins != plugins.end(); iterPlugins++) {
        WindowPlugin* plugin = *iterPlugins;
        plugin->setWorkspace(_workspace);
    }
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
        addProject(fileName.c_str());
        Project* project = loadProject(fileName);
        if (project != NULL) {
            _workspace->addProject(project);
            saveWorkspace(_workspace);
            reloadProjects();
            widget.taskView->setAnimated(false);
            widget.taskView->expandAll();
            refreshCollapsedState();
            widget.taskView->setAnimated(true);
        } else {
            showErrorMessage(lastErrorCode(), lastErrorDescription(), this);
            clearError();
        }
    }
}

void MainWindow::closeProject() {
    qDebug("MainWindow::closeProject()");
    if (_activeProject != NULL) {
        string* projectFileName = _activeProject->projectFileName();
        removeProject(projectFileName->c_str());
        _workspace->removeProject(_activeProject);
        saveWorkspace(_workspace);
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
    widget.taskView->setAnimated(false);
    widget.taskView->expandAll();
    vector<Element*>* elements = collapsedElements();
    Project* lastProject = NULL;
    for (vector<Element*>::iterator iter = elements->begin(); iter != elements->end(); iter++) {
        Element* elem = *iter;
        string* projName = elem->project();
        // Search for the project and leave it in lastProject variable
        if ((lastProject == NULL) || (lastProject->name()->compare(*projName) != 0)) {
            lastProject = searchProject(*_workspace->projects(), *projName);
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
    delete(elements);
    widget.taskView->setAnimated(true);
}

void MainWindow::setLastSelectedTask() {
    widget.taskView->setAnimated(false);
    Task* task = lastTrackedTask(*_workspace->projects());
    if (task != NULL) {
        QModelIndex index = _taskModel->index(task->project(), task);
        if (index.isValid()) {
            widget.taskView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
            _trackWindow->setCurrentTask(task);
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

    std::stringstream ssCurrentView;
    ssCurrentView << (int)_currentViewType;
    writePreference("current-view", ssCurrentView.str());
}

void MainWindow::restoreUserSessionState() {
    char* cstate = readPreference("last-window-state", "");
    std::string state(cstate);
    free(cstate);

    if (state.length() > 0) {
        std::vector<string> values = split(state, "++");
        Qt::WindowState wState = static_cast<Qt::WindowState>(atoi(values.at(0).c_str()));
        setWindowState(wState);
        string visible = "1";
        if (windowState() != Qt::WindowMaximized) {
            int left = atoi(values.at(1).c_str());
            int top = atoi(values.at(2).c_str());
            int width = atoi(values.at(3).c_str());
            int height = atoi(values.at(4).c_str());
            setGeometry(left, top, width, height);
            if (values.size() > 5) {
                visible = values.at(5);
            }
        } else {
            if (values.size() > 1) {
                visible = values.at(1);
            }
        }
        if (visible.compare("0")) {
            hide();
        }
    } else {
        setWindowState(Qt::WindowMaximized);;
    }

    std::string trackState(readPreference("last-track-window-state", ""));
    if (trackState.length() > 0) {
        std::vector<string> values = split(trackState, "++");
        int left = atoi(values.at(0).c_str());
        int top = atoi(values.at(1).c_str());
        int width = atoi(values.at(2).c_str());
        int height = atoi(values.at(3).c_str());
        _trackWindow->setGeometry(left, top, width, height);
    } else {
        _trackWindow->showIn(BOTTOM_RIGHT_CORNER);
    }

    char* filters = readPreference("filters", "0");
    if (strcmp(filters, "1") == 0) {
        _filterClosedAction->setChecked(true);
        filterClosedTasks();
    }
    free(filters);

    char* currentView = readPreference("current-view", "0");
    VIEW_TYPE type = (VIEW_TYPE)atoi(currentView);
    free(currentView);
    changeCurrentView(type);
}

void MainWindow::checkReleaseNotes() {
    std::string relVersion = getSettings()->lastReleaseNotes();
    bool show = true;
    if (relVersion.length() != 0) {
        Version lastRelease = getVersion(relVersion.c_str());
        Version currentVersion = getCurrentVersion();
        if (lastRelease == currentVersion) {
            show = false;
        }
    }

    if (show) {
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
    WorkingDetectionWindow* w = new WorkingDetectionWindow(_workspace->projects(), _workingDetector, _timeTracker, since, this);
    connect(w, SIGNAL(currentTaskChanged(Task*)), this, SLOT(setActiveTask(Task*)));
    connect(w, SIGNAL(currentTaskChanged(Task*)), _taskModel, SLOT(setTrackedTask(Task*)));
    w->exec();
}

void MainWindow::trackerStarted(Task* task, TaskLog* taskLog) {
    _activeTask = task;
    _activeLog = taskLog;
    _taskModel->setTrackedTask(task);
//    _timeWindow->setActiveTask(task);
    _idleDetector->start();
    _logWindow->refresh(task);
    _userPreferencesController->setLastTrackedTask(_taskModel->index(task->project(), task));
    _trayIcon->trackerStarted();
    _recordButton->setIcon(QIcon(":/img/play_running.png"));
    _workingDetector->stopDetection();
    _stopButton->setEnabled(true);
}

void MainWindow::filterClosedTasks() {
    if (_filterClosedAction->isChecked()) {
        _taskModel->addFilter(CLOSED_FILTER);
    } else {
        _taskModel->removeFilter(CLOSED_FILTER);
    }
    refreshCollapsedState();
}

void MainWindow::applyTemplate(QString templateName) {
    qDebug("MainWindow::applyTemplate: %s", templateName.toStdString().c_str());

    Template* tpl = readTemplate(templateName.toStdString());
    if (_activeTask != NULL) {
        Task* currentTask = _activeTask;
        currentTask->setTemplateName(tpl->name());
        //updateTask(currentTask);
        currentTask->processTemplate();
        saveProject(currentTask->project());
        reloadProjects();
        setActiveTask(currentTask);
    }
//    QObject* wid = QObject::sender();
//    qDebug("pressed: %s", wid->name());
}

void MainWindow::showPopup() {
    QList<QAction*> actions = _taskPopUpMenu->actions();
    for (QList<QAction*>::iterator iter = actions.begin(); iter != actions.end(); iter++) {
        QAction* act = (*iter);
        act->setVisible(false);
    }
    actions.at(5)->setVisible(true); // Create project is always visible
    if (_activeProject != NULL) {
        for (int x = 4; x < actions.size(); x++) {
            QAction* act = actions.at(x);
            act->setVisible(true);
        }
        actions.at(0)->setVisible(true); // New task
    }
    if (_activeTask != NULL) {
        for (int x = 1; x < 4; x++) {
            QAction* act = actions.at(x);
            act->setVisible(true);
        }
    }
    _taskPopUpMenu->popup(QCursor::pos());
}

void MainWindow::changeCurrentView(VIEW_TYPE type) {
    if (_currentView != NULL) {
        delete _currentView;
        _currentView = 0;
    }
    _currentViewType = type;
    switch (type) {
    case Gantt_View:
        _currentView = new GanttView(this);
        connect(((LogView*)_currentView)->verticalScrollBar(), SIGNAL(valueChanged(int)), widget.taskView->verticalScrollBar(), SLOT(setValue(int)));
        widget.taskView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        _ganttViewAction->setChecked(true);
        break;
    case Log_View:
        _currentView = new LogView(this);
        _logViewAction->setChecked(true);
        widget.taskView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        break;
    case Time_View:
        TimeView* timeView = new TimeView(this);
        connect(timeView->verticalScrollBar(), SIGNAL(valueChanged(int)), widget.taskView->verticalScrollBar(), SLOT(setValue(int)));
        widget.taskView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        _timeViewAction->setChecked(true);
        connect(timeView, SIGNAL(itemHoverEnter(QModelIndex)), _taskModel, SLOT(receiveItemHoverEnter(QModelIndex)));
        connect(timeView, SIGNAL(itemHoverLeave(QModelIndex)), _taskModel, SLOT(receiveItemHoverLeave(QModelIndex)));
        connect(timeView, SIGNAL(itemHoverEnter(QModelIndex)), widget.taskView, SLOT(dataIndexChanged(QModelIndex)));
        connect(timeView, SIGNAL(itemHoverLeave(QModelIndex)), widget.taskView, SLOT(dataIndexChanged(QModelIndex)));
        _currentView = timeView;
        break;
    }
    widget.splitter->addWidget(_currentView);
    _currentView->setModel(_taskModel);

    connect(widget.taskView, SIGNAL(collapsed(QModelIndex)), ((LogView*)_currentView), SLOT(collapse(QModelIndex)));
    connect(widget.taskView, SIGNAL(expanded(QModelIndex)), ((LogView*)_currentView), SLOT(expand(QModelIndex)));
    connect(_timeTracker, SIGNAL(trackerStarted(Task*,TaskLog*)), _currentView, SLOT(refresh()));
    _currentView->scrollToday();
    widget.taskView->resize(420, widget.taskView->size().height());
    refreshCollapsedState();
}

void MainWindow::onMenuChangeView(QAction* action) {
    int view = action->data().toInt();
    changeCurrentView((VIEW_TYPE)view);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type()==QEvent::KeyPress) {
        // key pressed

        // transforms QEvent into QKeyEvent
        QKeyEvent* pKeyEvent=static_cast<QKeyEvent*>(event);
        switch(pKeyEvent->key()) {
            case Qt::Key_Insert: {
                    createNewInlineTask();
                    break;
            }
        case Qt::Key_Delete: {
                deleteTask();
                break;
        }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void MainWindow::createWorkspace() {
    std::string* homeDir = getHomeDir();
    QString dir(homeDir->c_str());
    QString file = QFileDialog::getSaveFileName(this, "Create workspace", dir);
    if (file.size() > 0) {
        if (!file.endsWith(".dwk")) {
            file = file.append(".dwk");
        }
        _workspace = new Workspace(file.toStdString());
        getSettings()->addRecentWorkspace(file.toStdString());
        getSettings()->save();
        reloadProjects();
    }
    setupWorkspacesMenu();
    delete(homeDir);
}

void MainWindow::loadWorkspaceSlot(QString fileName) {
    _workspace = loadWorkspace(fileName.toStdString());
    if (_workspace) {
        getSettings()->addRecentWorkspace(_workspace->fileName());
        getSettings()->setLastWorkspace(fileName.toStdString());
        getSettings()->save();
        reloadProjects();
        setupWorkspacesMenu();
    }
}

void MainWindow::openWorkspace() {
    std::string* homeDir = getHomeDir();
    QString dir(homeDir->c_str());
    QString file = QFileDialog::getOpenFileName(this, "Open workspace", dir);
    if (file.size() > 0) {
        loadWorkspace(file.toStdString());
    }
    delete(homeDir);
}
