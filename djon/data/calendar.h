#ifndef CALENDAR_H
#define CALENDAR_H

#include "util.h"
#include <string>
#include <vector>

class Calendar
{
public:
    Calendar();
    ~Calendar();
    Calendar(const Calendar& orig);

    string name();
    bool isWorkingDay(const DateTime date);
    DTime startHour(const DateTime date);
    DTime endHour(const DateTime date);

private:
    std::string _name;

    // The following parameters will define the "common" behaviour
    bool _normalWorkingDays[7]; // Defines the days of the week that consider as normal work day (true for each day)
    DTime _normalStartHour; // Start hour in normal days
    DTime _normalEndHour; // End hour in normal days

    std::vector<DateTime> _holidays;
};

#endif // CALENDAR_H
