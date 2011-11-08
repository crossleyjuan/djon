// *********************************************************************************************************************
// file:
// author: Juan Pablo Crossley (crossleyjuan@gmail.com)
// created:
// updated:
// license:
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

#include "duration.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <string>

Duration::Duration() {
    _days = 0;
    _hours = 0;
    _minutes = 0;
}

Duration::Duration(int days, int hours, int minutes) {
    _days = days;
    _hours = hours;
    _minutes = minutes;
}

Duration::Duration(const std::string duration) {
    _days = 0;
    _hours = 0;
    _minutes = 0;
    // ##
    // ##d##H##m
    const char* cdur = duration.c_str();
    int len = duration.length();
    if (strcspn(cdur, "dhm") != len) {
        std::vector<std::string*>* tokens = tokenizer(duration, "dhm");
        int pos = 0;
        if (strcspn(cdur, "d") != len) {
            _days = atoi(tokens->at(pos)->c_str());
            pos++;
        }
        if (strcspn(cdur, "h") != len) {
            _hours = atoi(tokens->at(pos)->c_str());
            pos++;
        }
        if (strcspn(cdur, "m") != len) {
            _minutes = atoi(tokens->at(pos)->c_str());
        }
    } else {
        if (strlen(cdur) > 0) {
            _days = atoi(cdur);
        }
    }
}

Duration::Duration(const Duration& orig) {
    _days = orig._days;
    _hours = orig._hours;
    _minutes = orig._minutes;
}

Duration::~Duration() {
}


int Duration::days() {
    return _days;
}

void Duration::setDays(int days) {
    _days = days;
}

int Duration::hours() {
    return _hours;
}

void Duration::setHours(int hours) {
    _hours = hours;
}

int Duration::minutes() {
    return _minutes;
}

void Duration::setMinutes(int minutes) {
    _minutes = minutes;
}

char* Duration::toChar() {
    std::stringstream ss;
    ss << _days << "d";
    ss << _hours << "h";
    ss << _minutes << "m";
    std::string res = ss.str();
    return strcpy(res);
//    char* buffer = (char*)mmalloc(9);
//    memset(buffer, 0, 6);
//    sprintf(buffer, "%02d\\d%02d\\h%02d\\m", _days, _hours, _minutes);

//    return buffer;
}

