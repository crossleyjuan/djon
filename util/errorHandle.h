#ifndef ERRORHANDLE_H
#define ERRORHANDLE_H

/******************************************************************
Error Functions
*/
void setLastError(int errorCode, const char* errorDescription);
bool errorOcurred();
const char* getLastErrorDescription();
int getLastErrorCode();
void clearError();
/******************************************************************/

#endif // ERRORHANDLE_H
