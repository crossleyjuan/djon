#include "systrayicon.h"
#include <sstream>


SysTrayIcon::SysTrayIcon(QObject *parent)
    : QSystemTrayIcon(parent)
{
    _timer= new QTimer();
    _currentIcon = 0;
    _running = false;
    connect(_timer, SIGNAL(timeout()), this, SLOT(timeout()));
    trackerStopped();
}

void SysTrayIcon::trackerStarted() {
    _timer->start(250);
    _running = true;
}

void SysTrayIcon::trackerStopped() {
    setIcon(QIcon(":/img/djon.png"));
    if (_running) {
        _timer->stop();
        _running = false;
    }
}

void SysTrayIcon::timeout() {
    _currentIcon++;
    if (_currentIcon > 6) {
        _currentIcon = 1;
    }
    std::stringstream ss;
    ss << ":/img/djon_" << _currentIcon << ".png";
    std::string file = ss.str();
    setIcon(QIcon(file.c_str()));
}
