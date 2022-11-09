#pragma once


// CEventTool 대화 상자입니다.

class CEventTool : public CDialog
{
	DECLARE_DYNAMIC(CEventTool)

public:
	CEventTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEventTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EVENTTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
