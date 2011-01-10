#include "inputstream.h"

#include "project.h"
#include "task.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

InputStream::InputStream(FILE *pFile)
{
    _pFile = pFile;
}

char InputStream::readChar() {
    char v;
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
    char* res = (char*)malloc(len + 1);
    memset(res, 0, len + 1);
    for (int x = 0; x < len; x++) {
        char c = readChar();
        res[x] = c;
    }
    return res;
}

std::string* InputStream::readString() {
    char* c = readChars();
    return new std::string(c);
}

