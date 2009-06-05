#include "taskelement.h"
#include "ui_taskelement.h"
#include "utils.h"
#include "logtime.h"

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
}

Task* TaskElement::task() {
    return m_task;
}

TaskElement::~TaskElement()
{
    delete m_ui;
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

void TaskElement::startTimeRecord() {
    createTimer(m_task);
}

void TaskElement::stopTimeRecord() {
    LogTime* timer = getTimer(m_task);
    timer->stopTimer();
    QTime* time = timer->time();
    qDebug(time->toString(QString("hh:mm:ss")).toStdString().c_str());
}
