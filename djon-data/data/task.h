/* 
 * File:   Task.h
 * Author: cross
 *
 * Created on July 7, 2010, 11:19 AM
 */

#ifndef _TASK_H
#define	_TASK_H

#include "util.h"
#include "duration.h"
#include <vector>
#include <string>
#include <QColor>

class Project;
class Job;

class TaskLog {
public:
    std::string* id;
    DateTime* start; //YYYYMMDD.HHMMSS
    DateTime* end; //YYYYMMDD.HHMMSS
    DateTime* lastLap; //YYYYMMDD.HHMMSS
    std::string* logDescription;

    DTime totalTime() const;
};

class Task {
public:
    Task(Project* project, const std::string taskDef);
    Task(Project* project);

    Task(const Task& orig);
    virtual ~Task();

    char* toChar();

    int childCount();
    void setTotalTime(DTime* _totalTime);
    DTime totalTime() const;
    DTime totalTimeCurrentWeek() const;
    DTime totalTimeCurrentDay() const;

    void setStatus(std::string* _status);
    std::string* status() const;
    void setTemplateName(std::string* _templateName);
    std::string* templateName() const;
    void setJobs(std::vector<Job*> _jobs);
    std::vector<Job*> jobs() const;
    void setProject(Project* _project);
    Project* project() const;
    void setEndDate(DateTime* _endDate);
    DateTime* endDate() const;
    void setStartDate(DateTime* _startDate);
    DateTime* startDate() const;
    void setDuration(Duration _duration);
    Duration duration() const;
    void setLongDescription(std::string* _longDescription);
    std::string* longDescription() const;
    void setShortDescription(std::string* _shortDescription);
    std::string* shortDescription() const;
    void setId(string* _id);
    string* id() const;
    Task* parent() const;
    bool isClosed() const;
    void setClosed(bool);
    QColor taskColor() const;

    void processTemplate();

    void addLog(TaskLog* log);
    bool removeLog(TaskLog* log);
    std::vector<TaskLog*>* logs() const;
    std::vector<Task*>* subTasks() const;
    std::string* nextChildId();

    bool operator==(const Task& task) const;

private:
    string* _id;
    std::string* _shortDescription;
    std::string* _longDescription;
    Duration _duration;
    DateTime* _startDate;
    DateTime* _endDate;
    Project* _project;
    std::vector<Job*> _jobs;
    std::string* _templateName;
    std::string* _status;
    DTime* _totalTime; // Time in secs
    int _childCount;
    QColor _taskColor;

    std::vector<TaskLog*>* _logs;
    std::vector<Task*>* _subTasks;

};

/*** Global Task Related functions **/
Task* lastTrackedTask(vector<Project*> projects);

#endif	/* _TASK_H */

