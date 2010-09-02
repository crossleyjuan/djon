#ifndef DJONPREFERENCES_H
#define DJONPREFERENCES_H

#include "data.h"
#include <string>
#include <vector>

class Element {
public:
    Element(std::string* project, std::string* task = 0) {
        _project = project;
        _task = task;
    }

    bool isTask() {
        return (_task == NULL);
    }

    std::string* task() {
        return _task;
    }

    std::string* project() {
        return _project;
    }
private:
    std::string* _project;
    std::string* _task;
};

void addCollapsedElement(Project* project, Task* task = 0);
void removeCollapsedElement(Project* project, Task* task = 0);
std::vector<Element*>* collapsedElements();
void saveLastTrackedTask(Task* task);
Element* lastTrackedTaskId();

#endif // DJONPREFERENCES_H
