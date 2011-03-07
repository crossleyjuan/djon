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

UpdateManager::UpdateManager(QObject *parent) :
        QObject(parent)
{
    _timer = new QTimer(this);
    _updateDownloaded = false;
    _versionConfDownloaded = false;
    _httpRequestAborted = false;
    _http = new QHttp(this);
    _file = NULL;
    _mins = getSettings()->checkUpdate();
    _downloading = false;
    _isLastVersion = false;
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
        address = "http://d-jon.com/downloads/version.php?version=" + currentVersion;// readConfValue("version-file", "");
    }
    fileName = *getTempDir() + "/" + fileName;
    _file = new QFile(QString(fileName.c_str()));

    QUrl url(address.c_str());
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
            checkVersion();
        }
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
    std::string versioncont = std::string(readFile(const_cast<char*>(std::string(*getTempDir() + "/version.conf").c_str())));

    if (versioncont.find("version:") == -1) {
        versioncont = "version:" + versioncont + ";\nreleaseNotesURL:http://d-jon.com/downloads/releasenotes.php?version=" + VERSION + ";";
    }
    const char* cwebLastVersion = readValue(versioncont, "version");

    Version webVersion = getVersion(cwebLastVersion);
    Version currentVersion = getCurrentVersion();

    if (webVersion > currentVersion) {
        _isLastVersion = false;
    } else {
        _isLastVersion = true;
    }
    if (true || !_isLastVersion) {
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

void UpdateManager::pause() {
    _timer->stop();
}

void UpdateManager::resume() {
    startCheck();
}
