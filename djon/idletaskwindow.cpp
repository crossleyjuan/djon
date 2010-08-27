#include "idletaskwindow.h"
#include "ui_idletaskwindow.h"

#include "data.h"
#include "TaskModel.h"
#include "timetracker.h"

IdleTaskWindow::IdleTaskWindow(std::vector<Project*>* projects, TimeTracker* timeTracker, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::IdleTaskWindow)
{
    m_ui->setupUi(this);
    this->_projects = projects;
    m_ui->comboBox->setModel(new TaskModel(ONLY_TASKS, *projects));

    QTime current = QTime::currentTime();
    QString text("You have been idle since: " + current.toString(tr("hh:mm:ss AP")) + ", what do you want to do with the registered time?");
    m_ui->idleMessage->setText(text);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    _timeTracker = timeTracker;
    connect(this, SIGNAL(accepted()), this, SLOT(on_accepted()));
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
        _timeTracker->destroyCurrentRecord();
    } else if (m_ui->countToTask->isChecked()) {
        TreeComboBox* box = m_ui->comboBox;
        TaskModel* model = (TaskModel*)box->model();
        QModelIndex index = box->currentModelIndex();
        Task* task = model->task(index);
        _timeTracker->moveCurrentRecordToTask(task);
        emit currentTaskChanged(task);
    }
}
