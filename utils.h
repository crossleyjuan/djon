#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <QtCore/QDate>
#include "template.h"
#include <vector>

/*****************************************************************
String functions
*/
std::string toString(double a);
std::string toString(int a);
std::vector<string>* split(string str, string token);
/*****************************************************************/

QDateTime* toDateTime(int date);
int toInt(QDateTime date);

void logInfo(char* text);

bool endsWith(const char* text, const char* end);

vector<Template*>* readTemplates();

#endif // UTILS_H
