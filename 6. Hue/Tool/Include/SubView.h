#pragma once


// CSubView ���Դϴ�.

class CSubView : public CView
{
	DECLARE_DYNCREATE(CSubView)

protected:
	CSubView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CSubView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};


