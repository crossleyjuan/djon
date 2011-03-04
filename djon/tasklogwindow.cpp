#include <QtGui>
#include "tasklogwindow.h"
#include "ui_tasklogwindow.h"
#include "data.h"
#include "util.h"

#include "tasklogmodel.h"
#include "tasklogdelegate.h"
#include "timetracker.h"

TaskLogWindow::TaskLogWindow(TimeTracker* tracker, QWidget *parent) :
        QDockWidget(parent),
        m_ui(new Ui::TaskLogWindow)
{
    m_ui->setupUi(this);
    _tracker = tracker;
    _model = NULL;
    QItemEditorFactory* factory = new QItemEditorFactory();
    QItemEditorCreatorBase *textEditorCreator = new QStandardItemEditorCreator<QLineEdit>();
    factory->registerEditor(QVariant::String, textEditorCreator);

    QItemEditorCreatorBase *dateEditCreator = new QStandardItemEditorCreator<QDateTimeEdit>();
    factory->registerEditor(QVariant::DateTime, dateEditCreator);

    QItemEditorFactory::setDefaultFactory(factory);
    setupMenuActions();
    m_ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(m_ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequested(QPoint)));
}

TaskLogWindow::~TaskLogWindow()
{
    if (_model != NULL) {
        delete _model;
    }
    delete m_ui;
}

void TaskLogWindow::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TaskLogWindow::refresh(Task* task) {
    _task = task;
    if (_model != NULL) {
        delete(_model);
    }
    _model = new TaskLogModel(task);
    m_ui->tableView->setModel(_model);
    m_ui->tableView->setItemDelegate(new TaskLogDelegate());
    m_ui->tableView->setColumnWidth(0, 300);
    m_ui->tableView->setColumnWidth(1, 150);
    m_ui->tableView->setColumnWidth(2, 150);
    connect(_model, SIGNAL(timeChanged(Task*)), this, SLOT(logTimeChanged(Task*)));

    string sort(readPreference("log-sort", ""));
    if (sort.length() > 0) {
        std::vector<string> options = split(sort, "++");
        int column = atoi(options.at(0).c_str());
        int order = atoi(options.at(1).c_str());
        Qt::SortOrder sortOrder = (order == 0) ? Qt::AscendingOrder: Qt::DescendingOrder;
        m_ui->tableView->sortByColumn(column, sortOrder);
    }
}

void TaskLogWindow::setupMenuActions() {
    QAction* deleteTaskLog = _popUpMenu.addAction(QIcon(":/img/delete-task.png"), tr("Delete Task Log"));
    connect(deleteTaskLog, SIGNAL(triggered()), this, SLOT(deleteSelectedLogs()));
}

void TaskLogWindow::deleteSelectedLogs() {
    QMessageBox confBox(this);
    confBox.setWindowTitle("d-jon");
    confBox.setText("The selected logs will be deleted. Are you sure?");
    confBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int confRes = confBox.exec();

    if (confRes == QMessageBox::Yes) {
        QModelIndexList list = m_ui->tableView->selectionModel()->selectedIndexes();
        for (int x = 0; x < list.count(); x++) {
            QModelIndex index = list.at(x);
            if (!index.isValid()) {
                continue;
            }
            // Without this validation the selection depends on how many columns the view has, and will execute for each column
            if (index.column() == 0) {
                TaskLog* log = (TaskLog*)index.internalPointer();
                if (log != NULL) {
                    qDebug("Removing log id: %s", log->id->c_str());
                    _task->removeLog(log);
                    int res = saveProject(_task->project());
                    if (res != 0) {
                        const char* err = lastErrorDescription();
                        QMessageBox box;
                        box.setWindowTitle(tr("d-jon"));
                        box.setText(QString(err));
                        box.exec();
                        return;
                    }
                }
            }
        }
        refresh(_task);
        emit timeChanged(_task);
    }
}

void TaskLogWindow::contextMenuRequested(QPoint pos) {
    QModelIndex index = m_ui->tableView->indexAt(pos);
    if (index.isValid()) {
        TaskLog* log = (TaskLog*)index.internalPointer();
        if ((log != NULL) && (_tracker->status() == RUNNING)) {
            // If the log is the one that is being tracked the delete menu should not be shown
            if (log->id->compare(*_tracker->taskLog()->id) == 0) {
                return;
            }
        }
        _popUpMenu.popup(QCursor::pos());
    }
}

void TaskLogWindow::logTimeChanged(Task *task) {
    emit timeChanged(task);
}
