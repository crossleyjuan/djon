#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H

#include <istream>
#include <iostream>
#include <stdio.h>

class InputStream
{
public:
    InputStream(std::string fileName, FILE* pFile);
    unsigned char readChar();
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
    char* readChars(int length);
    const char* readFull();
    bool eof();

    std::string* readString();
    std::string fileName();

private:
    FILE* _pFile;
    std::string _fileName;
};

#endif // INPUTSTREAM_H
