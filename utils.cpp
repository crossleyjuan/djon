#include "utils.h"
#include <sstream>
#include <iostream>
#include <string.h>

using namespace std;

std::string toString(double a) {
    std::stringstream ss;
    ss << a;
    return ss.str();
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

QDateTime toDateTime(int date) {
    int year = date / 10000;
    int month = (date / 100) - (year * 100);
    int day = date - (month * 100) - (year * 10000);
    QDateTime dateTime(QDate(year, month, day));
    return dateTime;
}

int toInt(QDateTime dt) {
    int year = dt.date().year();
    int month = dt.date().month();
    int day = dt.date().day();
    int d = year * 10000 + month * 100 + day;

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
