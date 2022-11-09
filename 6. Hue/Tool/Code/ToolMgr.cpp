#include "stdafx.h"
#include "ToolMgr.h"
#include "Terrain.h"
#include "Export_Function.h"
#include "OrthoCamera.h"

#include "CubeBrick.h"
#include "CubeDeco.h"
#include "CubeSkull.h"
#include "CubeCrate.h"
#include "CubePlatform.h"
#include "CubeRope.h"
#include "CubeLaser.h"
#include "CubeSpikes.h"
#include "CubeStairs.h"
#include "CubeRock.h"
#include "CubeBalloon.h"

#include "MapTool.h"
#include "ObjectTool.h"
#include "ToolView.h"

#include "DecoObj.h"
#include "Chandlier.h"
#include "Door.h"
#include "DoorLock.h"
#include "Etc.h"
#include "KeyMgr.h"
IMPLEMENT_SINGLETON(CToolMgr)

CToolMgr::CToolMgr()
	: m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pKeyMgr(Engine::Get_KeyMgr())
	, m_pTimeMgr(Engine::Get_TimeMgr())
{
	m_eToolID = TOOL_ID::MAP_TOOL;
}


CToolMgr::~CToolMgr()
{
	Release();
}

HRESULT CToolMgr::Initialize(HWND _hWnd)
{
	m_hWnd = _hWnd;
	m_pGraphicDev = Engine::Get_GraphicDev();

	m_pGraphicDev->InitGraphicDev(Engine::CGraphicDev::MODE_WIN,
		m_hWnd,
		WINCX,
		WINCY);

	SetUpLight();

	m_pDevice = m_pGraphicDev->GetDevice();
	m_pOrthoCamera = COrthoCamera::Create(m_pDevice);
	Setup_Proj();

	FAILED_CHECK_RETURN(ResourceForStage(), E_FAIL);

	m_pMapTool->m_ComboBoxCubeList.ResetContent();
	m_pMapTool->m_ListBoxCubeNum.ResetContent();

	return S_OK;
}

void CToolMgr::SetUpLight()
{
	D3DLIGHT9 dir;
	ZeroMemory(&dir, sizeof(D3DLIGHT9));
	dir.Type = D3DLIGHT_DIRECTIONAL;
	dir.Diffuse = WHITE;
	dir.Specular = WHITE * 0.3f;
	dir.Ambient = WHITE * 1.f;
	dir.Direction = D3DXVECTOR3(0.f, 0.f, 1.f);
	Engine::CGraphicDev::GetInstance()->GetDevice()->SetLight(0, &dir);				//★
	Engine::CGraphicDev::GetInstance()->GetDevice()->LightEnable(0, true);			//★
	Engine::CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	Engine::CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
}

void CToolMgr::Setup_Proj()
{
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj,
		D3DX_PI * 0.5f,
		(float)WINCX / WINCY,
		1.f,
		1000.f);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &proj);

	m_pOrthoCamera->m_vecCameraPos.z = -20.f;
}

void CToolMgr::Setup_ClieProj()
{
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj,
		D3DXToRadian(45.f),
		(float)WINCX / WINCY,
		1.f,
		1000.f);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &proj);

	m_pOrthoCamera->m_vecCameraPos.z = -10.f;
}

void CToolMgr::Update()
{
	if (GetAsyncKeyState(VK_SPACE))
		Setup_ClieProj();
	else
		Setup_Proj();

	m_pKeyMgr->Update();
	m_pTimeMgr->Update();

	m_pOrthoCamera->Update();

	if (m_pTerrain)
		m_pTerrain->Update();

	for (auto mapiter : m_mapCubeVec)
	{
		for (auto iter : mapiter.second)
		{
			iter->Update();
		}
	}
	for (auto mapiter : m_mapObjList)
	{
		for (auto iter : mapiter.second)
		{
			iter->Update();
		}
	}

	/************************ (SongSong) ************************/
	/*  Platform 지우기  */
	if (Engine::CKeyMgr::GetInstance()->KeyDown(Engine::KEY_E))
	{
		auto& iter_find = m_mapCubeVec.find(L"CubePlatform");

		list<Engine::CCube*>& ListPlatform = iter_find->second;

		if (0 != ListPlatform.size())
		{
			Engine::Safe_Delete(ListPlatform.back());
			ListPlatform.erase(--ListPlatform.end());
		}
	}
}

