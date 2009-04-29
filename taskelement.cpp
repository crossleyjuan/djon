#include "taskelement.h"
#include <QHBoxLayout>
#include <QLabel>

TaskElement::TaskElement(Task* task, QWidget* parent) : QWidget(parent)
{
    m_task = task;
}

QColor getTaskColor() {
    return Qt::red;
}

void TaskElement::paintEvent(QPaintEvent*) {
    QPainter paint (this);
    paint.setPen(Qt::black);
    paint.setBrush(QColor(Qt::white));
    int margin = 5;
    paint.drawRect(margin, margin, this->width()-margin, this->height()-margin);

    paint.setFont(QFont("times", 10, QFont::Bold));
    paint.setPen(getTaskColor());
    paint.drawText(20, 20, QString(m_task->name.c_str()));
}
