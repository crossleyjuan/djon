#ifndef DEBUGINPUTSTREAM_H
#define DEBUGINPUTSTREAM_H
#include <string>
#include "util.h"
#include "inputstream.h"

class DebugInputStream: public InputStream
{
public:
    DebugInputStream(InputStream* stream);
    virtual ~DebugInputStream();

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

    std::string* readString();
    std::string fileName();

private:
    InputStream* _inputStream;

    Logger* _logger;
};

#endif // DEBUGINPUTSTREAM_H
