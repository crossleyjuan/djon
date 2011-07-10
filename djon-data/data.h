/* 
 * File:   data.h
 * Author: cross
 *
 * Created on July 7, 2010, 2:53 PM
 */

#ifndef _DATA_H
#define	_DATA_H

#include <vector>
#include "data/project.h"
#include "data/task.h"
#include "data/workspace.h"
#include "template.h"
#include "data/djonpreferences.h"
#include "data/settings.h"

std::vector<Project*>* loadProjects();
Project* loadProject(std::string fileName);
Workspace* loadWorkspace(std::string fileName);
void saveWorkspace(Workspace* workspace);
Project* searchProject(vector<Project*> projects, string name);

vector<Template*>* readTemplates();
Template* readTemplate(const string& templateName);
Template* defaultTemplate();

int saveProject(Project* project);

//int createProject(Project* project);
void addProject(const char* fileName);
void removeProject(const char* fileName);
//int updateProject(Project* project);
//int createTask(Task* task);
//int updateTask(Task* task);
//int deleteTask(Task* task);
TaskLog* createTaskLog(Task* task);
//int createTaskLog(Task* task, TaskLog* taskLog);
//int updateTaskLog(Task* task, TaskLog* taskLog);
//int deleteTaskLog(Task* task, TaskLog* taskLog);

#endif	/* _DATA_H */

