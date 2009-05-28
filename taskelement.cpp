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
}

Task* TaskElement::task() {
}

TaskElement::~TaskElement()
{
    delete m_ui;
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
