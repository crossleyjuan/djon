#ifndef PROJECTWRITER_H
#define PROJECTWRITER_H

#include "util.h"

class Project;
class Task;
class TaskLog;

class ProjectWriter
{
public:
    ProjectWriter(OutputStream* output);

    void writeProject(const Project& project);

private:
    void writeTask(const Task& task);
    void writeTaskLog(const TaskLog& taskLog);

    OutputStream* _outStream;
};

#endif // PROJECTWRITER_H
