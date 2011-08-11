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
    std::vector<std::string> recentWorkspaces();
    void setRecentWorkspaces(std::vector<std::string>);
    std::string lastWorkspace();
    void setLastWorkspace(std::string);
    long idleTimeOut();
    void setIdleTimeOut(long);
    int checkUpdate();
    void setCheckUpdate(int);
    int transparency();
    void setTransparency(int);
    void load();
    void save();

private:
    bool _closeToTray;
    bool _showSysTrayWarning;
    std::string _lastProjectDir;
    std::string _lastWorkspace;
    std::string _lastReleaseNotes;
    std::vector<std::string> _openProjects;
    std::vector<std::string> _recentWorkspaces;
    long _idleTimeOut;
    int _checkUpdate;
    int _transparency;// 0-100

    bool _closeToTrayChanged;
    bool _showSysTrayWarningChanged;
    bool _lastProjectDirChanged;
    bool _lastWorkspaceChanged;
    bool _lastReleaseNotesChanged;
    bool _openProjectsChanged;
    bool _recentWorkspacesChanged;
    bool _idleTimeOutChanged;
    bool _checkUpdateChanged;
    bool _transparencyChanged;
};

Settings* getSettings();
#endif // SETTINGS_H
