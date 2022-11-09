#pragma once
#include "afxcmn.h"


#include "MapTool.h"
#include "EventTool.h"
#include "PathExtractorTool.h"
#include "ObjectTool.h"
// CMyForm 폼 뷰입니다.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

public:
	CTabCtrl m_tabTool;
	afx_msg void OnTcnSelchangeTabTool(NMHDR *pNMHDR, LRESULT *pResult);


public:
	//Tool 변수
	CMapTool			m_MapTool;
	CEventTool			m_EventTool;
	CPathExtractorTool	m_PathExtractorTool;
	CObjectTool			m_ObjectTool;
};


