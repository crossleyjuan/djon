#include "template.h"

Template::Template(std::string name, std::vector<string>* statusList, std::vector<string>* jobList) {
    m_name = name;
    m_statusList = statusList;
    m_jobList = jobList;
}

std::string Template::name() {
    return m_name;
}

std::vector<string>* Template::statusList() {
    return m_statusList;
}

std::vector<string>* Template::jobList() {
    return m_jobList;
}
