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

	// ���� ��� -> ��� ��� ��ȯ.
	//PathRelativePathTo(����� ��ȯ, From, FILE_ATTRIBUTE_DIRECTORY,
	//					To, FILE_ATTRIBUTE_DIRECTORY)
	// From���� To�� ���� ��� ��θ� ������ִ� �Լ�. 
	// ��, From�� To�� ���� ����̺꿡 �����ؾ��Ѵ�.
	PathRelativePathTo(szRelativePath, szCurrentPath, FILE_ATTRIBUTE_DIRECTORY,
		strFullPath, FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CFileInfo::ExtractDirectory(const TCHAR* pFullPath, list<PATH_INFO*>& rPathInfoLst)
{
	// ��� Ž���� ���� MFC Ŭ����
	CFileFind	find;

	// CFileFind::FindFile(L"���ϸ�.Ȯ����" or L"������")
	// ���� �۾� ��� �� �ش� �����̳� ������ �����ϴ��� ������ Ȯ���ϴ� �Լ�.
	// �����Ѵٸ� TRUE ��ȯ, �׷��� ������ FALSE ��ȯ.

	// CFileFind::FindFile(L"*.*"): ���� �۾� ��ο� �ƹ� �����̳� ������ �����ϴ���?
	// CFileFind::FindFile(L"���/���ϸ�.Ȯ����"): �ش� ��ο� ������ �����ϴ���?
	// CFileFind::FindFile(L"���/*.*"): �ش� ��ο� �ƹ� �����̳� ������ �����ϴ���?

	wstring wstrImgPath = wstring(pFullPath) + L"\\*.*";

	// FindFile�� ȣ���ϰ��� FindNextFile�� ȣ���ؾ� ã�� ���Ͽ� ������ �� �ִ�.
	BOOL bIsExist = find.FindFile(wstrImgPath.c_str());

	while (bIsExist)
	{
		// ���� ��ο��� ���� �����̳� ������ ã�ƶ�.
		bIsExist = find.FindNextFile(); // BossMultiAttack

		if (find.IsDots()) // ã���� .�̳� ..���� ��Ŀ��� �ǳʶ�.
			continue;
		else if (find.IsDirectory()) // ã���� �������
			ExtractDirectory(find.GetFilePath(), rPathInfoLst);
		else // ������ ã�Ҵٸ� 
		{
			if(find.IsSystem())	// ã�� ������ �������ִ� �ý��� ���϶��
				continue;
				
			PATH_INFO* pPathInfo = new PATH_INFO;

			pPathInfo->iImgCount = ExtractFileCount(find.GetFilePath());

			///////////////////////////////////////////////////////////////////
						
			CString strRelative = ConvertRelativePath(find.GetFilePath());		

			// Replace ����� ��.
			strRelative.Replace(L"0.", L"%d.");

			pPathInfo->wstrRelativePath = strRelative;

			///////////////////////////////////////////////////////////////////

			TCHAR szRelative[MAX_STR] = L"";
			lstrcpy(szRelative, strRelative.GetString());

			// ..\Texture\Stage\Effect\BossMultiAttack
			PathRemoveFileSpec(szRelative);

			// PathFindFileName: ���� ��ο��� ���ϸ��� ã�Ƴ��� �Լ�.
			// ��, ��� �� ���ϸ��� ���ٸ� ���� �������� ã�Ƴ���.
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
// D:\������\4������\97A 1��\Framework97A1\Texture\
// Stage\Effect\BossMultiAttack\BossMultiAttack0.png
int CFileInfo::ExtractFileCount(const TCHAR* pFullPath)
{
	CFileFind find;

	TCHAR szFullPath[MAX_STR] = L"";
	lstrcpy(szFullPath, pFullPath);

	// szFullPath:
	// D:\������\4������\97A 1��\Framework97A1\Texture\
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
