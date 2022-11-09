#pragma once

#include "Engine_Include.h"

typedef struct tagPathInfo
{
	wstring wstrObjectKey = L"";
	wstring wstrStateKey = L"";
	wstring wstrRelativePath = L"";
	int		iImgCount = 0;
}PATH_INFO;

class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString ConvertRelativePath(const CString& strFullPath);
	static void ExtractDirectory(const TCHAR* pFullPath, list<PATH_INFO*>& rPathInfoLst);
	static int ExtractFileCount(const TCHAR* pFullPath);
};

