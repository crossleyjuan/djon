#include "currenttime.h"
#include "ui_currenttime.h"
#include "TaskModel.h"
#include "data.h"

CurrentTime::CurrentTime(std::vector<Project*>* projects, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::CurrentTime)
{
    ui->setupUi(this);
    _projects = projects;

    setupCombo();
}

void CurrentTime::setupCombo() {
}

CurrentTime::~CurrentTime()
{
    delete ui;
}

void CurrentTime::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void CurrentTime::updateTime(DTime& tm) {
    ui->timeEdit->setTime(*tm.toQTime());
}

void CurrentTime::setActiveTask(Task* activeTask) {
    _activeTask = activeTask;
//    ui->taskt->setText(activeTask->shortDescription());
}
