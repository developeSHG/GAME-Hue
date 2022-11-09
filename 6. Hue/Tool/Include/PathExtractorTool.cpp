// PathExtractorTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathExtractorTool.h"
#include "afxdialogex.h"

// CPathExtractorTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPathExtractorTool, CDialog)

CPathExtractorTool::CPathExtractorTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHEXTRACTORTOOL, pParent)
{

}

CPathExtractorTool::~CPathExtractorTool()
{
	for_each(m_PathInfoLst.begin(), m_PathInfoLst.end(), Engine::Safe_Delete<PATH_INFO*>);
	m_PathInfoLst.clear();
}

void CPathExtractorTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBoxPath);
}


BEGIN_MESSAGE_MAP(CPathExtractorTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CPathExtractorTool::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON1, &CPathExtractorTool::OnBnClickedButtonLoad)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CPathExtractorTool::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CPathExtractorTool 메시지 처리기입니다.

void CPathExtractorTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnDropFiles(hDropInfo);

	int iCount = DragQueryFile(hDropInfo, -1, nullptr, 0);

	TCHAR szFullPath[MAX_STR] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFullPath, MAX_STR);
		CFileInfo::ExtractDirectory(szFullPath, m_PathInfoLst);
	}

	wstring wstrCombined = L"";
	TCHAR szCount[MIN_STR] = L"";

	//if (!lstrcmpiW(szStateKey, L"S"))
	//{
	//	fin.getline(szRelativePath, MAX_STR); // 개행 단위로 끊어서 읽어들임.
	//	hr = LoadTexture(SINGLE_TEXTURE, szRelativePath, szObjectKey);

	//	if (FAILED(hr))
	//	{
	//		fin.close();
	//		return E_FAIL;
	//	}
	//	continue;
	//}

	for (auto& pPathInfo : m_PathInfoLst)
	{

		// 정수 -> 문자열로 치환하는 함수.
		if (1 == pPathInfo->iImgCount)
		{
			pPathInfo->wstrObjectKey = pPathInfo->wstrStateKey;
			pPathInfo->wstrStateKey = L"S";
			wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey + L"|" + pPathInfo->wstrRelativePath;
			m_ListBoxPath.AddString(wstrCombined.c_str());
			continue;
		}

		_itot_s(pPathInfo->iImgCount, szCount, 10);

		wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey +
			L"|" + szCount + L"|" + pPathInfo->wstrRelativePath;

		m_ListBoxPath.AddString(wstrCombined.c_str());
	}

	CreateHorizontalScroll();
}

void CPathExtractorTool::CreateHorizontalScroll()
{
	CString strName = L"";
	CSize size;
	int iCX = 0;

	CDC* pDC = m_ListBoxPath.GetDC();

	// CListBox::GetCount함수(리스트 박스 목록의 전체 개수)
	for (int i = 0; i < m_ListBoxPath.GetCount(); ++i)
	{
		m_ListBoxPath.GetText(i, strName);

		// CDC::GetTextExtent함수(현재 텍스트의 픽셀 단위 크기를 얻어오는 함수)
		size = pDC->GetTextExtent(strName);

		if (iCX < size.cx)
			iCX = size.cx;
	}

	// 현재 리스트박스의 수평 스크롤 최대 범위를 얻어오는 함수.
	if (iCX > m_ListBoxPath.GetHorizontalExtent())
		m_ListBoxPath.SetHorizontalExtent(iCX); // 수평 스크롤 최대범위 설정.

	m_ListBoxPath.ReleaseDC(pDC);
}

void CPathExtractorTool::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(
		FALSE,	/* 열기 TRUE, 저장 FALSE */
		L"txt", /* 디폴트 확장자 */
		L"제목없음.txt", /* 디폴트 파일이름 */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt||", /* 파일 형식 */
		this /* 부모 윈도우 */);

	TCHAR szCurDir[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szCurDir);
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szCurDir; // 절대경로

	if (IDOK == Dlg.DoModal())
	{
		// 파일 출력 객체
		wofstream fout;

		// 파일 개방 멤버 함수
		fout.open(Dlg.GetPathName());

		// 개방에 실패하면 true 반환.
		if (fout.fail())
		{
			AfxMessageBox(Dlg.GetPathName());
			AfxMessageBox(L"저장 실패");

			return;
		}

		wstring wstrCombined = L"";
		TCHAR szCount[MIN_STR] = L"";

		for (auto& pPathInfo : m_PathInfoLst)
		{
			// 정수 -> 문자열로 치환하는 함수.
			_itot_s(pPathInfo->iImgCount, szCount, 10);

			wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey +
				L"|" + szCount + L"|" + pPathInfo->wstrRelativePath;

			// 파일 출력
			fout << wstrCombined << endl;
		}

		fout.close();

		// 윈도우 기본 프로그램 실행
		WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
	}
}


void CPathExtractorTool::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(
		TRUE,	/* 열기 TRUE, 저장 FALSE */
		L"txt", /* 디폴트 확장자 */
		L"제목없음.txt", /* 디폴트 파일이름 */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt||", /* 파일 형식 */
		this /* 부모 윈도우 */);

	TCHAR szCurDir[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szCurDir);
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szCurDir; // 절대경로

	if (IDOK == Dlg.DoModal())
	{
		// C++ 파일 입력 객체
		wifstream fin;

		fin.open(Dlg.GetPathName());

		if (fin.fail())
		{
			AfxMessageBox(Dlg.GetPathName());
			AfxMessageBox(L"불러오기 실패");

			return;
		}

		if (!m_PathInfoLst.empty())
		{
			for_each(m_PathInfoLst.begin(), m_PathInfoLst.end(), Engine::Safe_Delete<PATH_INFO*>);
			m_PathInfoLst.clear();
		}

		m_ListBoxPath.ResetContent();

		PATH_INFO* pPathInfo = nullptr;
		TCHAR szObjectKey[MAX_STR] = L"";
		TCHAR szStateKey[MAX_STR] = L"";
		TCHAR szCount[MIN_STR] = L"";
		TCHAR szRelativePath[MAX_STR] = L"";

		wstring wstrCombined = L"";

		while (true)
		{
			fin.getline(szObjectKey, MAX_STR, '|');	// '|' 단위로 끊어서 읽어들임.
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MIN_STR, '|');
			fin.getline(szRelativePath, MAX_STR); // 개행 단위로 끊어서 읽어들임.

												  // 더이상 읽어올 내용이 없으면 break
			if (fin.eof())
				break;

			pPathInfo = new PATH_INFO;
			pPathInfo->wstrObjectKey = szObjectKey;
			pPathInfo->wstrStateKey = szStateKey;
			pPathInfo->iImgCount = _ttoi(szCount);
			pPathInfo->wstrRelativePath = szRelativePath;

			m_PathInfoLst.push_back(pPathInfo);

			wstrCombined = wstring(szObjectKey) + L"|" + szStateKey + L"|"
				+ szCount + L"|" + szRelativePath;
			m_ListBoxPath.AddString(wstrCombined.c_str());
		}

		CreateHorizontalScroll();
		fin.close();
	}
}



void CPathExtractorTool::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
