#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <Psapi.h>
#include <winver.h>
#include <atlstr.h>
using namespace std;

class Common
{
public:
	static string TChartoString(TCHAR *str);
};

