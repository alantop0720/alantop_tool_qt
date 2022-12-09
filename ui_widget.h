/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *lineEdit_search;
    QPushButton *pushButton;
    QHBoxLayout *horizontalLayout_2;
    QListView *listView;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label_6;
    QLabel *label;
    QLabel *label_3;
    QPushButton *pushButton_path;
    QLabel *label_4;
    QLabel *label_5;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *lineEdit_id;
    QLineEdit *lineEdit_name;
    QLineEdit *lineEdit_filename;
    QLineEdit *lineEdit_path;
    QLineEdit *lineEdit_type;
    QLineEdit *lineEdit_remark;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_insert;
    QPushButton *pushButton_del;
    QPushButton *pushButton_modify;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *lineEdit_newdir;
    QPushButton *pushButton_2;
    QLabel *label_info;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(1024, 768);
        Widget->setStyleSheet(QString::fromUtf8("*{\n"
"	font: 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"	\n"
"	font: 12pt \"Consolas\";\n"
"\n"
"\n"
"}"));
        verticalLayout_5 = new QVBoxLayout(Widget);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        lineEdit_search = new QLineEdit(Widget);
        lineEdit_search->setObjectName(QStringLiteral("lineEdit_search"));

        horizontalLayout_4->addWidget(lineEdit_search);

        pushButton = new QPushButton(Widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_4->addWidget(pushButton);


        verticalLayout_5->addLayout(horizontalLayout_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        listView = new QListView(Widget);
        listView->setObjectName(QStringLiteral("listView"));
        listView->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_2->addWidget(listView);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        tableView = new QTableView(Widget);
        tableView->setObjectName(QStringLiteral("tableView"));

        verticalLayout_3->addWidget(tableView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_6 = new QLabel(Widget);
        label_6->setObjectName(QStringLiteral("label_6"));

        verticalLayout->addWidget(label_6);

        label = new QLabel(Widget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        label_3 = new QLabel(Widget);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);

        pushButton_path = new QPushButton(Widget);
        pushButton_path->setObjectName(QStringLiteral("pushButton_path"));
        pushButton_path->setFlat(false);

        verticalLayout->addWidget(pushButton_path, 0, Qt::AlignLeft);

        label_4 = new QLabel(Widget);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout->addWidget(label_4);

        label_5 = new QLabel(Widget);
        label_5->setObjectName(QStringLiteral("label_5"));

        verticalLayout->addWidget(label_5);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        lineEdit_id = new QLineEdit(Widget);
        lineEdit_id->setObjectName(QStringLiteral("lineEdit_id"));
        lineEdit_id->setReadOnly(true);
        lineEdit_id->setClearButtonEnabled(false);

        verticalLayout_2->addWidget(lineEdit_id);

        lineEdit_name = new QLineEdit(Widget);
        lineEdit_name->setObjectName(QStringLiteral("lineEdit_name"));
        lineEdit_name->setMinimumSize(QSize(500, 0));

        verticalLayout_2->addWidget(lineEdit_name);

        lineEdit_filename = new QLineEdit(Widget);
        lineEdit_filename->setObjectName(QStringLiteral("lineEdit_filename"));
        lineEdit_filename->setMinimumSize(QSize(500, 0));

        verticalLayout_2->addWidget(lineEdit_filename);

        lineEdit_path = new QLineEdit(Widget);
        lineEdit_path->setObjectName(QStringLiteral("lineEdit_path"));
        lineEdit_path->setMinimumSize(QSize(600, 0));

        verticalLayout_2->addWidget(lineEdit_path);

        lineEdit_type = new QLineEdit(Widget);
        lineEdit_type->setObjectName(QStringLiteral("lineEdit_type"));
        lineEdit_type->setMinimumSize(QSize(500, 0));

        verticalLayout_2->addWidget(lineEdit_type);

        lineEdit_remark = new QLineEdit(Widget);
        lineEdit_remark->setObjectName(QStringLiteral("lineEdit_remark"));
        lineEdit_remark->setMinimumSize(QSize(500, 0));

        verticalLayout_2->addWidget(lineEdit_remark);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        pushButton_insert = new QPushButton(Widget);
        pushButton_insert->setObjectName(QStringLiteral("pushButton_insert"));

        horizontalLayout_3->addWidget(pushButton_insert);

        pushButton_del = new QPushButton(Widget);
        pushButton_del->setObjectName(QStringLiteral("pushButton_del"));

        horizontalLayout_3->addWidget(pushButton_del);

        pushButton_modify = new QPushButton(Widget);
        pushButton_modify->setObjectName(QStringLiteral("pushButton_modify"));

        horizontalLayout_3->addWidget(pushButton_modify);


        verticalLayout_3->addLayout(horizontalLayout_3);


        verticalLayout_4->addLayout(verticalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        lineEdit_newdir = new QLineEdit(Widget);
        lineEdit_newdir->setObjectName(QStringLiteral("lineEdit_newdir"));

        horizontalLayout_5->addWidget(lineEdit_newdir);

        pushButton_2 = new QPushButton(Widget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_5->addWidget(pushButton_2);


        verticalLayout_4->addLayout(horizontalLayout_5);


        horizontalLayout_2->addLayout(verticalLayout_4);


        verticalLayout_5->addLayout(horizontalLayout_2);

        label_info = new QLabel(Widget);
        label_info->setObjectName(QStringLiteral("label_info"));
        label_info->setWordWrap(true);

        verticalLayout_5->addWidget(label_info);

        QWidget::setTabOrder(tableView, lineEdit_remark);
        QWidget::setTabOrder(lineEdit_remark, lineEdit_filename);
        QWidget::setTabOrder(lineEdit_filename, lineEdit_type);
        QWidget::setTabOrder(lineEdit_type, lineEdit_path);
        QWidget::setTabOrder(lineEdit_path, lineEdit_name);
        QWidget::setTabOrder(lineEdit_name, listView);
        QWidget::setTabOrder(listView, pushButton);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Widget", "\346\220\234\347\264\242", Q_NULLPTR));
        label_6->setText(QApplication::translate("Widget", "id", Q_NULLPTR));
        label->setText(QApplication::translate("Widget", "name", Q_NULLPTR));
        label_3->setText(QApplication::translate("Widget", "filename", Q_NULLPTR));
        pushButton_path->setText(QApplication::translate("Widget", "path", Q_NULLPTR));
        label_4->setText(QApplication::translate("Widget", "type", Q_NULLPTR));
        label_5->setText(QApplication::translate("Widget", "remark", Q_NULLPTR));
        pushButton_insert->setText(QApplication::translate("Widget", "\345\242\236\345\212\240", Q_NULLPTR));
        pushButton_del->setText(QApplication::translate("Widget", "\345\210\240\351\231\244", Q_NULLPTR));
        pushButton_modify->setText(QApplication::translate("Widget", "\344\277\256\346\224\271", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("Widget", "\346\233\264\346\226\260\347\233\256\345\275\225", Q_NULLPTR));
        label_info->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
