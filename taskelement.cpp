#include "taskelement.h"
#include "ui_taskelement.h"
#include "utils.h"
#include <QTimer>
#include <QPicture>

TaskElement::TaskElement(Project* project, Task* task, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::TaskElement)
{
    m_ui->setupUi(this);
    m_ui->txtShort->setText(QString(task->shortDescription.c_str()));
    m_ui->txtDuration->setText(QString(toString(task->duration).c_str()));

    m_ui->txtShort->installEventFilter(this);
    m_ui->txtDuration->installEventFilter(this);

    m_task = task;
    m_project = project;
    m_logTime = NULL;
    m_timeRunning = false;

    refreshTime();
}

Task* TaskElement::task() {
    return m_task;
}

TaskElement::~TaskElement()
{
    delete m_ui;
}

void TaskElement::refreshTime() {
    int seconds = 0;
    if (m_logTime != NULL) {
        seconds = m_logTime->seconds();
    }
    int currentTaskTime = m_task->totalTime;
    currentTaskTime += seconds;
    QTime* totaltime = toTime(currentTaskTime);

    m_ui->txtCurrentTime->setText(totaltime->toString("hh:mm:ss"));
}

bool TaskElement::eventFilter( QObject *obj, QEvent *ev ) {
    if ((obj == m_ui->txtShort) || (obj == m_ui->txtDuration)) {
        if (ev->type() == QEvent::FocusIn) {
            taskFocus(this);
        }
    }
    return QWidget::eventFilter(obj, ev);

}

void TaskElement::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TaskElement::on_txtShort_editingFinished() {
    m_task->shortDescription = m_ui->txtShort->text().toStdString();
    updateTask(m_project, m_task);
}

void TaskElement::on_txtDuration_editingFinished()
{
    m_task->duration = atoi(m_ui->txtDuration->text().toStdString().c_str());
    updateTask(m_project, m_task);
}

void TaskElement::paintEvent(QPaintEvent *pEvent) {
    if (m_timeRunning) {
/*
        QPicture pic;
        pic.load(QString("images/xclock.png"));
        QPainter p(this);
        p.begin(this);
        p.drawPicture(QPoint(0, 0), pic);
        p.end();
*/
    } else {

    }
}

void TaskElement::timeout() {
    refreshTime();
}

void TaskElement::startTimeRecord() {
    m_logTime = createTimer(m_project, m_task);
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
    m_timer->start(1000);
    m_timeRunning = true;
}

void TaskElement::resetCurrentTimer() {
    m_logTime->stopTimer();
    m_timer->stop();
    m_logTime = NULL;
    m_timeRunning = false;
}

void TaskElement::stopTimeRecord() {
    m_logTime->stopTimer();
    m_timer->stop();
    QTime* time = m_logTime->time();
    qDebug(time->toString(QString("hh:mm:ss")).toStdString().c_str());
    saveTimer(m_logTime);
    m_logTime = NULL;
    m_timeRunning = false;
}
