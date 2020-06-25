#include "QtWidgetsApplicationProcess.h"

QtWidgetsApplicationProcess::QtWidgetsApplicationProcess(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	this->setWindowTitle("Process�����");
	initMenu();
	timer = new QTimer();
	timer->start(5000);
	connect(timer, &QTimer::timeout, this, &QtWidgetsApplicationProcess::flushProcess);
	//connect(ui.treeProcessWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(popItemMenu(const QPoint&)));//�������Ҽ�

	ui.treeProcessWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.treeProcessWidget, &QTreeWidget::customContextMenuRequested, this, &QtWidgetsApplicationProcess::popItemMenu);//�������Ҽ�
	connect(ui.treeProcessWidget, &QTreeWidget::itemClicked, this, [=](QTreeWidgetItem *item, int column) {
		qDebug() << "111:" << item->data(1, Qt::UserRole) << endl;
	});//�������Ҽ�
}

void QtWidgetsApplicationProcess::flushProcess() {
	ProcessInfo *pInfo = new ProcessInfo();
	list<processInfo> listsProcessInfo = pInfo->GetAllProcessInfo();
	if (listsProcessInfo.size() == 0) {
		QMessageBox::warning(this, "����", "���̻�ȡʧ�ܣ�", "OK");
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
	menu = ui.menuBar->addMenu("����");
	showAllProcess = true;

	QAction *action = new QAction(tr("��ʾ����"), this);

	QMenu *menuChild = new QMenu(this);
	QAction *showAllMenu = new QAction("ȫ������");
	connect(showAllMenu, &QAction::triggered, this, [=]() {
		showAllProcess = false;
		flushProcess();
	});

	QAction *showWindows = new QAction("��ʾ�����");
	connect(showWindows, &QAction::triggered, this, &QtWidgetsApplicationProcess::showActiveWindows);

	QAction *showPathMenu = new QAction("·���ǿս���");
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
	labelHead->append("��������");
	labelHead->append("��������");
	labelHead->append("����id");
	labelHead->append("����·��");

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
		//��ȡ���ڱ���
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
		QString getName;//���յõ��Ĵ�����
		LPTSTR lpValueName = new TCHAR[250];
		hWndlength = GetWindowTextLength(hwndItem);
		GetWindowText(hwndItem, lpValueName, hWndlength + 1);
		getName = QString::fromWCharArray(lpValueName);

		if (getName != "") {
			qDebug() << "��ǰ��������" << getName;
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
	QAction *killAction = new QAction(tr("ɱ��ָ������"));
	connect(killAction, &QAction::triggered, this, [=]() {
		HANDLE killHandle = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
		BOOL bResult = TerminateProcess(killHandle, 0);
		if (bResult) {
			QMessageBox::warning(this, "֪ͨ", "����" + nameItem + "�ɹ��رգ�", "OK");
		}
	});

	processMenu->addAction(killAction);
	processMenu->show();
}
