#ifndef PLUGINSETTINGS_H
#define PLUGINSETTINGS_H

#include <QDialog>
#include <vector>
#include <string>
#include "core-defs.h"

namespace Ui {
    class PluginSettings;
}

class PluginSettings : public QDialog
{
    Q_OBJECT

public:
    explicit PluginSettings(QWidget *parent = 0);
    ~PluginSettings();

private:
    Ui::PluginSettings *ui;
    CorePluginSettings _settings;

private:
    std::vector<std::string*>* getDataSources();
    void populateDSN();


private slots:
    void on_buttonBox_accepted();
};

#endif // PLUGINSETTINGS_H
