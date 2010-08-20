#include "stringfunctions.h"
#include "util.h"
#include <string.h>
#include <stdlib.h>

char* strcpy(std::string str) {
    return strcpy((char*)str.c_str());
}

char* strcpy(char* str) {
    int len = strlen(str);
    char* result = (char*)mmalloc(len + 1);
    memset(result, 0, len +1);
    memcpy(result, str, len);
    return result;
}

bool endsWith(char* source, char* check) {
    if (strlen(source) < strlen(check)) {
        return false;
    }
    char* test = source + (strlen(source) - strlen(check));
    if (strcmp(test, check) == 0) {
        return true;
    } else {
        return false;
    }
}

