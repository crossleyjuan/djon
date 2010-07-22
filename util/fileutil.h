/* 
 * File:   fileutil.h
 * Author: cross
 *
 * Created on July 7, 2010, 3:08 PM
 */

#ifndef _FILEUTIL_H
#define	_FILEUTIL_H

#include <vector>
#include <string>

char* readFile(char* fileName);
int writeFile(const std::string& fileName, const std::string& text, bool append);
int getdir (char* dir, std::vector<char*> &files, char* extension);

#endif	/* _FILEUTIL_H */

