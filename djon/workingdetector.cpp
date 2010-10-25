#include "workingdetector.h"

WorkingDetector::WorkingDetector()
{
    _timer = new QTimer(this);
    _checkTime = 3*60*1000; // 3 minutes will wait until the start detection actually detects working
}

WorkingDetector::~WorkingDetector() {
    delete(_timer);
    _timer = 0;
}

void WorkingDetector::startDetection() {
    _timer->start(_checkTime);
    connect(_timer, SIGNAL(timeout()), this, SLOT(internalStart()));
}

void WorkingDetector::stopDetection() {
    disconnect(_timer, SIGNAL(timeout()), this, SLOT(checkIdle()));
    disconnect(_timer, SIGNAL(timeout()), this, SLOT(internalStart()));
    disconnect(_timer, SIGNAL(timeout()), this, SLOT(notify()));
}

void WorkingDetector::checkIdle() {
    long idlesecs = idleTime();
    if (idlesecs <= 1) {
        stopDetection();
        _lastDetected = new DateTime();
        connect(_timer, SIGNAL(timeout()), this, SLOT(notify()));
        _timer->start(2*60*1000);//Time to wait until notify
    }
}

void WorkingDetector::notify() {
    long idlesecs = idleTime();
    _timer->stop();
    disconnect(_timer, SIGNAL(timeout()), this, SLOT(notify()));
    if (idlesecs < 20) {
        emit workingDetected(*_lastDetected);
    } else {
        // The user stopped working again and everything should restart from the initial detection
        internalStart();
    }
}

void WorkingDetector::internalStart() {
    disconnect(_timer, SIGNAL(timeout()), this, SLOT(internalStart()));
    _timer->start(1000);
    connect(_timer, SIGNAL(timeout()), this, SLOT(checkIdle()));
}
