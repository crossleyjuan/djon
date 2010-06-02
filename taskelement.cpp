#include "taskelement.h"
#include "ui_taskelement.h"
#include "utils.h"
#include <QTimer>
#include <QPicture>
#include <QRadioButton>

TaskElement::TaskElement(Project* project, Task* task, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::TaskElement)
{
    m_ui->setupUi(this);

    m_ui->txtShort->installEventFilter(this);
    m_ui->txtDuration->installEventFilter(this);

    m_task = task;
    m_project = project;
    m_logTime = NULL;
    m_timeRunning = false;
    m_active = false;

    m_ui->txtShort->installEventFilter(this);
    m_ui->txtDuration->installEventFilter(this);

    refreshTask();
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
            taskSelected(this);
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

void TaskElement::refreshTask() {
    m_task = readTask(m_project, m_task->id);
    m_ui->txtShort->setText(QString(m_task->shortDescription.c_str()));
    m_ui->txtDuration->setText(QString(toString(m_task->duration).c_str()));
    refreshTime();
}

void TaskElement::startTimeRecord() {
    if (!m_timeRunning) {
        m_logTime = createTimer(m_project, m_task);
        m_timer = new QTimer(this);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
        m_timer->start(1000);
        m_timeRunning = true;
    }
}

void TaskElement::resetCurrentTimer() {
    if (m_timeRunning) {
        m_logTime->stopTimer();
        m_timer->stop();
        m_logTime = NULL;
        m_timeRunning = false;
        refreshTask();
    }
}

void TaskElement::stopTimeRecord() {
    if (m_timeRunning) {
        m_logTime->stopTimer();
        m_timer->stop();
        QTime* time = m_logTime->time();
        qDebug(time->toString(QString("hh:mm:ss")).toStdString().c_str());
        saveTimer(m_logTime);
        m_logTime = NULL;
        m_timeRunning = false;
        refreshTask();
    }
}

bool TaskElement::isTimeRunning() {
    return m_timeRunning;
}

void TaskElement::setActive(bool _active) {
    m_active = _active;
    Qt::CheckState state;
    if (_active) {
        state = Qt::Checked;
    } else {
        state = Qt::Unchecked;
    }
    m_ui->checkBox->setCheckState(state);
}

void TaskElement::selectTask() {
    taskActive(this);
    setActive(true);
}

void TaskElement::on_checkBox_stateChanged(int )
{
    if (m_ui->checkBox->checkState() == Qt::Checked) {
        taskActive(this);
    }
}

