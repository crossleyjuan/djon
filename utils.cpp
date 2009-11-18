#include "utils.h"
#include <sstream>
#include <iostream>
#include <string.h>
#include "template.h"
#include "fileutils.h"
#include <stdlib.h>
#include <uuid/uuid.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

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

vector<Template*>* m_templates;

vector<Template*>* readTemplates() {
    if (m_templates != NULL) {
        return m_templates;
    }
    m_templates = new vector<Template*>();
    char* home = getenv("HOME");
    string path = string(home) + "/.djon/templates/";

    vector<string> files;
    int res = getdir(path, files, "tpl");

    if (res == 0) {
        for (vector<string>::iterator it = files.begin(); it != files.end(); it++) {
            string fileName = path + (*it);

            hashmap* conf = readFile(fileName);

            string templateName = conf->find("template-name")->second;
            string statusList = conf->find("status")->second;
            string jobList = conf->find("job")->second;

            vector<string>* vecStatus = split(statusList, ",");
            vector<string>* vecJobList = split(jobList, ",");

            Template* tpl = new Template(templateName, vecStatus, vecJobList);
            m_templates->push_back(tpl);
        }
    }

    return m_templates;
}

Template* readTemplate(string* name) {
    vector<Template*>* templates = readTemplates();
    for (vector<Template*>::iterator it = templates->begin(); it != templates->end(); it++) {
        Template* t = *it;
        if (t->name().compare(*name) == 0) {
            return t;
        }
    }
    return NULL;
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
    uuid_t t;
    uuid_generate(t);

    char ch[36];
    memset(ch, 0, 36);
    uuid_unparse(t, ch);
    string* res = new string(ch);
    qDebug(ch);
    return res;
}

bool makedir(const char* path) {
    int status = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (status != 0) {
        return false;
    } else {
        return true;
    }
}
