#ifndef TASKLOGWINDOW_H
#define TASKLOGWINDOW_H

#include <QtGui/QDockWidget>

namespace Ui {
    class TaskLogWindow;
}

class Task;

class TaskLogWindow : public QDockWidget {
    Q_OBJECT
public:
    TaskLogWindow(QWidget *parent = 0);
    ~TaskLogWindow();
    void refresh(Task* task);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TaskLogWindow *m_ui;
    Task* _task;
};

#endif // TASKLOGWINDOW_H
