#ifndef PROJECTREADER_H
#define PROJECTREADER_H

#include "inputstream.h"

class Project;
class Task;
class TaskLog;

class ProjectReader
{
public:
    ProjectReader(InputStream* input);

    Project* readProject();

private:
    Task* readTask(Project* project);
    TaskLog* readTaskLog(Task* task);

    InputStream* _inputStream;
};

#endif // PROJECTREADER_H
