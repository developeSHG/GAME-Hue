// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"
#include "ToolMgr.h"
#include "ToolView.h"

// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabTool);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyForm::OnTcnSelchangeTabTool)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	CString tab1 = L"Map Tool";
	CString tab2 = L"Event Tool";
	CString tab3 = L"PathExtractor Tool";
	CString tab4 = L"Object Tool";
	m_tabTool.SetItemSize(CSize(50, 30));

	m_tabTool.InsertItem(1, tab1);
	m_tabTool.InsertItem(2, tab2);
	m_tabTool.InsertItem(3, tab3);
	m_tabTool.InsertItem(4, tab4);

	CRect rect;
	m_tabTool.GetWindowRect(&rect);


	if (nullptr == m_MapTool.GetSafeHwnd())
		m_MapTool.Create(IDD_MAPTOOL, &m_tabTool);
	if (nullptr == m_EventTool.GetSafeHwnd())
		m_EventTool.Create(IDD_EVENTTOOL, &m_tabTool);
	if (nullptr == m_PathExtractorTool.GetSafeHwnd())
		m_PathExtractorTool.Create(IDD_PATHEXTRACTORTOOL, &m_tabTool);
	if (nullptr == m_ObjectTool.GetSafeHwnd())
		m_ObjectTool.Create(IDD_OBJECTTOOL, &m_tabTool);

	m_MapTool.ShowWindow(SW_SHOW);

	CToolMgr::GetInstance()->m_pMapTool = &m_MapTool;
}


void CMyForm::OnTcnSelchangeTabTool(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	*pResult = 0;

	int iTabIndex = m_tabTool.GetCurSel();

	CToolMgr::GetInstance()->m_pMapTool->OptionReset();
	CToolMgr::GetInstance()->m_pToolView->m_pPickExistCube = nullptr;

	switch (iTabIndex)
	{
	case 0:
		m_MapTool.ShowWindow(SW_SHOW);
		m_EventTool.ShowWindow(SW_HIDE);
		m_PathExtractorTool.ShowWindow(SW_HIDE);
		m_ObjectTool.ShowWindow(SW_HIDE);
		m_ObjectTool.m_bOpen = false;
		break;

	case 1:
		m_MapTool.ShowWindow(SW_HIDE);
		m_EventTool.ShowWindow(SW_SHOW);
		m_PathExtractorTool.ShowWindow(SW_HIDE);
		m_ObjectTool.ShowWindow(SW_HIDE);
		m_ObjectTool.m_bOpen = false;
		break;

	case 2:
		m_MapTool.ShowWindow(SW_HIDE);
		m_EventTool.ShowWindow(SW_HIDE);
		m_PathExtractorTool.ShowWindow(SW_SHOW);
		m_ObjectTool.ShowWindow(SW_HIDE);
		m_ObjectTool.m_bOpen = false;

		break;

	case 3:
		m_MapTool.ShowWindow(SW_HIDE);
		m_EventTool.ShowWindow(SW_HIDE);
		m_PathExtractorTool.ShowWindow(SW_HIDE);
		m_ObjectTool.ShowWindow(SW_SHOW);
		m_ObjectTool.m_bOpen = true;
		break;

	}

}
