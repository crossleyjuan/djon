#include "calendar.h"

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
}

string Calendar::name() {
    return _name;
}

bool Calendar::isWorkingDay(const DateTime date) {
    int dayOfWeek = date.dayOfTheWeek();
    return _normalWorkingDays[dayOfWeek - 1];
}

DTime Calendar::startHour(const DateTime date) {
    return _normalStartHour;
}

DTime Calendar::endHour(const DateTime date) {
    return _normalEndHour;
}
