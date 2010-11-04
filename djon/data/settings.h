#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <vector>

class Settings
{
public:
    Settings();
    bool closeToTray();
    void setCloseToTray(bool);
    bool showSysTrayWarning();
    void setShowSysTrayWarning(bool);
    std::string lastProjectDir();
    void setLastProjectDir(std::string);
    std::string lastReleaseNotes();
    void setLastReleaseNotes(std::string);
    std::vector<std::string> openProjects();
    void setOpenProjects(std::vector<std::string>);
    long idleTimeOut();
    void setIdleTimeOut(long);
    void load();
    void save();

private:
    bool _closeToTray;
    bool _showSysTrayWarning;
    std::string _lastProjectDir;
    std::string _lastReleaseNotes;
    std::vector<std::string> _openProjects;
    long _idleTimeOut;

    bool _closeToTrayChanged;
    bool _showSysTrayWarningChanged;
    bool _lastProjectDirChanged;
    bool _lastReleaseNotesChanged;
    bool _openProjectsChanged;
    bool _idleTimeOutChanged;
};

Settings* getSettings();
#endif // SETTINGS_H
