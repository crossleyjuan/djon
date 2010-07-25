#include "dtime.h"
#include <stdio.h>
#include <stdlib.h>

DTime::DTime() {
    _secs = 0;
}

DTime::DTime(long secs) {
    _secs = secs;
}

DTime::DTime(DTime& time) {
    this->_secs = time._secs;
}

DTime::~DTime() {
    _secs = 0;
}

long DTime::secs() {
    return _secs;
}

/*
QTime* DTime::toQTime() {
    int secs = _secs;

    int hour = (secs / 3600);
    secs -= (hour * 3600);
    int min = (secs / 60);
    secs -= (min * 60);
    QTime* res = new QTime(hour, min, secs);
    return res;
}
*/

void DTime::time(int& hour, int& min, int& sec) const {
    int tmpSecs = _secs;
    hour = tmpSecs / 3600;
    tmpSecs -= hour * 3600;
    min = tmpSecs / 60;
    tmpSecs -= min * 60;
    sec = tmpSecs;
}

const char* DTime::toChar() {

    char* buffer = (char*)malloc(20);
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

void DTime::add(DTime& time) {
    _secs += time.secs();
}
