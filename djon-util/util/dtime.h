#ifndef DTIME_H
#define DTIME_H

#include <QTime>

class DTime
{
public:
    DTime();
    DTime(long secs);
    DTime(int hours, int minutes, int secs);
    DTime(const QTime& time);
    DTime(const DTime& time);
    DTime(std::string time); // HHMMSS
    ~DTime();

    long totalSecs() const;
    long totalMinutes() const;

    int hour() const;
    int minutes() const;
    int seconds() const;

    QTime toQTime() const;
    char* toChar() const;
    QString toQString() const;

    void add(long secs);
    void add(const DTime time);

    DTime operator +(const DTime& dtime) const;
    void operator ++(int secs);


private:
    long _secs;
    void time(int& hour, int& min, int& secs) const;
};

#endif // DTIME_H
