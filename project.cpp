#include "project.h"
#include <map>
#include <cstdlib>
#include "utils.h"

using namespace std;

void readJobs(Task* task) {
    string path = task->project->path;

    vector<string> files;
    int res = getdir(path + "/" + task->id, files, "jdj");
    if (res == 0) {
        for (vector<string>::iterator it = files.begin(); it != files.end(); it++) {
            string fileName = path + (*it);

            hashmap* values = readFile(fileName);
            Job* job = new Job();
            job->name = values->find("job-name")->second;
            job->description = values->find("job-description")->second;
            job->estimated = atoi(values->find("job-estimated-time")->second.c_str());
            job->status = atoi(values->find("job-status")->second.c_str());
            job->usedtime = atoi(values->find("job-current-time")->second.c_str());
            task->jobs.push_back(job);
        }
    }
}

Task* readTask(Project* project, string taskId) {
    string path = project->path;
    string fileName = path + taskId + ".tsk";

    hashmap* values = readFile(fileName);

    Task* task = new Task();
    task->id = READ_ELEMENT(values, "task-id");
    task->shortDescription =  READ_ELEMENT(values, "short-description");
    task->longDescription = READ_ELEMENT(values, "long-description");
    task->duration = atoi(READ_ELEMENT(values, "duration").c_str());
    task->startDate = atoi(READ_ELEMENT(values, "startdate").c_str());
    task->endDate = atoi(READ_ELEMENT(values, "enddate").c_str());
    task->templateName = READ_ELEMENT(values, "template-name");
    task->status = READ_ELEMENT(values, "status");
    task->totalTime = atoi(READ_ELEMENT(values, "total-time").c_str());

    return task;
}

std::vector<Task*> readTasks(Project* project) {
    string path = project->path;

    vector<string> files;
    std::vector<Task*> tasks;
    int res = getdir(path, files, "tsk");
    if (res == 0) {
        for (vector<string>::iterator it = files.begin(); it != files.end(); it++) {
            string fileName = (*it);

            Task* task = readTask(project, fileName.substr(0, fileName.length() - 4));

            tasks.push_back(task);
        }
    }
    return tasks;
}

hashmap* Task::hashValues() {
    hashmap* values = new hashmap();

    values->insert(pair<string, string>("task-id", this->id));
    values->insert(pair<string, string>("short-description", this->shortDescription));
    values->insert(pair<string, string>("long-description", this->longDescription));
    values->insert(pair<string, string>("duration", toString(this->duration)));
    values->insert(pair<string, string>("enddate", toString((int)this->endDate)));
    values->insert(pair<string, string>("startdate", toString((int)this->startDate)));
    values->insert(pair<string, string>("template-name", this->templateName));
    values->insert(pair<string, string>("status", this->status));
    values->insert(pair<string, string>("total-time", toString((int)this->totalTime)));

    return values;
}

Project* readProject(string path, string projectName) {
    logInfo("Reading project");
    Project* project = new Project();
    if (path[path.length()-1] != '/') {
        path.append("/");
    }
    string fileName (path);
    fileName += projectName + string(".djon");
    hashmap* mapValues = readFile(fileName);

    hashmap::iterator it = mapValues->find("project-name");
    project->name = (*it).second;
    project->description = mapValues->find("project-description")->second;
    project->path = path;

    return project;
}

Task* createTask(Project* project, Task* task) {
    task->id = toString((int)readTasks(task->project).size() + 1);
    writeFile(project->path + "/" + task->id + ".tsk", task->hashValues());
    return task;
}

void updateTask(Project* project, Task* task) {
    writeFile(project->path + "/" + task->id + ".tsk", task->hashValues());
}
