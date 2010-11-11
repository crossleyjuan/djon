#include "stringfunctions.h"
#include "util.h"
#include <string.h>
#include <stdlib.h>

char* strcpy(std::string str) {
    return strcpy(const_cast<char*>(str.c_str()), str.length());
}

char* strcpy(char* str, int len) {
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

std::vector<string*>* tokenizer(const std::string source, const char* tokens) {
    std::vector<string*>* result = new std::vector<string*>();
    char* csource = strcpy(source);
    char* token = strtok(csource, tokens);
    while (token != NULL) {
        std::string* item = new string(token);
        result->push_back(item);
        token = strtok(NULL, tokens);
    }
    free(csource);

    return result;
}
