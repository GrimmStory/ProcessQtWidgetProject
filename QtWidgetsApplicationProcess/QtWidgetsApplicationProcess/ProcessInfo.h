#pragma once
#include <list>
#include <iostream>
#include <string>
#include <windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <Psapi.h>
#include <winver.h>
#include <atlstr.h>

#pragma comment(lib, "version.lib")
using namespace std;


struct processInfo {
	string name;
	int pid;
	string description;
	string processPath;
};

class ProcessInfo
{
private:
	BOOL EnableDebugPrivilege();
	BOOL DeviceDosPathToNtPath(TCHAR* pszDosPath, TCHAR* pszNtPath);

public:
	list<processInfo> GetAllProcessInfo();
	string TChartoString(TCHAR *str);
	

};

