// *********************************************************************************************************************
// file:    djonpreferences.cpp
// author:  Juan Pablo Crossley (crossleyjuan@gmail.com)
// created: Oct/1/2010
// updated: Nov/7/2011
// license: GPL 2
//
// This file is part of the djon project, all the source code is licensed under the terms of the GPL license,
// the application and libraries are provided as-is and free of use under the terms explained in the file COPYING.GPL
// the authors are not responsible for any problem that this program, or any of its libraries, may cause.
// Its authors create this application in order to make the world a better place to live, but you should use it on
// your own risks.
// 
// Also, be adviced that, the GPL license force the committers to ensure this application will be free of use, thus
// if you do any modification you will be required to provide it for free unless you use it for personal use (you may 
// charge yourself if you want), bare in mind that you will be required to provide a copy of the license terms that ensures
// this program will be open sourced and all its derivated work will be too.
// *********************************************************************************************************************

#include "djonpreferences.h"
#include "util.h"
#include <sstream>
#include <string>
#include <stdlib.h>

void addCollapsedElement(Project* project, Task* task) {
    string* projName = project->name();
    string* taskId = NULL;
    if (task != NULL) {
        taskId = task->id();
    }

    std::stringstream ss;
    ss << *projName << "++";
    if (task != NULL) {
        ss << *task->id()  << "--";
    } else {
        ss << "--";
    }

    string collapsedElement = ss.str();

    char* pref = readPreference(string("collapsed"), "");
    std::string currentPref = string(pref);

    size_t pos = currentPref.find(collapsedElement);
    if (pos == currentPref.npos) {
        currentPref.append(collapsedElement);
        writePreference(string("collapsed"), currentPref);
    }
    free(pref);
}

void removeCollapsedElement(Project* project, Task* task) {
    string* projName = project->name();
    string* taskId = NULL;
    if (task != NULL) {
        taskId = task->id();
    }

    std::stringstream ss;
    ss << *projName << "++";
    if (task != NULL) {
        ss << *task->id()  << "--";
    } else {
        ss << "--";
    }

    string collapsedElement = ss.str();

    char* pref = readPreference(string("collapsed"), "");
    std::string currentPref = string(pref);

    int pos = currentPref.find(collapsedElement);
    if (pos != currentPref.npos) {
        int end = collapsedElement.length();
        currentPref = currentPref.replace(pos, end, "");
        writePreference("collapsed", currentPref.c_str());
    }
    free(pref);
}

std::vector<Element*>* collapsedElements() {
    std::vector<Element*>* result = new std::vector<Element*>();
    char* pref = readPreference(string("collapsed"), "");
    std::string currentPref = string(pref);

    std::vector<string> elements = split(currentPref, "--");

    for (std::vector<string>::iterator iter = elements.begin(); iter != elements.end(); iter++) {
        string elem = *iter;
        std::vector<string> subelements = split(elem, "++");
        std::string* project = new std::string(subelements.at(0));
        std::string* task = NULL;
        if (subelements.size() > 1) {
            task = new string(subelements.at(1));
        }
        if ((task != NULL) && (task->length() == 0)) {
            task = NULL;
        }
        Element* collapsed = new Element(project, task);
        result->push_back(collapsed);
    }
    free(pref);
    return result;
}

void saveLastTrackedTask(Task* task) {
    stringstream ss;
    ss << *task->project()->name() << "++" << *task->id();

    string taskDef = ss.str();

    writePreference("last-tracked-task", taskDef);
}

 Element* lastTrackedTaskId() {
    char* pref = readPreference(string("last-tracked-task"), "");
    std::string currentPref = string(pref);
    free(pref);

    if (currentPref.length() > 0) {
        std::vector<string> elements = split(currentPref, "++");
        string* project = new string(elements.at(0));
        string* task = new string(elements.at(1));

        Element* element = new Element(project, task);
        return element;
    } else {
        return NULL;
    }
}
