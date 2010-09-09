#ifndef CURRENTTIME_H
#define CURRENTTIME_H

#include <vector>
#include <QDockWidget>

class TaskModel;
class DTime;
class Task;
class Project;


namespace Ui {
    class CurrentTime;
}

class CurrentTime : public QDockWidget {
    Q_OBJECT
public:
    CurrentTime(std::vector<Project*>* projects, QWidget *parent = 0);
    ~CurrentTime();

protected:
    void changeEvent(QEvent *e);

public slots:
    void updateTime(Task* task, DTime& currentLogTime, DTime& totalTaskTime);
    void setActiveTask(Task* task);

private:
    void setupCombo();
    Ui::CurrentTime *ui;
    std::vector<Project*>* _projects;
    Task* _activeTask;
};

#endif // CURRENTTIME_H
