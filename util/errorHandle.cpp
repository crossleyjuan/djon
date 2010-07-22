#include "errorHandle.h"

const char* lastError;
int lastErrorCode;

void setLastError(int errorCode, const char* errorDescription) {
    lastErrorCode = errorCode;
    lastError = errorDescription;
}

bool errorOcurred() {
    return (lastError > 0);
}

const char* getLastErrorDescription() {
    return lastError;
}

int getLastErrorCode() {
    return lastErrorCode;
}

void clearError() {
    lastError = 0;
    lastErrorCode = 0;
}
