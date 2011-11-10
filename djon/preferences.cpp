// *********************************************************************************************************************
// file:    preferences.cpp
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
