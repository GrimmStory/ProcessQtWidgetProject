#include "Common.h"

string Common::TChartoString(TCHAR * STR)
{
	int iLen = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, NULL);
	char* chRtn = new char[iLen * sizeof(char)];
	WideCharToMultiByte(CP_ACP, 0, (LPCWCH)STR, -1, chRtn, iLen, NULL, NULL);
	string str(chRtn);
	return str;
}
