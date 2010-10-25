#ifndef DURATIONCONTROL_H
#define DURATIONCONTROL_H

#include <QLineEdit>
#include "duration.h"

class DurationControl : public QLineEdit
{
    Q_OBJECT
public:
    explicit DurationControl(QWidget* parent=0);
    explicit DurationControl(QString &text, QWidget* parent=0);
    QString text() const;
    Duration duration();

signals:

public slots:
    void setText(QString text);
    void setDuration(const Duration duration);

private:
    Duration _duration;
};

#endif // DURATIONCONTROL_H
