#include "project.h"
#include <map>
#include <cstdlib>
#include "utils.h"

using namespace std;

void readJobs(Task* task) {
    string path = task->project.path;

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

void readTasks(Project* project) {
    string path = project->path;

    vector<string> files;
    int res = getdir(path, files, "tsk");
    if (res == 0) {
        for (vector<string>::iterator it = files.begin(); it != files.end(); it++) {
            string fileName = path + (*it);

            hashmap* values = readFile(fileName);
            Task* task = new Task();
            task->id = values->find("task-id")->second;
            task->shortDescription = values->find("short-description")->second;
            task->longDescription = values->find("long-description")->second;
            task->duration = atoi(values->find("duration")->second.c_str());
            task->startDate = atoi(values->find("startdate")->second.c_str());
            task->endDate = atoi(values->find("enddate")->second.c_str());
            project->tasks.push_back(task);
        }
    }
}

hashmap* Task::hashValues() {
    hashmap* values = new hashmap();

    values->insert(pair<string, string>("task-id", this->id));
    values->insert(pair<string, string>("short-description", this->shortDescription));
    values->insert(pair<string, string>("long-description", this->longDescription));
    values->insert(pair<string, string>("duration", toString(this->duration)));
    values->insert(pair<string, string>("enddate", toString((int)this->endDate)));
    values->insert(pair<string, string>("startdate", toString((int)this->startDate)));

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

    readTasks(project);
    return project;
}
