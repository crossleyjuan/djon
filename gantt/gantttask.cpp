#include "gantttask.h"

GanttTask::GanttTask()
{
}


void GanttTask::setName(std::string name) {
    m_name = name;
}

void GanttTask::setStartDate(QDate* startDate) {
    m_startDate = startDate;
}

void GanttTask::setEndDate(QDate* endDate) {
    m_endDate = endDate;
}

QDate* GanttTask::startDate() {
    return m_startDate;
}

QDate* GanttTask::endDate() {
    return m_endDate;
}
