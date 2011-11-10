// *********************************************************************************************************************
// file:    stringfunctions.cpp
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
