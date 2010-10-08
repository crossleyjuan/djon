#ifndef SYSTRAYICON_H
#define SYSTRAYICON_H

#include <QSystemTrayIcon>
#include <QTimer>

class SysTrayIcon: public QSystemTrayIcon
{
Q_OBJECT
public:
    SysTrayIcon(QObject *parent = 0);

public slots:
    void trackerStarted();
    void trackerStopped();
    void timeout();

private:
    int _currentIcon;
    bool _running;
    QTimer* _timer;
};

#endif // SYSTRAYICON_H
