#include "djonpreferences.h"
#include "util.h"
#include <sstream>
#include <string>

void addCollapsedElement(Project* project, Task* task) {
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

void removeCollapsedElement(Project* project, Task* task) {
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

std::vector<Element*>* collapsedElements() {
    std::vector<Element*>* result = new std::vector<Element*>();
    std::string currentPref = string(readPreference(string("collapsed"), ""));

    std::vector<string> elements = split(currentPref, "--");

    for (std::vector<string>::iterator iter = elements.begin(); iter != elements.end(); iter++) {
        string elem = *iter;
        std::vector<string> subelements = split(elem, "++");
        std::string* project = new std::string(subelements.at(0));
        std::string* task = NULL;
        if (subelements.size() > 1) {
            task = new string(subelements.at(1));
        }
        if ((task != NULL) && (task->length() == 0)) {
            task = NULL;
        }
        Element* collapsed = new Element(project, task);
        result->push_back(collapsed);
    }
    return result;
}

void saveLastTrackedTask(Task* task) {
    stringstream ss;
    ss << *task->project()->name() << "++" << *task->id();

    string taskDef = ss.str();

    writePreference("last-tracked-task", taskDef);
}

 Element* lastTrackedTaskId() {
    std::string currentPref = string(readPreference(string("last-tracked-task"), ""));

    if (currentPref.length() > 0) {
        std::vector<string> elements = split(currentPref, "++");
        string* project = new string(elements.at(0));
        string* task = new string(elements.at(1));

        Element* element = new Element(project, task);
        return element;
    } else {
        return NULL;
    }
}
