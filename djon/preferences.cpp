#include "preferences.h"

Preferences::Preferences()
{
}

std::string Preferences::lastWindowState() {
    return _lastWindowState;
}

void Preferences::setLastWindowState(std::string lastWindowState) {
    _lastWindowState = lastWindowState;
}

std::string Preferences::lastTrackWindowState() {
    return _lastTrackWindowState;
}

void Preferences::setLastTrackWindowState(std::string lastTrackWindowState) {
    _lastTrackWindowState = lastTrackWindowState;
}

std::string Preferences::collapsed() {
    return _collapsed;
}

void Preferences::setCollapsed(std::string collapsed) {
    _collapsed = collapsed;
}

std::string Preferences::lastTrackedTask() {
    return _lastTrackedTask;
}

void Preferences::setLastTrackedTask(std::string lastTrackedTask) {
    _lastTrackedTask = lastTrackedTask;
}

std::string Preferences::logSort() {
    return _logSort;
}

void Preferences::setLogSort(std::string logSort) {
    _logSort = logSort;
}
