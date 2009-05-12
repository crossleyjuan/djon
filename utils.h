#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <QtCore/QDate>

std::string toString(double a);
std::string toString(int a);

QDateTime toDateTime(int date);
int toInt(QDateTime date);

void logInfo(char* text);

bool endsWith(const char* text, const char* end);

#endif // UTILS_H
