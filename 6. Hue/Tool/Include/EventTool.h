#pragma once


// CEventTool ��ȭ �����Դϴ�.

class CEventTool : public CDialog
{
	DECLARE_DYNAMIC(CEventTool)

public:
	CEventTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEventTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EVENTTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
