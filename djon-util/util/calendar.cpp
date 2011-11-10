// *********************************************************************************************************************
// file:    calendar.cpp
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

#include "calendar.h"

#include <map>
#include <vector>
#include <string>

map<std::string, Calendar*> __calendars;
bool __calendarsLoaded;
Calendar* __defaultCalendar;

void loadCalendars() {
    std::string* homeDir = getHomeDir();
    std::string djonDir = *homeDir + "/.djon";
    std::string calDir = djonDir + "/calendars";

    std::vector<char*> files;
    //int res = getdir(const_cast<char*>(path.c_str()), files, "tpl");
    if (getdir(const_cast<char*>(calDir.c_str()), files, "cal") == 0) {
        for (std::vector<char*>::iterator filesIter = files.begin(); filesIter != files.end(); filesIter++) {
            std::string calFile = calDir + "/" + std::string(*filesIter);
            qDebug("Loading calendar: %s", calFile.c_str());
            char* calDef = readFile(const_cast<char*>(calFile.c_str()));
            Calendar* cal = new Calendar(calDef);
            //qDebug("Calendar %s loaded", cal->name());
            __calendars.insert(pair<std::string, Calendar*>(cal->name(), cal));
        }
    }
    qDebug("Calendar size: %d", __calendars.size());
    __calendarsLoaded = true;

    delete(homeDir);
}

map<std::string, Calendar*> calendars() {
    if (!__calendarsLoaded) {
        loadCalendars();
    }
    return __calendars;
}

Calendar* calendar(std::string name) {
    if (!__calendarsLoaded) {
        loadCalendars();
    }
    map<std::string, Calendar*>::iterator iter = __calendars.find(name);
    if (iter != __calendars.end()) {
        return iter->second;
    } else {
        return defaultCalendar();
    }
}

Calendar* defaultCalendar() {
    if (__defaultCalendar == NULL) {
        __defaultCalendar = new Calendar();
    }
    return __defaultCalendar;
}

void releaseCalendars() {
    if (__calendarsLoaded) {
        for (map<std::string, Calendar*>::iterator iter = __calendars.begin(); iter != __calendars.end(); iter++) {
            Calendar* cal = iter->second;
            delete(cal);
            cal = 0;
        }
    }
}

Calendar::Calendar(char* calDef) {
    std::string definition(calDef);
    // Process name
    int pos = definition.find(' ');
    _name = definition.substr(0, pos);
    pos++;
    // Process working days
    std::string workDays = definition.substr(pos, 7);
    for (int x = 0; x < 7; x++) {
        _normalWorkingDays[x] = (workDays.at(x) == '1');
    }
    pos += 7;
    std::string startHour = definition.substr(pos, 4);
    _normalStartHour = DTime(startHour);
    pos += 4;
    std::string endHour = definition.substr(pos, 4);
    _normalEndHour = DTime(endHour);
    pos += 4;
    while (pos < definition.length()) {
        std::string holDay = definition.substr(pos, 8);
        DateTime date(holDay);
        _holidays.push_back(date);
        pos += 8;
    }
}

Calendar::Calendar()
{
    for (int x = 0; x < 7; x++) {
        _normalWorkingDays[x] = false;
    }
    for (int x = 0; x < 5; x++) {
        _normalWorkingDays[x] = true;
    }
    _normalStartHour = DTime(8, 0, 0);
    _normalEndHour = DTime(17, 0, 0);
}

Calendar::~Calendar() {
}

Calendar::Calendar(const Calendar& orig) {
    this->_name = orig._name;
    this->_holidays = orig._holidays;
    this->_normalEndHour = orig._normalEndHour;
    this->_normalStartHour = orig._normalStartHour;
    for (int x = 0; x < 7; x++) {
        this->_normalWorkingDays[x] = orig._normalWorkingDays[x];
    }
}

string Calendar::name() const {
    return _name;
}

bool Calendar::isWorkingDay(const DateTime date) const {
    int dayOfWeek = date.dayOfTheWeek();
    return _normalWorkingDays[dayOfWeek - 1];
}

DTime Calendar::startHour(const DateTime date) const {
    return _normalStartHour;
}

DTime Calendar::endHour(const DateTime date) const {
    return _normalEndHour;
}

DTime Calendar::startHour() const {
    return _normalStartHour;
}

DTime Calendar::endHour() const {
    return _normalEndHour;
}
