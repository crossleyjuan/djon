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
            char* calDef = readFile(const_cast<char*>(calFile.c_str()));
            Calendar* cal = new Calendar(calDef);
            __calendars.insert(pair<std::string, Calendar*>(cal->name(), cal));
        }
    }
    __calendarsLoaded = true;

    delete(homeDir);
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
