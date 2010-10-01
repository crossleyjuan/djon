#include "duration.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Duration::Duration() {
    _days = 0;
    _hours = 0;
    _minutes = 0;
}

Duration::Duration(int days, int hours, int minutes) {
    _days = days;
    _hours = hours;
    _minutes = minutes;
}

Duration::Duration(const std::string duration) {
    _days = 0;
    _hours = 0;
    _minutes = 0;
    // ##
    // ##d##H##m
    if (duration.find('x') != std::string::npos) {
        char* cdur = const_cast<char*>(duration.c_str());
        char* temp = strtok(cdur, "x");
        _days = atoi(temp);
        temp = strtok(NULL, "x");
        _hours = atoi(temp);
        temp = strtok(NULL, "x");
        _minutes = atoi(temp);
    } else {
        if (duration.length() > 0) {
            _days = atoi(duration.c_str());
        }
    }
}

Duration::Duration(const Duration& orig) {
    _days = orig._days;
    _hours = orig._hours;
    _minutes = orig._minutes;
}

Duration::~Duration() {
}


int Duration::days() {
    return _days;
}

void Duration::setDays(int days) {
    _days = days;
}

int Duration::hours() {
    return _hours;
}

void Duration::setHours(int hours) {
    _hours = hours;
}

int Duration::minutes() {
    return _minutes;
}

void Duration::setMinutes(int minutes) {
    _minutes = minutes;
}

char* Duration::toChar() {
    char* buffer = (char*)mmalloc(9);
    memset(buffer, 0, 6);
    sprintf(buffer, "%02dx%02dx%02d", _days, _hours, _minutes);

    return buffer;
}

