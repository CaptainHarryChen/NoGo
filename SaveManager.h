#pragma once

#include <string>

//打开文件
std::string GetOpenFile(const wchar_t* filter = NULL, const wchar_t* title = NULL, const wchar_t* initDirectory = NULL);

//保存文件
std::string GetSaveFile(const wchar_t* filter = NULL, const wchar_t* title = NULL, const wchar_t* initDirectory = NULL);
