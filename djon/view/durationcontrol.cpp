#include "durationcontrol.h"

DurationControl::DurationControl(QWidget* parent) :
    QLineEdit(parent)
{
//    setInputMask(tr("00d00h00m"));
}

DurationControl::DurationControl(const QString &text, QWidget* parent) :
        QLineEdit(text, parent)
{
//    setInputMask(tr("00d00h00m"));
}
