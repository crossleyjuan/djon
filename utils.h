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
QTime* toTime(int seconds);
int toSeconds(QTime time);

int toInt(QDateTime date);

void logInfo(char* text);

bool endsWith(const char* text, const char* end);

vector<Template*>* readTemplates();
Template* readTemplate(string* templateName);

std::string* uuid();

#endif // UTILS_H
