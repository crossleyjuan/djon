#include "dialog.h"
#include "ui_dialog.h"

#include "data.h"
#include "util.h"

Dialog::Dialog(const Workspace* workspace, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->_workspace = workspace;
    showTasks();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::showTasks() {
    vector<Project*>* projects = const_cast< vector<Project*>* >(_workspace->projects());
    int row = 0;
    ui->taskGrid->setColumnCount(2);
    DateTime today = DateTime::today(false);
    for (std::vector<Project*>::iterator iterProject = projects->begin(); iterProject != projects->end(); iterProject++) {
        Project* project = *iterProject;
        vector<Task*>* tasks = project->tasks();
        for (vector<Task*>::iterator iterT = tasks->begin(); iterT != tasks->end(); iterT++) {
            Task* task = *iterT;
            vector<TaskLog*>* logs = task->logs();
            DTime taskTime;
            for (vector<TaskLog*>::iterator iLog = logs->begin(); iLog != logs->end(); iLog++) {
                TaskLog* log = *iLog;
                if (*log->start == today) {
                    taskTime = taskTime + log->totalTime();
                }
            }
            if (taskTime.totalSecs() > 0) {
                ui->taskGrid->setRowCount(row+1);
                ui->taskGrid->setItem(row, 0, new QTableWidgetItem(QString(task->shortDescription()->c_str())));
                ui->taskGrid->setItem(row, 1, new QTableWidgetItem(taskTime.toQString()));
                row++;
            }
            delete(logs);
        }
        delete(tasks);
    }
    delete(projects);
}
