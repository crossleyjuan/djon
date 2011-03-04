#ifndef TASKLOGWINDOW_H
#define TASKLOGWINDOW_H

#include <QtGui/QDockWidget>
#include <QMenu>
#include <QPoint>

namespace Ui {
    class TaskLogWindow;
}

class Task;
class TaskLogModel;
class TimeTracker;

class TaskLogWindow : public QDockWidget {
    Q_OBJECT
public:
    TaskLogWindow(TimeTracker* tracker, QWidget *parent = 0);
    ~TaskLogWindow();
    void refresh(Task* task);

protected:
    void changeEvent(QEvent *e);

public slots:
    void deleteSelectedLogs();
    void contextMenuRequested(QPoint pos);

private slots:
    void logTimeChanged(Task* task);

signals:
    void timeChanged(Task* task);

private:
    void setupMenuActions();

    Ui::TaskLogWindow *m_ui;
    Task* _task;
    QMenu _popUpMenu;
    TaskLogModel* _model;
    TimeTracker* _tracker;
};

#endif // TASKLOGWINDOW_H
