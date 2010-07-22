#ifndef DTIME_H
#define DTIME_H

#include <QTime>

class DTime
{
public:
    DTime();
    DTime(long secs);
    DTime(DTime& time);
    ~DTime();

    long secs();
    QTime* toQTime();
    const char* toChar();

    void add(long secs);
    void add(DTime& time);

private:
    long _secs;
};

#endif // DTIME_H
