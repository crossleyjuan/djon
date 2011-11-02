#include "outputstream.h"
#include <string.h>
#include <cstdio>
#include <iostream>
#include <boost/crc.hpp>

OutputStream::OutputStream(FILE *pFile)
{
    _pFile = pFile;
}

/* Write 1 byte in the output */
void OutputStream::writeChar (unsigned char v)
{
    fwrite(&v, 1, 1, _pFile);
}

/* Write 2 bytes in the output (little endian order) */
void OutputStream::writeInt (int v)
{
    unsigned char c = (v & 255);
    unsigned char c2= ((v >> 8) & 255);
    writeChar (c);
    writeChar (c2);
}

/* Write 4 bytes in the output (little endian order) */
void OutputStream::writeLong (long v)
{
    writeInt ((v) & 0xffff);
    writeInt ((v >> 16) & 0xffff);
}

/* Write a 4 byte float in the output */
void OutputStream::writeFloatIEEE (float v)
{
    fwrite(&v, 1, sizeof(v), _pFile);
}

/* Write a 8 byte double in the output */
void OutputStream::writeDoubleIEEE (double v)
{
    fwrite(&v, 1, sizeof(v), _pFile);
}

void OutputStream::writeChars(const char *text, int len) {
    writeInt(len);
    fwrite(text, 1, len, _pFile);
}

void OutputStream::writeString(const std::string* text) {
    if (text == NULL) {
        writeChars("", 0);
    } else {
        const char* c = text->c_str();
        int l = strlen(c);
        writeChars(c, l);
    }
}

long OutputStream::crc32(int pos) {
    fflush(_pFile);
    long originalPos = currentPos();
    fseek(_pFile, 0, SEEK_END);
    int bufferSize = currentPos();
    bufferSize -= pos;
    seek(pos);

    char buffer[bufferSize];
    memset(buffer, 0, bufferSize);
    fread(buffer, 1, bufferSize, _pFile);

    boost::crc_32_type crc;
    crc.process_bytes(buffer, bufferSize);
    long result = crc.checksum();
    std::cout << "CRC: Output" << result << std::endl;

    // back to the original position
    seek(originalPos);
    return result;
}

void OutputStream::seek(long i) {
    fflush(_pFile);
    fseek (_pFile, i, SEEK_SET);
}

long OutputStream::currentPos() const {
    return ftell(_pFile);
}
