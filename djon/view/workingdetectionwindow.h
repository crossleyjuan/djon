#ifndef WORKINGDETECTIONWINDOW_H
#define WORKINGDETECTIONWINDOW_H

#include <QDialog>
#include <vector>
#include "util.h"

class Project;
class TimeTracker;
class Task;
class WorkingDetector;

namespace Ui {
    class WorkingDetectionWindow;
}

class WorkingDetectionWindow : public QDialog {
    Q_OBJECT
public:
    explicit WorkingDetectionWindow(std::vector<Project*>* projects, WorkingDetector* workingDetector, TimeTracker* timeTracker, const DateTime since, QWidget *parent = 0);
    ~WorkingDetectionWindow();

protected:
    void changeEvent(QEvent *e);

public slots:
    void on_accepted();

private:
    Ui::WorkingDetectionWindow *m_ui;
    std::vector<Project*>* _projects;
    DateTime* _since;
    TimeTracker* _timeTracker;
    WorkingDetector* _workingDetector;

signals:
    void currentTaskChanged(Task* task);
};

#endif // WORKINGDETECTIONWINDOW_H
