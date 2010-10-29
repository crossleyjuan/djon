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

class TrackControlWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TrackControlWindow(std::vector<Project*>* projects, TimeTracker* timeTracker, QWidget *parent = 0);
    ~TrackControlWindow();
    void setProjects(std::vector<Project*>* projects);

public slots:
    void refresh(Task* task);
    void trackerStateChanged(Task* task);
    void setCurrentTask(Task* task);
    void updateCurrentTime();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    bool eventFilter(QObject *, QEvent *);

private:

    Ui::TrackControlWindow *ui;
    std::vector<Project*>* _projects;
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
