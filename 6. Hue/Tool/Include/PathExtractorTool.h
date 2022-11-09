#pragma once
#include "afxwin.h"

#include "Engine_Include.h"
#include "FileInfo.h"
#include "MFCEnum.h"

// CPathExtractorTool ��ȭ �����Դϴ�.

class CPathExtractorTool : public CDialog
{
	DECLARE_DYNAMIC(CPathExtractorTool)

public:
	CPathExtractorTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathExtractorTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHEXTRACTORTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();

private:	
	// user function
	void CreateHorizontalScroll();

public:
	CListBox m_ListBoxPath;

public:
	list<PATH_INFO*>	m_PathInfoLst;
	afx_msg void OnLbnSelchangeList1();
};
