#pragma once
#include "afxcmn.h"


#include "MapTool.h"
#include "EventTool.h"
#include "PathExtractorTool.h"
#include "ObjectTool.h"
// CMyForm �� ���Դϴ�.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

public:
	CTabCtrl m_tabTool;
	afx_msg void OnTcnSelchangeTabTool(NMHDR *pNMHDR, LRESULT *pResult);


public:
	//Tool ����
	CMapTool			m_MapTool;
	CEventTool			m_EventTool;
	CPathExtractorTool	m_PathExtractorTool;
	CObjectTool			m_ObjectTool;
};


