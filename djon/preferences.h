#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <string>

class Preferences
{
public:
    Preferences();
    std::string lastWindowState();
    void setLastWindowState(std::string);
    std::string lastTrackWindowState();
    void setLastTrackWindowState(std::string);
    std::string collapsed();
    void setCollapsed(std::string);
    std::string lastTrackedTask();
    void setLastTrackedTask(std::string);
    std::string logSort();
    void setLogSort(std::string);

private:
    std::string _lastWindowState;
    std::string _lastTrackWindowState;
    std::string _collapsed;
    std::string _lastTrackedTask;
    std::string _logSort;
};

#endif // PREFERENCES_H
