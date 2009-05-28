#include "taskelement.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include "utils.h"

TaskElement::TaskElement(Task* task, QWidget* parent) : QWidget(parent)
{
    m_task = task;
    setMaximumHeight(30);
    QGridLayout* lay = new QGridLayout();
    m_txtName = new QLineEdit(QString(task->shortDescription.c_str()));
    lay->addWidget(m_txtName, 0, 0);
    m_txtDuration = new QLineEdit(QString(toString(task->duration).c_str()));
    m_txtDuration->setMaximumWidth(40);
    m_txtDuration->setFocusPolicy(Qt::ClickFocus);
    lay->addWidget(m_txtDuration, 0, 1);
    setLayout(lay);
}

void TaskElement::paintEvent(QPaintEvent*) {
/*
    QPainter paint (this);
    paint.setPen(Qt::black);
    paint.setBrush(QColor(230, 230, 230));
    int margin = 5;
    paint.drawRect(margin, margin, this->width()-margin, this->height()-margin);

    paint.setFont(QFont("times", 10, QFont::Bold));
    paint.setPen(QColor(0, 0, 124));
    paint.drawText(20, 20, QString(m_task->shortDescription.c_str()));
*/
}

void TaskElement::mousePressEvent(QMouseEvent *event) {
    clicked(this);
}

Task* TaskElement::getTask() {
    return m_task;

}
