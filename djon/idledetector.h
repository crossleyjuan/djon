#ifndef IDLEDETECTOR_H
#define IDLEDETECTOR_H

#include <QTimer>
#include <QObject>

class IdleDetector : public QObject
{
   Q_OBJECT

public:
    IdleDetector();
    void start();
    void stop();

signals:
    void idleTimeOut();

private:
    int m_idleMaxSecs;
    QTimer* timer;

public slots:
    void refreshIdleMaxSecs();

private slots:
    void timeout();

};

#endif // IDLEDETECTOR_H
