// EventTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "EventTool.h"
#include "afxdialogex.h"


// CEventTool ��ȭ �����Դϴ�.

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


// CEventTool �޽��� ó�����Դϴ�.
