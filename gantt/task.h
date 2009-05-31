#ifndef TASK_H
#define TASK_H

#include <string>
#include <QDate>

class Task
{
public:
    Task();

public:
    void setName(std::string name);
    void setStartDate(QDate startDate);
    void setEndDate(QDate endDate);

private:
    std::string m_name;
    QDate m_startDate;
    QDate m_endDate;
};

#endif // TASK_H
