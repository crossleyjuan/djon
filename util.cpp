#include "util.h"

#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "config.h"
#include <stdlib.h>

#ifndef WINDOWS
#include <uuid/uuid.h>
#else
#include <QUuid>
#include <direct.h>
#endif

std::string toString(double a) {
    std::stringstream ss;

    ss << a;
    string res = ss.str();
    return res;
}

std::string toString(double a, int fixedPrecision) {
    std::stringstream ss;

    ss.precision(fixedPrecision);
    ss.setf(ios::fixed,ios::floatfield);

    ss << a;
    string res = ss.str();
    return res;
}

std::string toString(int a) {
    std::stringstream ss;
    ss << a;
    string s = ss.str();
    return s;
}

void logInfo(char* text) {
    cout << text << endl;
}

QDateTime* toDateTime(int date) {
    int year = date / 10000;
    int month = (date / 100) - (year * 100);
    int day = date - (month * 100) - (year * 10000);
    QDateTime* dateTime = new QDateTime(QDate(year, month, day));
    return dateTime;
}

QDateTime* toDateTime(double date) {
    int year = date / 10000;
    int month = (date / 100) - (year * 100);
    int day = date - (month * 100) - (year * 10000);

    int datepart = (int)date;
    double time = date - datepart;
    int hour = time*100;
    time = (time * 100) - hour;
    int minute = time * 100;
    time = (time * 100) - minute;
    int secs = time*100;

    QDateTime* dateTime = new QDateTime(QDate(year, month, day), QTime(hour, minute, secs, 0));

    return dateTime;
}

int toInt(QDateTime dt) {
    int year = dt.date().year();
    int month = dt.date().month();
    int day = dt.date().day();
    int d = year * 10000 + month * 100 + day;

    return d;
}

double toDouble(QDateTime dt) {
    int year = dt.date().year();
    int month = dt.date().month();
    int day = dt.date().day();
    double d = year * 10000 + month * 100 + day;

    double time = (double)dt.time().hour()/(double)100
                  + ((double)dt.time().minute() / (double)10000)
                  + ((double)dt.time().second() / (double)1000000);
    d = d + time;
    return d;
}

bool endsWith(const char* text, const char* end) {
    if (strlen(end) > strlen(text)) {
        return false;
    }
    int pos = 0;
    while (pos < strlen(end)) {
        if (end[strlen(end) - pos] != text[strlen(text) - pos]) {
            return false;
        }
        pos++;
    }
    return true;
}

std::vector<string>* split(string str, string token) {
//    char* s = (char*) malloc(str.size());
    const char* delim = token.c_str();

    //strcpy(s, str.c_str());
    char* s = (char*)str.c_str();

    vector<string>* res = new vector<string>();
    char* ptr;
    ptr = strtok(s, delim);
    res->push_back(string(ptr));
    while ((ptr = strtok(NULL, delim)) != NULL) {
        res->push_back(string(ptr));
    }

    return res;
}


QTime* toTime(int seconds) {
    int hour = seconds / 3600;
    seconds -= (hour * 3600);
    int min = (seconds / 60);
    seconds -= (min * 60);
    int secs = seconds;

    QTime* time = new QTime(hour, min, secs, 0);
    return time;
}

int toSeconds(QTime time) {
    int seconds = time.second() + (time.minute() * 60) + (time.hour() * 3600);
    return seconds;
}

std::string* uuid() {
#ifndef WINDOWS
    uuid_t t;
    uuid_generate(t);

    char ch[36];
    memset(ch, 0, 36);
    uuid_unparse(t, ch);
    string* res = new string(ch);
    qDebug(ch);
    return res;
#else
    QUuid uuid = QUuid::createUuid();
    QString suuid = uuid.toString();

    string res = suuid.toStdString();

    // removes the { } characters
    res = res.substr(1, res.size() - 2);
    return new string(res);
#endif
}

bool makedir(const char* path) {
#ifndef WINDOWS
    int status = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#else
    int status = mkdir(path);
#endif
    if (status != 0) {
        return false;
    } else {
        return true;
    }
}

std::string* getHomeDir() {
#ifndef WINDOWS
    string* home = new string(getenv("HOME"));
#else
    std::string homeDrive = std::string(getenv("HOMEDRIVE"));
    std::string homePath = std::string(getenv("HOMEPATH"));
    string* home = new string(homeDrive + homePath);
#endif
    return home;
}

