// *********************************************************************************************************************
// file:    util.cpp
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

#include "util.h"

#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "config.h"
#include <stdlib.h>

#ifndef WINDOWS
#include <uuid/uuid.h>
#endif
#ifdef LINUX
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/scrnsaver.h>
#endif
#ifdef MAC
#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <IOKit/IOKitLib.h>
#endif
#ifdef WINDOWS
#include <Windows.h>
#include <Winuser.h>
#include <QUuid>
#include <direct.h>
#endif

std::string getConfigFileName() {
    std::string* homeDir = getHomeDir();
    std::string confFileName = *homeDir + "/.djon/djon.conf";
    delete (homeDir);
    return confFileName;
}

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
    for (unsigned int pos = 0; strlen(end) > pos; pos++) {
        if (end[strlen(end) - pos] != text[strlen(text) - pos]) {
            return false;
        }
    }
    return true;
}

std::vector<string> split(const string str, string token) {
//    char* s = (char*) malloc(str.size());
    const char* delim = token.c_str();

    //strcpy(s, str.c_str());
    char* s = strcpy((char*)str.c_str());

    vector<string> res;
    char* ptr;
    ptr = strtok(s, delim);
    while (ptr != NULL) {
        res.push_back(string(ptr));
        ptr = strtok(NULL, delim);
    }

    free (s);
    return res;
}


