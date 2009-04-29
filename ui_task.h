/********************************************************************************
** Form generated from reading ui file 'task.ui'
**
** Created: Mon Apr 20 23:42:18 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TASK_H
#define UI_TASK_H

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
    QLineEdit *lineEdit;
    QLabel *label_2;
    QTextEdit *textEdit;
    QLabel *label_3;
    QLineEdit *lineEdit_2;
    QLabel *label_4;
    QDateEdit *dateEdit;
    QLabel *label_5;
    QDateEdit *dateEdit_2;
    QLabel *label_6;
    QComboBox *comboBox;

    void setupUi(QDialog *TaskDialog)
    {
        if (TaskDialog->objectName().isEmpty())
            TaskDialog->setObjectName(QString::fromUtf8("TaskDialog"));
        TaskDialog->resize(441, 433);
        buttonBox = new QDialogButtonBox(TaskDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(90, 390, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        formLayoutWidget = new QWidget(TaskDialog);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(20, 10, 411, 371));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(formLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        lineEdit = new QLineEdit(formLayoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        textEdit = new QTextEdit(formLayoutWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, textEdit);

        label_3 = new QLabel(formLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        lineEdit_2 = new QLineEdit(formLayoutWidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEdit_2);

        label_4 = new QLabel(formLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        dateEdit = new QDateEdit(formLayoutWidget);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));

        formLayout->setWidget(3, QFormLayout::FieldRole, dateEdit);

        label_5 = new QLabel(formLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_5);

        dateEdit_2 = new QDateEdit(formLayoutWidget);
        dateEdit_2->setObjectName(QString::fromUtf8("dateEdit_2"));

        formLayout->setWidget(4, QFormLayout::FieldRole, dateEdit_2);

        label_6 = new QLabel(formLayoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_6);

        comboBox = new QComboBox(formLayoutWidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        formLayout->setWidget(5, QFormLayout::FieldRole, comboBox);


        retranslateUi(TaskDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), TaskDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), TaskDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(TaskDialog);
    } // setupUi

    void retranslateUi(QDialog *TaskDialog)
    {
        TaskDialog->setWindowTitle(QApplication::translate("TaskDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TaskDialog", "Name", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("TaskDialog", "Description", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("TaskDialog", "Duration", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("TaskDialog", "Start Date", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("TaskDialog", "End Date", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("TaskDialog", "Status", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(TaskDialog);
    } // retranslateUi

};

namespace Ui {
    class TaskDialog: public Ui_TaskDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TASK_H
