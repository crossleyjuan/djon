#ifndef FILEINPUTSTREAM_H
#define FILEINPUTSTREAM_H

#include "inputstream.h"
#include <istream>
#include <iostream>
#include <stdio.h>

class FileInputStream: public InputStream
{
public:
    FileInputStream(std::string fileName, FILE* pFile);
    virtual ~FileInputStream();

    virtual unsigned char readChar();
    /* Reads 2 bytes in the input (little endian order) */
    virtual int readInt ();
    /* Reads 4 bytes in the input (little endian order) */
    virtual long readLong ();
    /* Reads a 4 byte float in the input */
    virtual float readFloatIEEE ();
    /* Reads a 8 byte double in the input */
    virtual double readDoubleIEEE ();
    /* Read a chars */
    virtual char* readChars();
    virtual char* readChars(int length);
    virtual const char* readFull();
    virtual long currentPos() const;
    virtual void seek(long i);
    virtual long crc32();

    virtual std::string* readString();
    virtual std::string fileName();
    virtual bool eof();

private:
    FILE* _pFile;
    std::string _fileName;
};

#endif // FILEINPUTSTREAM_H
