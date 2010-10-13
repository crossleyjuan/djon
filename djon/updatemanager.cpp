#include "updatemanager.h"
#include "util.h"
#include "config.h"
#include <QMessageBox>
#include <QProcess>
#ifdef WINDOWS
#include <windows.h>
#endif
#include <QUrl>
#include <QByteArray>

UpdateManager::UpdateManager(QObject *parent) :
        QObject(parent)
{
    _timer = new QTimer(this);
    _mins = 10;
    _updateDownloaded = false;
    _versionConfDownloaded = false;
    _httpRequestAborted = false;
    _http = new QHttp(this);
    _file = NULL;
    _downloading = false;
    connect(_timer, SIGNAL(timeout()), this, SLOT(check()));

    connect(_http, SIGNAL(requestFinished(int,bool)),
            this, SLOT(httpRequestFinished(int,bool)));
    connect(_http, SIGNAL(dataReadProgress(int,int)),
            this, SLOT(updateDataReadProgress(int,int)));
    connect(_http, SIGNAL(responseHeaderReceived(QHttpResponseHeader)),
            this, SLOT(readResponseHeader(QHttpResponseHeader)));
//    connect(http, SIGNAL(authenticationRequired(QString,quint16,QAuthenticator*)),
//            this, SLOT(slotAuthenticationRequired(QString,quint16,QAuthenticator*)));
}

void UpdateManager::startCheck(int mins) {
    _mins = mins;
    _timer->start(_mins * 60000);
    check();
}

void UpdateManager::downloadUpdater() {
//    const char*  = ;//"update_djon.exe";
    std::string fileName;
    const char* address;
    if (!_versionConfDownloaded) {
        fileName = string("version.conf");
        address = readConfValue("version-file", "");
    } else {
        fileName = string("update_djon.exe");
        address = readConfValue("updater-address", "");
    }
    fileName = *getTempDir() + "/" + fileName;
    _file = new QFile(QString(fileName.c_str()));

    QUrl url(address);
    QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
    _http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());
    _httpRequestAborted = false;

    QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
    if (path.isEmpty())
        path = "/";

    _httpGetId = _http->get(path, _file);
    _downloading = true;
}

void UpdateManager::httpRequestFinished(int requestId, bool error)
{
    if (requestId != _httpGetId)
        return;
    if (_httpRequestAborted) {
        if (_file) {
            _file->close();
            _file->remove();
            delete _file;
            _file = 0;
        }

        return;
    }

    _file->close();
    _downloading = false;
    if (error) {
        _file->remove();
    } else {
        delete _file;
        _file = 0;
        if (!_versionConfDownloaded) {
            _versionConfDownloaded = true;
        } else if (!_updateDownloaded) {
            _updateDownloaded = true;
        }
        processNextStep();
    }

}

void UpdateManager::readResponseHeader(const QHttpResponseHeader &responseHeader)
{
    switch (responseHeader.statusCode()) {
    case 200:                   // Ok
    case 301:                   // Moved Permanently
    case 302:                   // Found
    case 303:                   // See Other
    case 307:                   // Temporary Redirect
        // these are not error conditions
        break;

    default:
        _httpRequestAborted = true;
        _downloading = false;
        _http->abort();
    }
}

void UpdateManager::updateDataReadProgress(int bytesRead, int totalBytes)
{
    if (_httpRequestAborted)
        return;
}

void UpdateManager::check() {
    _timer->stop();
    const char* updater = readConfValue("updater", "");
    if (*updater != '/0') {
        processNextStep();
    } else {
        _timer->stop();
    }
    _timer->start(_mins * 60000);
}

void UpdateManager::processNextStep() {
    if (!_versionConfDownloaded || !_updateDownloaded) {
        downloadUpdater();
    } else {
        int res = QMessageBox::question(NULL, "d-jon update", "A new version of d-jon is available, do you want to update now?", QMessageBox::Yes, QMessageBox::No);
        if (res == QMessageBox::Yes) {
            char* cfile = "updater.exe";

#ifdef WINDOWS
            ShellExecuteA(NULL, "open", cfile, NULL, NULL, SW_SHOWNORMAL);
            exit(0);
#endif
        }
    }
}
