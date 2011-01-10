#include "outputstream.h"
#include <string.h>

OutputStream::OutputStream(FILE *pFile)
{
    _pFile = pFile;
}

/* Write 1 byte in the output */
void OutputStream::writeChar (char v)
{
    fwrite(&v, 1, 1, _pFile);
}

/* Write 2 bytes in the output (little endian order) */
void OutputStream::writeInt (int v)
{
    writeChar ((v) & 0xff);
    writeChar ((v >> 8) & 0xff);
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
    for (int x = 0; x < len; x++) {
        writeChar(text[x]);
    }
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
