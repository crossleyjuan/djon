#include <QTableView>
#include "taskdialog.h"
#include "ui_taskdialog.h"
#include "sstream"
#include "utils.h"
#include "template.h"
#include "logitemmodel.h"

TaskDialog::TaskDialog(Project* project, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::TaskDialog)
{
    m_project = project;
    m_task = new Task();

    m_ui->setupUi(this);
    m_ui->startDate->setDate(QDate::currentDate());
    m_ui->endDate->setDate(QDate::currentDate());
    populateTemplate();
    populateStatus();
    populateTableLog();
    m_ui->tabWidget->setCurrentIndex(0);
}

TaskDialog::TaskDialog(Project* project, Task* task, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::TaskDialog)
{
    m_ui->setupUi(this);
    populateTemplate();

    m_task = task;
    m_ui->shortDescription->setText(QString(task->shortDescription.c_str()));
    m_ui->description->setPlainText(QString(task->longDescription.c_str()));
    m_ui->duration->setText(QString(toString(task->duration).c_str()));
    m_ui->startDate->setDateTime(*toDateTime((int)m_task->startDate));
    m_ui->endDate->setDateTime(*toDateTime((int)m_task->endDate));
    m_ui->totalTime->setTime(*toTime(m_task->totalTime));

    for (int x = 0; x < m_ui->cboTemplate->count(); x++) {
        if (m_task->templateName.compare(m_ui->cboTemplate->itemText(x).toStdString()) == 0) {
            m_ui->cboTemplate->setCurrentIndex(x);
            break;
        }
    }
    populateStatus();
    for (int x = 0; x < m_ui->status->count(); x++) {
        if (m_task->status.compare(m_ui->status->itemText(x).toStdString()) == 0) {
            m_ui->status->setCurrentIndex(x);
            break;
        }
    }

    m_ui->cboTemplate->setEditable(false);
    m_project = project;
    populateTableLog();
    m_ui->tabWidget->setCurrentIndex(0);
}

TaskDialog::~TaskDialog()
{
    delete m_ui;
}

void TaskDialog::populateTemplate() {
    m_templates = readTemplates();
    for (vector<Template*>::iterator iter = m_templates->begin(); iter != m_templates->end(); iter++) {
        Template* temp = *iter;
        m_ui->cboTemplate->addItem(QString(temp->name().c_str()));
    }

    connect(m_ui->cboTemplate, SIGNAL(currentIndexChanged(int)), this, SLOT(populateStatus()));
}

void TaskDialog::populateStatus() {
    m_ui->status->clear();
    Template* tpl = (Template*)m_templates->at(m_ui->cboTemplate->currentIndex());
    vector<string>* statusList = tpl->statusList();
    for (vector<string>::iterator iter = statusList->begin(); iter != statusList->end(); iter++) {
        string status = *iter;
        m_ui->status->addItem(QString(status.c_str()).trimmed());
    }

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
    m_task->shortDescription = m_ui->shortDescription->text().toStdString();
    m_task->longDescription = m_ui->description->document()->toPlainText().toStdString();
    m_task->duration = m_ui->duration->text().toInt();
    m_task->endDate = toInt(m_ui->endDate->dateTime());
    m_task->startDate = toInt(m_ui->startDate->dateTime());
    m_task->templateName = m_ui->cboTemplate->currentText().toStdString();
    m_task->status = m_ui->status->currentText().toStdString();
    m_task->project = m_project;
    m_task->totalTime = toSeconds(m_ui->totalTime->time());

    if (m_task->id.length() == 0) {
        m_task = createTask(m_project, m_task);
    } else {
        updateTask(m_project, m_task);
    }

    taskChanged(m_task);
}

void TaskDialog::on_buttonBox_rejected()
{

}

void TaskDialog::populateTableLog() {
    LogItemModel* model = new LogItemModel(m_project, m_task);
    QTableView* table = m_ui->logView;
    table->setModel(model);
}
