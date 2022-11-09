// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "Engine_Include.h"
#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "ToolMgr.h"
#include "MapTool.h"
#include "ObjectTool.h"
#include "ResourceMgr.h"

#include "Cube.h"
#include "Obj.h"

#include "OrthoCamera.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//static int iCountKey = 0;
// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView 생성/소멸
CToolView::CToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.


}

CToolView::~CToolView()
{

}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.

}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	//스크롤 크기 지정

	CScrollView::SetScrollSizes(MM_TEXT, CSize(1600, 1400));

	//화면 크기 조정
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());


	//NULL_CHECK_MSG(pFrameWnd, L"pFrameWnd is nullptr");

	// GetWindowRect함수: 테두리를 포함한 전체 윈도우 렉트를 얻어옴. 이 때 좌표는 스크린 좌표 기준.
	// GetClientRect함수: 테두리를 미포함한 클라이언트 영역을 얻어옴. 이 때 좌표는 클라이언트 좌표 기준.
	RECT rcFrame = {};
	pFrameWnd->GetWindowRect(&rcFrame);
	::SetRect(&rcFrame, 0, 0, rcFrame.right - rcFrame.left, rcFrame.bottom - rcFrame.top);
	RECT rcView = {};
	GetClientRect(&rcView);

	// 현재 프레임윈도우와 뷰의 x, y 간격.
	int iCX = rcFrame.right - rcView.right;
	int iCY = rcFrame.bottom - rcView.bottom;

	// FrameWnd 위치, 크기 재조정.
	pFrameWnd->SetWindowPos(nullptr, 0, 0, WINCX + iCX, WINCY + iCY, SWP_NOZORDER);

	CToolMgr::GetInstance()->Initialize(m_hWnd);
	m_pToolMgr = CToolMgr::GetInstance();
	m_pMapTool = m_pToolMgr->m_pMapTool;
	m_pObjectTool = m_pToolMgr->m_pObjectTool;
	m_pToolMgr->m_pToolView = this;

	m_bPickingOn = true;
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonDown(nFlags, point);

	D3DXVECTOR3 vMouse =
	{
		float(point.x) + CScrollView::GetScrollPos(0) - WINCX / 2.f + (m_pToolMgr->m_pOrthoCamera->Get_CameraPos().x * -m_pToolMgr->m_pOrthoCamera->Get_CameraPos().z),
		float(point.y) + CScrollView::GetScrollPos(1) - WINCY / 2.f + (m_pToolMgr->m_pOrthoCamera->Get_CameraPos().y * m_pToolMgr->m_pOrthoCamera->Get_CameraPos().z),
		0.f
	};

	vMouse.x /= m_pToolMgr->m_pOrthoCamera->Get_CameraPos().z * -1.f;
	vMouse.y /= m_pToolMgr->m_pOrthoCamera->Get_CameraPos().z;

	/************************ (SongSong) ************************/
	/*  Picking 체크  */
	if (FAILED(PickingCheck(vMouse, m_bPickingOn)))
		return;


	// 1. 맵툴이라면
	if (!m_pObjectTool->m_bOpen)
	{
		/************************ (SongSong) ************************/
		/*  옵션체크  */
		m_pMapTool->OptionCheck();

		m_bPickingOn = true;
		float iTerrainITV = (float)m_pMapTool->GetDlgItemInt(IDC_TERRAIN_ITV);
		iTerrainITV = 1.7f;
		float fTerrainITV = iTerrainITV / 2.f;

		/************************ (SongSong) ************************/
		/*  픽킹되서 같다면 정보보여주기  */
		bool bExist = FALSE;
		for (auto& mapiter : m_pToolMgr->m_mapCubeVec)
		{
			for (auto& iter : mapiter.second)
			{
				if (int(vMouse.x) == int((iter)->Get_CubeInfo().vPos.x)
					&& int(vMouse.y) == int((iter)->Get_CubeInfo().vPos.y))
				{
					if (TRUE == m_pMapTool->m_CheckBtnPicking.GetCheck())
					{
						/************************ (SongSong) ************************/
						/*  컨트롤 변수 초기화  */
						for (size_t i = 0; i < Engine::CUBE_CLIPPING_TYPE::CUBE_TYPE_END - 3; ++i)
							m_pMapTool->m_RadioBtnClippingOption[i].SetCheck(FALSE);
						for (size_t i = 0; i < Engine::CUBE_COLOR::CUBE_COLOR_END; ++i)
						{
							m_pMapTool->m_RadioBtnCubeColor[i].SetCheck(FALSE);
							m_pMapTool->m_RadioBtnCubeColor[i].EnableWindow(FALSE);
						}

						m_pMapTool->m_RadioBtnClippingOption[iter->Get_CubeInfo().eType].SetCheck(TRUE);
						if (Engine::CUBE_CLIPPING_TYPE::CUBE_CullBackAndB == iter->Get_CubeInfo().eType)
							m_pMapTool->m_RadioBtnClippingOption[Engine::CUBE_CLIPPING_TYPE::CUBE_CullB].SetCheck(TRUE);

						if (Engine::CUBE_COLOR::CUBE_COLOR_END != iter->Get_CubeInfo().tColor)
						{
							for (size_t i = 0; i < Engine::CUBE_COLOR::CUBE_COLOR_END; ++i)
								m_pMapTool->m_RadioBtnCubeColor[i].EnableWindow(TRUE);
							m_pMapTool->m_RadioBtnCubeColor[iter->Get_CubeInfo().tColor].SetCheck(TRUE);
						}
						// 거의 Skull용
						if (Engine::CUBE_CLIPPING_TYPE::CUBE_CullB == iter->Get_CubeInfo().eType
							|| Engine::CUBE_CLIPPING_TYPE::CUBE_CullBackAndB == iter->Get_CubeInfo().eType)
						{
							for (size_t i = 0; i < Engine::CUBE_COLOR::CUBE_COLOR_END; ++i)
								m_pMapTool->m_RadioBtnCubeColor[i].EnableWindow(TRUE);
						}

						/*if (Engine::CUBE_MOVING_DIR::CUBE_MOVING_DIR_END != iter->Get_CubeInfo().tPlatformInfo.eDir)
						{
							for (size_t i = 0; i < Engine::CUBE_MOVING_DIR::CUBE_MOVING_DIR_END; ++i)
								m_pMapTool->m_RadioBtnMovingDir[i].EnableWindow(TRUE);
							m_pMapTool->m_RadioBtnMovingDir[iter->Get_CubeInfo().tPlatformInfo.eDir].SetCheck(TRUE);
						}*/

						/************************ (SongSong) ************************/
						/*  옵션체크  */
						m_pMapTool->OptionCheck();

						/************************ (SongSong) ************************/
						/*  큐브선택 주소대입  */
						m_pPickExistCube = iter;
					}

					bExist = TRUE;
					break;
				}
			}
			m_pMapTool->Invalidate(FALSE);
			Invalidate(FALSE);
			if (TRUE == bExist)
				return;
		}


		/************************ (SongSong) ************************/
		/*  툴매니저 큐브버퍼생성  */
		if (-1 == m_pMapTool->m_ListBoxCubeNum.GetCurSel())
			return;

		Engine::CUBE_INFO tCubeInfo;
		//tCubeInfo.vPos = { float(x) * iTerrainITV + fTerrainITV, float(z) * iTerrainITV + fTerrainITV, 0.f };
		tCubeInfo.vPos = { vMouse.x, vMouse.y, 0.f };
		tCubeInfo.vScale = { fTerrainITV, fTerrainITV, fTerrainITV / 2.f };
		tCubeInfo.eType = m_pMapTool->m_eCubeClippingType;
		tCubeInfo.tColor = m_pMapTool->m_eCubeColor;
		lstrcpy(tCubeInfo.tTex.szTex, m_pMapTool->m_wstrCubeTexKey.c_str());
		tCubeInfo.tTex.iIndex = m_pMapTool->m_ListBoxCubeNum.GetCurSel();

		m_pMapTool->UpdateData(TRUE);
		/************************ (SongSong) ************************/
		/*  Scale 체크(Crate, Platform 등등)  */
		if (TRUE == m_pMapTool->m_editScaleCX.IsWindowEnabled() && 0.f != m_pMapTool->m_fScaleCX
			&& TRUE == m_pMapTool->m_editScaleCY.IsWindowEnabled() && 0.f != m_pMapTool->m_fScaleCY)
		{
			tCubeInfo.vPos.y += (float(m_pMapTool->m_fScaleCY) / 2.f) - tCubeInfo.vScale.y;
			tCubeInfo.vScale = { float(m_pMapTool->m_fScaleCX) / 2.f, float(m_pMapTool->m_fScaleCY) / 2.f, fTerrainITV / 2.f };
		}

		/************************ (SongSong) ************************/
		/*  MovingDetail 체크(Platform 등등)  */
		if (TRUE == m_pMapTool->m_editMovingDist.IsWindowEnabled() && 0.f != m_pMapTool->m_fMovingDist
			&& TRUE == m_pMapTool->m_editMovingSpeed.IsWindowEnabled() && 0.f != m_pMapTool->m_fMovingSpeed)
		{
			m_pMapTool->MovingDetailDirCheck(tCubeInfo);
		}
		m_pMapTool->UpdateData(FALSE);

		TCHAR* pBufferName = nullptr;

		switch (tCubeInfo.eType)
		{
		case Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL:
			pBufferName = L"Buffer_CubeNormal";
			break;
		case Engine::CUBE_CLIPPING_TYPE::CUBE_FRONT:
			pBufferName = L"Buffer_CubeFront";
			break;
		case Engine::CUBE_CLIPPING_TYPE::CUBE_CullB:
			pBufferName = L"Buffer_CubeCullB";
			break;
		case Engine::CUBE_CLIPPING_TYPE::CUBE_CullL:
			pBufferName = L"Buffer_CubeCullL";
			break;
		case Engine::CUBE_CLIPPING_TYPE::CUBE_CullR:
			pBufferName = L"Buffer_CubeCullR";
			break;
		case Engine::CUBE_CLIPPING_TYPE::CUBE_CullLR:
			pBufferName = L"Buffer_CubeCullLR";
			break;
		case Engine::CUBE_CLIPPING_TYPE::CUBE_CullBackAndB:
			pBufferName = L"Buffer_CubeCullBackAndB";
			break;
		}

		PickCubeSendToEngine();

		m_pToolMgr->CreateCube(m_pMapTool->m_wstrCubeTypeKey, tCubeInfo);

		m_pMapTool->Invalidate(FALSE);
		Invalidate(FALSE);

	}
	// 오브젝트 툴이라면
	else if (m_pObjectTool->m_bOpen)
	{
		m_bPickingOn = false;

		Engine::OBJ_INFO tObjInfo;
		tObjInfo.vPos = { vMouse.x, vMouse.y, 0.f };
		tObjInfo.vScale = { 1.0f, 1.0f, 1.0f };
		tObjInfo.eType = m_pObjectTool->m_eCubeClippingType;
		lstrcpy(tObjInfo.tTex.szTex, m_pObjectTool->m_wstrCubeTexKey.c_str());
		tObjInfo.tTex.iIndex = m_pObjectTool->m_ListBoxObjectNum.GetCurSel();
		tObjInfo.iKey = iCountKey++;

		TCHAR* pBufferName = nullptr;
		pBufferName = L"Buffer_CubeNormal";
		
		PickCubeSendToEngine();

		// 정보 입력
		D3DXVECTOR3 vPos = tObjInfo.vPos;
		D3DXVECTOR3 vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vScale = tObjInfo.vScale;
		// 정보 보내기
		m_pObjectTool->UpdateInfo(vPos,vRot,vScale, false);
		wstring SumKey = m_pObjectTool->m_wstrCubeTexKey + L"_" +to_wstring(tObjInfo.iKey);
		m_pObjectTool->m_ObjectListBox.AddString(SumKey.c_str());

		m_pToolMgr->CreateObject(m_pObjectTool->m_wstrCubeTypeKey, tObjInfo);

		m_pObjectTool->Invalidate(FALSE);
		Invalidate(FALSE);
	}
}

