#include "taskdialog.h"
#include "ui_taskdialog.h"

TaskDialog::TaskDialog(Task* task, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::TaskDialog)
{
    m_task = task;
    if (!m_task) {
        m_task = new Task();
    }
    m_ui->setupUi(this);
}

TaskDialog::~TaskDialog()
{
    delete m_ui;
}

void TaskDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TaskDialog::on_buttonBox_accepted()
{

}
