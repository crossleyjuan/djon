#ifndef TRACKCONTROLWINDOW_H
#define TRACKCONTROLWINDOW_H

#include <QWidget>
#include <vector>
#include <QModelIndex>

class Project;
class TimeTracker;
class Task;
class TaskModel;

namespace Ui {
    class TrackControlWindow;
}

enum TRACK_POSITION {
    BOTTOM_RIGHT_CORNER
};

class TrackControlWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TrackControlWindow(TimeTracker* timeTracker, QWidget *parent = 0);
    ~TrackControlWindow();
    void setModel(TaskModel* model);
    void showIn(TRACK_POSITION position);

public slots:
    void refresh(Task* task);
    void refreshSettings();
    void trackerStateChanged(Task* task);
    void setCurrentTask(Task* task);
    void updateCurrentTime();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    bool eventFilter(QObject *, QEvent *);

private:
    Ui::TrackControlWindow *ui;
//    std::vector<Project*>* _projects;
    TimeTracker* _timeTracker;
    Task* _currentTask;
    TaskModel* _taskModel;
    QPoint _dragPosition;

private slots:
    void on_actionButton_clicked();
    void on_pushButton_clicked();
    void currentIndexChanged(const QModelIndex& index);
};

#endif // TRACKCONTROLWINDOW_H