void CToolMgr::Render()
{
	m_pDevice->Clear(0, NULL,
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(255, 0, 0, 255),
		1.f, // Z버퍼의 초기화 값
		0);

	m_pDevice->BeginScene();

	//m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//툴에 따라 render 변경.
	if (m_pTerrain)
		m_pTerrain->Render();

	for (auto mapiter : m_mapCubeVec)
	{
		for (auto iter : mapiter.second)
		{
			iter->Render();
		}
	}

	for (auto mapiter : m_mapObjList)
	{
		for (auto iter : mapiter.second)
		{
			iter->Render();
		}
	}

	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, m_hWnd, NULL);
}

void CToolMgr::Release(void)
{
	for (auto mapiter : m_mapCubeVec)
	{
		for (auto iter : mapiter.second)
		{
			Safe_Delete(iter);
		}
		mapiter.second.clear();
	}
	m_mapCubeVec.clear();

	for (auto mapiter : m_mapObjList)
	{
		for (auto iter : mapiter.second)
		{
			Safe_Delete(iter);
		}
		mapiter.second.clear();
	}
	m_mapObjList.clear();

	Engine::Safe_Delete(m_pOrthoCamera);
	Engine::Safe_Delete(m_pTerrain);
	Engine::Safe_Single_Destroy(m_pTimeMgr);
	Engine::Safe_Single_Destroy(m_pResourceMgr);
	Engine::Safe_Single_Destroy(m_pKeyMgr);
	Engine::Safe_Single_Destroy(m_pGraphicDev);
}

void CToolMgr::Render_Begin()
{
	D3DXVECTOR3 m_vecCameraPos = D3DXVECTOR3(0.f, 0.f, -5.f);
	D3DXVECTOR3 m_vecLookAt = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 m_vecUp = D3DXVECTOR3(0.f, 1.f, 0.f);

	D3DXMATRIX v;
	D3DXMatrixLookAtLH(&v, &m_vecCameraPos, &m_vecLookAt, &m_vecUp);
	m_pDevice->SetTransform(D3DTS_VIEW, &v);

	/************************ (SongSong) ************************/
	/*  직교투영  */
	D3DXMATRIX proj;
	D3DXMatrixOrthoLH(&proj,
		2.f,
		2.f,
		1.f,
		1000.f);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &proj);

	m_pDevice->Clear(0, NULL,
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(255, 255, 0, 0),
		1.f, // Z버퍼의 초기화 값
		0);

	m_pDevice->BeginScene();

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0X00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

}

void CToolMgr::Render_End(HWND hWnd)
{
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, hWnd, NULL);

	Setup_Proj();
}

HRESULT CToolMgr::ResourceForStage(void)
{
	// Rc 버퍼
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Buffer(m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_RCTEX,
		L"Buffer_RcTex"), E_FAIL);

	// Cube 버퍼
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Buffer(m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_CUBE_NORMAL,
		L"Buffer_CubeNormal"), E_FAIL);

	// Cube 버퍼
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Buffer(m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_CUBE_FRONT,
		L"Buffer_CubeFront"), E_FAIL);

	// Cube 버퍼
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Buffer(m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_CUBE_CullB,
		L"Buffer_CubeCullB"), E_FAIL);

	//CullBackAndB  -> SKULL
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Buffer(m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_CUBE_CullBackAndB,
		L"Buffer_CubeCullBackAndB"), E_FAIL);

	// Cube 버퍼
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Buffer(m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_CUBE_CullL,
		L"Buffer_CubeCullL"), E_FAIL);

	// Cube 버퍼
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Buffer(m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_CUBE_CullR,
		L"Buffer_CubeCullR"), E_FAIL);

	// Cube 버퍼
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Buffer(m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_CUBE_CullLR,
		L"Buffer_CubeCullLR"), E_FAIL);

	m_pMapTool->ResourceForStage();
	m_pObjectTool->ResourceForStage();
	return S_OK;
}

void CToolMgr::Set_Handle(HWND _hWnd)
{
	m_hWnd = _hWnd;
}

void CToolMgr::CreateTerrain(int vtxX, int vtxZ, int vtxItv)
{
	m_pTerrain = CTerrain::Create(m_pDevice, vtxX, vtxZ, vtxItv);
}

