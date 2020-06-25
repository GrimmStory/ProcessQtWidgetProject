#pragma once
#pragma execution_character_set("utf-8")

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include <QTimer>
#include <QDebug>
#include <qmenu.h>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include "ProcessInfo.h"
#include "Common.h"
#include "ui_QtWidgetsApplicationProcess.h"

class QtWidgetsApplicationProcess : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplicationProcess(QWidget *parent = Q_NULLPTR);
	~QtWidgetsApplicationProcess();
private slots:
	void flushProcess();
	void showActiveWindows();
	void popItemMenu(const QPoint &pos);
private:
    Ui::QtWidgetsApplicationProcessClass ui;
	void initMenu();
	QTimer *timer;
	QMenu *menu;

	bool showAllProcess;
};
