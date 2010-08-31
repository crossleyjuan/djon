#include "userpreferencescontroller.h"
#include "util.h"
#include "data.h"
#include "TaskModel.h"
#include <sstream>
#include <string>

using namespace std;

UserPreferencesController::UserPreferencesController(const TaskModel* model) {
    _model = model;
}

//std::vector<QModelIndex>* UserPreferencesController::collapsed() {
//    string collapsed(readPreference(string("collapsed"), ""));
//
//    std::vector<string*>* tokens = split(collapsed, "--");
//
//    std::vector<QModelIndex>* result = new std::vector<QModelIndex>();
//    for (std::vector<string*>::iterator iter = tokens->begin(); iter != tokens->end(); iter++) {
//        string* token = *iter;
//        vector<string*>* subtokens = split(*token, "++");
//        string* project = subtokens->at(0);
//        string* task = subtokens->at(1);
//    }
//    delete(tokens);
//}

void UserPreferencesController::collapsed(const QModelIndex index) {
    Project* project = _model->project(index);
    Task* task = _model->task(index);
    string* projName = project->name();
    string* taskId = NULL;
    if (task != NULL) {
        taskId = task->id();
    }

    std::stringstream ss;
    ss << *projName << "++";
    if (task != NULL) {
        ss << *task->id()  << "--";
    } else {
        ss << "--";
    }

    string collapsedElement = ss.str();

    std::string currentPref = string(readPreference(string("collapsed"), ""));

    size_t pos = currentPref.find(collapsedElement);
    if (pos == currentPref.npos) {
        currentPref.append(collapsedElement);
        writePreference(string("collapsed"), currentPref);
    }

}

void UserPreferencesController::expanded(const QModelIndex& index) {
    Project* project = _model->project(index);
    Task* task = _model->task(index);
    string* projName = project->name();
    string* taskId = NULL;
    if (task != NULL) {
        taskId = task->id();
    }

    std::stringstream ss;
    ss << *projName << "++";
    if (task != NULL) {
        ss << *task->id()  << "--";
    } else {
        ss << "--";
    }

    string collapsedElement = ss.str();

    std::string currentPref = string(readPreference(string("collapsed"), ""));

    int pos = currentPref.find(collapsedElement);
    if (pos != currentPref.npos) {
        int end = collapsedElement.length();
        currentPref = currentPref.replace(pos, end, "");
        writePreference("collapsed", currentPref.c_str());
    }
}
