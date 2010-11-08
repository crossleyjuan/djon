#include "fileutil.h"
#include "stringfunctions.h"
#include "errorHandle.h"
#include "util.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

char* readFile(char* fileName) {
    ifstream ifs;

    ifs.open(fileName, ifstream::in);

    stringbuf* buffer = new stringbuf();

    while (ifs.good()) {
        ifs >> buffer;
    }
    ifs.close();

    string str = buffer->str();

    delete(buffer);

    char* result = strcpy(str);

    return result;
}

int writeFile(const std::string& fileName, const std::string& text, bool append) {
    ofstream ofs;

    ios_base::openmode flags;
    if (append) {
        flags = ios_base::app | ios_base::out;
    } else {
        flags = ios_base::out;
    }
    ofs.open(fileName.c_str(),  flags);
    if (ofs.fail() != 0) {
        setLastError(1, (std::string("Failed to open the file: ") + fileName).c_str());
        return 1;
    }

    ofs << text;
    ofs.close();
    return 0;
}

int getdir (const char* dir, vector<char*> &files, const char* extension)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir)) == NULL) {
        setLastError(1, "Error opening the dir: %s", dir);
        return 1;
    }

    int len = strlen(extension) + 2;
    char* fileextension = (char*)mmalloc(len);

    memset(fileextension, 0, len);
    fileextension[0] = '.';
    strcpy(fileextension+1, extension);

    while ((dirp = readdir(dp)) != NULL) {
        char* currentFile (dirp->d_name);
        if (endsWith(currentFile, fileextension)) {
            files.push_back(strcpy(currentFile));
        }
    }
    closedir(dp);
    
    free(fileextension);
    return 0;
}

bool existFile(const char* fileName) {
    ifstream ifs;

    ifs.open(fileName, ifstream::in);

    bool exists = true;
    if (ifs.fail()) {
        exists = false;
    }
    ifs.close();
    return exists;
}

bool existDir(const char* dir) {
    DIR *dp;
    struct dirent *dirp;
    bool exists = true;
    if((dp  = opendir(dir)) == NULL) {
        exists = false;
    }
    return exists;
}