long countChar(const char* s, const char c) {
    long num = 0;
    for (unsigned int x = 0; x < strlen(s); x++) {
        if (s[x] == c) {
            num++;
        }
    }
    return num;
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

std::string* getTempDir() {
#ifndef WINDOWS
    string* tmp = new string("/tmp");
#else
    string* tmp = new std::string(getenv("TMP"));
#endif
    return tmp;
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

char* readValue(std::string cont, std::string key) {
    hashmap* map = parseTextFormat(cont);
    std::string value = READ_ELEMENT(map, key);

    char* res = NULL;
    if (value.size() == 0) {
        res = strcpy("");
    } else {
        res = strcpy(value);
    }

    delete(map);
    return res;
}

char* readConfValue(const string& name, const char* def) {
    std::string confFileName = getConfigFileName();
    char* conf = readFile(const_cast<char*> (confFileName.c_str()));

    char* res = readValue(string(conf), name);
    if (strlen(res) == 0) {
        free(res);
        res = strcpy(def);
    }
    free(conf);
    return res;
}

string replaceValue(string cont, string key, string value) {
    int pos = cont.find(key + ":");
    string newValue = key + ":" + value + ";\n";
    if (pos != cont.npos) {
        int end = cont.find(";", pos) + 2; //; and \n
        cont = cont.replace(pos, end - pos, newValue);
    } else {
        cont = cont.append(newValue);
    }

    return cont;
}

int writeConfValue(const string& name, const string& value) {
    std::string confFileName = getConfigFileName();
    std::string conf = std::string(readFile(const_cast<char*> (confFileName.c_str())));

    conf = replaceValue(conf, name, value);

    int res = writeFile(confFileName, conf, false);

    return res;
}

void readElement(string& readed, stringstream &str) {

    int c;
    do {
        c = str.get();
        if (c != ';') {
            if ((readed.length() != 0) || ((c != 13) && (c != 10))) {
                if (str.good()) {
                    char str[2];
                    str[0] = c;
                    str[1] = '\0';
                    readed += str;
                } else {
                    break;
                }
            }
        }
    } while (c != ';');
}

hashmap* parseTextFormat(std::string text) {
    hashmap* mapValue = new hashmap();
    stringstream in(text);
    while (in.good()) {
        string element;
        readElement(element, in);
        if (element.length() > 0) {
            string name = element.substr(0, element.find_first_of(":"));
            string value = element.substr(element.find_first_of(":") + 1, element.length() - element.find_first_of(":"));
            QString qValue = QString(value.c_str());
            qValue = qValue.trimmed();
            value = qValue.toStdString();
            mapValue->insert(pair<string, string > (name, value));
        }
    }
    return mapValue;
}

void* mmalloc(size_t size) {
    void* p = malloc(size);
    if (p == NULL) {
        qDebug("Out of memory!!!");
        exit(EXIT_FAILURE);
    }
    return p;
}

int writePreference(const std::string& key, const std::string& value) {
    std::string* home = getHomeDir();
    std::string fileName = home->append("/.djon/djon.user");

    qDebug("writePreference key: %s, value: %s", key.c_str(), value.c_str());
    std::string currentData = std::string(readFile(const_cast<char*> (fileName.c_str())));

    qDebug("currentData %s", currentData.c_str());
    currentData = replaceValue(currentData, key, value);
    qDebug("newData %s", currentData.c_str());

    int res = writeFile(fileName, currentData, false);

    delete(home);

    return res;
}

char* readPreference(const std::string& key, const char* def) {
    std::string* home = getHomeDir();
    std::string fileName = home->append("/.djon/djon.user");

    string cont = readFile(const_cast<char*>(fileName.c_str()));

    char* value = readValue(cont, key);

    if (strlen(value) == 0) {
        free(value);
        value = strcpy(def);
    }

    delete (home);

    return value;
}


long idleTime() {
    long idlesecs;
#ifdef LINUX
    bool _idleDetectionPossible;
    XScreenSaverInfo *_mit_info;

    int event_base, error_base;
    if(XScreenSaverQueryExtension(QX11Info::display(), &event_base, &error_base))
        _idleDetectionPossible = true;
    else
        _idleDetectionPossible = false;
    _mit_info = XScreenSaverAllocInfo();

    XScreenSaverQueryInfo(QX11Info::display(), QX11Info::appRootWindow(), _mit_info);

    idlesecs = (_mit_info->idle/1000);

#endif
#ifdef WINDOWS

    LASTINPUTINFO lif;
    lif.cbSize = sizeof(LASTINPUTINFO);
    GetLastInputInfo(&lif);
    DWORD tickCount = GetTickCount();
    idlesecs = (tickCount - lif.dwTime) / 1000;

#endif
#ifdef MAC
    idlesecs = -1;//int64_t
    io_iterator_t iter = 0;
    if (IOServiceGetMatchingServices(kIOMasterPortDefault, IOServiceMatching("IOHIDSystem"), &iter) == KERN_SUCCESS) {
        io_registry_entry_t entry = IOIteratorNext(iter);
        if (entry) {
            CFMutableDictionaryRef dict = NULL;
            if (IORegistryEntryCreateCFProperties(entry, &dict, kCFAllocatorDefault, 0) == KERN_SUCCESS) {
                CFNumberRef obj = (CFNumberRef)CFDictionaryGetValue(dict, CFSTR("HIDIdleTime"));
                if (obj) {
                    int64_t nanoseconds = 0;
                    if (CFNumberGetValue(obj, kCFNumberSInt64Type, &nanoseconds)) {
                        idlesecs = (nanoseconds >> 30); // Divide by 10^9 to convert from nanoseconds to seconds.
                    }
                }
                CFRelease(dict);
            }
            IOObjectRelease(entry);
        }
        IOObjectRelease(iter);
    }

//    bool _idleDetectionPossible;
//    XScreenSaverInfo *_mit_info;

//    int event_base, error_base;
//    Display* display = XOpenDisplay(0);
//    if(XScreenSaverQueryExtension(display, &event_base, &error_base))
//        _idleDetectionPossible = true;
//    else
//        _idleDetectionPossible = false;
//    _mit_info = XScreenSaverAllocInfo();

//    int i = DefaultScreen(display);
//    int res = XScreenSaverQueryInfo(display, RootWindow(display, i), _mit_info);

//    if (res != 0) {
//        idlesecs = (_mit_info->idle/1000);
//    }
#endif

    return idlesecs;
}

void checkConfigFile() {
    std::string confFileName = getConfigFileName();
    if (!existFile(confFileName.c_str())) {
        std::string* homeDir = getHomeDir();
        std::string djonDir = *homeDir + "/.djon";

        if (!existDir(djonDir.c_str())) {
            if (!makedir(djonDir.c_str())) {
                setLastError(9, "d-jon home directory cannot be created, please check you have permissions to create: %s", djonDir.c_str());
                return;
            }
        }
        std::string prjDir = djonDir + "/Projects";

        if (!existDir(prjDir.c_str())) {
            if (!makedir(prjDir.c_str())) {
                setLastError(7, "Project directory cannot be created, please check you have permissions to create: %s", prjDir.c_str());
                return;
            }
        }
        std::string tplDir = djonDir + "/templates";
        if (!existDir(tplDir.c_str())) {
            if (!makedir(tplDir.c_str())) {
                setLastError(8, "Templates directory cannot be created, please check you have permissions to create: %s", tplDir.c_str());
                return;
            }
            qDebug("Creating default template");
            std::string commonTemplate = "template-name: CommonTask*;\ntemplate-description: Common Task;\nstatus: NEW,IN PROGRESS,CLOSED*;\n";
            writeFile(tplDir + "/CommonTask.tpl", commonTemplate, false);
        }
        writeConfValue("last-project-dir", prjDir);
    }
    string *user = new string(readConfValue("user-id", ""));
    if (user->length() == 0) {
        delete(user);
        user = uuid();
        writeConfValue("user-id", *user);
        delete(user);
    }
}

Version getCurrentVersion() {
    return getVersion(VERSION);
}

Version getVersion(const char* version) {
    return Version(std::string(version));
}
