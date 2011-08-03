#include "pluginmanager.h"
#include "plugins/WindowPlugin.h"
#include "util.h"
#include <QPluginLoader>
#include <QDir>
#include <string>
#include <QCoreApplication>


std::vector<WindowPlugin*> PluginManager::_plugins;

void PluginManager::loadPlugins() {
    std::string* homeDir = getHomeDir();
    QString dir(((std::string)*homeDir + "/.djon/plugins").c_str());
//    QString dir("c:/workspace/personal/djon/libs");
    QCoreApplication::addLibraryPath(dir);

    QDir pluginsDir(dir);
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            WindowPlugin* wplugin = qobject_cast<WindowPlugin *>(plugin);
            if (wplugin) {
                _plugins.push_back(wplugin);
            }
        }
    }

//    delete(homeDir);
}

std::vector<WindowPlugin*> PluginManager::plugins() {
    return _plugins;
}
