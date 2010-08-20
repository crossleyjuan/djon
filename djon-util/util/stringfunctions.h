/* 
 * File:   stringfunctions.h
 * Author: cross
 *
 * Created on July 8, 2010, 2:54 PM
 */

#ifndef _STRINGFUNCTIONS_H
#define	_STRINGFUNCTIONS_H

#include <string>

char* strcpy(char* str);
char* strcpy(std::string str);
bool endsWith(char* source, char* check);

#endif	/* _STRINGFUNCTIONS_H */

