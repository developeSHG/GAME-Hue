#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(const CString& strFullPath)
{
	TCHAR szCurrentPath[MAX_STR] = L"";
	TCHAR szRelativePath[MAX_STR] = L"";
	
	GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 절대 경로 -> 상대 경로 변환.
	//PathRelativePathTo(상대경로 반환, From, FILE_ATTRIBUTE_DIRECTORY,
	//					To, FILE_ATTRIBUTE_DIRECTORY)
	// From에서 To로 가는 상대 경로를 만들어주는 함수. 
	// 단, From과 To는 같은 드라이브에 존재해야한다.
	PathRelativePathTo(szRelativePath, szCurrentPath, FILE_ATTRIBUTE_DIRECTORY,
		strFullPath, FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CFileInfo::ExtractDirectory(const TCHAR* pFullPath, list<PATH_INFO*>& rPathInfoLst)
{
	// 경로 탐색에 관한 MFC 클래스
	CFileFind	find;

	// CFileFind::FindFile(L"파일명.확장자" or L"폴더명")
	// 현재 작업 경로 상에 해당 파일이나 폴더가 존재하는지 유무를 확인하는 함수.
	// 존재한다면 TRUE 반환, 그렇지 않으면 FALSE 반환.

	// CFileFind::FindFile(L"*.*"): 현재 작업 경로에 아무 파일이나 폴더가 존재하는지?
	// CFileFind::FindFile(L"경로/파일명.확장자"): 해당 경로에 파일이 존재하는지?
	// CFileFind::FindFile(L"경로/*.*"): 해당 경로에 아무 파일이나 폴더가 존재하는지?

	wstring wstrImgPath = wstring(pFullPath) + L"\\*.*";

	// FindFile을 호출하고나서 FindNextFile을 호출해야 찾은 파일에 접근할 수 있다.
	BOOL bIsExist = find.FindFile(wstrImgPath.c_str());

	while (bIsExist)
	{
		// 같은 경로에서 다음 파일이나 폴더를 찾아라.
		bIsExist = find.FindNextFile(); // BossMultiAttack

		if (find.IsDots()) // 찾은게 .이나 ..같은 마커라면 건너뜀.
			continue;
		else if (find.IsDirectory()) // 찾은게 폴더라면
			ExtractDirectory(find.GetFilePath(), rPathInfoLst);
		else // 파일을 찾았다면 
		{
			if(find.IsSystem())	// 찾은 파일이 숨겨져있는 시스템 파일라면
				continue;
				
			PATH_INFO* pPathInfo = new PATH_INFO;

			pPathInfo->iImgCount = ExtractFileCount(find.GetFilePath());

			///////////////////////////////////////////////////////////////////
						
			CString strRelative = ConvertRelativePath(find.GetFilePath());		

			// Replace 기억할 것.
			strRelative.Replace(L"0.", L"%d.");

			pPathInfo->wstrRelativePath = strRelative;

			///////////////////////////////////////////////////////////////////

			TCHAR szRelative[MAX_STR] = L"";
			lstrcpy(szRelative, strRelative.GetString());

			// ..\Texture\Stage\Effect\BossMultiAttack
			PathRemoveFileSpec(szRelative);

			// PathFindFileName: 현재 경로에서 파일명을 찾아내는 함수.
			// 단, 경로 상에 파일명이 없다면 말단 폴더명을 찾아낸다.
			pPathInfo->wstrStateKey = PathFindFileName(szRelative);

			///////////////////////////////////////////////////////////////////

			// ..\Texture\Stage\Effect
			PathRemoveFileSpec(szRelative);
			pPathInfo->wstrObjectKey = PathFindFileName(szRelative);

			rPathInfoLst.push_back(pPathInfo);
			break;
		}
	}
	
}

// pFullPath: 
// D:\김태윤\4개월차\97A 1반\Framework97A1\Texture\
// Stage\Effect\BossMultiAttack\BossMultiAttack0.png
int CFileInfo::ExtractFileCount(const TCHAR* pFullPath)
{
	CFileFind find;

	TCHAR szFullPath[MAX_STR] = L"";
	lstrcpy(szFullPath, pFullPath);

	// szFullPath:
	// D:\김태윤\4개월차\97A 1반\Framework97A1\Texture\
	// Stage\Effect\BossMultiAttack
	PathRemoveFileSpec(szFullPath);

	wstring wstrFind = wstring(szFullPath) + L"\\*.*";

	BOOL bIsExist = find.FindFile(wstrFind.c_str());

	int iCount = 0;

	while (bIsExist)
	{
		bIsExist = find.FindNextFile();

		if(find.IsDots())
			continue;
		else if(find.IsSystem())
			continue;

		++iCount;
	}

	return iCount;
}
