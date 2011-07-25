/* 
 * File:   stringfunctions.h
 * Author: cross
 *
 * Created on July 8, 2010, 2:54 PM
 */

#ifndef _STRINGFUNCTIONS_H
#define	_STRINGFUNCTIONS_H

#include <string>
#include <vector>

char* strcpy(char* str, int len);
char* strcpy(std::string str);
bool endsWith(char* source, char* check);
std::vector<std::string*>* tokenizer(const std::string source, const char* tokens);
std::string format(const char * fmt, ...);

#endif	/* _STRINGFUNCTIONS_H */

