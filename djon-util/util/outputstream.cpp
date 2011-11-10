// *********************************************************************************************************************
// file:    outputstream.cpp
// author:  Juan Pablo Crossley (crossleyjuan@gmail.com)
// created: Oct/1/2010
// updated: Nov/7/2011
// license: GPL 2
//
// This file is part of the djon project, all the source code is licensed under the terms of the GPL license,
// the application and libraries are provided as-is and free of use under the terms explained in the file COPYING.GPL
// the authors are not responsible for any problem that this program, or any of its libraries, may cause.
// Its authors create this application in order to make the world a better place to live, but you should use it on
// your own risks.
// 
// Also, be adviced that, the GPL license force the committers to ensure this application will be free of use, thus
// if you do any modification you will be required to provide it for free unless you use it for personal use (you may 
// charge yourself if you want), bare in mind that you will be required to provide a copy of the license terms that ensures
// this program will be open sourced and all its derivated work will be too.
// *********************************************************************************************************************

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
