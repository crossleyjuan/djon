// *********************************************************************************************************************
// file:
// author: Juan Pablo Crossley (crossleyjuan@gmail.com)
// created:
// updated:
// license:
// 
// This file is part of the djon project, all the source code is licensed under the terms of the GPL license,
// the application and libraries are provided as-is and free of use under the terms explained in the file COPYING.GPL
// the authors are not responsible for any problem that this program, or any of its libraries, may cause.
// Its authors create this application in order to make the world a better place to live, but you should use it on
// your own risks.
// 
// Also, be adviced that, the GPL license force the committers to ensure this application will be free of use, thus
// if you do any modification you will be required to provide it for free unless you use it for personal use (you may 
// charge yourself if you want), bare in mind that you will be required to provide a copy of the license terms that ensures
// this program will be open sourced and all its derivated work will be too.
// *********************************************************************************************************************

#include "workingdetectionwindow.h"
#include "ui_workingdetectionwindow.h"

#include "data.h"
#include "TaskModel.h"
#include "timetracker.h"
#include "workingdetector.h"
#include <sstream>
#include <string>

WorkingDetectionWindow::WorkingDetectionWindow(const std::vector<Project*>* projects, WorkingDetector* workingDetector, TimeTracker* timeTracker, const DateTime since, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::WorkingDetectionWindow)
{
    m_ui->setupUi(this);
    this->_projects = projects;
    m_ui->comboBox->setModel(new TaskModel(ONLY_TASKS, *projects));

    _since = new DateTime(since);
    _timeTracker = timeTracker;
    _workingDetector = workingDetector;
    std::stringstream ssMessage;

    ssMessage << "d-jon detected that you have been working since: ";
    ssMessage << _since->toQDateTime().toString(tr("hh:mm:ss AP")).toStdString();
    ssMessage << ", what do you want to do with the detected time?";

    std::string mess = ssMessage.str();
    QString text(mess.c_str());
    m_ui->idleMessage->setText(text);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    m_ui->comboBox->setEnabled(false);
    m_ui->comboBox->setMinimumWidth(350);
    _lastTrackedTask = lastTrackedTask(*projects);
    if (_lastTrackedTask != NULL) {
        std::stringstream ssLastActivity;
        ssLastActivity << *_lastTrackedTask->shortDescription();
        Task* parent = _lastTrackedTask->parent();
        while (parent != NULL) {
            ssLastActivity << " - " << *parent->shortDescription();
            parent = parent->parent();
        }
        ssLastActivity << " - " << *_lastTrackedTask->project()->name();

        m_ui->lblLastActivity->setText(ssLastActivity.str().c_str());
        m_ui->lblLastActivity->setVisible(true);
        m_ui->countToLastTask->setVisible(true);
    } else {
        m_ui->lblLastActivity->setVisible(false);
        m_ui->countToLastTask->setVisible(false);
    }

    connect(this, SIGNAL(accepted()), this, SLOT(on_accepted()));
    connect(m_ui->countToTask, SIGNAL(toggled(bool)), m_ui->comboBox, SLOT(setEnabled(bool)));
}

WorkingDetectionWindow::~WorkingDetectionWindow()
{
    delete m_ui;
}

void WorkingDetectionWindow::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void WorkingDetectionWindow::on_accepted()
{
    if (m_ui->countToTask->isChecked()) {
        TreeComboBox* box = m_ui->comboBox;
        TaskModel* model = (TaskModel*)box->model();
        QModelIndex index = box->currentModelIndex();
        Task* task = model->task(index);
        TaskLog* log = createTaskLog(task);
        log->start = _since;
        log->end = new DateTime();
        saveProject(task->project());
        _timeTracker->startRecord(task, log, _since);
    } else if (m_ui->countToLastTask->isChecked()) {
        TaskLog* log = createTaskLog(_lastTrackedTask);
        log->start = _since;
        log->end = new DateTime();
        saveProject(_lastTrackedTask->project());
        _timeTracker->startRecord(_lastTrackedTask, log, _since);
    } else if (m_ui->dontCount->isChecked()) {
        _workingDetector->startDetection();
    }
}

