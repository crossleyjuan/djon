#include "dtime.h"

#include <sstream>

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

QTime* DTime::toQTime() {
    int secs = _secs;

    int hour = (secs / 3600);
    secs -= (hour * 3600);
    int min = (secs / 60);
    secs -= (min * 60);
    QTime* res = new QTime(hour, min, secs);
    return res;
}

const char* DTime::toChar() {
    QTime* time = toQTime();

    std::stringstream ss;
    ss << time->hour() << ":" << time->minute() << ":" << time->second();

    delete (time);
    return ss.str().c_str();
}

void DTime::add(long secs) {
    _secs += secs;
}

void DTime::add(DTime& time) {
    _secs += time.secs();
}
