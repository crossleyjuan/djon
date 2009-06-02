#include "taskelement.h"
#include "ui_taskelement.h"
#include "utils.h"

TaskElement::TaskElement(Task* task, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::TaskElement)
{
    m_ui->setupUi(this);
    m_ui->txtShort->setText(QString(task->shortDescription.c_str()));
    m_ui->txtDuration->setText(QString(toString(task->duration).c_str()));
    m_ui->txtShort->installEventFilter(this);
    m_ui->txtDuration->installEventFilter(this);
    m_task = task;
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
            taskFocus(m_task);
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

