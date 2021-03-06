// *********************************************************************************************************************
// file:    DateTime.cpp
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

#include "DateTime.h"
#include "util.h"
#include "calendar.h"
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <stdio.h>


DateTime::DateTime(string dateTime) {
    loadDate(dateTime);
}

DateTime::DateTime(const QDateTime& dateTime) {
    initialize(dateTime);
}

DateTime::DateTime(const QDate& date) {
    initialize(date);
}

DateTime::DateTime(double value) {
    _year = value / 10000;
    value -= (_year * 10000);
    _month = value / 100;
    value -= (_month * 100);
    _day = value;
    value -= _day;
    _hour = value * 100;
    _min = value * 10000;
    _secs = value * 1000000;
}

void DateTime::initialize(const QDateTime& dateTime) {
    _year = dateTime.date().year();
    _month = dateTime.date().month();
    _day = dateTime.date().day();
    _hour = dateTime.time().hour();
    _min = dateTime.time().minute();
    _secs = dateTime.time().second();
}

void DateTime::initialize(const QDate& date) {
    _year = date.year();
    _month = date.month();
    _day = date.day();
    _hour = 0;
    _min = 0;
    _secs = 0;
}

DateTime::DateTime(int year, int month, int day, int hour, int min, int secs) {
    _year = year;
    _month = month;
    _day = day;
    _hour = hour;
    _min = min;
    _secs = secs;
}

void toInt(char* chr, int* dest) {
    // Non empty string
    if (*chr != '\0') {
        *dest = atoi(chr);
    }
}

void DateTime::loadDate(string dateTime) {
    _year = 0;
    _month = 0;
    _day = 0;
    _hour = 0;
    _min = 0;
    _secs = 0;
    char year[5];
    memset(year, 0, 5);
    char month[3];
    memset(month, 0, 3);
    char day[3];
    memset(day, 0, 3);
    char hour[3];
    memset(hour, 0, 3);
    char min[3];
    memset(min, 0, 3);
    char secs[3];
    memset(secs, 0, 3);

    const char* cdate = dateTime.c_str();
    memcpy(year, cdate, 4);
    memcpy(month, cdate + 4, 2);
    memcpy(day, cdate + 6, 2);
    memcpy(hour, cdate + 9, 2);
    memcpy(min, cdate + 11, 2);
    memcpy(secs, cdate + 13, 2);
    toInt(year, &_year);
    toInt(month, &_month);
    toInt(day, &_day);
    toInt(hour, &_hour);
    toInt(min, &_min);
    toInt(secs, &_secs);
}

DateTime::DateTime() {
    initialize(QDateTime::currentDateTime());
}

void DateTime::setSecs(int _secs) {
    this->_secs = _secs;
}

int DateTime::getSecs() const {
    return _secs;
}

void DateTime::setMin(int _min) {
    this->_min = _min;
}

int DateTime::getMin() const {
    return _min;
}

void DateTime::setHour(int _hour) {
    this->_hour = _hour;
}

int DateTime::getHour() const {
    return _hour;
}

void DateTime::setDay(int _day) {
    this->_day = _day;
}

int DateTime::getDay() const {
    return _day;
}

void DateTime::setMonth(int _month) {
    this->_month = _month;
}

int DateTime::getMonth() const {
    return _month;
}

void DateTime::setYear(int _year) {
    this->_year = _year;
}

int DateTime::getYear() const {
    return _year;
}

DateTime::DateTime(const DateTime& orig) {
    _year = orig._year;
    _month = orig._month;
    _day = orig._day;
    _hour = orig._hour;
    _min = orig._min;
    _secs = orig._secs;
}

DateTime::~DateTime() {
}

char* DateTime::toChar() const {
    char* buffer = (char*)mmalloc(16);
    memset(buffer, 0, 16);
    sprintf(buffer, "%04d%02d%02d.%02d%02d%02d", _year, _month, _day, _hour, _min, _secs);

    return buffer;
}

QDateTime DateTime::toQDateTime() const {
    QDate date(_year, _month, _day);
    QTime tim(_hour, _min, _secs);
    QDateTime dateTime(date, tim);
    return dateTime;
}

double DateTime::toDouble() const {
    double date = (_year * 10000) + (_month * 100) + _day + ((double)_hour / 100) + ((double)_min/10000) + ((double)_secs/1000000);

    return date;
}

bool DateTime::operator >(const DateTime& dateTime) const {
    return toDouble() > dateTime.toDouble();
}

bool DateTime::operator <(const DateTime& dateTime) const {
    return toDouble() < dateTime.toDouble();
}

bool DateTime::operator >=(const DateTime& dateTime) const {
    return toDouble() >= dateTime.toDouble();
}

bool DateTime::operator <=(const DateTime& dateTime) const {
    return toDouble() <= dateTime.toDouble();
}

bool DateTime::operator ==(const DateTime& dateTime) const {
    DateTime test = dateTime;
    DateTime thisDate = *this;
    if ((test.getHour() == 0) && (test.getMin() == 0) && (test.getSecs() == 0)) {
        thisDate.setHour(0);
        thisDate.setMin(0);
        thisDate.setSecs(0);
    }
    return test.toDouble() == thisDate.toDouble();
}

long DateTime::operator -(const DateTime& dateTimeRight) const {
    DateTime left = *this; // Copy this
    DateTime right = dateTimeRight; // copy the argument
    int secs = right.toQDateTime().secsTo(left.toQDateTime());

    return secs;
}

DateTime DateTime::startDayOfWeek() {
    QDate today = QDate::currentDate();
    while (today.dayOfWeek() != 1) {
        today = today.addDays(-1);
    }
    DateTime result(today.year(), today.month(), today.day());
    return result;
}

DateTime DateTime::startDayOfNextWeek() {
    QDate today = QDate::currentDate();
    while (today.dayOfWeek() != 7) {
        today = today.addDays(1);
    }
    today.addDays(1);
    DateTime result(today.year(), today.month(), today.day());
    return result;
}

DateTime DateTime::today(bool includeTime) {
    QDateTime today = QDateTime::currentDateTime();
    DateTime result(today.date().year(), today.date().month(), today.date().day());
    if (includeTime) {
        result.setHour(today.time().hour());
        result.setMin(today.time().minute());
        result.setSecs(today.time().second());
    }
    return result;
}

DateTime DateTime::addDays(int days) const {
    QDateTime date = toQDateTime();
    QDateTime newDate = date.addDays(days);
    DateTime dtNew(newDate);

    return dtNew;
}

DateTime DateTime::addDays(int days, const Calendar calendar) const {
    DateTime res = *(this);
    while (days > 0) {
        res = res.addDays(1);
        while (!calendar.isWorkingDay(res)) {
            res = res.addDays(1);
        }
        days--;
    }
    return res;
}

int DateTime::daysTo(const DateTime& dt) const {
    QDateTime thisDate = toQDateTime();
    QDateTime qdt = dt.toQDateTime();

    int days = thisDate.daysTo(qdt);

    return days;
}

int DateTime::dayOfTheWeek() const {
    return toQDateTime().date().dayOfWeek();
}

DTime DateTime::time() const {
    int hour = getHour();
    int minute = getMin();
    int secs = getSecs();
    return DTime(hour, minute, secs);
}
