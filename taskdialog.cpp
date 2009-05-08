#include "taskdialog.h"
#include "ui_taskdialog.h"
#include "sstream"

TaskDialog::TaskDialog(Project* project, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::TaskDialog)
{
    m_project = project;
    m_task = new Task();

    m_ui->setupUi(this);
}

TaskDialog::TaskDialog(Task* task, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::TaskDialog)
{
    m_ui->setupUi(this);

    m_task = task;
    m_ui->name->setText(QString(task->name.c_str()));
    m_ui->description->setPlainText(QString(task->description.c_str()));
    std::stringstream ss;
    ss << std::stringstream::scientific << task->duration;
    m_ui->duration->setText(QString(ss.str().c_str()));
    m_project = &task->project;

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
    m_task->name = m_ui->name->text().toStdString();
    m_task->description = m_ui->description->document()->toPlainText().toStdString();
    m_task->duration = m_ui->duration->text().toInt();
    m_task->endDate = m_ui->endDate->text().toDouble();
    m_task->startDate = m_ui->endDate->text().toDouble();

    writeFile(m_project->path + "/" + m_task->name + ".tsk", m_task->hashValues());
}
