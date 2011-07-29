#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "djon-plugins_global.h"
#include "plugins/WindowPlugin.h"
#include <vector>

class DJONPLUGINSSHARED_EXPORT PluginManager {
public:
    static void loadPlugins();

    static std::vector<WindowPlugin*> plugins();
private:
    static std::vector<WindowPlugin*> _plugins;
};

#endif // PLUGINMANAGER_H
