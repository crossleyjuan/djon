#include "config.h"
#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QFile>

class UpdateManager : public QObject
{
    Q_OBJECT
public:
    explicit UpdateManager(QObject *parent = 0);
    ~UpdateManager();
    void startCheck();
    void pause();
    void resume();

signals:
    void versionFound();

public slots:
    void check();
    void requestFinished(QNetworkReply* reply);

private:
    void downloadUpdater();
    void processNextStep();
    void checkVersion();

    QTimer* _timer;
    int _mins;
    QNetworkAccessManager* _manager;
    bool _downloading;
    int _httpGetId;
    QFile* _file;
    bool _isLastVersion;

    bool _versionConfDownloaded;
    bool _updateDownloaded;
};

#endif // UPDATEMANAGER_H
