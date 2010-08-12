#include "errorHandle.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QMessageBox>
#include <sstream>
#include <string>

char* _lastError;
int _lastErrorCode;

void setLastError(int errorCode, const char* errorDescription, ...) {
    va_list varlist;
    va_start(varlist, errorDescription);

    int len = strlen(errorDescription) * 2;
    _lastError = (char*)malloc(len);
    memset(_lastError, 0, len);
    vsprintf(_lastError, errorDescription, varlist);
    va_end(varlist);

    _lastErrorCode = errorCode;
}

bool errorOcurred() {
    return (_lastError > 0);
}

const char* lastErrorDescription() {
    return _lastError;
}

int lastErrorCode() {
    return _lastErrorCode;
}

void clearError() {
    _lastError = 0;
    _lastErrorCode = 0;
}

