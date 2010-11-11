#include "trackcontrolwindow.h"
#include "ui_trackcontrolwindow.h"
#include "TaskModel.h"
#include "data.h"
#include "timetracker.h"
#include "settings.h"
#include <QMouseEvent>
#include <QDesktopWidget>

TrackControlWindow::TrackControlWindow(TimeTracker* timeTracker, QWidget *parent) :
        QWidget(parent),
        ui(new Ui::TrackControlWindow)
{
    ui->setupUi(this);
    _timeTracker = timeTracker;
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowShadeButtonHint |Qt::WindowStaysOnTopHint | Qt::Tool);
//    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowShadeButtonHint |Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_Hover, true);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(QModelIndex)), this, SLOT(currentIndexChanged(QModelIndex)));
    installEventFilter(this);
    ui->comboBox->installEventFilter(this);
    refreshSettings();
}

TrackControlWindow::~TrackControlWindow()
{
    delete ui;
}

void TrackControlWindow::setModel(TaskModel* model) {
    _taskModel = model;
    ui->comboBox->setModel(_taskModel);
    ui->timeEdit->setTime(QTime(0,0,0,0));
}

void TrackControlWindow::updateCurrentTime() {
    if (_timeTracker->status() == RUNNING) {
        ui->timeEdit->setTime(_timeTracker->trackedTime().toQTime());
    } else {
        if (_currentTask != NULL) {
            ui->timeEdit->setTime(_currentTask->totalTime().toQTime());
        } else {
            ui->timeEdit->setTime(QTime(0, 0, 0, 0));
        }
    }
}

void TrackControlWindow::refresh(Task* task) {
    _currentTask = task;
    if (_currentTask != NULL) {
        QModelIndex index = _taskModel->index(task->project(), task);
        if (index.isValid()) {
            ui->comboBox->setCurrentModelIndex(index);
        }
    } else {
        ui->comboBox->setCurrentModelIndex(QModelIndex());
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

bool TrackControlWindow::eventFilter(QObject *obj, QEvent *evt) {
    if (evt->type() == QEvent::HoverEnter) {
        this->setWindowOpacity(1);
    }
    if (evt->type() == QEvent::HoverLeave) {
        refreshSettings();
    }
    return QWidget::eventFilter(obj, evt);
}

void TrackControlWindow::refreshSettings() {
    if (getSettings()->transparency() == 0) {
        hide();
    } else {
        this->setWindowOpacity((double)getSettings()->transparency() / (double)100);
        show();
    }
}

void TrackControlWindow::showIn(TRACK_POSITION position) {
    switch (position) {
    case BOTTOM_RIGHT_CORNER:
        int x1 = qApp->desktop()->width() - this->width();
        int y1 = qApp->desktop()->height() - this->height() - 30; // taskbar size
        QRect rect = geometry();
        rect.setX(x1);
        rect.setY(y1);
        rect.setWidth(geometry().width());
        rect.setHeight(geometry().height());
        this->setGeometry(rect);
    }
}
