#include "QtWidgetsApplicationProcess.h"

QtWidgetsApplicationProcess::QtWidgetsApplicationProcess(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	this->setWindowTitle("Process浏览器");
	initMenu();
	timer = new QTimer();
	timer->start(5000);
	connect(timer, &QTimer::timeout, this, &QtWidgetsApplicationProcess::flushProcess);
	//connect(ui.treeProcessWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(popItemMenu(const QPoint&)));//检测鼠标右键

	ui.treeProcessWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.treeProcessWidget, &QTreeWidget::customContextMenuRequested, this, &QtWidgetsApplicationProcess::popItemMenu);//检测鼠标右键
	connect(ui.treeProcessWidget, &QTreeWidget::itemClicked, this, [=](QTreeWidgetItem *item, int column) {
		qDebug() << "111:" << item->data(1, Qt::UserRole) << endl;
	});//检测鼠标右键
}

void QtWidgetsApplicationProcess::flushProcess() {
	ProcessInfo *pInfo = new ProcessInfo();
	list<processInfo> listsProcessInfo = pInfo->GetAllProcessInfo();
	if (listsProcessInfo.size() == 0) {
		QMessageBox::warning(this, "错误", "进程获取失败！", "OK");
		return;
	}

	int processCount = ui.treeProcessWidget->topLevelItemCount();
	while (processCount > 0) {
		ui.treeProcessWidget->takeTopLevelItem(processCount);
		processCount--;
	}

	list<processInfo>::iterator it;
	for (it = listsProcessInfo.begin(); it != listsProcessInfo.end(); it++) {

		struct processInfo processItem = *it;
		if (showAllProcess && processItem.processPath == "") {
			continue;
		}
		QTreeWidgetItem *item = new QTreeWidgetItem();
		item->setText(0, QString::fromLocal8Bit(processItem.name.data()));
		item->setText(1, QString::fromLocal8Bit(processItem.description.data()));
		item->setData(1, Qt::UserRole, QString::fromLocal8Bit(processItem.description.data()));
		item->setText(2, QString::number(processItem.pid));
		item->setData(2, Qt::UserRole, processItem.pid);
		item->setText(3, QString::fromLocal8Bit(processItem.processPath.data()));
		item->setData(3, Qt::UserRole, QString::fromLocal8Bit(processItem.processPath.data()));

		ui.treeProcessWidget->addTopLevelItem(item);
	}
}

QtWidgetsApplicationProcess::~QtWidgetsApplicationProcess()
{
	delete timer;
	delete menu;
}

void QtWidgetsApplicationProcess::initMenu()
{
	menu = ui.menuBar->addMenu("进程");
	showAllProcess = true;

	QAction *action = new QAction(tr("显示进程"), this);

	QMenu *menuChild = new QMenu(this);
	QAction *showAllMenu = new QAction("全部进程");
	connect(showAllMenu, &QAction::triggered, this, [=]() {
		showAllProcess = false;
		flushProcess();
	});

	QAction *showWindows = new QAction("显示活动窗口");
	connect(showWindows, &QAction::triggered, this, &QtWidgetsApplicationProcess::showActiveWindows);

	QAction *showPathMenu = new QAction("路径非空进程");
	connect(showPathMenu, &QAction::triggered, this, [=]() {
		showAllProcess = true;
		flushProcess();
	});
	menuChild->addAction(showAllMenu);
	menuChild->addAction(showPathMenu);

	action->setMenu(menuChild);
	menu->addAction(action);
	menu->addAction(showWindows);

	QStringList *labelHead = new QStringList();
	labelHead->append("进程名称");
	labelHead->append("进程描述");
	labelHead->append("进程id");
	labelHead->append("进程路径");

	ui.treeProcessWidget->setHeaderLabels(*labelHead);

	ui.treeProcessWidget->header()->setDefaultAlignment(Qt::AlignCenter);
}

HWND m_hwndFind[1000] = { 0 };
int  m_num = 0;

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	if (::GetWindowLong(hWnd, GWL_STYLE) & WS_VISIBLE)
	{
		char sBuf[256];
		//获取窗口标题
		::GetWindowText(hWnd, (LPWSTR)sBuf, 256);
		m_hwndFind[m_num] = hWnd;
		m_num++;
	}
	return TRUE;
}

void QtWidgetsApplicationProcess::showActiveWindows()
{
	EnumWindows(EnumWindowsProc, 0);
	for (int i = 0; i < m_num; i++) {
		HWND hwndItem = m_hwndFind[i];
		int hWndlength;
		QString getName;//最终得到的窗体名
		LPTSTR lpValueName = new TCHAR[250];
		hWndlength = GetWindowTextLength(hwndItem);
		GetWindowText(hwndItem, lpValueName, hWndlength + 1);
		getName = QString::fromWCharArray(lpValueName);

		if (getName != "") {
			qDebug() << "当前窗口名：" << getName;
		}
		
	}
	m_num = 0;
}

void QtWidgetsApplicationProcess::popItemMenu(const QPoint &pos)
{
	
	QTreeWidgetItem *item = ui.treeProcessWidget->itemAt(pos);
	int pid = item->data(2, Qt::UserRole).toInt();
	QString nameItem = item->text(0);

	QMenu *processMenu = new QMenu();
	QAction *killAction = new QAction(tr("杀死指定进程"));
	connect(killAction, &QAction::triggered, this, [=]() {
		HANDLE killHandle = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
		BOOL bResult = TerminateProcess(killHandle, 0);
		if (bResult) {
			QMessageBox::warning(this, "通知", "进程" + nameItem + "成功关闭！", "OK");
		}
	});

	processMenu->addAction(killAction);
	processMenu->show();
}
