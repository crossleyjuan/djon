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
class DateTime;
class DTime;

class Project {
public:
    Project();
    Project(string projDef);
    Project(const Project& orig);
    virtual ~Project();

    string* name();
    void setName(string* name);
    string* description();
    void setDescription(string* description);
//    int getLastTaskId();

    std::vector<Task*>* tasks();
    void addTask(Task* task);
    std::vector<Task*>* subTasks(string* prefix); // prefix should be 1, 2.1, etc.
    std::vector<Task*>* subTasks(); // return the direct sub tasks
    void removeTask(Task* task);

    DateTime* startDate();
    DateTime* endDate();
    DTime* totalTime();
    Task* task(string& id);
    std::string* nextChildId();
    char* toChar();

private:
    string* _name;
    string* _description;
//    int _lastTaskId;

    std::vector<Task*>* _tasks;
    std::map<string, Task*>* _tasksMap;
};

#endif	/* _PROJECT_H */

