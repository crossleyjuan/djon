/* 
 * File:   datautil.h
 * Author: cross
 *
 * Created on July 7, 2010, 2:01 PM
 */

#ifndef _DATAUTIL_H
#define	_DATAUTIL_H

#include <map>
#include <string.h>
#include <string>

typedef std::map<std::string, std::string> hashmap;

hashmap* parseTextFormat(std::string text);

#define READ_ELEMENT(col,name) \
    ((col->find(name) != col->end())? col->find(name)->second : (char*)"")

#endif	/* _DATAUTIL_H */

