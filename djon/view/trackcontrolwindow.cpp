#include "trackcontrolwindow.h"
#include "ui_trackcontrolwindow.h"
#include "TaskModel.h"
#include "data.h"
#include "timetracker.h"
#include <QMouseEvent>

TrackControlWindow::TrackControlWindow(std::vector<Project*>* projects, TimeTracker* timeTracker, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrackControlWindow)
{
    ui->setupUi(this);
    _timeTracker = timeTracker;
    refreshProjects(projects); // Qt::FramelessWindowHint |
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowShadeButtonHint |Qt::WindowStaysOnTopHint | Qt::Tool);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(QModelIndex)), this, SLOT(currentIndexChanged(QModelIndex)));
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

void TrackControlWindow::updateCurrentTime() {
    ui->timeEdit->setTime(*_timeTracker->trackedTime().toQTime());
}

void TrackControlWindow::refresh(Task* task) {
    _currentTask = task;
    QModelIndex index = _taskModel->index(task->project(), task);
    if (index.isValid()) {
        ui->comboBox->setCurrentModelIndex(index);
    }
    updateCurrentTime();
}

void TrackControlWindow::trackerStateChanged(Task* task) {
    refresh(task);
    TIMERSTATUS status = _timeTracker->status();
    switch (status) {
    case RUNNING:
        ui->actionButton->setIcon(QIcon(":/img/stop.png"));
//        ui->comboBox->setReadOnly(true);
        break;
    case STOPPED:
        ui->actionButton->setIcon(QIcon(":/img/start.png"));
        ui->comboBox->setReadOnly(false);
        break;
    case PAUSED:
        ui->actionButton->setIcon(QIcon(":/img/start.png"));
        ui->comboBox->setReadOnly(false);
        break;
    }
}

void TrackControlWindow::on_pushButton_clicked()
{
    this->close();
}

void TrackControlWindow::mousePressEvent(QMouseEvent *event)
 {
     if (event->button() == Qt::LeftButton) {
         _dragPosition = event->globalPos() - frameGeometry().topLeft();
         event->accept();
     }
 }

void TrackControlWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - _dragPosition);
        event->accept();
    }
}

void TrackControlWindow::on_actionButton_clicked()
{
    if (_timeTracker->status() == STOPPED) {
        QModelIndex index = ui->comboBox->currentModelIndex();
        if (index.isValid()) {
            Task* task = _taskModel->task(index);
            if (task != NULL) {
                this->_timeTracker->startRecord(task);
            }
        }
    } else {
        _timeTracker->stopRecord();
    }
}

void TrackControlWindow::setCurrentTask(Task *task) {
    refresh(task);
}

void TrackControlWindow::currentIndexChanged(const QModelIndex &index) {
    if (_timeTracker->status() == RUNNING) {
        Task* task = _taskModel->task(index);
        if (task != NULL) {
            _timeTracker->startRecord(task);
        }
    }
}
