// SubView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "SubView.h"

// CSubView

IMPLEMENT_DYNCREATE(CSubView, CView)

CSubView::CSubView()
{

}

CSubView::~CSubView()
{
}

BEGIN_MESSAGE_MAP(CSubView, CView)
END_MESSAGE_MAP()


// CSubView �׸����Դϴ�.

void CSubView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// CSubView �����Դϴ�.

#ifdef _DEBUG
void CSubView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CSubView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSubView �޽��� ó�����Դϴ�.


void CSubView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}