void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CScrollView::OnRButtonDown(nFlags, point);

	D3DXVECTOR3 vMouse =
	{
		float(point.x) + CScrollView::GetScrollPos(0) - WINCX / 2.f + (m_pToolMgr->m_pOrthoCamera->Get_CameraPos().x * -m_pToolMgr->m_pOrthoCamera->Get_CameraPos().z),
		float(point.y) + CScrollView::GetScrollPos(1) - WINCY / 2.f + (m_pToolMgr->m_pOrthoCamera->Get_CameraPos().y * m_pToolMgr->m_pOrthoCamera->Get_CameraPos().z),
		0.f
	};

	vMouse.x /= m_pToolMgr->m_pOrthoCamera->Get_CameraPos().z * -1.f;
	vMouse.y /= m_pToolMgr->m_pOrthoCamera->Get_CameraPos().z;

	/*  Picking 체크  */

	if (FAILED(PickingCheck(vMouse, m_bPickingOn)))
		return;

	if (!m_pObjectTool->m_bOpen)
	{
		/*  픽킹되서 같다면 삭제  */
		bool bErase = FALSE;
		for (auto& mapiter : m_pToolMgr->m_mapCubeVec)
		{
			for (list<Engine::CCube*>::iterator& listIter = mapiter.second.begin(); listIter != mapiter.second.end(); )
			{
				if (int(vMouse.x) == int((*listIter)->Get_CubeInfo().vPos.x)
					&& int(vMouse.y) == int((*listIter)->Get_CubeInfo().vPos.y))
				{
					Safe_Delete(*listIter);
					listIter = mapiter.second.erase(listIter);

					bErase = TRUE;
					break;
				}
				else
					++listIter;
			}
			if (TRUE == bErase)
				break;
		}

		m_pMapTool->Invalidate(FALSE);
		Invalidate(FALSE);
	}
	else
	{
		/*  픽킹되서 같다면 삭제  */
		bool bErase = FALSE;
		for (auto& mapiter : m_pToolMgr->m_mapObjList)
		{
			for (list<Engine::CObj*>::iterator& listIter = mapiter.second.begin(); listIter != mapiter.second.end(); )
			{
				if (int(vMouse.x) == int((*listIter)->Get_CubeInfo().vPos.x)
					&& int(vMouse.y) == int((*listIter)->Get_CubeInfo().vPos.y))
				{
					// 리스트 삭제
					m_pObjectTool->m_ObjectListBox.DeleteString((*listIter)->Get_CubeInfo().iKey);

					Safe_Delete(*listIter);
					listIter = mapiter.second.erase(listIter);
					bErase = TRUE;
					break;
				}
				else
					++listIter;
			}
	
			if (TRUE == bErase)
			{
				// 리스트 갱신
				iCountKey = 0;
				m_pObjectTool->m_ObjectListBox.ResetContent();
				for (auto& mapiter : m_pToolMgr->m_mapObjList)
				{
					for (list<Engine::CObj*>::iterator& listIter = mapiter.second.begin(); listIter != mapiter.second.end(); )
					{
						(*listIter)->Set_ObjKey(iCountKey);

						// 정보 보내기
						wstring Convert(&(*listIter)->Get_CubeInfo().tTex.szTex[0]);
						wstring SumKey = Convert + L"_" + to_wstring(iCountKey);
						m_pObjectTool->m_ObjectListBox.AddString(SumKey.c_str());

						++iCountKey;
						++listIter;
					}
				}
				break;
			}

		}

		m_pObjectTool->Invalidate(FALSE);
		Invalidate(FALSE);
	}
}

