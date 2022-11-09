// SubView.cpp : 구현 파일입니다.
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


// CSubView 그리기입니다.

void CSubView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CSubView 진단입니다.

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


// CSubView 메시지 처리기입니다.


void CSubView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
