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

class TaskLogWindow;
class TimeTracker;
class CurrentTime;
class TaskModel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();
private:
    void createTaskDelegate();
    void createTaskLog();
    void createCurrentTimeWindow();
    void createTray();
    void setupActions();
    void reloadProjects();
    void reloadTasks();

    Ui::MainWindow widget;
    TaskLogWindow* _logWindow;
    std::vector<Project*>* _projects;
    IdleDetector* _idleDetector;
    TimeTracker* _timeTracker;
    TaskLog* _activeLog;
    Project* _activeProject;
    Task* _activeTask;
    CurrentTime* _timeWindow;
    TaskModel* _taskModel;
    QSystemTrayIcon* _trayIcon;
    QMenu* _taskPopUpMenu;

public:
    void closeEvent(QCloseEvent *event);

public slots:
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
    void exportProjects();
    void taskContextMenuRequested(QPoint pos);

    void setActiveTask(Task* task);
    void setActiveTaskLog(Task* task, TaskLog* taskLog);

};

#endif	/* _MAINWINDOW_H */
