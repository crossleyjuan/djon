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
#include "workingdetector.h"
#include "view/ganttscene.h"

class TaskLogWindow;
class TimeTracker;
class CurrentTime;
class TaskModel;
class TaskHeaderView;
class UpdateManager;
class UserPreferencesController;
class SysTrayIcon;

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
    void refreshCollapsedState();
    void setLastSelectedTask();
    void checkReleaseNotes();

    Ui::MainWindow widget;
    TaskLogWindow* _logWindow;
    std::vector<Project*>* _projects;
    IdleDetector* _idleDetector;
    WorkingDetector* _workingDetector;
    TimeTracker* _timeTracker;
    TaskHeaderView* _taskHeader;
    TaskLog* _activeLog;
    Project* _activeProject;
    Task* _activeTask;
    CurrentTime* _timeWindow;
    TaskModel* _taskModel;
    UserPreferencesController* _userPreferencesController;
    QAction* _recordButton;

    SysTrayIcon* _trayIcon;
    QMenu* _taskPopUpMenu;
    UpdateManager* _updateManager;

public:
    void closeEvent(QCloseEvent *event);

public slots:
    void initialize();
    void selectTaskChanged(QModelIndex current, QModelIndex previous);
    void timeStopped(Task* task, TaskLog* taskLog);
    void idleTimeOut();
    void workingDetected(const DateTime since);
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
    void collapse(const QModelIndex& index);
    void expand(const QModelIndex& index);
    void aboutToQuit();
    void saveWindowState();
    void restoreSavedWindowState();
    void showReleaseNotes();

    void setActiveTask(Task* task);
    void setActiveTaskLog(Task* task, TaskLog* taskLog);
    void restoreWindowState();
    void trackerStarted(Task* task, TaskLog* taskLog);

    void settings();
};

#endif	/* _MAINWINDOW_H */
