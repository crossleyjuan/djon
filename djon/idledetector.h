#ifndef IDLEDETECTOR_H
#define IDLEDETECTOR_H

#include "util.h"
#include <QTimer>
#include <QObject>

class IdleDetector : public QObject
{
   Q_OBJECT

public:
    IdleDetector();
    void start();
    void stop();
    DateTime* idleSince();

signals:
    void idleTimeOut();

private:
    int m_idleMaxSecs;
    QTimer* timer;
    DateTime* _idleSince;

public slots:
    void refreshIdleMaxSecs();

private slots:
    void timeout();

};

#endif // IDLEDETECTOR_H
