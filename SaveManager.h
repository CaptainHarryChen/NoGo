#pragma once

#include <string>
#include "GameRule.h"

//打开文件窗口
std::string GetOpenFile(const wchar_t* filter = NULL, const wchar_t* title = NULL, const wchar_t* initDirectory = NULL);

//保存文件窗口
std::string GetSaveFile(const wchar_t* filter = NULL, const wchar_t* title = NULL, const wchar_t* initDirectory = NULL);

bool ReadSave(std::string path, Color ret[9][9], Color& human);
void WriteSave(std::string path, Color A[9][9], Color human);