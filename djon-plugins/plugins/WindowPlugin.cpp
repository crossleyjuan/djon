#include "WindowPlugin.h"


WindowPlugin::WindowPlugin() {
}

WindowPlugin::WindowPlugin(const WindowPlugin& plugin) {
    _workspace = plugin._workspace;
}

WindowPlugin::~WindowPlugin() {

}

void WindowPlugin::setWorkspace(const Workspace* workspace) {
    _workspace = workspace;
}