HRESULT CToolView::PickingCheck(D3DXVECTOR3& vMouse, bool bOn)
{
	if (!bOn)
		return S_OK;

	int iTerrainX = m_pMapTool->GetDlgItemInt(IDC_TERRAIN_X);
	int iTerrainZ = m_pMapTool->GetDlgItemInt(IDC_TERRAIN_Z);
	float iTerrainITV = (float)m_pMapTool->GetDlgItemInt(IDC_TERRAIN_ITV);
	iTerrainITV = 1.7f;
	float fTerrainITV = iTerrainITV / 2.f;

	if (1 >= iTerrainX || 1 >= iTerrainZ || 0 >= iTerrainITV)
		return E_FAIL;

	/************************ (SongSong) ************************/
	/*  큐브선택 nullptr 주소대입  */
	m_pPickExistCube = nullptr;

	bool bPicking = FALSE;
	for (int z = 0; z < iTerrainZ - 1; ++z)
	{
		for (int x = 0; x < iTerrainX - 1; ++x)
		{
			//float fTerrainITV = 1.7f / 2.f;

			float fCenterX = float(x) * iTerrainITV + fTerrainITV;
			float fCenterY = float(z) * iTerrainITV + fTerrainITV;

			if (fCenterX - fTerrainITV <= vMouse.x && fCenterX + fTerrainITV >= vMouse.x
				&& fCenterY - fTerrainITV <= vMouse.y && fCenterY + fTerrainITV >= vMouse.y)
			{
				vMouse.x = fCenterX;
				vMouse.y = fCenterY;
				bPicking = TRUE;
				break;
			}
		}
		if (TRUE == bPicking)
			break;
	}
	if (FALSE == bPicking)
		return E_FAIL;


	return S_OK;
}

void CToolView::PickCubeSendToEngine()
{

	for (auto mapiter : m_pToolMgr->m_mapCubeVec)
	{
		for (auto iter : mapiter.second)
		{
			m_pToolMgr->m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, iter->GetBufferName(), iter->Get_Vertex());
		}
	}
	for (auto mapiter : m_pToolMgr->m_mapObjList)
	{
		for (auto iter : mapiter.second)
		{
			m_pToolMgr->m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, iter->GetBufferName(), iter->Get_Vertex());
		}
	}
}
