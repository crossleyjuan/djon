#include "coreplugin.h"
#include <QWidget>
#include <QtPlugin>

//Coreplugin::Coreplugin()
//{

//}

//Coreplugin::Coreplugin(const Coreplugin& plugin)
//{

//}

Coreplugin::~Coreplugin() {
}

const char* Coreplugin::pluginName() const {
    return "CorePlugin";
}

const char* Coreplugin::pluginMenuText() const {
    return "TimeSheet Export";
}

QIcon Coreplugin::pluginIcon() const {
    return QIcon();
}

void Coreplugin::showDefaultScreen(QWidget* parent) {
}


void Coreplugin::setWorkspace(const Workspace* workspace) {
    _workspace = workspace;
}

Q_EXPORT_PLUGIN2(Coreplugin, Coreplugin)
