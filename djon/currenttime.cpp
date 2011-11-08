// *********************************************************************************************************************
// file:
// author: Juan Pablo Crossley (crossleyjuan@gmail.com)
// created:
// updated:
// license:
// 
// This file is part of the djon project, all the source code is licensed under the terms of the GPL license,
// the application and libraries are provided as-is and free of use under the terms explained in the file COPYING.GPL
// the authors are not responsible for any problem that this program, or any of its libraries, may cause.
// Its authors create this application in order to make the world a better place to live, but you should use it on
// your own risks.
// 
// Also, be adviced that, the GPL license force the committers to ensure this application will be free of use, thus
// if you do any modification you will be required to provide it for free unless you use it for personal use (you may 
// charge yourself if you want), bare in mind that you will be required to provide a copy of the license terms that ensures
// this program will be open sourced and all its derivated work will be too.
// *********************************************************************************************************************

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

void CurrentTime::updateTime(Task* task, DTime& currentLogTime, DTime& totalTaskTime) {
    ui->currentTime->setText(QString(currentLogTime.toChar()));
    ui->totalTime->setText(QString(totalTaskTime.toChar()));
}

void CurrentTime::setActiveTask(Task* activeTask) {
    _activeTask = activeTask;
    if (_activeTask != NULL) {
    //    ui->comboBox->setCurrentIndex(3);
        string description = *activeTask->shortDescription();
        Task* tsk = activeTask->parent();
        while (tsk != NULL) {
            description.insert(0, *tsk->shortDescription() + " / ");
            tsk = tsk->parent();
        }
        Project* proj = activeTask->project();
        description.insert(0, *proj->name() + " / ");

        ui->currentTask->setText(QString(description.c_str()));
        DTime totalTime = _activeTask->totalTime();
        DTime tm;
        ui->totalTime->setText(totalTime.toChar());
        ui->currentTime->setText(tm.toChar());
    } else {
        ui->currentTask->setText(tr(""));
        ui->totalTime->setText(tr(""));
        ui->currentTime->setText(tr(""));
    }
}