void CToolMgr::CreateCube(const wstring& wstrCubeKey, Engine::CUBE_INFO& _tCubeInfo)
{
	auto& iter = m_mapCubeVec.find(wstrCubeKey);

	if (iter == m_mapCubeVec.end())
	{
		m_mapCubeVec[wstrCubeKey] = list<Engine::CCube*>();
	}

	Engine::CCube*	pCube = nullptr;

	if (L"CubeDeco" == wstrCubeKey)
		pCube = CCubeDeco::Create(m_pDevice, _tCubeInfo);
	else if (L"CubeBrick" == wstrCubeKey)
		pCube = CCubeBrick::Create(m_pDevice, _tCubeInfo);
	else if (L"CubeCrate" == wstrCubeKey)
		pCube = CCubeCrate::Create(m_pDevice, _tCubeInfo);
	else if (L"CubePlatform" == wstrCubeKey)
		pCube = CCubePlatform::Create(m_pDevice, _tCubeInfo);
	else if (L"CubeSkull" == wstrCubeKey)
		pCube = CCubeSkull::Create(m_pDevice, _tCubeInfo);
	else if (L"CubeRope" == wstrCubeKey)
		pCube = CCubeRope::Create(m_pDevice, _tCubeInfo);
	else if (L"CubeLaser" == wstrCubeKey)
		pCube = CCubeLaser::Create(m_pDevice, _tCubeInfo);
	else if (L"CubeSpikes" == wstrCubeKey)
		pCube = CCubeSpikes::Create(m_pDevice, _tCubeInfo);
	else if (L"CubeStairs" == wstrCubeKey)
		pCube = CCubeStairs::Create(m_pDevice, _tCubeInfo);
	else if (L"CubeRock" == wstrCubeKey)
		pCube = CCubeRock::Create(m_pDevice, _tCubeInfo);
	else if (L"CubeBalloon" == wstrCubeKey)
		pCube = CCubeBalloon::Create(m_pDevice, _tCubeInfo);

	switch (_tCubeInfo.tColor)
	{
	case Engine::CUBE_COLOR::CUBE_RED:
		pCube->SetMaterial(RED);
		break;
	case Engine::CUBE_COLOR::CUBE_ORANGE:
		pCube->SetMaterial(ORANGE);
		break;
	case Engine::CUBE_COLOR::CUBE_GREEN:
		pCube->SetMaterial(GREEN);
		break;
	case Engine::CUBE_COLOR::CUBE_YELLOW:
		pCube->SetMaterial(YELLOW);
		break;
	case Engine::CUBE_COLOR::CUBE_BLUE:
		pCube->SetMaterial(BLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_SKYBLUE:
		pCube->SetMaterial(SKYBLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_PINK:
		pCube->SetMaterial(PINK);
		break;
	case Engine::CUBE_COLOR::CUBE_PURPLE:
		pCube->SetMaterial(PURPLE);
		break;
	case Engine::CUBE_COLOR::CUBE_COLOR_END:
		pCube->SetMaterial(DARKGREY);
		break;
	}

	m_mapCubeVec[wstrCubeKey].push_back(pCube);

	m_pToolView->m_pPickExistCube = pCube;
}

void CToolMgr::CreateObject(const wstring & wstrObjKey, Engine::OBJ_INFO & _tObjInfo)
{
	auto& iter = m_mapObjList.find(wstrObjKey);

	if (iter == m_mapObjList.end())
	{
		m_mapObjList[wstrObjKey] = list<Engine::CObj*>();
	}

	Engine::CObj*	pObj = nullptr;

	if (L"Object_Axe" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_Boat" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_Boss" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_Candle" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_Chandlier" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_Door" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_DoorLock" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_Etc" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_FirePot" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_Grass" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_Key" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_Lantern" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_Mushroom" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_Pipe" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_PotPlant" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_Rope" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_Statue" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_Topiary" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_Tree" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_Trinket" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_Trophie" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);
	else if (L"Object_Vase" == wstrObjKey)
		pObj = CDecoObj::Create(m_pDevice, _tObjInfo);

	m_mapObjList[wstrObjKey].push_back(pObj);
	m_pToolView->m_pPickExistObj = nullptr;
}
