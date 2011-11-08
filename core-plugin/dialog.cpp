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

#include "dialog.h"
#include "ui_dialog.h"

#include "data.h"
#include "util.h"
#include "itemdelegator.h"
#include "taskitemdelegator.h"
#include "assert.h"
#include "pluginsettings.h"
#include <QVBoxLayout>
#include <QMessageBox>

Dialog::Dialog(const Workspace* workspace, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->_workspace = workspace;
    _manager.connect();
    ui->dateEdit->setDate(QDate::currentDate());
    setupGrid();
    showTasks();
    //Width
    ui->taskGrid->setColumnWidth(0, 150);
    ui->taskGrid->setColumnWidth(1, 50);
    ui->taskGrid->setColumnWidth(2, 150);
    ui->taskGrid->setColumnWidth(3, 150);
    QVBoxLayout* vertLayout = ui->verticalLayout;
    vertLayout->setAlignment(Qt::AlignTop);
    ((QHBoxLayout*)ui->horizontalLayout_2)->setAlignment(Qt::AlignLeft);
}

Dialog::~Dialog()
{
    delete ui;
}

DTime calcTime(Task* task, QDate date) {
    vector<TaskLog*>* logs = task->logs(false);
    DTime result;
    for (vector<TaskLog*>::iterator i = logs->begin(); i != logs->end(); i++) {
        TaskLog* log = *i;
        if (*log->start == DateTime(date)) {
            result.add(log->totalTime());
        }
    }
    delete(logs);
    return result;
}

void Dialog::showTasks() {
    vector<Project*>* projects = const_cast< vector<Project*>* >(_workspace->projects());
    int row = 0;
    ui->taskGrid->setColumnCount(5);
    ui->taskGrid->clear();
    ui->taskGrid->setRowCount(0);
    ui->taskGrid->setHorizontalHeaderLabels(QStringList() << tr("Task")
                                                        << tr("Time") << tr("TimeSheet Project")  << tr("TimeSheet Task") << tr("Description"));
    for (std::vector<Project*>::iterator iterProject = projects->begin(); iterProject != projects->end(); iterProject++) {
        Project* project = *iterProject;
        vector<Task*>* tasks = project->tasks();
        for (vector<Task*>::iterator iterT = tasks->begin(); iterT != tasks->end(); iterT++) {
            Task* task = *iterT;
            DTime taskTime = calcTime(task, ui->dateEdit->date());
            if ((task->childCount() == 0) && ( taskTime.totalSecs() > 0)) {
                ui->taskGrid->setRowCount(row+1);
                QTableWidgetItem* djonTask = new QTableWidgetItem();
                djonTask->setData(Qt::DisplayRole, QString(task->shortDescription()->c_str()));
                QString taskData;
                taskData.append(QString(project->id()->c_str()));
                taskData.append(" ");
                taskData.append(QString(task->id()->c_str()));

                djonTask->setData(Qt::UserRole, taskData);
                ui->taskGrid->setItem(row, 0, djonTask);

                ui->taskGrid->setItem(row, 1, new QTableWidgetItem(taskTime.toQString()));

                Map map = _manager.map(task);
                if (map.coreProject.id.length() > 0) {
                    QTableWidgetItem* projectItem = new QTableWidgetItem();
                    projectItem->setData(Qt::UserRole, map.coreProject.index);
                    projectItem->setData(Qt::DisplayRole, map.coreProject.name);;
                    ui->taskGrid->setItem(row, 2, projectItem);
                }
                if (map.coreTask.id.length() > 0) {
                    QTableWidgetItem* taskItem = new QTableWidgetItem();
                    taskItem->setData(Qt::UserRole, map.coreTask.index);
                    taskItem->setData(Qt::DisplayRole, map.coreTask.name);
                    ui->taskGrid->setItem(row, 3, taskItem);
                }
                QTableWidgetItem* descriptionItem = new QTableWidgetItem();
                descriptionItem->setData(Qt::DisplayRole, QString(task->shortDescription()->c_str()));
                ui->taskGrid->setItem(row, 4, descriptionItem);
                row++;
            }
        }
        delete(tasks);
    }
    delete(projects);
}

void Dialog::setupGrid() {
    QList<CoreProject> projects = _manager.projects();
    ui->taskGrid->setItemDelegateForColumn(2, new ItemDelegator(projects, ui->taskGrid));

    QList<CoreTask> tasks = _manager.tasks();
    ui->taskGrid->setItemDelegateForColumn(3, new TaskItemDelegator(tasks, ui->taskGrid));
}

void Dialog::on_pushButton_2_clicked()
{
    qDebug("on_pushButton_2_clicked()");
    QAbstractItemModel* model = ui->taskGrid->model();
    int rows = model->rowCount(QModelIndex());
    CoreProject project;
    CoreTask task;
    int recordsExported = 0;
    for (int x = 0; x < rows; x++) {
        QModelIndex index = model->index(x, 2, QModelIndex());
        if (index.isValid()) {
            if (model->data(index, Qt::UserRole).isValid()) {
                int projectIndex = model->data(index, Qt::UserRole).toInt();
                project = _manager.projects().at(projectIndex);
                qDebug("on_pushButton_2_clicked project set: %s", project.name.toStdString().c_str());
            }
        }
        index = model->index(x, 3, QModelIndex());
        if (index.isValid()) {
            if (model->data(index, Qt::UserRole).isValid()) {
                int taskIndex = model->data(index, Qt::UserRole).toInt();
                task = _manager.tasks().at(taskIndex);
                qDebug("on_pushButton_2_clicked task set: %s", task.name.toStdString().c_str());
            }
        }
        QString description;
        index = model->index(x, 4, QModelIndex());
        if (index.isValid()) {
            description = model->data(index, Qt::DisplayRole).toString();
        }
        if ((task.id.length() > 0) && (project.id.length() > 0) && (description.length() > 0)) {
            Map map;
            map.coreProject = project;
            map.coreTask = task;
            map.description = description;
            index = model->index(x, 0, QModelIndex());
            if (model->data(index, Qt::UserRole).isValid()) {
                QString taskData = model->data(index, Qt::UserRole).toString();
                QStringList sData = taskData.split(" ");
                map.task.projectId = sData.at(0);
                map.task.taskId = sData.at(1);
                qDebug("on_pushButton_2_clicked djontask set: %s", map.task.taskId.toStdString().c_str());
            }

            index = model->index(x, 1, QModelIndex());
            DTime dtime;
            if (model->data(index, Qt::DisplayRole).isValid()) {
                QString time = model->data(index, Qt::DisplayRole).toString();
                map.time = DTime(time.toStdString());
            }
            map.date = DateTime(ui->dateEdit->date());
            if (_manager.saveMap(map)) {
                recordsExported++;
            }
        }
    }
    QMessageBox::information(this, tr("Core Plugin"), QString(format("Exported: %d records", recordsExported).c_str()));
    qDebug("out on_pushButton_2_clicked()");
}

void Dialog::on_dateEdit_dateChanged(QDate date)
{
    showTasks();
}

void Dialog::on_pushButton_clicked()
{
    PluginSettings* psettings = new PluginSettings(this);
    psettings->exec();
}
