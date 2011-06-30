/* 
 * File:   project.h
 * Author: cross
 *
 * Created on July 7, 2010, 11:13 AM
 */

#ifndef _PROJECT_H
#define	_PROJECT_H

#include <vector>
#include <string>
#include <map>

using namespace std;

class Task;
class TaskLog;
class DateTime;
class DTime;
class Calendar;

class Project {
public:
    enum PROJECTTYPE {
        NORMAL,
        TODO
    };

    Project();
    Project(string projDef);
    Project(const Project& orig);
    virtual ~Project();

    string* id() const;
    void setId(string* id);
    string* name() const;
    void setName(string* name);
    string* description() const;
    void setDescription(string* description);
    string* projectFileName() const;
    void setProjectFileName(string* projectFileName);
    void setType(PROJECTTYPE type);
    PROJECTTYPE type() const;
//    int getLastTaskId();

    std::vector<Task*>* tasks() const;
    std::vector<TaskLog*>* logs() const;
    void addTask(Task* task);
    std::vector<Task*>* subTasks(string* prefix) const; // prefix should be 1, 2.1, etc.
    std::vector<Task*>* subTasks() const; // return the direct sub tasks
    int removeTask(Task* task);

    DateTime* startDate();
    DateTime* endDate();
    DTime totalTime() const;
    DTime totalTimeCurrentWeek() const;
    DTime totalTimeCurrentDay() const;
    Calendar* projectDefaultCalendar() const;
    int taskCount() const;

    Task* task(string id);
    std::string* nextChildId();
    char* toChar();
    bool operator==(const Project& prj) const;

private:
    string* _id;
    string* _name;
    string* _description;
    string* _projectFileName;
    PROJECTTYPE _type;
    Calendar* _projectDefaultCalendar;
//    int _lastTaskId;

    std::vector<Task*>* _tasks;
    std::map<string, Task*>* _tasksMap;
};

#endif	/* _PROJECT_H */

