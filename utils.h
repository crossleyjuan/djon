#ifndef UTILS_H
#define UTILS_H

#include <sstream>

string toString(double a) {
    std::stringstream ss;
    ss << scientific << a;
    return ss.str();
}

string toString(int a) {
    std::stringstream ss;
    ss << scientific << a;
    return ss.str();
}

void logInfo(char* text) {
    cout << text << endl;
}

#endif // UTILS_H
