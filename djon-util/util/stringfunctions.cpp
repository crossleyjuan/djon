#include "stringfunctions.h"
#include "util.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sstream>

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

std::string format(const char * fmt, ...) {
  char* buffer = (char*)malloc(1000);
  memset(buffer, 0, 1000);
  va_list args;
  va_start (args, fmt);
  vsprintf (buffer,fmt, args);
  va_end(args);
  std::string result;
  std::stringstream ss;
  ss << buffer;
  result = ss.str();
  free(buffer);
  return result;
}
