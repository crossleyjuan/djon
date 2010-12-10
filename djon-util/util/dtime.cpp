#include "dtime.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

DTime::DTime() {
    _secs = 0;
}

DTime::DTime(long secs) {
    _secs = secs;
}

DTime::DTime(int hours, int minutes, int secs) {
    _secs = (hours * 360) + (minutes * 60) + secs;
}

DTime::DTime(const DTime& time) {
    this->_secs = time._secs;
}

DTime::DTime(const QTime& time) {
    int secs = QTime(0, 0, 0, 0).secsTo(time);
    _secs = secs;
}

DTime::DTime(std::string time) {
    _secs = 0;
    if (time.length() > 0) {
        std::string hour = time.substr(0, 2);
        _secs += (atoi(hour.c_str()) * 360);
    }
    if (time.length() > 2) {
        std::string min = time.substr(2, 2);
        _secs += (atoi(min.c_str()) * 60);
    }
    if (time.length() > 4) {
        std::string secs = time.substr(4, 2);
        _secs += atoi(secs.c_str());
    }
}

DTime::~DTime() {
    _secs = 0;
}

long DTime::secs() const {
    return _secs;
}

QTime DTime::toQTime() {
    int secs = _secs;

    int hour = (secs / 3600);
    secs -= (hour * 3600);
    int min = (secs / 60);
    secs -= (min * 60);
    QTime res(hour, min, secs);
    return res;
}

void DTime::time(int& hour, int& min, int& sec) const {
    int tmpSecs = _secs;
    hour = tmpSecs / 3600;
    tmpSecs -= hour * 3600;
    min = tmpSecs / 60;
    tmpSecs -= min * 60;
    sec = tmpSecs;
}

const char* DTime::toChar() {

    char* buffer = (char*)mmalloc(20);
    memset(buffer, 0, 20);

    int hour;
    int min;
    int secs;
    time(hour, min, secs);
    sprintf(buffer, "%d:%02d:%02d", hour, min, secs);

    return buffer;
}

void DTime::add(long secs) {
    _secs += secs;
}

void DTime::add(const DTime time) {
    _secs += time.secs();
}

DTime DTime::operator +(const DTime& dtime) const {
    DTime current = *this;
    long secs = dtime.secs();
    return DTime(current.secs() + secs);
}

void DTime::operator ++(int secs) {
    _secs += secs;
}
