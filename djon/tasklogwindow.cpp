#include <QtGui>
#include "tasklogwindow.h"
#include "ui_tasklogwindow.h"
#include "data.h"

#include "tasklogmodel.h"
#include "tasklogdelegate.h"

TaskLogWindow::TaskLogWindow(QWidget *parent) :
    QDockWidget(parent),
    m_ui(new Ui::TaskLogWindow)
{
    m_ui->setupUi(this);
    QItemEditorFactory* factory = new QItemEditorFactory();
    QItemEditorCreatorBase *textEditorCreator = new QStandardItemEditorCreator<QLineEdit>();
    factory->registerEditor(QVariant::String, textEditorCreator);

    QItemEditorCreatorBase *dateEditCreator = new QStandardItemEditorCreator<QDateTimeEdit>();
    factory->registerEditor(QVariant::DateTime, dateEditCreator);

    QItemEditorFactory::setDefaultFactory(factory);
}

TaskLogWindow::~TaskLogWindow()
{
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
    TaskLogModel* model = new TaskLogModel(task);
    m_ui->tableView->setModel(model);
    m_ui->tableView->setItemDelegate(new TaskLogDelegate());
    m_ui->tableView->setColumnWidth(0, 300);
    m_ui->tableView->setColumnWidth(1, 150);
    m_ui->tableView->setColumnWidth(2, 150);

    string sort(readPreference("log-sort", ""));
    if (sort.length() > 0) {
        std::vector<string*>* options = split(sort, "++");
        int column = atoi(options->at(0)->c_str());
        int order = atoi(options->at(1)->c_str());
        Qt::SortOrder sortOrder = (order == 0) ? Qt::AscendingOrder: Qt::DescendingOrder;
        m_ui->tableView->sortByColumn(column, sortOrder);
    }
}

