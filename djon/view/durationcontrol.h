#ifndef DURATIONCONTROL_H
#define DURATIONCONTROL_H

#include <QLineEdit>

class DurationControl : public QLineEdit
{
    Q_OBJECT
public:
    explicit DurationControl(QWidget* parent=0);
    explicit DurationControl(const QString &, QWidget* parent=0);
signals:

public slots:

};

#endif // DURATIONCONTROL_H
