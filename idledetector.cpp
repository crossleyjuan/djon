#include "idledetector.h"
#include "config.h"
#include "util.h"

#ifndef WINDOWS

#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/scrnsaver.h>

#else
#include <Windows.h>
#include <Winuser.h>
#endif

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
    long idlesecs;
#ifndef WINDOWS
    bool _idleDetectionPossible;
    XScreenSaverInfo *_mit_info;

    int event_base, error_base;
    if(XScreenSaverQueryExtension(QX11Info::display(), &event_base, &error_base))
        _idleDetectionPossible = true;
    else
        _idleDetectionPossible = false;
    _mit_info = XScreenSaverAllocInfo();

    XScreenSaverQueryInfo(QX11Info::display(), QX11Info::appRootWindow(), _mit_info);

    idlesecs = (_mit_info->idle/1000);

#else

    LASTINPUTINFO lif;
    lif.cbSize = sizeof(LASTINPUTINFO);
    GetLastInputInfo(&lif);
    DWORD tickCount = GetTickCount();
    idlesecs = (tickCount - lif.dwTime) / 1000;

#endif
    if (idlesecs > m_idleMaxSecs) {
        timer->stop();
        idleTimeOut();
    }
}

void IdleDetector::refreshIdleMaxSecs() {
    m_idleMaxSecs = atoi(readConfValue("idle-timeout", "300"));
}
