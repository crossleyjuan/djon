/* 
 * File:   DateTime.h
 * Author: cross
 *
 * Created on July 7, 2010, 1:34 PM
 */

#ifndef _DATETIME_H
#define	_DATETIME_H

#include "dtime.h"

#include <string>
#include <QDateTime>


using namespace std;

class Calendar;

class DateTime {
public:
    DateTime();
    DateTime(int year, int month, int day, int hour = 0, int min = 0, int secs = 0);
    DateTime(const QDateTime& dateTime);
    DateTime(const QDate& date);
    DateTime(string dateTime);
    DateTime(double value);
    DateTime(const DateTime& orig);
    virtual ~DateTime();

    static DateTime startDayOfWeek();
    static DateTime startDayOfNextWeek();
    static DateTime today(bool includeTime = false);
    DateTime addDays(int days) const;
    DateTime addDays(int days, const Calendar calendar) const;
    int daysTo(const DateTime& dt) const;
    int dayOfTheWeek() const;

    char* toChar() const;
    QDateTime toQDateTime() const;
    double toDouble() const;
    void setSecs(int _secs);
    int getSecs() const;
    void setMin(int _min);
    int getMin() const;
    void setHour(int _hour);
    int getHour() const;
    void setDay(int _day);
    int getDay() const;
    void setMonth(int _month);
    int getMonth() const;
    void setYear(int _year);
    int getYear() const;
    DTime time() const;

    bool operator >(const DateTime& dateTime) const;
    bool operator <(const DateTime& dateTime) const;
    bool operator >=(const DateTime& dateTime) const;
    bool operator <=(const DateTime& dateTime) const;
    long operator -(const DateTime& dateTimeRight) const;
    bool operator ==(const DateTime& dateTime) const;

private:
    void initialize(const QDateTime& dateTime);
    void initialize(const QDate& date);

    int _year;
    int _month;
    int _day;
    int _hour;
    int _min;
    int _secs;

    void loadDate(string dateTime);
};

#endif	/* _DATETIME_H */

