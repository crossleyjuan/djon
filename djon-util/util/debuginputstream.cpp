// *********************************************************************************************************************
// file:
// author: Juan Pablo Crossley (crossleyjuan@gmail.com)
// created:
// updated:
// license:
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

#include "debuginputstream.h"

DebugInputStream::DebugInputStream(InputStream* stream)
{
    _inputStream = stream;
    _logger = getLogger(NULL);
}

DebugInputStream::~DebugInputStream() {
    if (_logger) {
        delete(_logger);
        _logger = 0;
    }
}

unsigned char DebugInputStream::readChar() {
    unsigned char res = _inputStream->readChar();
    _logger->debug(format("%d", res));
    return res;
}

/* Reads 2 bytes in the input (little endian order) */
int DebugInputStream::readInt () {
    int res = _inputStream->readInt();
    _logger->debug(format("%d", res));
    return res;
}

/* Reads 4 bytes in the input (little endian order) */
long DebugInputStream::readLong () {
    long res = _inputStream->readLong();
    _logger->debug(format("%d", res));
    return res;
}

/* Reads a 4 byte float in the input */
float DebugInputStream::readFloatIEEE () {
    float res = _inputStream->readFloatIEEE();
    _logger->debug(format("%d", res));
    return res;
}

/* Reads a 8 byte double in the input */
double DebugInputStream::readDoubleIEEE () {
    double res = _inputStream->readDoubleIEEE();
    _logger->debug(format("%d", res));
    return res;
}

/* Read a chars */
char* DebugInputStream::readChars() {
    char* res = _inputStream->readChars();
    _logger->debug(format("%s", res));
    return res;
}

std::string* DebugInputStream::readString() {
    std::string* res = _inputStream->readString();
    _logger->debug(format("%s", res->c_str()));
    return res;
}

std::string DebugInputStream::fileName() {
    std::string res = _inputStream->fileName();
    _logger->debug(format("%s", res.c_str()));
    return res;
}

char* DebugInputStream::readChars(int length) {
    char* res = _inputStream->readChars(length);
    _logger->debug(format("%s", res));
    return res;
}

const char* DebugInputStream::readFull() {
    const char* res = _inputStream->readFull();
    _logger->debug(format("%s", res));
    return res;
}

long DebugInputStream::currentPos() const {
    long res = _inputStream->currentPos();
    _logger->debug(format("%d", res));
    return res;
}

void DebugInputStream::seek(long i) {
    _inputStream->seek(i);
}

long DebugInputStream::crc32() {
    return _inputStream->crc32();
}
