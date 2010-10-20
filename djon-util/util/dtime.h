#ifndef DTIME_H
#define DTIME_H

#include <QTime>

class DTime
{
public:
    DTime();
    DTime(long secs);
    DTime(const QTime& time);
    DTime(const DTime& time);
    ~DTime();

    long secs() const;
    QTime* toQTime();
    const char* toChar();

    void add(long secs);
    void add(DTime& time);

    DTime operator +(const DTime& dtime) const;


private:
    long _secs;
    void time(int& hour, int& min, int& secs) const;
};

#endif // DTIME_H
