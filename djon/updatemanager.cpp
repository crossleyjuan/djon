#include "updatemanager.h"
#include "util.h"
#include "config.h"
#include "data.h"
#include <QMessageBox>
#include <QProcess>
#ifdef WINDOWS
#include <windows.h>
#endif
#include <QUrl>
#include <QByteArray>
#include <string>
#include <sstream>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QImageReader>

UpdateManager::UpdateManager(QObject *parent) :
        QObject(parent)
{
    _timer = new QTimer(this);
    _updateDownloaded = false;
    _versionConfDownloaded = false;
    _manager = NULL;
    _file = NULL;
    _mins = getSettings()->checkUpdate();
    _downloading = false;
    _isLastVersion = false;
    _manager = NULL;

    connect(_timer, SIGNAL(timeout()), this, SLOT(check()));

    _manager = new QNetworkAccessManager(this);
    connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
}

UpdateManager::~UpdateManager() {
    if (_manager != NULL) {
        delete(_manager);
    }
    _manager = NULL;
}

void UpdateManager::startCheck() {
    // zero will deactivate the check
    if (_mins > 0) {
        _timer->start(_mins * 60000);
        check();
    }
}

void UpdateManager::downloadUpdater() {
    std::string fileName;
    std::string address;
    if (!_versionConfDownloaded) {
        fileName = string("version.conf");
        std::string currentVersion = getCurrentVersion();
        string userId(readConfValue("user-id", ""));
        address = "http://d-jon.com/downloads/version.php?version=" + currentVersion;// readConfValue("version-file", "");
        address = address.append("&userId=" + userId);
    }
    fileName = *getTempDir() + "/" + fileName;
    _file = new QFile(QString(fileName.c_str()));

    if (_file->exists()) {
        _file->remove();
    }
    QUrl url(address.c_str());

    _manager->get(QNetworkRequest(url));

    _downloading = true;
}

void UpdateManager::requestFinished(QNetworkReply *reply)
{
    // no error received?
    bool error = false;
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  // bytes
        _file->open(QFile::WriteOnly);
        int res = _file->write(bytes);
        if (res < 0) {
            error = true;
        }
        _file->flush();
        _file->close();
    }
    // Some http error received
    else
    {
        // handle errors here
        error = true;
    }

    _downloading = false;

    if (error) {
        _file->remove();
        delete(_file);
    } else {
        if (!_versionConfDownloaded) {
            _versionConfDownloaded = true;
            checkVersion();
        }
    }

    // We receive ownership of the reply object
    // and therefore need to handle deletion.
    reply->deleteLater();
}

void UpdateManager::check() {
    if (_downloading) {
        return;
    }

    if (_isLastVersion) {
        // only reset these values if the last check returned that this is
        // the last version
        _isLastVersion = false;
        _versionConfDownloaded = false;
        _updateDownloaded = false;
    }

    _timer->stop();
    processNextStep();
    _timer->start(_mins * 60000);
}

void UpdateManager::processNextStep() {
    if (_isLastVersion) {
        return;
    }
    if (!_versionConfDownloaded) {
        downloadUpdater();
    }
}

void UpdateManager::checkVersion() {
    std::string tempFileName = *getTempDir() + "/version.conf";
    _isLastVersion = true;
    if (existFile(tempFileName.c_str())) {
        std::string versioncont = std::string(readFile(const_cast<char*>(tempFileName.c_str())));

        if (versioncont.find("version:") == -1) {
            versioncont = "version:" + versioncont + ";\nreleaseNotesURL:http://d-jon.com/downloads/releasenotes.php?version=" + VERSION + ";";
        }
        const char* cwebLastVersion = readValue(versioncont, "version");
        if (strlen(cwebLastVersion) > 0) {
            Version webVersion = getVersion(cwebLastVersion);
            Version currentVersion = getCurrentVersion();

            if (webVersion > currentVersion) {
                _isLastVersion = false;
            }
            if (!_isLastVersion) {
                QMessageBox box;
                box.setWindowTitle("d-jon update available");
                box.setTextFormat(Qt::RichText);
                std::stringstream ss;
                ss << "A new version of d-jon is available at <a href=\"http://d-jon.com/downloads.html\">http://d-jon.com/downloads.html</a><br/>";
                ss << "please take a look of the <a href=\"";
                std::string releaseNotesURL = readValue(versioncont, "releaseNotesURL");
                ss << releaseNotesURL << "\">release notes</a> to know what is new in this release.";
                std::string message = ss.str();
                box.setText(message.c_str());
                box.exec();
            }
        }
        _file->remove();
        delete(_file);
        _file = NULL;
    }
}

void UpdateManager::pause() {
    _timer->stop();
}

void UpdateManager::resume() {
    startCheck();
}
