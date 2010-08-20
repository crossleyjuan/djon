#include "updatemanager.h"
#include "util.h"
#include "config.h"
#include <QMessageBox>
#include <QProcess>

UpdateManager::UpdateManager(QObject *parent) :
    QObject(parent)
{
    _timer = new QTimer(this);
    _mins = 10;
    connect(_timer, SIGNAL(timeout()), this, SLOT(check()));
}

void UpdateManager::startCheck(int mins) {
    _mins = mins;
    _timer->start(_mins * 60000);
    check();
}

void UpdateManager::check() {
    _timer->stop();
    const char* updater = readConfValue("updater", "");
    if (*updater != '\0') {
        if (strcmp(updater, "local") == 0) {
            const char* versionFile = readConfValue("version-file", "");
            if (*versionFile != '\0') {
                const char* version = readFile(const_cast<char*>(versionFile));
                if ((*version != '\0') && (strcmp(version, VERSION) != 0)) {
                    int res = QMessageBox::question(NULL, "d-jon update", "A new version of d-jon is available, do you want to update now?", QMessageBox::Yes, QMessageBox::No);
                    if (res == QMessageBox::Yes) {
                        QProcess* process = new QProcess();
                        process->start("updater.exe");
                        exit(0);
                    }
                }
            }
        } else {
            setLastError(6, "The updater type: %s is not supported", updater);
            _timer->stop();
        }
    } else {
        _timer->stop();
    }
    _timer->start(_mins * 60000);
}
