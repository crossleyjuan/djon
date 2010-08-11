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
#include <sstream>

MainWindow::MainWindow() {
    widget.setupUi(this);
    _activeProject = NULL;
    _activeTask = NULL;
    _activeLog = NULL;
    _taskHeader = NULL;

    _projects = loadProjects();
    if (_projects->size() == 0) {
        ProjectWizard* wizard = new ProjectWizard();
        int res = 1;
        if (wizard->exec() == QWizard::Accepted) {
            res = createProject(wizard->project());
            if (res != 0) {
                exit(0);
            }
            _projects = loadProjects();
        } else {
            exit(0);
        }
    }
    reloadProjects();
    createTaskLogWindow();
    createCurrentTimeWindow();

    setupActions();
    setWindowState(Qt::WindowMaximized);

    widget.taskView->setColumnWidth(0, 300);
//    _idleDetector = new IdleDetector(5*60);// 5*60
    _idleDetector = new IdleDetector();// 5*60
    connect(_idleDetector, SIGNAL(idleTimeOut()), this, SLOT(idleTimeOut()));

    _timeTracker = new TimeTracker();
    connect(_timeTracker, SIGNAL(timeChanged(DTime&)), _timeWindow, SLOT(updateTime(DTime&)));
    connect(_timeTracker, SIGNAL(timeStopped(Task*,TaskLog*)), this, SLOT(timeStopped(Task*, TaskLog*)));
    connect(qApp, SIGNAL(aboutToQuit()), _timeTracker, SLOT(stopRecord()));

    createTray();
}

void MainWindow::createTaskLogWindow() {
    _logWindow = new TaskLogWindow();
    _logWindow->setAllowedAreas(Qt::BottomDockWidgetArea);

    addDockWidget(Qt::BottomDockWidgetArea, _logWindow);
}

TaskDelegate* MainWindow::createTaskDelegate() {
    TaskDelegate* delegate = new TaskDelegate(_projects);
    return delegate;
}

MainWindow::~MainWindow() {
}

void MainWindow::selectTaskChanged(QModelIndex current, QModelIndex previous) {
    TaskModel* model = (TaskModel*)current.model();
    _activeTask = model->task(current);
    _activeProject = model->project(current);
    if (_activeTask != NULL) {
        _logWindow->refresh(_activeTask);
    }
}

void MainWindow::setupActions() {
    QToolBar* bar = new QToolBar("Options");
    addToolBar(bar);

    QMenuBar* menuBar = new QMenuBar();
    _taskPopUpMenu = new QMenu(widget.taskView);

    setMenuBar(menuBar);

    QMenu* prjMenu = menuBar->addMenu(tr("Project"));
    QMenu* trcMenu = menuBar->addMenu(tr("Tracker"));
    QMenu* optMenu = menuBar->addMenu(tr("Options"));

    QAction* newProject = bar->addAction(QIcon(":/img/new-project.png"), tr("Create Project"));
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
    QAction* editProject = prjMenu->addAction(QIcon(":/img/edit-project.png"), tr("Edit Project Information"));

    prjMenu->addSeparator();
    prjMenu->addAction(newTask);
    prjMenu->addAction(editTask);
    prjMenu->addAction(deleteTask);
    _taskPopUpMenu->addAction(newTask);
    _taskPopUpMenu->addAction(editTask);
    _taskPopUpMenu->addAction(deleteTask);
    //    prjMenu->addAction(completeTask);
    prjMenu->addSeparator();
    QAction* import = prjMenu->addAction(QIcon(":/img/import-projects.png"), tr("Import Projects"));
    QAction* expAction = prjMenu->addAction(QIcon(":/img/exportar.png"), tr("Export project information"));
    prjMenu->addSeparator();
    QAction* quit = prjMenu->addAction(QIcon(":/img/quit.png"), tr("Quit"));

    QAction* settings = optMenu->addAction(QIcon(":/img/settings.png"), tr("Settings"));

    connect(newProject, SIGNAL(triggered()), this, SLOT(createNewProject()));
    connect(editProject, SIGNAL(triggered()), this, SLOT(editProject()));
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
    // This will enforce the new TaskLog
    _timeTracker->startRecord(_activeTask);
    IdleTaskWindow* w = new IdleTaskWindow(_projects, _timeTracker);
    connect(w, SIGNAL(currentTaskChanged(Task*)), _timeWindow, SLOT(setActiveTask(Task*)));
    w->exec();
}

void MainWindow::startRecord() {
    if (_activeTask != NULL) {
        if (_timeTracker->status() == RUNNING) {
            _timeTracker->stopRecord();
        }
        _timeWindow->setActiveTask(_activeTask);
        _timeTracker->startRecord(_activeTask);
        _idleDetector->start();
        _logWindow->refresh(_activeTask);
    }
}

void MainWindow::stopRecord() {
    _timeTracker->stopRecord();
    _logWindow->refresh(_timeTracker->task());
    _idleDetector->stop();
}

void MainWindow::setActiveTask(Task* task) {

}

void MainWindow::setActiveTaskLog(Task* task, TaskLog* taskLog) {

}

void MainWindow::createCurrentTimeWindow() {
    _timeWindow = new CurrentTime(_projects);
    //    _timeWindow->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::BottomRightCorner);

    addDockWidget(Qt::BottomDockWidgetArea, _timeWindow);
}

void MainWindow::createNewTask() {
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
        reloadTasks();
    }
}

void MainWindow::editNewTask() {
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
    ProjectWizard* wizard = new ProjectWizard();
    int res = 1;
    if (wizard->exec() == QWizard::Accepted) {
        res = createProject(wizard->project());
        if (res != 0) {
            QMessageBox box;
            box.setWindowTitle(tr("d-Jon"));
            string errorDescription = string("An error ocurred creating the project file. Error: ");
            errorDescription += getLastErrorDescription();
            box.setText(tr(errorDescription.c_str()));
            box.exec();
        } else {
            _projects = loadProjects();
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
    if (_projects->size() == 1) {
        _activeProject = _projects->at(0);
    }
    reloadTasks();
}

void MainWindow::reloadTasks() {
    _taskModel = new TaskModel(WITH_TIMES, *_projects);
    widget.taskView->setModel(_taskModel);
//    TaskModel* model2 = new TaskModel(ONLY_TASKS, *_projects);
    widget.ganttView->setModel(_taskModel);
    widget.ganttView->setIndentation(0);
    if (_taskHeader == NULL) {
        _taskHeader = new TaskHeaderView(_projects, Qt::Horizontal, widget.ganttView);
    } else {
        _taskHeader->refresh();
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
    _idleDetector->stop();
}

void MainWindow::deleteTask() {
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
    ExportDialog dialog(*_projects, this);
    dialog.exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
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
    _taskPopUpMenu->popup(QCursor::pos());
}

void MainWindow::settings() {
    DialogSettings settings;
    int res = settings.exec();
    if (res == QDialog::Accepted) {
        _idleDetector->refreshIdleMaxSecs();
    }
}

void MainWindow::restoreWindowState() {
    if (isHidden()) {
        showMaximized();
    } else {
        show();
    }
}

void MainWindow::editProject() {
    ProjectDialog* dialog = new ProjectDialog(_activeProject, this);
    int res = dialog->exec();
    if (res == QDialog::Accepted) {
        updateProject(dialog->project());
    }
}

void MainWindow::importProjects() {
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
        }
    }
}
