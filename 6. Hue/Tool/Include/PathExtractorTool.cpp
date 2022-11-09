// PathExtractorTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathExtractorTool.h"
#include "afxdialogex.h"

// CPathExtractorTool ��ȭ �����Դϴ�.

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


// CPathExtractorTool �޽��� ó�����Դϴ�.

void CPathExtractorTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
	//	fin.getline(szRelativePath, MAX_STR); // ���� ������ ��� �о����.
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

		// ���� -> ���ڿ��� ġȯ�ϴ� �Լ�.
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

	// CListBox::GetCount�Լ�(����Ʈ �ڽ� ����� ��ü ����)
	for (int i = 0; i < m_ListBoxPath.GetCount(); ++i)
	{
		m_ListBoxPath.GetText(i, strName);

		// CDC::GetTextExtent�Լ�(���� �ؽ�Ʈ�� �ȼ� ���� ũ�⸦ ������ �Լ�)
		size = pDC->GetTextExtent(strName);

		if (iCX < size.cx)
			iCX = size.cx;
	}

	// ���� ����Ʈ�ڽ��� ���� ��ũ�� �ִ� ������ ������ �Լ�.
	if (iCX > m_ListBoxPath.GetHorizontalExtent())
		m_ListBoxPath.SetHorizontalExtent(iCX); // ���� ��ũ�� �ִ���� ����.

	m_ListBoxPath.ReleaseDC(pDC);
}

void CPathExtractorTool::OnBnClickedButtonSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(
		FALSE,	/* ���� TRUE, ���� FALSE */
		L"txt", /* ����Ʈ Ȯ���� */
		L"�������.txt", /* ����Ʈ �����̸� */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt||", /* ���� ���� */
		this /* �θ� ������ */);

	TCHAR szCurDir[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szCurDir);
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szCurDir; // ������

	if (IDOK == Dlg.DoModal())
	{
		// ���� ��� ��ü
		wofstream fout;

		// ���� ���� ��� �Լ�
		fout.open(Dlg.GetPathName());

		// ���濡 �����ϸ� true ��ȯ.
		if (fout.fail())
		{
			AfxMessageBox(Dlg.GetPathName());
			AfxMessageBox(L"���� ����");

			return;
		}

		wstring wstrCombined = L"";
		TCHAR szCount[MIN_STR] = L"";

		for (auto& pPathInfo : m_PathInfoLst)
		{
			// ���� -> ���ڿ��� ġȯ�ϴ� �Լ�.
			_itot_s(pPathInfo->iImgCount, szCount, 10);

			wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey +
				L"|" + szCount + L"|" + pPathInfo->wstrRelativePath;

			// ���� ���
			fout << wstrCombined << endl;
		}

		fout.close();

		// ������ �⺻ ���α׷� ����
		WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
	}
}


void CPathExtractorTool::OnBnClickedButtonLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(
		TRUE,	/* ���� TRUE, ���� FALSE */
		L"txt", /* ����Ʈ Ȯ���� */
		L"�������.txt", /* ����Ʈ �����̸� */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt||", /* ���� ���� */
		this /* �θ� ������ */);

	TCHAR szCurDir[MAX_STR] = L"";

	GetCurrentDirectory(MAX_STR, szCurDir);
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szCurDir; // ������

	if (IDOK == Dlg.DoModal())
	{
		// C++ ���� �Է� ��ü
		wifstream fin;

		fin.open(Dlg.GetPathName());

		if (fin.fail())
		{
			AfxMessageBox(Dlg.GetPathName());
			AfxMessageBox(L"�ҷ����� ����");

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
			fin.getline(szObjectKey, MAX_STR, '|');	// '|' ������ ��� �о����.
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MIN_STR, '|');
			fin.getline(szRelativePath, MAX_STR); // ���� ������ ��� �о����.

												  // ���̻� �о�� ������ ������ break
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
