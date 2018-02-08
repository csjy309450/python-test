/********************************************************************************
** Form generated from reading UI file 'dlg_sort.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLG_SORT_H
#define UI_DLG_SORT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_dlg_sort
{
public:
    QGridLayout *gridLayout_4;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QComboBox *comboBox;
    QComboBox *comboBox_2;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QSpacerItem *verticalSpacer_2;
    QPushButton *pushButton_3;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QLabel *label_4;
    QComboBox *comboBox_3;
    QComboBox *comboBox_4;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QLabel *label_5;
    QLabel *label_6;
    QComboBox *comboBox_5;
    QComboBox *comboBox_6;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QDialog *dlg_sort)
    {
        if (dlg_sort->objectName().isEmpty())
            dlg_sort->setObjectName(QString::fromUtf8("dlg_sort"));
//        dlg_sort->resize(379, 398);
        dlg_sort->setCursor(QCursor(Qt::ArrowCursor));
        dlg_sort->setFixedSize(QSize(379, 398));
        gridLayout_4 = new QGridLayout(dlg_sort);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        groupBox = new QGroupBox(dlg_sort);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        gridLayout->addWidget(comboBox, 0, 1, 1, 1);

        comboBox_2 = new QComboBox(groupBox);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));

        gridLayout->addWidget(comboBox_2, 1, 1, 1, 3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);


        gridLayout_4->addWidget(groupBox, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton = new QPushButton(dlg_sort);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(dlg_sort);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        verticalLayout->addWidget(pushButton_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        pushButton_3 = new QPushButton(dlg_sort);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setCheckable(true);
        pushButton_3->setChecked(false);

        verticalLayout->addWidget(pushButton_3);


        gridLayout_4->addLayout(verticalLayout, 0, 1, 2, 1);

        verticalSpacer = new QSpacerItem(20, 71, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer, 1, 0, 1, 1);

        groupBox_2 = new QGroupBox(dlg_sort);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 0, 0, 1, 1);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 1, 0, 1, 1);

        comboBox_3 = new QComboBox(groupBox_2);
        comboBox_3->setObjectName(QString::fromUtf8("comboBox_3"));

        gridLayout_2->addWidget(comboBox_3, 0, 1, 1, 1);

        comboBox_4 = new QComboBox(groupBox_2);
        comboBox_4->setObjectName(QString::fromUtf8("comboBox_4"));

        gridLayout_2->addWidget(comboBox_4, 1, 1, 1, 3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 0, 2, 1, 1);


        gridLayout_4->addWidget(groupBox_2, 2, 0, 1, 1);

        groupBox_3 = new QGroupBox(dlg_sort);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_3->addWidget(label_5, 0, 0, 1, 1);

        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_3->addWidget(label_6, 1, 0, 1, 1);

        comboBox_5 = new QComboBox(groupBox_3);
        comboBox_5->setObjectName(QString::fromUtf8("comboBox_5"));

        gridLayout_3->addWidget(comboBox_5, 0, 1, 1, 1);

        comboBox_6 = new QComboBox(groupBox_3);
        comboBox_6->setObjectName(QString::fromUtf8("comboBox_6"));

        gridLayout_3->addWidget(comboBox_6, 1, 1, 1, 3);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_3, 0, 2, 1, 1);


        gridLayout_4->addWidget(groupBox_3, 3, 0, 1, 1);

        groupBox_2->setVisible(false);
        groupBox_3->setVisible(false);

        retranslateUi(dlg_sort);

        QMetaObject::connectSlotsByName(dlg_sort);
    } // setupUi

    void retranslateUi(QDialog *dlg_sort)
    {
        dlg_sort->setWindowTitle(QApplication::translate("dlg_sort", "sort", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("dlg_sort", "&Primary Key 0", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("dlg_sort", "Column:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("dlg_sort", "Order:", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("dlg_sort", "OK", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("dlg_sort", "Cancel", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("dlg_sort", "&More", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("dlg_sort", "&Primary Key 1", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("dlg_sort", "Column:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("dlg_sort", "Order:", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("dlg_sort", "&Primary Key 2", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("dlg_sort", "Column:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("dlg_sort", "Order:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dlg_sort: public Ui_dlg_sort {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLG_SORT_H
