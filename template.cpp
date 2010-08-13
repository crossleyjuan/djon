#include "template.h"

Template::Template(std::string* name, std::string* description, std::vector<string*>* statusList, std::vector<string*>* subTaskList) {
    if (name->at(name->length() - 1) == '*') {
        _default = true;
        name = new string(name->substr(0, name->length() - 1));
    }
    _name = name;
    _description = description;
    _statusList = statusList;

    for (std::vector<string*>::iterator it = statusList->begin(); it != statusList->end(); it++) {
        string status = **it;
        if (status[status.length()-1] == '*') {
            _closedStatus = new string(status);
        }
    }
    _subTaskList = subTaskList;
}

std::string* Template::name() {
    return _name;
}

std::string* Template::description() {
    return _description;
}

std::string* Template::closedStatus() {
    return _closedStatus;
}

std::vector<string*>* Template::statusList() {
    return _statusList;
}

std::vector<string*>* Template::subTaskList() {
    return _subTaskList;
}

bool Template::isDefault() const {
    return _default;
}
