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
