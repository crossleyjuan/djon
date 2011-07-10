#include "idletaskwindow.h"
#include "ui_idletaskwindow.h"

#include "data.h"
#include "TaskModel.h"
#include "timetracker.h"
#include "util.h"
#include <sstream>
#include <string>
#include <QMessageBox>

IdleTaskWindow::IdleTaskWindow(const std::vector<Project*>* projects, TimeTracker* timeTracker, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::IdleTaskWindow)
{
    m_ui->setupUi(this);
    this->_projects = projects;
    m_ui->comboBox->setModel(new TaskModel(ONLY_TASKS, *projects));

    std::stringstream ssMessage;
    ssMessage << "The timer was running and registering time to: ";
    ssMessage << *timeTracker->task()->shortDescription() << ".\n";
    ssMessage << "You have been idle since: ";
    ssMessage << timeTracker->lastLapTime()->toQDateTime().toString(tr("hh:mm:ss AP")).toStdString();
    ssMessage << ", what do you want to do with the registered time?";

    std::string mess = ssMessage.str();
    QString text(mess.c_str());
    m_ui->idleMessage->setText(text);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    _timeTracker = timeTracker;
    m_ui->comboBox->setEnabled(false);
    m_ui->comboBox->setMinimumWidth(350);
    connect(this, SIGNAL(accepted()), this, SLOT(on_accepted()));
    connect(m_ui->countToTask, SIGNAL(toggled(bool)), m_ui->comboBox, SLOT(setEnabled(bool)));
    adjustSize();
}

IdleTaskWindow::~IdleTaskWindow()
{
    delete m_ui;
}

void IdleTaskWindow::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void IdleTaskWindow::on_accepted()
{
    if (m_ui->dontCount->isChecked()) {
        _timeTracker->removeLapTime();
        _timeTracker->dropRecordedTime();
    } else if (m_ui->countToTask->isChecked()) {
        TreeComboBox* box = m_ui->comboBox;
        TaskModel* model = (TaskModel*)box->model();
        QModelIndex index = box->currentModelIndex();
        Task* task = model->task(index);
        _timeTracker->moveLappedRecordToTask(task);
        emit currentTaskChanged(task);
    }
    _timeTracker->cleanLapTime();
}

void IdleTaskWindow::done(int res) {
    bool valid = true;
    if (res == QDialog::Accepted) {
        if (m_ui->countToTask->isChecked()) {
            TreeComboBox* box = m_ui->comboBox;
            TaskModel* model = (TaskModel*)box->model();
            QModelIndex index = box->currentModelIndex();
            Task* task = model->task(index);
            if (task == NULL) {
                QMessageBox box(this);
                box.setWindowTitle(tr("d-jon idle window"));
                box.setText("You didn't select a destination task for the current time, please select a task from the list.");
                box.setStandardButtons(QMessageBox::Ok);
                box.exec();
                valid = false;
            }
        }
    }
    if (valid) {
        QDialog::done(res);
    }
}

