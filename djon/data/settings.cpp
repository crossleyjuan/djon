#include "settings.h"
#include "util.h"
#include <stdlib.h>
#include <sstream>

// Global variable with loaded settings
Settings* __settings;

Settings* getSettings() {
    if (__settings == NULL) {
        __settings = new Settings();
        __settings->load();
    }
    return __settings;
}

Settings::Settings()
{
}

bool Settings::closeToTray() {
    return _closeToTray;
}

void Settings::setCloseToTray(bool closeToTray) {
    _closeToTray = closeToTray;
    _closeToTrayChanged = true;
}

bool Settings::showSysTrayWarning() {
    return _showSysTrayWarning;
}

void Settings::setShowSysTrayWarning(bool showSysTrayWarning) {
    _showSysTrayWarning = showSysTrayWarning;
    _showSysTrayWarningChanged = true;
}

std::string Settings::lastProjectDir() {
    return _lastProjectDir;
}

void Settings::setLastProjectDir(std::string lastProjectDir) {
    _lastProjectDir = lastProjectDir;
    _lastProjectDirChanged = true;
}

std::string Settings::lastReleaseNotes() {
    return _lastReleaseNotes;
}

void Settings::setLastReleaseNotes(std::string lastReleaseNotes) {
    _lastReleaseNotes = lastReleaseNotes;
    _lastReleaseNotesChanged = true;
}

std::vector<std::string> Settings::openProjects() {
    return _openProjects;
}

void Settings::setOpenProjects(std::vector<std::string> openProjects) {
    _openProjects = openProjects;
    _openProjectsChanged = true;
}

long Settings::idleTimeOut() {
    return _idleTimeOut;
}

void Settings::setIdleTimeOut(long idleTimeOut) {
    _idleTimeOut = idleTimeOut;
    _idleTimeOutChanged = true;
}

void Settings::load() {
    const char* close = readConfValue("close-to-systray", "1");
    setCloseToTray(strcmp(close, "1") == 0);
    const char* showWarning = readConfValue("show-systray-warning", "1");
    setShowSysTrayWarning(strcmp(showWarning, "1") == 0);
    const char* lastProjectDir = readConfValue("last-project-dir");
    setLastProjectDir(strcpy(lastProjectDir));
    setLastReleaseNotes(std::string(readConfValue("last-release-notes", "")));
    long idleTimeOut = atoi(readConfValue("idle-timeout"));
    setIdleTimeOut(idleTimeOut);
    const char* cProjects = readConfValue("open-projects", "");
    std::vector<std::string*>* projects = split(std::string(cProjects), ",");
    std::vector<std::string> openProjects;
    for (std::vector<std::string*>::iterator iter = projects->begin(); iter != projects->end(); iter++) {
        string* prj = *iter;
        openProjects.push_back(*prj);
    }
    setOpenProjects(openProjects);
    _closeToTrayChanged = false;
    _showSysTrayWarningChanged = false;
    _lastProjectDirChanged = false;
    _lastReleaseNotesChanged = false;
    _openProjectsChanged = false;
    _idleTimeOutChanged = false;
}

void Settings::save() {
    if (_closeToTrayChanged) {
        writeConfValue("close-to-systray", _closeToTray ? "1": "0");
    }
    if (_showSysTrayWarningChanged) {
        writeConfValue("show-systray-warning", _showSysTrayWarning ? "1": "0");
    }
    if (_lastProjectDirChanged) {
        writeConfValue("last-project-dir", _lastProjectDir);
    }
    if (_lastReleaseNotesChanged) {
        writeConfValue("last-release-notes", _lastReleaseNotes);
    }
    if (_openProjectsChanged) {
        std::string projects;
        for (std::vector<std::string>::iterator iter = _openProjects.begin(); iter != _openProjects.end(); iter++) {
            std::string prj = *iter;
            if (projects.length() > 0) {
                projects = projects.append(",");
            }
            projects.append(prj);
        }
        writeConfValue("open-projects", projects);
    }
    if (_idleTimeOutChanged) {
        std::stringstream ssIdle;
        ssIdle << _idleTimeOut;
        writeConfValue("idle-timeout", ssIdle.str());
    }

    _closeToTrayChanged = false;
    _showSysTrayWarningChanged = false;
    _lastProjectDirChanged = false;
    _lastReleaseNotesChanged = false;
    _openProjectsChanged = false;
    _idleTimeOutChanged = false;
}
