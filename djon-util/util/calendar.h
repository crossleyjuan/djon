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
    Calendar(char* calDef);

    string name() const;
    bool isWorkingDay(const DateTime date) const;
    DTime startHour(const DateTime date) const;
    DTime startHour() const;
    DTime endHour(const DateTime date) const;
    DTime endHour() const;

private:
    std::string _name;

    // The following parameters will define the "common" behaviour
    bool _normalWorkingDays[7]; // Defines the days of the week that consider as normal work day (true for each day)
    DTime _normalStartHour; // Start hour in normal days
    DTime _normalEndHour; // End hour in normal days

    std::vector<DateTime> _holidays;
};


// Public functions
Calendar* calendar(std::string name);
Calendar* defaultCalendar();
void releaseCalendars();

#endif // CALENDAR_H
