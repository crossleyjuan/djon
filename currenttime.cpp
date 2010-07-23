#include "currenttime.h"
#include "ui_currenttime.h"
#include "TaskModel.h"
#include "data.h"
#include "taskcombobox.h"
#include <QtGui>

class TreeView : public QTreeView {
public:
    TreeView(QWidget* parent = 0) : QTreeView(parent) {
    }

    QSize minimumSizeHint() const {
        return QSize(100, 100);
    }

    QSize sizeHint() const {
        return QSize(100, 100);
    }

};
/*
class TreeComboBox : public QComboBox
{
public:
    TreeComboBox(QWidget* parent = 0) : QComboBox(parent), skipNextHide(false)
    {
        TreeView* v = new TreeView(this);
        setView(v);
        this->setMaxVisibleItems(100);
        v->header()->setFixedHeight(0);
        //v->header()->hide();
        v->viewport()->installEventFilter(this);
    }

    bool eventFilter(QObject* object, QEvent* event)
    {
        if (event->type() == QEvent::MouseButtonPress && object == view()->viewport())
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QModelIndex index = view()->indexAt(mouseEvent->pos());
            if (!view()->visualRect(index).contains(mouseEvent->pos()))
                skipNextHide = true;
        }
        return false;
    }

    virtual void showPopup()
    {
        setRootModelIndex(QModelIndex());
        QComboBox::showPopup();
    }

    virtual void hidePopup()
    {
        setRootModelIndex(view()->currentIndex().parent());
        setCurrentIndex(view()->currentIndex().row());
        if (skipNextHide)
            skipNextHide = false;
        else
            QComboBox::hidePopup();
    }

private:
    bool skipNextHide;
};
*/

CurrentTime::CurrentTime(std::vector<Project*>* projects, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::CurrentTime)
{
    ui->setupUi(this);
    _projects = projects;

    TaskModel* model = new TaskModel(*_projects, this);
    ui->comboBox->setModel(model);
}

CurrentTime::~CurrentTime()
{
    delete ui;
}

void CurrentTime::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void CurrentTime::updateTime(DTime& tm) {
    ui->timeEdit->setTime(*tm.toQTime());
}

void CurrentTime::setActiveTask(Task* activeTask) {
    _activeTask = activeTask;
//    ui->taskt->setText(activeTask->shortDescription());
}
