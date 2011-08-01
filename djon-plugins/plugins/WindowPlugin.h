#ifndef WINDOWPLUGIN_H
#define WINDOWPLUGIN_H

#include <QObject>
#include <QIcon>
#include "data.h"

// DJONPLUGINSSHARED_EXPORT
class WindowPlugin: public QObject {
public:
    virtual ~WindowPlugin() {}

    virtual void setWorkspace(const Workspace* workspace) = 0;
    virtual const char* pluginName() const = 0;
    virtual const char* pluginMenuText() const = 0;
    virtual QIcon pluginIcon() const = 0;
    virtual void showDefaultScreen(QWidget* parent = 0) = 0;

};

Q_DECLARE_INTERFACE(WindowPlugin,
                    "com.djon.Plugin.WindowPlugin/1.0")

#endif // WINDOWPLUGIN_H
