#include <QTableView>
#include "taskdialog.h"
#include "ui_taskdialog.h"
#include "sstream"
#include "util.h"
#include "template.h"
#include "tasklogmodel.h"

TaskDialog::TaskDialog(Project* project, string* id, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::TaskDialog)
{
    m_project = project;
    m_task = new Task(project);
    _update = false;
    _id = id;

    m_ui->setupUi(this);
    m_ui->startDate->setDate(QDate::currentDate());
    m_ui->endDate->setDate(QDate::currentDate());
    populateTemplate();
    populateStatus();
    m_ui->tabWidget->setCurrentIndex(0);
    m_ui->cboTemplate->setFocus();
    connect(m_ui->duration, SIGNAL(editingFinished()), this, SLOT(refreshEndDate()));
    connect(m_ui->startDate, SIGNAL(dateChanged(QDate)), this, SLOT(refreshEndDate()));
}

TaskDialog::TaskDialog(Project* project, Task* task, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::TaskDialog)
{
    m_ui->setupUi(this);
    populateTemplate();

    m_task = task;
    _update = true;
    _id = task->id();
    m_ui->shortDescription->setText(QString(task->shortDescription()->c_str()));
    if (task->longDescription() != NULL) {
        m_ui->description->setPlainText(QString(task->longDescription()->c_str()));
    }
    m_ui->duration->setDuration(task->duration());
    m_ui->startDate->setDateTime(m_task->startDate()->toQDateTime());
    m_ui->endDate->setDateTime(m_task->endDate()->toQDateTime());

    for (int x = 0; x < m_ui->cboTemplate->count(); x++) {
        if (m_task->templateName()->compare(m_ui->cboTemplate->itemText(x).toStdString()) == 0) {
            m_ui->cboTemplate->setCurrentIndex(x);
            break;
        }
    }
    populateStatus();
    for (int x = 0; x < m_ui->status->count(); x++) {
        if (m_task->status()->compare(m_ui->status->itemText(x).toStdString()) == 0) {
            m_ui->status->setCurrentIndex(x);
            break;
        }
    }

    m_project = project;
    m_ui->tabWidget->setCurrentIndex(0);
    m_ui->cboTemplate->setVisible(false);
    m_ui->lblTemplate->setVisible(false);
    m_ui->shortDescription->setFocus();

    connect(m_ui->duration, SIGNAL(editingFinished()), this, SLOT(refreshEndDate()));
    connect(m_ui->startDate, SIGNAL(dateChanged(QDate)), this, SLOT(refreshEndDate()));
}

TaskDialog::~TaskDialog()
{
    delete m_ui;
}

void TaskDialog::populateTemplate() {
    m_templates = readTemplates();
    for (vector<Template*>::iterator iter = m_templates->begin(); iter != m_templates->end(); iter++) {
        Template* temp = *iter;
        m_ui->cboTemplate->addItem(QString(temp->description()->c_str()), QString(temp->name()->c_str()));
    }

    connect(m_ui->cboTemplate, SIGNAL(currentIndexChanged(int)), this, SLOT(populateStatus()));
}

void TaskDialog::populateStatus() {
    m_ui->status->clear();
    Template* tpl = (Template*)m_templates->at(m_ui->cboTemplate->currentIndex());
    vector<string> statusList = tpl->statusList();
    for (vector<string>::iterator iter = statusList.begin(); iter != statusList.end(); iter++) {
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

void TaskDialog::done(int res) {
    bool valid = true;

    if (res == 1) {
        if (m_ui->shortDescription->text().length() == 0) {
            QMessageBox box(this);
            box.setText(tr("The short description of the task is required"));
            box.setWindowTitle(tr("d-jon"));
            box.exec();
            m_ui->shortDescription->setFocus();
            valid = false;
        }
        if (valid) {
            DateTime start(m_ui->startDate->dateTime());
            DateTime end(m_ui->endDate->dateTime());
            if (end < start) {
                QMessageBox box(this);
                box.setText(tr("End Date cannot be lesser than Start Date"));
                box.setWindowTitle(tr("d-jon"));
                box.exec();
                m_ui->endDate->setFocus();
                valid = false;
            }
        }
    }
    if (valid && (res == 1)) {
        save();
    }
    if (valid) {
        QDialog::done(res);
    }
}

void TaskDialog::save() {
    m_task->setId(_id);
    m_task->setShortDescription(new string(m_ui->shortDescription->text().toStdString()));
    m_task->setLongDescription(new string(m_ui->description->document()->toPlainText().toStdString()));
    m_task->setDuration(m_ui->duration->duration());
    m_task->setEndDate(new DateTime(m_ui->endDate->dateTime()));
    m_task->setStartDate(new DateTime(m_ui->startDate->dateTime()));
    QString templateName = m_ui->cboTemplate->itemData(m_ui->cboTemplate->currentIndex()).toString();
    m_task->setTemplateName(new string(templateName.toStdString()));
    m_task->setStatus(new string(m_ui->status->currentText().toStdString()));
    m_task->setProject(m_project);

    if (!_update) {
        m_project->addTask(m_task);
        if (errorOcurred()) {
            return;
        }
        m_task->processTemplate();
    }
    saveProject(m_project);
}

void TaskDialog::on_buttonBox_accepted()
{
}

void TaskDialog::on_buttonBox_rejected()
{

}

Task* TaskDialog::task() {
    return m_task;
}

void TaskDialog::refreshEndDate() {
    Duration duration = m_ui->duration->duration();
    if (duration.days() > 0) {
        DateTime* startDate = new DateTime(m_ui->startDate->dateTime());
        DateTime endDate = startDate->addDays(duration.days() - 1, *m_project->projectDefaultCalendar());
        m_ui->endDate->setDateTime(endDate.toQDateTime());
    }
}

