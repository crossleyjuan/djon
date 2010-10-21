#include "idledetector.h"
#include "config.h"
#include "util.h"
#include <stdlib.h>

IdleDetector::IdleDetector()
{
    refreshIdleMaxSecs();
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void IdleDetector::start() {
    timer->start(1000);
}

void IdleDetector::stop() {
    timer->stop();
}

void IdleDetector::timeout() {
    long idlesecs = idleTime();
    if (idlesecs > m_idleMaxSecs) {
        timer->stop();
        emit idleTimeOut();
    }
}

void IdleDetector::refreshIdleMaxSecs() {
    m_idleMaxSecs = atoi(readConfValue("idle-timeout", "300"));
}
