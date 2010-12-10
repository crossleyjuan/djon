#include "logview.h"
#include "TaskModel.h"

LogView::LogView(QWidget *parent) :
    QWidget(parent)
{
    QLayout* lay = new QVBoxLayout();
    _model = NULL;
    _logScene = NULL;
    _headerScene = NULL;
    _headerView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _headerView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _todayPos = 0;
    connect(_logView.horizontalScrollBar(), SIGNAL(valueChanged(int)), _headerView.horizontalScrollBar(), SLOT(setValue(int)));
    lay->addWidget(&_headerView);
    lay->addWidget(&_logView);
    lay->setSpacing(0);
    lay->setContentsMargins(0, 0, 0, 0);
    setLayout(lay);
}

void LogView::createHeader() {
    _headerScene->clear();

    QSize size = headerSizeHint();

    QLinearGradient grad(0, 0, 0, size.height());
    int dark = 210;
    grad.setColorAt(1, QColor(dark, dark, dark));
    grad.setColorAt(0.7, QColor(dark + 20, dark + 20, dark + 20));
    grad.setColorAt(0, Qt::white);
    QBrush background(grad);
    QPen pen(QColor(220, 220, 220));
    QSize sizeHint = _logScene->viewSizeHint();
    _headerScene->addRect(0, 0, sizeHint.width() + verticalScrollBar()->width() + 200, size.height() + 1, pen, background)->setZValue(0);

    int textSize = 30;
    int margin = 15;
    int columnSize = 45;

    DateTime startDate = _logScene->startDate().addDays(-1);
    DateTime today;
    today.setHour(0);
    today.setMin(0);
    today.setSecs(0);
    for (int x = 0; x < _logScene->totalDays(); x++) {
        QPen textPen(Qt::black);
        QGraphicsSimpleTextItem* text = _headerScene->addSimpleText(startDate.addDays(1).toQDateTime().toString("dd-MMM"), QFont("Arial", 8));
        text->setPos(x * 45 + 2, 3);
        text->setVisible(true);
//        text->setPen(textPen);
        text->setZValue(1);
        if (startDate == today) {
            _todayPos = (x * 45 + 2);
        }
        startDate = startDate.addDays(1);
    }
    _headerView.setMaximumHeight(headerSizeHint().height());
}

QSize LogView::headerSizeHint() {
    QVariant vSize = _model->headerData(0, Qt::Vertical, Qt::SizeHintRole);
    QSize sizeHint = QSize(10, 19);
    if (vSize.canConvert<QSize>()) {
        sizeHint = qvariant_cast<QSize>(vSize);
    }
    return sizeHint;
}


void LogView::setModel(TaskModel *taskModel) {
    _model = taskModel;
    _logScene = new LogScene(this);
    _headerScene = new QGraphicsScene(this);
    _headerView.setScene(_headerScene);
    _headerView.setAlignment(Qt::AlignLeft | Qt::AlignTop);
    _logView.setScene(_logScene);
    _logView.setAlignment(Qt::AlignLeft | Qt::AlignTop);

    _logScene->setModel(_model);
    createHeader();
}

QScrollBar* LogView::horizontalScrollBar() {
    return _logView.horizontalScrollBar();
}

QScrollBar* LogView::verticalScrollBar() {
    return _logView.verticalScrollBar();
}

void LogView::collapse(const QModelIndex& index) {
    _logScene->collapse(index);
}

void LogView::expand(const QModelIndex& index) {
    _logScene->expand(index);
}

void LogView::refresh() {
    _logScene->refresh();
    createHeader();
}

void LogView::scrollToday() {
    if (_todayPos > 0) {
        QPoint currentPos = this->_logView.pos();
        currentPos.setX(_todayPos);
        this->_logView.centerOn(currentPos);
    }
}
