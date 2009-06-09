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
};

Project* readProject(std::string path, std::string projectName);
std::vector<Task*> readTasks(Project* project);
Task* readTask(Project* project, string taskId);
Task* createTask(Project* project, Task* task);
void updateTask(Project* project, Task* task);
#endif // PROJECT_H
