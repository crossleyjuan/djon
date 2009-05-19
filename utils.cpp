#include "utils.h"
#include <sstream>
#include <iostream>
#include <string.h>
#include "template.h"
#include "fileutils.h"
#include <stdlib.h>

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

std::vector<string>* split(string str, string token) {
    char* s = (char*) malloc(str.size());
    const char* delim = token.c_str();

    strcpy(s, str.c_str());

    vector<string>* res = new vector<string>();
    char* ptr;
    ptr = strtok(s, delim);
    res->push_back(string(ptr));
    while ((ptr = strtok(NULL, delim)) != NULL) {
        res->push_back(string(ptr));
    }
    delete s;
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
