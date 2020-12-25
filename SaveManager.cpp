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
	ofn.nFilterIndex = 1;
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

	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	std::string path_image = "";
	if (GetOpenFileName(&ofn))
	{
		path_image = Lpcwstr2String(ofn.lpstrFile);
		//if (path_image.length() < 5 || path_image.substr(path_image.length() - 5) != ".save")
		//	path_image += ".save";
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
	ofn.nFilterIndex = 1;
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

bool ReadSave(std::string path, Color ret[9][9], Color &human)
{
	FILE* f;
	if (fopen_s(&f, path.c_str(), "rb"))
		return false;
	char s[100] = { 0 };
	if (fread(s, 8, 1, f) != 1)
	{
		fclose(f);
		return false;
	}
	if (strcmp(s, "nogosave") != 0)
	{
		fclose(f);
		return false;
	}

	if (fread(s, 1, 1, f) != 1)
	{
		fclose(f);
		return false;
	}
	if (s[0] == 1)
		human = Color::BLACK;
	else if (s[0] == 2)
		human = Color::WHITE;
	else
	{
		fclose(f);
		return false;
	}

	if (fread(s, 81, 1, f) != 1)
	{
		fclose(f);
		return false;
	}
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			if (s[i * 9 + j] == 0)
				ret[i][j] = Color::SPACE;
			else if (s[i * 9 + j] == 1)
				ret[i][j] = Color::BLACK;
			else if (s[i * 9 + j] == 2)
				ret[i][j] = Color::WHITE;
			else
			{
				fclose(f);
				return false;
			}
		}
	fclose(f);
	return true;
}

void WriteSave(std::string path, Color A[9][9], Color human)
{
	FILE* f;
	if (fopen_s(&f, path.c_str(), "wb"))
		return;
	fwrite("nogosave", 8, 1, f);
	if (human == Color::BLACK)
		fwrite("\001", 1, 1, f);
	else if (human == Color::WHITE)
		fwrite("\002", 1, 1, f);
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			if (A[i][j] == Color::SPACE)
				fwrite("\000", 1, 1, f);
			else if (A[i][j] == Color::BLACK)
				fwrite("\001", 1, 1, f);
			else if (A[i][j] == Color::WHITE)
				fwrite("\002", 1, 1, f);
		}
	fclose(f);
}
