/* 
 * File:   util.h
 * Author: cross
 *
 * Created on July 7, 2010, 1:33 PM
 */

#ifndef _UTIL_H
#define	_UTIL_H

#include "util/DateTime.h"
#include "util/dtime.h"
#include "util/fileutil.h"
#include "util/stringfunctions.h"
#include "util/errorHandle.h"
#include "util/version.h"
#include "util/calendar.h"
#include "util/logger.h"

// Data information
#include "util/debuginputstream.h"
#include "util/fileinputstream.h"
#include "util/inputstream.h"
#include "util/outputstream.h"


#include <string>
#include <QtCore/QDate>
#include <vector>
#include <map>

/*****************************************************************
  Type Definitions and macros
*/
typedef std::map<std::string, std::string> hashmap;

#define READ_ELEMENT(col,name) \
    ((col->find(name) != col->end())? col->find(name)->second : (char*)"")

/*****************************************************************
String functions
*/
std::string toString(double a);
std::string toString(double a, int fixedPrecision);
std::string toString(int a);
std::vector<string> split(string str, string token);
long countChar(const char* s, const char c);
/*****************************************************************/

QDateTime* toDateTime(int date);
QDateTime* toDateTime(double date);
QTime* toTime(int seconds);
int toSeconds(QTime time);

int toInt(QDateTime date);
double toDouble(QDateTime dt);

void logInfo(char* text);

long idleTime();

bool endsWith(const char* text, const char* end);

std::string* uuid();


bool makedir(const char* path);
std::string* getHomeDir();
std::string* getTempDir();
hashmap* parseTextFormat(std::string text);

void checkConfigFile();
char* readConfValue(const string& name, const char* def = 0);
int writeConfValue(const string& name, const string& value);
char* readValue(std::string cont, std::string key);

int writePreference(const std::string& key, const std::string& value);
char* readPreference(const std::string& key, const char* def);

Version getCurrentVersion();
Version getVersion(const char* version);

/***********************************************************************
* Memory functions
***********************************************************************/
void* mmalloc(size_t size);

/***********************************************************************/

#endif	/* _UTIL_H */

