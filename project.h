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
    std::vector<Task*> tasks;
};

struct Job {
    std::string name;
    std::string description;
    int estimated;
    int status;
    int usedtime;
};

struct Task {
    std::string name;
    std::string description;
    int duration;
    double startDate;
    double endDate;
    Project project;
    std::vector<Job*> jobs;
    hashmap* hashValues();
};

Project* readProject(std::string path, std::string projectName);

#endif // PROJECT_H
