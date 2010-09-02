#ifndef DJONPREFERENCES_H
#define DJONPREFERENCES_H

#include "data.h"
#include <string>
#include <vector>

class CollapsedElement {
public:
    CollapsedElement(std::string* project, std::string* task = 0) {
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
std::vector<CollapsedElement*>* collapsedElements();

#endif // DJONPREFERENCES_H
