#ifndef TASK_H
#define TASK_H

#include <string>
#include <QDate>

class GanttTask
{
public:
    GanttTask();

public:
    void setName(std::string name);
    void setStartDate(QDate* startDate);
    void setEndDate(QDate* endDate);
    QDate* startDate();
    QDate* endDate();

private:
    std::string m_name;
    QDate* m_startDate;
    QDate* m_endDate;
};

#endif // TASK_H
