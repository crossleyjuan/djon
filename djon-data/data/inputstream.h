#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H

#include <istream>
#include <iostream>
#include <stdio.h>

class InputStream
{
public:
    virtual unsigned char readChar() = 0;
    /* Reads 2 bytes in the input (little endian order) */
    virtual int readInt () = 0;
    /* Reads 4 bytes in the input (little endian order) */
    virtual long readLong () = 0;
    /* Reads a 4 byte float in the input */
    virtual float readFloatIEEE () = 0;
    /* Reads a 8 byte double in the input */
    virtual double readDoubleIEEE () = 0;
    /* Read a chars */
    virtual char* readChars() = 0;
    virtual char* readChars(int length) = 0;
    virtual const char* readFull() = 0;
    virtual long crc32() = 0;
    virtual long currentPos() const = 0;
    virtual void seek(long i) = 0;
    bool eof();

    virtual std::string* readString() = 0;
    virtual std::string fileName() = 0;

};

#endif // INPUTSTREAM_H
