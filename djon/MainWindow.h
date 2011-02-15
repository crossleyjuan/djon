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
class TrackControlWindow;
class AbstractViewer;

enum VIEW_TYPE {
    Gantt_View,
    Log_View,
    Time_View
};

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
    void setupTemplateMenu(QMenu* menu);
    void reloadProjects();
    void reloadTasks();
    void showErrorMessage(int errorCode, const char* errorMessage, QWidget* parent);
    void showErrorMessage(const char* errorMessage, QWidget* parent);
    void refreshCollapsedState();
    void setLastSelectedTask();
    void checkReleaseNotes();
    void showPopup();
    void changeCurrentView(VIEW_TYPE type);

    Ui::MainWindow widget;
    TaskLogWindow* _logWindow;
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
    QSignalMapper* _templateMapper;

    // Actions
    QAction* _recordButton;
    QAction* _stopButton;
    QAction* _filterClosedAction;
    QAction* _ganttViewAction;
    QAction* _logViewAction;
    QAction* _timeViewAction;

    TrackControlWindow *_trackWindow;

    SysTrayIcon* _trayIcon;
    QMenu* _taskPopUpMenu;
    UpdateManager* _updateManager;
    AbstractViewer* _currentView;
    VIEW_TYPE _currentViewType;

public:
    void closeEvent(QCloseEvent *event);

private slots:
    void onMenuChangeView(QAction* action);

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
    void saveUserSessionState();
    void restoreUserSessionState();
    void showReleaseNotes();
    void filterClosedTasks();

    void setActiveTask(Task* task);
    void setActiveTaskLog(Task* task, TaskLog* taskLog);
    void trackerStarted(Task* task, TaskLog* taskLog);
    void restoreWindowState();
    void applyTemplate(QString templateName);

    void settings();
};

#endif	/* _MAINWINDOW_H */
