#include "config.h"
#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <QObject>
#include <QTimer>
#include <QHttp>
#include <QHttpResponseHeader>
#include <QFile>

class UpdateManager : public QObject
{
    Q_OBJECT
public:
    explicit UpdateManager(QObject *parent = 0);
    void startCheck(int mins);

signals:
    void versionFound();

public slots:
    void check();
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);
    void updateDataReadProgress(int bytesRead, int totalBytes);

private:
    void downloadUpdater();
    void processNextStep();

    QTimer* _timer;
    int _mins;
    bool _httpRequestAborted;
    bool _downloading;
    QHttp* _http;
    int _httpGetId;
    QFile* _file;

    bool _versionConfDownloaded;
    bool _updateDownloaded;
};

#endif // UPDATEMANAGER_H
