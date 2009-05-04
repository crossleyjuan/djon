#include "project.h"
#include <map>
#include <cstdlib>
#include "utils.h"

using namespace std;

void readJobs(Task* task) {
    string path = task->project.path;

    vector<string> files;
    int res = getdir(path + "/" + task->name, files, "jdj");
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
            task->name = values->find("task-name")->second;
            task->description = values->find("description")->second;
            task->duration = atoi(values->find("duration")->second.c_str());
            project->tasks.push_back(task);
        }
    }
}

hashmap* Task::hashValues() {
    hashmap* values = new hashmap();

    values->insert(pair<string, string>("task-name", this->name));
    values->insert(pair<string, string>("description", this->description));
    values->insert(pair<string, string>("duration", toString(this->duration)));
    values->insert(pair<string, string>("enddate", toString(this->endDate)));
    values->insert(pair<string, string>("startdate", toString(this->startDate)));

    return values;
}

Project* readProject(string path, string projectName) {
    Project* project = new Project();
    string fileName (path);
    fileName += projectName + string(".djon");
    hashmap* mapValues = readFile(fileName);

    hashmap::iterator it = mapValues->find("project-name");
    project->name = (*it).second;
    project->path = path;

    readTasks(project);
    return project;
}
