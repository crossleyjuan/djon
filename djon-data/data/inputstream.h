#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H

#include <istream>
#include <iostream>
#include <stdio.h>

class InputStream
{
public:
    InputStream(FILE* pFile);
    char readChar();
    /* Reads 2 bytes in the input (little endian order) */
    int readInt ();
    /* Reads 4 bytes in the input (little endian order) */
    long readLong ();
    /* Reads a 4 byte float in the input */
    float readFloatIEEE ();
    /* Reads a 8 byte double in the input */
    double readDoubleIEEE ();
    /* Read a chars */
    char* readChars();
    std::string* readString();

private:
    FILE* _pFile;
};

#endif // INPUTSTREAM_H
