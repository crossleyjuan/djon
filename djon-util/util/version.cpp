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

#include "version.h"
#include "util.h"
#include <stdlib.h>

Version::Version(std::string version)
{
    _version = version;

    _mayor = 0;
    _minor = 0;
    _revision = 0;
    std::vector<string> values = split(version, ".");
    _mayor = atoi(values.at(0).c_str());
    if (values.size() > 1) {
        _minor = atoi(values.at(1).c_str());
    } else {
        _minor = 0;
    }
    if (values.size() > 2) {
        _revision = atoi(values.at(2).c_str());
    }
}

Version::Version(const Version &version) {
    _mayor = version._mayor;
    _revision = version._revision;
    _version = version._version;
}

Version::~Version() {
}

bool Version::operator>(const Version& version) {
    Version* current = this;
    if (current->_mayor > version._mayor) {
        return true;
    } else if ((current->_mayor == version._mayor) && (current->_minor > version._minor)) {
        return true;
    } else if ((current->_mayor == version._mayor) && (current->_minor == version._minor) && (current->_revision > version._revision)) {
        return true;
    } else {
        return false;
    }
}

bool Version::operator>=(const Version& version) {
    Version* current = this;
    if (*current == version) {
        return true;
    } else {
        return *current > version;
    }
}

bool Version::operator<(const Version& version) {
    Version* current = this;
    if (current->_mayor < version._mayor) {
        return true;
    } else if ((current->_mayor == version._mayor) && (current->_minor < version._minor)) {
        return true;
    } else if ((current->_mayor == version._mayor) && (current->_minor == version._minor) && (current->_revision < version._revision)) {
        return true;
    } else {
        return false;
    }
}

bool Version::operator<=(const Version& version) {
    Version* current = this;
    if (*current == version) {
        return true;
    } else {
        return *current < version;
    }
}

bool Version::operator==(const Version& version) {
    Version* current = this;
    return ((current->_mayor == version._mayor) && (current->_minor == version._minor) && (current->_revision == version._revision));
}

Version::operator std::string() {
    return _version;
}
