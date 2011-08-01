#ifndef WINDOWPLUGIN_H
#define WINDOWPLUGIN_H

#include <QtPlugin>
#include <QIcon>
/*
#include <QObject>

*/
class Workspace;
//
class WindowPlugin {
public:
    virtual ~WindowPlugin() {}
    virtual const char* pluginName() const = 0;
    virtual void setWorkspace(const Workspace* workspace) = 0;
    virtual const char* pluginMenuText() const = 0;
    virtual QIcon pluginIcon() const = 0;
    virtual void showDefaultScreen(QWidget* parent = 0) = 0;
};

Q_DECLARE_INTERFACE(WindowPlugin,
                    "com.djon.Plugin.WindowPlugin/1.0")

#endif // WINDOWPLUGIN_H
