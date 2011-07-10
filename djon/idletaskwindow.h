#ifndef IDLETASKWINDOW_H
#define IDLETASKWINDOW_H

#include <QtGui/QDialog>
#include <vector>

class Project;
class Task;
class TimeTracker;

namespace Ui {
    class IdleTaskWindow;
}

class IdleTaskWindow : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(IdleTaskWindow)
public:
    explicit IdleTaskWindow(const std::vector<Project*>* projects, TimeTracker* timeTracker, QWidget *parent = 0);
    virtual ~IdleTaskWindow();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::IdleTaskWindow *m_ui;
    const std::vector<Project*>* _projects;
    TimeTracker* _timeTracker;

public slots:
    void on_accepted();
    virtual void done(int);

signals:
    void currentTaskChanged(Task* task);


};

#endif // IDLETASKWINDOW_H
