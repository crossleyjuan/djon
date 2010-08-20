#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <QObject>
#include <QTimer>

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
private:
    QTimer* _timer;
    int _mins;
};

#endif // UPDATEMANAGER_H
