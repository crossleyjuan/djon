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

public slots:
    void selectTaskChanged(QModelIndex current, QModelIndex previous);
    void idleTimeOut();
    void startRecord();
    void stopRecord();
    void createNewTask();
    void editNewTask();
    int createNewProject();

    void setActiveTask(Task* task);
    void setActiveTaskLog(Task* task, TaskLog* taskLog);
};

#endif	/* _MAINWINDOW_H */
