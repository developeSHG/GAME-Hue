
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "Tool.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "SubView.h"
#include "MyForm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,

};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("도구 모음을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("상태 표시줄을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	//// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);

	SetMenu(nullptr);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
   cs.style &= ~FWS_ADDTOTITLE;
   cs.lpszName = L"Hue Tool";

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_MainSplitt.CreateStatic(this, 1, 2);			//this가 프레임윈도우
	int id = m_MainSplitt.IdFromRowCol(0, 0);

	//마지막 id는 부모의 어디에 소속될지 정하는것.
	//위의 메인스프릿 크리에이트에도 WS_CHILD | WS_VISIBLE가 디폴트로 들어간다.
	//다만 미니스프릿 생성 시 id값을 줘야하므로 매개변수를 써야하는 것.
	//m_MiniSplitt.CreateStatic(&m_MainSplitt, 2, 1, WS_CHILD | WS_VISIBLE, id);		//미니스프릿의 부모는 메인스프릿이다.

	//분할된 영역에 view를 배치 (2개 배치)
	//뷰 클래스들은 생성자가 protected로 되어 있으므로 마음대로 동적할당 불가능하다.
	//-> RUNTIME_CLASS 매크로를 사용.

	m_MainSplitt.SetColumnInfo(0, WINCX, 0);		//열의 너비 지정 (3번째 인자는 최소 크기)
	m_MainSplitt.SetRowInfo(0, WINCY, 0);
	//m_MainSplitt.SetColumnInfo(1, TOOLCX, TOOLCX);		//열의 너비 지정 (3번째 인자는 최소 크기)

	m_MainSplitt.CreateView(0, 0, RUNTIME_CLASS(CMyForm), CSize(399, WINCY), pContext);
	m_MainSplitt.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(WINCX - 399, WINCY), pContext);
	//m_MiniSplitt.draw


	return TRUE;		/*CFrameWnd::OnCreateClient(lpcs, pContext);*/  // 여기서 뷰를 생성하므로 우리가 제어해야한다.
}
