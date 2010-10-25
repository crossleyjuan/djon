#ifndef DURATION_H
#define DURATION_H

#include <string>

class Duration
{
public:
    Duration();
    Duration(int days, int hours, int minutes);
    Duration(const std::string duration);
    Duration(const Duration& orig);
    ~Duration();

    int days();
    void setDays(int days);
    int hours();
    void setHours(int hours);
    int minutes();
    void setMinutes(int minutes);
    char* toChar();

private:
    int _days;
    int _hours;
    int _minutes;
};

#endif // DURATION_H
