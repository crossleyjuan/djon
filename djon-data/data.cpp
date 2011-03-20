#include "data.h"
#include "util.h"
#include "config.h"
#include "settings.h"
#include "data/projectwriter.h"
#include "data/outputstream.h"
#include "data/inputstream.h"
#include "data/projectreader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>


using namespace std;


std::vector<Project*>* loadProjects() {
    qDebug("loadProjects()");

    std::vector<Project*>* projects = new std::vector<Project*>();

    std::vector<string> newOpenProjects;
    vector<string> prjs = getSettings()->openProjects();

    for (vector<string>::iterator it = prjs.begin(); it != prjs.end(); it++) {
        string fileName = (*it);

        FILE* pFile;

        pFile = fopen(fileName.c_str(), "rb");
        if (pFile == NULL) {
            //backward compatibility with 1.1 (the projects were saved in the projects folder
            std::string home = *getHomeDir() + "/.djon/Projects/";
            fileName = home + fileName;
            pFile = fopen(fileName.c_str(), "rb");
        }

        if (pFile != NULL) {
            InputStream is(fileName, pFile);

            ProjectReader reader(&is);
            Project* project = reader.readProject();

            project->setProjectFileName(new string(fileName));
            newOpenProjects.push_back(fileName);

            projects->push_back(project);

            if (errorOcurred()) {
                return NULL;
            }
            fclose(pFile);
        }

    }
    getSettings()->setOpenProjects(newOpenProjects);
    getSettings()->save();
    qDebug("out loadProjects()");
    return projects;
}

//std::vector<Project*>* loadProjects2() {
//    qDebug("loadProjects()");
//    const char* lastDir = getLastDir();

//    std::vector<Project*>* projects = new std::vector<Project*>();

//    vector<string> prjs = getSettings()->openProjects();

//    for (vector<string>::iterator it = prjs.begin(); it != prjs.end(); it++) {
//        string fileName = (*it);
//        stringstream ss;
//        ss << lastDir;
//        ss << "/";
//        ss << fileName;
//        ss << ".djon";
//        string fullPath = ss.str();

//        char* projDef = readFile((char*)fileName.c_str());
//        Project* project = new Project(string(projDef));

//        project->setProjectFileName(new string(fileName));
//        projects->push_back(project);
//        loadTasks(project);
//        if (errorOcurred()) {
//            return NULL;
//        }
//        loadTaskLogs(project);
//        if (errorOcurred()) {
//            return NULL;
//        }
//        free(projDef);
//    }
//    qDebug("out loadProjects()");
//    return projects;
//}

vector<Template*>* m_templates;

vector<Template*>* readTemplates() {
    if (m_templates != NULL) {
        return m_templates;
    }
    m_templates = new vector<Template*>();
    string* home = getHomeDir();
    string path = *home + "/.djon/templates/";

    vector<char*> files;
    int res = getdir(const_cast<char*>(path.c_str()), files, "tpl");

    if (res == 0) {
        for (vector<char*>::iterator it = files.begin(); it != files.end(); it++) {
            string fileName = path + std::string(*it);

            char* tempDef = readFile(const_cast<char*>(fileName.c_str()));
            hashmap* conf = parseTextFormat(string(tempDef));

            string* templateName = new std::string(READ_ELEMENT(conf, "template-name"));
            string* templateDescription = new std::string(READ_ELEMENT(conf, "template-description"));
            string statusList = READ_ELEMENT(conf, "status");
            string subTaskList = READ_ELEMENT(conf, "subtasks");

            vector<string> vecStatus = split(statusList, ",");
            vector<string> vecSubTaskList = split(subTaskList, ",");

            Template* tpl = new Template(templateName, templateDescription, vecStatus, vecSubTaskList);
            if ((*templateName)[templateName->length() - 1] == '*') {
                m_templates->insert(m_templates->begin(), tpl);
            } else {
                m_templates->push_back(tpl);
            }
            free(tempDef);
        }
    }

    delete(home);
    return m_templates;
}

Template* readTemplate(const string& name) {
    vector<Template*>* templates = readTemplates();
    for (vector<Template*>::iterator it = templates->begin(); it != templates->end(); it++) {
        Template* t = *it;
        if (t->name()->compare(name) == 0) {
            return t;
        }
    }
    return NULL;
}

Template* defaultTemplate() {
    if (readTemplates()->size() > 0) {
        return *readTemplates()->begin();
    } else {
        return NULL;
    }
}

TaskLog* createTaskLog(Task* task) {
    qDebug("TaskLog* createTaskLog(Task* task)");
    TaskLog* log = new TaskLog();
    log->id = uuid();
    task->addLog(log);
    return log;
}

void addProject(const char* fileName) {
    std::vector<string> currentProjects = getSettings()->openProjects();
    bool found = false;
    for (std::vector<string>::iterator iter = currentProjects.begin(); iter != currentProjects.end(); iter++) {
        string item = *iter;
        if (item.compare(fileName) == 0) {
            found = true;
            break;
        }
    }
    if (!found) {
        Settings* settings = getSettings();
        vector<string> projects = settings->openProjects();
        projects.push_back(fileName);
        settings->setOpenProjects(projects);
        settings->save();
    }
}

void removeProject(const char* fileName) {
    vector<string> openProjects = getSettings()->openProjects();
    vector<string> newProjects;
    for (vector<string>::iterator iter = openProjects.begin(); iter != openProjects.end(); iter++) {
        string project = *iter;
        if (project.compare(fileName) != 0) {
            newProjects.push_back(project);
        }
    }
    getSettings()->setOpenProjects(newProjects);
    getSettings()->save();
}

Project* searchProject(vector<Project*> projects, string name) {
    vector<Project*>::iterator iterProj = projects.begin();
    for (; iterProj != projects.end(); iterProj++) {
        Project* prj = *iterProj;
        if (prj->name()->compare(name) == 0) {
            return prj;
            break;
        }
    }
    return NULL;
}

int saveProject(Project *project) {

    stringstream fileName;
    const char* lastDir = getSettings()->lastProjectDir().c_str();
    fileName << lastDir << "/" << *project->name() << ".djon";

    FILE* pFile;
    pFile = fopen(fileName.str().c_str(), "wb");

    string sversion = getCurrentVersion();
    const char* version = sversion.c_str();
    fwrite(version, 1, 13, pFile);

    OutputStream os(pFile);
    ProjectWriter writer(&os);
    writer.writeProject(*project);
    project->setProjectFileName(new string(fileName.str()));

    fclose(pFile);

    return 0;
}
