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
#include "template.h"

class Template;

std::vector<Project*>* loadProjects();
vector<Template*>* readTemplates();
Template* readTemplate(string templateName);
int createProject(Project* project);
int createTask(Task* task);
int updateTask(Task* task);
int createTaskLog(Task* task, TaskLog* taskLog);
int updateTaskLog(Task* task, TaskLog* taskLog);


#endif	/* _DATA_H */

