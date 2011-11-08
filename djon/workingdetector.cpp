// *********************************************************************************************************************
// file:
// author: Juan Pablo Crossley (crossleyjuan@gmail.com)
// created:
// updated:
// license:
// 
// This file is part of the djon project, all the source code is licensed under the terms of the GPL license,
// the application and libraries are provided as-is and free of use under the terms explained in the file COPYING.GPL
// the authors are not responsible for any problem that this program, or any of its libraries, may cause.
// Its authors create this application in order to make the world a better place to live, but you should use it on
// your own risks.
// 
// Also, be adviced that, the GPL license force the committers to ensure this application will be free of use, thus
// if you do any modification you will be required to provide it for free unless you use it for personal use (you may 
// charge yourself if you want), bare in mind that you will be required to provide a copy of the license terms that ensures
// this program will be open sourced and all its derivated work will be too.
// *********************************************************************************************************************

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
