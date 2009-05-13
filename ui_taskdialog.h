/********************************************************************************
** Form generated from reading ui file 'taskdialog.ui'
**
** Created: Wed May 13 06:37:37 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TASKDIALOG_H
#define UI_TASKDIALOG_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDateEdit>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TaskDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *shortDescription;
    QLabel *label_2;
    QTextEdit *description;
    QLabel *label_3;
    QLineEdit *duration;
    QLabel *label_4;
    QDateEdit *startDate;
    QLabel *label_5;
    QDateEdit *endDate;
    QLabel *label_6;
    QComboBox *status;

    void setupUi(QDialog *TaskDialog)
    {
        if (TaskDialog->objectName().isEmpty())
            TaskDialog->setObjectName(QString::fromUtf8("TaskDialog"));
        TaskDialog->resize(502, 445);
        buttonBox = new QDialogButtonBox(TaskDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(150, 400, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        formLayoutWidget = new QWidget(TaskDialog);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 10, 481, 371));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(formLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        shortDescription = new QLineEdit(formLayoutWidget);
        shortDescription->setObjectName(QString::fromUtf8("shortDescription"));

        formLayout->setWidget(0, QFormLayout::FieldRole, shortDescription);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        description = new QTextEdit(formLayoutWidget);
        description->setObjectName(QString::fromUtf8("description"));

        formLayout->setWidget(1, QFormLayout::FieldRole, description);

        label_3 = new QLabel(formLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        duration = new QLineEdit(formLayoutWidget);
        duration->setObjectName(QString::fromUtf8("duration"));

        formLayout->setWidget(2, QFormLayout::FieldRole, duration);

        label_4 = new QLabel(formLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        startDate = new QDateEdit(formLayoutWidget);
        startDate->setObjectName(QString::fromUtf8("startDate"));
        startDate->setCalendarPopup(true);
        startDate->setDate(QDate(2009, 4, 20));

        formLayout->setWidget(3, QFormLayout::FieldRole, startDate);

        label_5 = new QLabel(formLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_5);

        endDate = new QDateEdit(formLayoutWidget);
        endDate->setObjectName(QString::fromUtf8("endDate"));
        endDate->setCalendarPopup(true);

        formLayout->setWidget(4, QFormLayout::FieldRole, endDate);

        label_6 = new QLabel(formLayoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_6);

        status = new QComboBox(formLayoutWidget);
        status->setObjectName(QString::fromUtf8("status"));

        formLayout->setWidget(5, QFormLayout::FieldRole, status);


        retranslateUi(TaskDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), TaskDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), TaskDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(TaskDialog);
    } // setupUi

    void retranslateUi(QDialog *TaskDialog)
    {
        TaskDialog->setWindowTitle(QApplication::translate("TaskDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TaskDialog", "Short", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("TaskDialog", "Description", 0, QApplication::UnicodeUTF8));
        description->setHtml(QApplication::translate("TaskDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("TaskDialog", "Duration", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("TaskDialog", "Start Date", 0, QApplication::UnicodeUTF8));
        startDate->setDisplayFormat(QApplication::translate("TaskDialog", "M/d/yyyy", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("TaskDialog", "End Date", 0, QApplication::UnicodeUTF8));
        endDate->setDisplayFormat(QApplication::translate("TaskDialog", "M/d/yyyy", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("TaskDialog", "Status", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(TaskDialog);
    } // retranslateUi

};

namespace Ui {
    class TaskDialog: public Ui_TaskDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TASKDIALOG_H
