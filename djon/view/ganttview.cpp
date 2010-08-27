#include "ganttview.h"
#include "TaskModel.h"

GanttView::GanttView(QWidget *parent) :
    QWidget(parent)
{
    QLayout* lay = new QVBoxLayout();
    _model = NULL;
    _ganttScene = NULL;
    _headerScene = NULL;
    _headerView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _headerView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(_ganttView.horizontalScrollBar(), SIGNAL(valueChanged(int)), _headerView.horizontalScrollBar(), SLOT(setValue(int)));
    lay->addWidget(&_headerView);
    lay->addWidget(&_ganttView);
    lay->setSpacing(0);
    lay->setContentsMargins(0, 0, 0, 0);
    setLayout(lay);
}

void GanttView::createHeader() {
    _headerScene->clear();

    QSize size = headerSizeHint();

    QLinearGradient grad(0, 0, 0, size.height());
    int dark = 210;
    grad.setColorAt(1, QColor(dark, dark, dark));
    grad.setColorAt(0.7, QColor(dark + 20, dark + 20, dark + 20));
    grad.setColorAt(0, Qt::white);
    QBrush background(grad);
    QPen pen(QColor(220, 220, 220));
    QSize sizeHint = _ganttScene->viewSizeHint();
    _headerScene->addRect(0, 0, sizeHint.width(), size.height() + 1, pen, background)->setZValue(0);

    int textSize = 30;
    int margin = 15;
    int columnSize = 45;

    QDateTime startDate = *_ganttScene->startDate().addDays(-1).toQDateTime();
    for (int x = 0; x < _ganttScene->totalDays(); x++) {
        QPen textPen(Qt::black);
        QGraphicsSimpleTextItem* text = _headerScene->addSimpleText(startDate.toString("dd-MMM"), QFont("Arial", 8));
        text->setPos(x * 45 + 2, 3);
        text->setVisible(true);
//        text->setPen(textPen);
        text->setZValue(1);
        startDate = startDate.addDays(1);
    }
    _headerView.setMaximumHeight(headerSizeHint().height());
}

QSize GanttView::headerSizeHint() {
    QVariant vSize = _model->headerData(0, Qt::Vertical, Qt::SizeHintRole);
    QSize sizeHint = QSize(10, 19);
    if (vSize.canConvert<QSize>()) {
        sizeHint = qvariant_cast<QSize>(vSize);
    }
    return sizeHint;
}


void GanttView::setModel(TaskModel *taskModel) {
    _model = taskModel;
    _ganttScene = new GanttScene(this);
    _headerScene = new QGraphicsScene(this);
    _headerView.setScene(_headerScene);
    _headerView.setAlignment(Qt::AlignLeft | Qt::AlignTop);
    _ganttView.setScene(_ganttScene);
    _ganttView.setAlignment(Qt::AlignLeft | Qt::AlignTop);

    _ganttScene->setModel(_model);
    createHeader();
}

QScrollBar* GanttView::horizontalScrollBar() {
    return _ganttView.horizontalScrollBar();
}

QScrollBar* GanttView::verticalScrollBar() {
    return _ganttView.verticalScrollBar();
}

void GanttView::collapse(const QModelIndex& index) {
    _ganttScene->collapse(index);
}

void GanttView::expand(const QModelIndex& index) {
    _ganttScene->expand(index);
}

void GanttView::refresh() {
    _ganttScene->refresh();
    createHeader();
}
