#ifndef WORKINGDETECTOR_H
#define WORKINGDETECTOR_H

#include <QObject>
#include <QTimer>

#include "util.h"

class WorkingDetector : public QObject
{
    Q_OBJECT
public:
    WorkingDetector();
    ~WorkingDetector();

public slots:
    void startDetection();
    void stopDetection();
private slots:
    void internalStart();
    void checkIdle();
    void notify();

signals:
    void workingDetected(const DateTime since);

private:
    DateTime* _lastDetected;
    QTimer* _timer;
    long _checkTime; // this will be the time used to avoid invalid idle detections
};

#endif // WORKINGDETECTOR_H
