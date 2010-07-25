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
    m_ui->comboBox->setModel(new TaskModel(*projects));

    /*
    setWindowFlags(Qt::Dialog |
                   Qt::WindowStaysOnTopHint |
                   Qt::CustomizeWindowHint |
                   Qt::WindowTitleHint);
                   */
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
    }
}
