#include "taskdialog.h"
#include "ui_taskdialog.h"
#include "sstream"
#include "utils.h"

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
    m_ui->shortDescription->setText(QString(task->shortDescription.c_str()));
    m_ui->description->setPlainText(QString(task->longDescription.c_str()));
    m_ui->duration->setText(QString(toString(task->duration).c_str()));
    QDateTime startDate;
    startDate.setTime_t(task->startDate);
    m_ui->startDate->setDateTime(startDate);
    QDateTime endDate;
    endDate.setTime_t(task->endDate);
    m_ui->endDate->setDateTime(endDate);
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
    if (m_task->id.length() == 0) {
        m_task->id = toString((int)m_project->tasks.size() + 1);
    }
    m_task->shortDescription = m_ui->shortDescription->text().toStdString();
    m_task->longDescription = m_ui->description->document()->toPlainText().toStdString();
    m_task->duration = m_ui->duration->text().toInt();
    m_task->endDate = m_ui->startDate->dateTime().toTime_t();
    m_task->startDate = m_ui->endDate->dateTime().toTime_t();

    writeFile(m_project->path + "/" + m_task->id + ".tsk", m_task->hashValues());
}
