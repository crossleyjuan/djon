#include "trackcontrolwindow.h"
#include "ui_trackcontrolwindow.h"
#include "TaskModel.h"
#include "data.h"
#include "timetracker.h"

TrackControlWindow::TrackControlWindow(std::vector<Project*>* projects, TimeTracker* timeTracker, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrackControlWindow)
{
    ui->setupUi(this);
    _timeTracker = timeTracker;
    refreshProjects(projects);
    setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
}

TrackControlWindow::~TrackControlWindow()
{
    delete ui;
}

void TrackControlWindow::refreshProjects(std::vector<Project*>* projects) {
    _projects = projects;
    _taskModel = new TaskModel(ONLY_TASKS, *_projects, this);
    ui->comboBox->setModel(_taskModel);
}

void TrackControlWindow::refresh(Task* task) {
    _currentTask = task;
    QModelIndex index = _taskModel->index(task->project(), task);
    if (index.isValid()) {
        int row = 0;
        while (index.parent() != QModelIndex()) {
            row += index.row();
            index = index.parent();
        }
        ui->comboBox->setCurrentIndex(row);
    }
    ui->timeEdit->setTime(*_timeTracker->trackedTime().toQTime());
}

void TrackControlWindow::trackerStateChanged(Task* task) {
    refresh(task);
    TIMERSTATUS status = _timeTracker->status();
    switch (status) {
    case RUNNING:
        ui->actionButton->setIcon(QIcon(":/img/play_running.png"));
        break;
    case STOPPED:
        ui->actionButton->setIcon(QIcon(":/img/start.png"));
        break;
    case PAUSED:
        ui->actionButton->setIcon(QIcon(":/img/start.png"));
        break;
    }
}

void TrackControlWindow::on_pushButton_clicked()
{
    this->close();
}
