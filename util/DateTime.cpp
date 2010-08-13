    /*
 * File:   DateTime.cpp
 * Author: cross
 * 
 * Created on July 7, 2010, 1:34 PM
 */

#include "DateTime.h"
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

void DateTime::initialize(const QDateTime& dateTime) {
    _year = dateTime.date().year();
    _month = dateTime.date().month();
    _day = dateTime.date().day();
    _hour = dateTime.time().hour();
    _min = dateTime.time().minute();
    _secs = dateTime.time().second();
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

char* DateTime::toChar() {
    char* buffer = (char*)malloc(16);
    memset(buffer, 0, 16);
    sprintf(buffer, "%04d%02d%02d.%02d%02d%02d", _year, _month, _day, _hour, _min, _secs);

    return buffer;
}

QDateTime* DateTime::toQDateTime() {
    QDate date(_year, _month, _day);
    QTime tim(_hour, _min, _secs);
    QDateTime* dateTime = new QDateTime(date, tim);
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

long DateTime::operator -(const DateTime& dateTimeRight) const {
    DateTime left = *this; // Copy this
    DateTime right = dateTimeRight; // copy the argument
    int secs = right.toQDateTime()->secsTo(*left.toQDateTime());

    return secs;
}

