#include "idledetector.h"
#include "config.h"
#include "util.h"
#include "data.h"
#include <stdlib.h>

IdleDetector::IdleDetector()
{
    refreshIdleMaxSecs();
    timer = new QTimer();
    _idleSince = NULL;
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
        QDateTime current = QDateTime::currentDateTime();
        current = current.addSecs(idlesecs * -1);
        DateTime since(current);
        _idleSince = new DateTime(since);
        timer->stop();
        emit idleTimeOut();
    }
}

void IdleDetector::refreshIdleMaxSecs() {
    m_idleMaxSecs = getSettings()->idleTimeOut();
}

DateTime* IdleDetector::idleSince() {
    if (_idleSince != NULL) {
        return _idleSince;
    } else {
        return new DateTime(DateTime::today(true));
    }
}
