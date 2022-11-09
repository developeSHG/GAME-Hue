// EventTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "EventTool.h"
#include "afxdialogex.h"


// CEventTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEventTool, CDialog)

CEventTool::CEventTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_EVENTTOOL, pParent)
{

}

CEventTool::~CEventTool()
{
}

void CEventTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEventTool, CDialog)
END_MESSAGE_MAP()


// CEventTool 메시지 처리기입니다.
