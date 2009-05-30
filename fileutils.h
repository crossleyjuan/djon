#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;

typedef map<string, string> hashmap;
#define READ_ELEMENT(col,name) \
    ((col->find(name) != col->end())? col->find(name)->second : "")
//    col->find(name)->second

int getdir (string dir, vector<string> &files, string extension);
hashmap* readFile(string fileName);
void writeFile(string fileName, hashmap* values);
#endif // FILEUTILS_H
