#ifndef OUTPUTSTREAM_H
#define OUTPUTSTREAM_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

class OutputStream
{
public:
    OutputStream(FILE* pFile);
    void writeChar (char v);
    /* Write 2 bytes in the output (little endian order) */
    void writeInt (int v);
    /* Write 4 bytes in the output (little endian order) */
    void writeLong (long v);
    /* Write a 4 byte float in the output */
    void writeFloatIEEE (float v);
    /* Write a 8 byte double in the output */
    void writeDoubleIEEE (double v);
    /* Write a char */
    void writeChars(const char* text, int len);
    void writeString(const std::string* text);

private:
    FILE* _pFile;
};

#endif // OUTPUTSTREAM_H