// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�
CToolView::CToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.


}

CToolView::~CToolView()
{

}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CToolView �׸���

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.

}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	//��ũ�� ũ�� ����

	CScrollView::SetScrollSizes(MM_TEXT, CSize(1600, 1400));

	//ȭ�� ũ�� ����
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());


	//NULL_CHECK_MSG(pFrameWnd, L"pFrameWnd is nullptr");

	// GetWindowRect�Լ�: �׵θ��� ������ ��ü ������ ��Ʈ�� ����. �� �� ��ǥ�� ��ũ�� ��ǥ ����.
	// GetClientRect�Լ�: �׵θ��� �������� Ŭ���̾�Ʈ ������ ����. �� �� ��ǥ�� Ŭ���̾�Ʈ ��ǥ ����.
	RECT rcFrame = {};
	pFrameWnd->GetWindowRect(&rcFrame);
	::SetRect(&rcFrame, 0, 0, rcFrame.right - rcFrame.left, rcFrame.bottom - rcFrame.top);
	RECT rcView = {};
	GetClientRect(&rcView);

	// ���� ������������� ���� x, y ����.
	int iCX = rcFrame.right - rcView.right;
	int iCY = rcFrame.bottom - rcView.bottom;

	// FrameWnd ��ġ, ũ�� ������.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
	/*  Picking üũ  */
	if (FAILED(PickingCheck(vMouse, m_bPickingOn)))
		return;


	// 1. �����̶��
	if (!m_pObjectTool->m_bOpen)
	{
		/************************ (SongSong) ************************/
		/*  �ɼ�üũ  */
		m_pMapTool->OptionCheck();

		m_bPickingOn = true;
		float iTerrainITV = (float)m_pMapTool->GetDlgItemInt(IDC_TERRAIN_ITV);
		iTerrainITV = 1.7f;
		float fTerrainITV = iTerrainITV / 2.f;

		/************************ (SongSong) ************************/
		/*  ��ŷ�Ǽ� ���ٸ� ���������ֱ�  */
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
						/*  ��Ʈ�� ���� �ʱ�ȭ  */
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
						// ���� Skull��
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
						/*  �ɼ�üũ  */
						m_pMapTool->OptionCheck();

						/************************ (SongSong) ************************/
						/*  ť�꼱�� �ּҴ���  */
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
		/*  ���Ŵ��� ť����ۻ���  */
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
		/*  Scale üũ(Crate, Platform ���)  */
		if (TRUE == m_pMapTool->m_editScaleCX.IsWindowEnabled() && 0.f != m_pMapTool->m_fScaleCX
			&& TRUE == m_pMapTool->m_editScaleCY.IsWindowEnabled() && 0.f != m_pMapTool->m_fScaleCY)
		{
			tCubeInfo.vPos.y += (float(m_pMapTool->m_fScaleCY) / 2.f) - tCubeInfo.vScale.y;
			tCubeInfo.vScale = { float(m_pMapTool->m_fScaleCX) / 2.f, float(m_pMapTool->m_fScaleCY) / 2.f, fTerrainITV / 2.f };
		}

		/************************ (SongSong) ************************/
		/*  MovingDetail üũ(Platform ���)  */
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
	// ������Ʈ ���̶��
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

		// ���� �Է�
		D3DXVECTOR3 vPos = tObjInfo.vPos;
		D3DXVECTOR3 vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vScale = tObjInfo.vScale;
		// ���� ������
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CScrollView::OnRButtonDown(nFlags, point);

	D3DXVECTOR3 vMouse =
	{
		float(point.x) + CScrollView::GetScrollPos(0) - WINCX / 2.f + (m_pToolMgr->m_pOrthoCamera->Get_CameraPos().x * -m_pToolMgr->m_pOrthoCamera->Get_CameraPos().z),
		float(point.y) + CScrollView::GetScrollPos(1) - WINCY / 2.f + (m_pToolMgr->m_pOrthoCamera->Get_CameraPos().y * m_pToolMgr->m_pOrthoCamera->Get_CameraPos().z),
		0.f
	};

	vMouse.x /= m_pToolMgr->m_pOrthoCamera->Get_CameraPos().z * -1.f;
	vMouse.y /= m_pToolMgr->m_pOrthoCamera->Get_CameraPos().z;

	/*  Picking üũ  */

	if (FAILED(PickingCheck(vMouse, m_bPickingOn)))
		return;

	if (!m_pObjectTool->m_bOpen)
	{
		/*  ��ŷ�Ǽ� ���ٸ� ����  */
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
		/*  ��ŷ�Ǽ� ���ٸ� ����  */
		bool bErase = FALSE;
		for (auto& mapiter : m_pToolMgr->m_mapObjList)
		{
			for (list<Engine::CObj*>::iterator& listIter = mapiter.second.begin(); listIter != mapiter.second.end(); )
			{
				if (int(vMouse.x) == int((*listIter)->Get_CubeInfo().vPos.x)
					&& int(vMouse.y) == int((*listIter)->Get_CubeInfo().vPos.y))
				{
					// ����Ʈ ����
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
				// ����Ʈ ����
				iCountKey = 0;
				m_pObjectTool->m_ObjectListBox.ResetContent();
				for (auto& mapiter : m_pToolMgr->m_mapObjList)
				{
					for (list<Engine::CObj*>::iterator& listIter = mapiter.second.begin(); listIter != mapiter.second.end(); )
					{
						(*listIter)->Set_ObjKey(iCountKey);

						// ���� ������
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
	/*  ť�꼱�� nullptr �ּҴ���  */
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
