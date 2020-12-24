#include "SaveManager.h"

#include <windows.h>
#include <Commdlg.h>

std::string Lpcwstr2String(LPCWSTR lps)
{
	int len = WideCharToMultiByte(CP_ACP, 0, lps, -1, NULL, 0, NULL, NULL);
	if (len <= 0)
		return "";
	char* dest = new char[len];
	WideCharToMultiByte(CP_ACP, 0, lps, -1, dest, len, NULL, NULL);
	dest[len - 1] = 0;
	std::string str(dest);
	delete[] dest;
	return str;
}

std::string GetOpenFile(const wchar_t* filter, const wchar_t* title, const wchar_t* initDirectory)
{
	OPENFILENAME ofn;
	char szFile[300];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = (LPWSTR)szFile;
	ofn.lpstrFile[0] = '\0';
	LPTSTR lpstrCustomFilter;
	DWORD nMaxCustFilter;
	ofn.nFilterIndex = 1;
	LPTSTR lpstrFile;
	ofn.nMaxFile = sizeof(szFile);
	if (filter)
		ofn.lpstrFilter = filter;
	else
		ofn.lpstrFilter = TEXT("所有文件\0*.*\0Text\0*.TXT\0");
	if (title)
		ofn.lpstrTitle = title;
	else
		ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	if(initDirectory)
		ofn.lpstrInitialDir = initDirectory;
	else
		ofn.lpstrInitialDir = NULL;

	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	std::string path_image = "";
	if (GetOpenFileName(&ofn))
	{
		path_image = Lpcwstr2String(ofn.lpstrFile);
		return path_image;
	}
	return "";
}

std::string GetSaveFile(const wchar_t* filter, const wchar_t* title, const wchar_t* initDirectory)
{
	OPENFILENAME ofn;
	char szFile[300];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = (LPWSTR)szFile;
	ofn.lpstrFile[0] = '\0';
	LPTSTR lpstrCustomFilter;
	DWORD nMaxCustFilter;
	ofn.nFilterIndex = 1;
	LPTSTR lpstrFile;
	ofn.nMaxFile = sizeof(szFile);
	if (filter)
		ofn.lpstrFilter = filter;
	else
		ofn.lpstrFilter = TEXT("所有文件\0*.*\0Text\0*.TXT\0");
	if (title)
		ofn.lpstrTitle = title;
	else
		ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	if (initDirectory)
		ofn.lpstrInitialDir = initDirectory;
	else
		ofn.lpstrInitialDir = NULL;

	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	std::string path_image = "";
	if (GetSaveFileName(&ofn))
	{
		path_image = Lpcwstr2String(ofn.lpstrFile);
		return path_image;
	}
	return "";
}
