#ifndef COREPLUGIN_H
#define COREPLUGIN_H

#include "core-plugin_global.h"
// #include "pluginmanager.h"
#include "plugins/WindowPlugin.h"

class COREPLUGINSHARED_EXPORT Coreplugin: public WindowPlugin {
    Q_OBJECT
    Q_INTERFACES(WindowPlugin)
public:
//    Coreplugin();
//    Coreplugin(const Coreplugin& plugin);

    virtual ~Coreplugin();

    virtual const char* pluginName() const;
    virtual const char* pluginMenuText() const;
    virtual QIcon pluginIcon() const;
    virtual void setWorkspace(const Workspace* workspace);

public slots:
    virtual void showDefaultScreen(QWidget* parent = 0);

private:
    const Workspace* _workspace;
};

#endif // COREPLUGIN_H
