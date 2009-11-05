#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include <vector>
#include "fileutils.h"

struct Task;

struct Project {
    std::string name;
    std::string description;
    std::string path;
    int lastTaskId;
    hashmap* hashValues();
};

struct Job {
    int id;
    std::string name;
    std::string description;
    int estimated;
    int status;
    int usedtime;
    Task* task;
};

struct Task {
    std::string id;
    std::string shortDescription;
    std::string longDescription;
    int duration;
    uint startDate;
    uint endDate;
    Project* project;
    std::vector<Job*> jobs;
    std::string templateName;
    std::string status;
    hashmap* hashValues();
    uint totalTime;// Time in secs
    int childCount;
};

void updateProject(Project* project);
Project* readProject(std::string path, std::string projectName);
void refreshProject(Project* project);
std::vector<Task*> readTasks(Project* project);
Task* readTask(Project* project, string taskId);
Task* createTask(Project* project, Task* task);
void updateTask(Project* project, Task* task);
void resetTimes(Project* project);

#endif // PROJECT_H
