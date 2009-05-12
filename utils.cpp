#include "utils.h"
#include <sstream>
#include <iostream>
#include <string.h>

using namespace std;

std::string toString(double a) {
    std::stringstream ss;
    ss << dec << a;
    return ss.str();
}

std::string toString(int a) {
    std::stringstream ss;
    ss << scientific << a;
    return ss.str();
}

void logInfo(char* text) {
    cout << text << endl;
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
