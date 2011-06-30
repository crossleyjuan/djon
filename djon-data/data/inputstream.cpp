#include "inputstream.h"

#include "project.h"
#include "task.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>

InputStream::InputStream(std::string fileName, FILE *pFile)
{
    _pFile = pFile;
    _fileName = fileName;
}

unsigned char InputStream::readChar() {
    unsigned char v;
    fread(&v, 1, 1, _pFile);
    return v;
}

/* Reads 2 bytes in the input (little endian order) */
int InputStream::readInt () {
    int v = readChar() | readChar() << 8;
    return v;
}

/* Reads 4 bytes in the input (little endian order) */
long InputStream::readLong () {
    int v = readInt();
    v = v << 16;
    v = readInt() | v;

    return v;
}

/* Reads a 4 byte float in the input */
float InputStream::readFloatIEEE () {
    float f;
    fread(&f, 1, sizeof(f), _pFile);
    return f;
}

/* Reads a 8 byte double in the input */
double InputStream::readDoubleIEEE () {
    double d;
    fread(&d, 1, sizeof(d), _pFile);
    return d;
}

/* Read a chars */
char* InputStream::readChars() {
    int len = readInt();
    char* res = readChars(len);
    return res;
}

std::string* InputStream::readString() {
    char* c = readChars();
    std::string* res = new std::string(c);
    free(c);
    return res;
}

std::string InputStream::fileName() {
    return _fileName;
}

char* InputStream::readChars(int length) {
    char* res = (char*)malloc(length+1);
    memset(res, 0, length+1);
    fread(res, 1, length, _pFile);
    return res;
}

const char* InputStream::readFull() {
    fseek(_pFile, 0, SEEK_SET);
    std::stringstream ss;
    char buffer[1024];
    int readed = 0;
    while (!feof(_pFile)) {
        memset(buffer, 0, 1024);
        readed = fread(buffer, 1, 1023, _pFile);
        ss << buffer;
    }
    std::string str = ss.str();
    return strdup(str.c_str());
}

bool InputStream::eof() {
    return feof(_pFile);
}

