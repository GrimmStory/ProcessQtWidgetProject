#include "ProcessInfo.h"


BOOL ProcessInfo::EnableDebugPrivilege()
{
	HANDLE hToken;
	BOOL fOk = FALSE;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) {
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);

		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);

		fOk = (GetLastError() == ERROR_SUCCESS);
		CloseHandle(hToken);
	}
	return fOk;
}

BOOL ProcessInfo::DeviceDosPathToNtPath(TCHAR * pszDosPath, TCHAR * pszNtPath)
{
	static TCHAR    szDriveStr[MAX_PATH] = { 0 };
	static TCHAR    szDevName[MAX_PATH] = { 0 };
	TCHAR            szDrive[3];
	INT             cchDevName;
	INT             i;

	//检查参数  
	if (IsBadReadPtr(pszDosPath, 1) != 0)return FALSE;
	if (IsBadWritePtr(pszNtPath, 1) != 0)return FALSE;


	//获取本地磁盘字符串  
	ZeroMemory(szDriveStr, ARRAYSIZE(szDriveStr));
	ZeroMemory(szDevName, ARRAYSIZE(szDevName));
	if (GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr))
	{
		for (i = 0; szDriveStr[i]; i += 4)
		{
			if (!lstrcmpi(&(szDriveStr[i]), _T("A:\\")) || !lstrcmpi(&(szDriveStr[i]), _T("B:\\")))
				continue;

			szDrive[0] = szDriveStr[i];
			szDrive[1] = szDriveStr[i + 1];
			szDrive[2] = '\0';
			if (!QueryDosDevice(szDrive, szDevName, MAX_PATH))//查询 Dos 设备名  
				return FALSE;

			cchDevName = lstrlen(szDevName);
			if (_tcsnicmp(pszDosPath, szDevName, cchDevName) == 0)//命中  
			{
				lstrcpy(pszNtPath, szDrive);//复制驱动器  
				lstrcat(pszNtPath, pszDosPath + cchDevName);//复制路径  

				return TRUE;
			}
		}
	}

	lstrcpy(pszNtPath, pszDosPath);

	return FALSE;
}

 string ProcessInfo::TChartoString(TCHAR * STR)
{
	int iLen = WideCharToMultiByte(CP_ACP, 0, (LPCWCH)STR, -1, NULL, 0, NULL, NULL);
	char* chRtn = new char[iLen * sizeof(char)];
	WideCharToMultiByte(CP_ACP, 0, (LPCWCH)STR, -1, chRtn, iLen, NULL, NULL);
	string str(chRtn);
	return str;

}

list<processInfo> ProcessInfo::GetAllProcessInfo()
{
	list<processInfo> lists;
	PROCESSENTRY32	pe32;
	pe32.dwSize = sizeof(pe32);
	EnableDebugPrivilege();
	HANDLE hprocessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);


	BOOL bResult = Process32First(hprocessSnap, &pe32);
	int num(0);

	while (bResult) {
		struct processInfo pInfo;
		char temp[300];
		WideCharToMultiByte(CP_ACP, 0, (LPCWCH)pe32.szExeFile, -1, temp, sizeof(temp), NULL, NULL);
		pInfo.name = string(temp);

		int id = pe32.th32ProcessID;
		pInfo.pid = id;

		HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pe32.th32ProcessID);

		char szFullPath[MAX_PATH];
		ZeroMemory(szFullPath, MAX_PATH);

		TCHAR tsFileDosPath[MAX_PATH + 1];
		TCHAR outDosPath[MAX_PATH + 1];
		LPDWORD info = NULL;
		GetProcessImageFileName(pHandle, tsFileDosPath, MAX_PATH);
		DeviceDosPathToNtPath(tsFileDosPath, outDosPath);


		if (!PathFileExists(outDosPath)) {
			pInfo.processPath = "";
		}
		else {
			pInfo.processPath = TChartoString(outDosPath);
		}

		DWORD dwVerSetZero = 0;
		DWORD dwVerInfoSize = GetFileVersionInfoSize(outDosPath, 0);
		LPVOID pBlock = malloc(dwVerInfoSize);
		if (!GetFileVersionInfo(outDosPath, 0, dwVerInfoSize, pBlock)) {
			pInfo.description = "";
			lists.push_front(pInfo);
			bResult = Process32Next(hprocessSnap, &pe32);
			continue;
		}

		DWORD* pVerValue;
		UINT nSize = 0;
		DWORD  m_dwLangCharset = 0;
		CHAR *tmpstr = NULL;
		string regStr;
		string fileDescription = "FileDescription";
		string translation = "\\VarFileInfo\\Translation";

		int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)translation.c_str(), -1, NULL, 0);
		WCHAR *newStr = new WCHAR[len + 1];
		memset(newStr, 0, len * 2 + 2);
		MultiByteToWideChar(CP_ACP, 0, (LPCSTR)translation.c_str(), -1, (LPWSTR)newStr, len);

		VerQueryValue(pBlock, (LPCWSTR)newStr, (void **)&pVerValue, &nSize);

		m_dwLangCharset = MAKELONG(HIWORD(pVerValue[0]), LOWORD(pVerValue[0]));
		tmpstr = new (std::nothrow) CHAR[128];
		sprintf_s(tmpstr, 128, "\\StringFileInfo\\%08lx\\%s", m_dwLangCharset, fileDescription.c_str());
		LPVOID lpData;
		if (::VerQueryValueA((void *)pBlock, tmpstr, &lpData, &nSize))
			regStr = (char*)lpData;
		pInfo.description = regStr;
		lists.push_front(pInfo);
		bResult = Process32Next(hprocessSnap, &pe32);
	}
	CloseHandle(hprocessSnap);
	return lists;
}
