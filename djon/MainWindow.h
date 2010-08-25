/* 
 * File:   MainWindow.h
 * Author: cross
 *
 * Created on July 9, 2010, 8:50 AM
 */

#ifndef _MAINWINDOW_H
#define	_MAINWINDOW_H

#include "ui_MainWindow.h"
#include "data.h"
#include <vector>
#include "taskdelegate.h"
#include "idledetector.h"
#include "view/ganttscene.h"

class TaskLogWindow;
class TimeTracker;
class CurrentTime;
class TaskModel;
class TaskHeaderView;
class UpdateManager;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();
private:
    TaskDelegate* createTaskDelegate();
    void createTaskLogWindow();
    void createCurrentTimeWindow();
    void createTray();
    void setupActions();
    void reloadProjects();
    void reloadTasks();
    void showErrorMessage(int errorCode, const char* errorMessage, QWidget* parent);
    void showErrorMessage(const char* errorMessage, QWidget* parent);

    Ui::MainWindow widget;
    TaskLogWindow* _logWindow;
    std::vector<Project*>* _projects;
    IdleDetector* _idleDetector;
    TimeTracker* _timeTracker;
    TaskHeaderView* _taskHeader;
    TaskLog* _activeLog;
    Project* _activeProject;
    Task* _activeTask;
    CurrentTime* _timeWindow;
    TaskModel* _taskModel;
    GanttScene* _ganttScene;

    QSystemTrayIcon* _trayIcon;
    QMenu* _taskPopUpMenu;
    UpdateManager* _updateManager;

public:
    void closeEvent(QCloseEvent *event);

public slots:
    void initialize();
    void selectTaskChanged(QModelIndex current, QModelIndex previous);
    void timeStopped(Task* task, TaskLog* taskLog);
    void idleTimeOut();
    void startRecord();
    void stopRecord();
    void createNewTask();
    void editNewTask();
    void deleteTask();
    void completeTask();
    int createNewProject();
    void openProject();
    void editProject();
    void closeProject();
    void exportProjects();
    void importProjects();
    void taskContextMenuRequested(QPoint pos);

    void setActiveTask(Task* task);
    void setActiveTaskLog(Task* task, TaskLog* taskLog);
    void restoreWindowState();

    void settings();
};

#endif	/* _MAINWINDOW_H */
