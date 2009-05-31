#include "task.h"

Task::Task()
{
}


void Task::setName(std::string name) {
    m_name = name;
}

void Task::setStartDate(QDate startDate) {
    m_startDate = startDate;
}

void Task::setEndDate(QDate endDate) {
    m_endDate = endDate;
}
