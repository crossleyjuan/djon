#include "template.h"

Template::Template(std::string name, std::vector<string>* statusList, std::vector<string>* jobList) {
    m_name = name;
    m_statusList = statusList;

    for (std::vector<string>::iterator it = statusList->begin(); it != statusList->end(); it++) {
        string status = *it;
        if (status[status.length()-1] == '*') {
            m_closedStatus = status;
        }
    }
    m_jobList = jobList;
}

std::string Template::name() {
    return m_name;
}

std::string Template::closedStatus() {
    return m_closedStatus;
}

std::vector<string>* Template::statusList() {
    return m_statusList;
}

std::vector<string>* Template::jobList() {
    return m_jobList;
}
