#include "durationcontrol.h"
#include <sstream>
#include <string>

DurationControl::DurationControl(QWidget* parent) :
    QLineEdit(parent)
{
}

DurationControl::DurationControl(QString &text, QWidget* parent) :
    QLineEdit(text, parent)
{
}

QString DurationControl::text() const {
    QString text = QLineEdit::text();
    return text;
}

void DurationControl::setText(QString text) {
    QLineEdit::setText(text);
}


Duration DurationControl::duration() {
    _duration = Duration(QLineEdit::text().toStdString());
    return _duration;
}


void DurationControl::setDuration(const Duration duration) {
    std::stringstream ss;
    _duration = duration;
    if (_duration.days() > 0) {
        ss << _duration.days() << "d";
    }
    if (_duration.hours() > 0) {
        ss << _duration.hours() << "h";
    }
    if (_duration.minutes() > 0) {
        ss << _duration.minutes() << "m";
    }
    std::string dur = ss.str();
    setText(QString(dur.c_str()));
}
