
// ToolView.h : CToolView Ŭ������ �������̽�
//

#pragma once

BEGIN(Engine)

class CCube;
class CObj;
END

class CToolDoc;
class CToolMgr;
class CMapTool;
class CObjectTool;
class CToolView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

	// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

	// �۾��Դϴ�.
public:

	// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

public:
	HRESULT	PickingCheck(D3DXVECTOR3& vMouse, bool bOn);
	void	PickCubeSendToEngine();

public:
	CToolMgr*		m_pToolMgr;
	CMapTool*		m_pMapTool;
	CObjectTool*	m_pObjectTool;

	Engine::CCube*	m_pPickExistCube = nullptr;
	Engine::CObj*	m_pPickExistObj = nullptr;

	bool			m_bPickingOn = false;
	int				iCountKey  = 0;
};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
{
	return reinterpret_cast<CToolDoc*>(m_pDocument);
}
#endif

