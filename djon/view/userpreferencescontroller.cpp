#include "userpreferencescontroller.h"
#include "data.h"
#include "TaskModel.h"
#include "djonpreferences.h"

using namespace std;

UserPreferencesController::UserPreferencesController(const TaskModel* model) {
    _model = model;
}

void UserPreferencesController::collapsed(const QModelIndex index) {
    Project* project = _model->project(index);
    Task* task = _model->task(index);

    addCollapsedElement(project, task);
}

void UserPreferencesController::expanded(const QModelIndex& index) {
    Project* project = _model->project(index);
    Task* task = _model->task(index);
    removeCollapsedElement(project, task);
}

void UserPreferencesController::setLastTrackedTask(const QModelIndex& index) {
    Project* project = _model->project(index);
    Task* task = _model->task(index);
    saveLastTrackedTask(task);
}
