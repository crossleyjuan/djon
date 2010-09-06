#ifndef DTIME_H
#define DTIME_H

#include <QTime>

class DTime
{
public:
    DTime();
    DTime(long secs);
    DTime(const QTime& time);
    DTime(DTime& time);
    ~DTime();

    long secs() const;
    QTime* toQTime();
    const char* toChar();

    void add(long secs);
    void add(DTime& time);

private:
    long _secs;
    void time(int& hour, int& min, int& secs) const;
};

#endif // DTIME_H
