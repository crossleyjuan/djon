#include "taskelement.h"
#include <QHBoxLayout>
#include <QLabel>

TaskElement::TaskElement(Task* task, QWidget* parent) : QWidget(parent)
{
    m_task = task;
    setMaximumHeight(30);
}

void TaskElement::paintEvent(QPaintEvent*) {

    QPainter paint (this);
    paint.setPen(Qt::black);
    paint.setBrush(QColor(230, 230, 230));
    int margin = 5;
    paint.drawRect(margin, margin, this->width()-margin, this->height()-margin);

    paint.setFont(QFont("times", 10, QFont::Bold));
    paint.setPen(QColor(0, 0, 124));
    paint.drawText(20, 20, QString(m_task->shortDescription.c_str()));

}

void TaskElement::mousePressEvent(QMouseEvent *event) {
    clicked(this);
}

Task* TaskElement::getTask() {
    return m_task;

}
