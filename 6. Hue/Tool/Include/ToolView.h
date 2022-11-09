
// ToolView.h : CToolView 클래스의 인터페이스
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
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

	// 특성입니다.
public:
	CToolDoc* GetDocument() const;

	// 작업입니다.
public:

	// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 생성된 메시지 맵 함수
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

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
{
	return reinterpret_cast<CToolDoc*>(m_pDocument);
}
#endif

