#pragma once

#include <string>

//���ļ�
std::string GetOpenFile(const wchar_t* filter = NULL, const wchar_t* title = NULL, const wchar_t* initDirectory = NULL);

//�����ļ�
std::string GetSaveFile(const wchar_t* filter = NULL, const wchar_t* title = NULL, const wchar_t* initDirectory = NULL);
