#ifndef ERRORHANDLE_H
#define ERRORHANDLE_H

#include <QObject>
/******************************************************************
Error Functions
*/
void setLastError(int errorCode, const char* errorDescription, ...);
bool errorOcurred();
const char* lastErrorDescription();
int lastErrorCode();
void clearError();
/******************************************************************/

#endif // ERRORHANDLE_H
