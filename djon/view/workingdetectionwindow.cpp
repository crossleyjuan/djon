#include "workingdetectionwindow.h"
#include "ui_workingdetectionwindow.h"

#include "data.h"
#include "TaskModel.h"
#include "timetracker.h"
#include "workingdetector.h"
#include <sstream>
#include <string>

WorkingDetectionWindow::WorkingDetectionWindow(std::vector<Project*>* projects, WorkingDetector* workingDetector, TimeTracker* timeTracker, const DateTime since, QWidget *parent) :
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
    _lastTrackedTask = lastTrackedTask(*projects);
    if (_lastTrackedTask != NULL) {
        std::stringstream ssLastActivity;
        ssLastActivity << _lastTrackedTask->shortDescription();
        Task* parent = _lastTrackedTask->parent();
        while (parent != NULL) {
            ssLastActivity << " - " << parent->shortDescription();
            parent = parent->parent();
        }
        ssLastActivity << " - " << _lastTrackedTask->project()->name();

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
        createTaskLog(task, log);
        _timeTracker->startRecord(task, log, _since);
    } else if (m_ui->countToLastTask->isChecked()) {
        TaskLog* log = createTaskLog(_lastTrackedTask);
        log->start = _since;
        log->end = new DateTime();
        createTaskLog(_lastTrackedTask, log);
        _timeTracker->startRecord(_lastTrackedTask, log, _since);
    } else if (m_ui->dontCount->isChecked()) {
        _workingDetector->startDetection();
    }
}

