/********************************************************************************
** Form generated from reading UI file 'QtWidgetsApplicationProcess.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTWIDGETSAPPLICATIONPROCESS_H
#define UI_QTWIDGETSAPPLICATIONPROCESS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtWidgetsApplicationProcessClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QWidget *widget;
    QGridLayout *gridLayout;
    QTreeWidget *treeProcessWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtWidgetsApplicationProcessClass)
    {
        if (QtWidgetsApplicationProcessClass->objectName().isEmpty())
            QtWidgetsApplicationProcessClass->setObjectName(QString::fromUtf8("QtWidgetsApplicationProcessClass"));
        QtWidgetsApplicationProcessClass->resize(600, 400);
        centralWidget = new QWidget(QtWidgetsApplicationProcessClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setAutoFillBackground(true);
        gridLayout = new QGridLayout(widget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        treeProcessWidget = new QTreeWidget(widget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeProcessWidget->setHeaderItem(__qtreewidgetitem);
        treeProcessWidget->setObjectName(QString::fromUtf8("treeProcessWidget"));
        treeProcessWidget->header()->setVisible(true);

        gridLayout->addWidget(treeProcessWidget, 0, 0, 1, 1);


        horizontalLayout->addWidget(widget);

        QtWidgetsApplicationProcessClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtWidgetsApplicationProcessClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        QtWidgetsApplicationProcessClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtWidgetsApplicationProcessClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        QtWidgetsApplicationProcessClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtWidgetsApplicationProcessClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QtWidgetsApplicationProcessClass->setStatusBar(statusBar);

        retranslateUi(QtWidgetsApplicationProcessClass);

        QMetaObject::connectSlotsByName(QtWidgetsApplicationProcessClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtWidgetsApplicationProcessClass)
    {
        QtWidgetsApplicationProcessClass->setWindowTitle(QApplication::translate("QtWidgetsApplicationProcessClass", "QtWidgetsApplicationProcess", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtWidgetsApplicationProcessClass: public Ui_QtWidgetsApplicationProcessClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTWIDGETSAPPLICATIONPROCESS_H
