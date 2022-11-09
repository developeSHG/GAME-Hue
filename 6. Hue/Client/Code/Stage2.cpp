#include "stdafx.h"
#include "Stage2.h"

#include "Export_Function.h"
#include "Engine_Include.h"
#include "Include.h"

/************************ (SongSong) ************************/
/*  GameObject  */
#include "Player.h"
#include "Cube.h"

/************************ (SongSong) ************************/
/*  UI  */
#include "StaticCamera.h"
#include "ColorCircle.h"
#include "CubeMgr.h"
#include "Stage3.h"
#include "SceneSelector.h"

CStage2::CStage2(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
	, m_pManagement(Engine::Get_Management())
	, m_pResourceMgr(Engine::Get_ResourceMgr())
{

}

CStage2::~CStage2(void)
{
	Release();
}

HRESULT CStage2::Initialize(void)
{
	/************************ (SongSong) ************************/
	/*  Cube 로드  */
	CCubeMgr::GetInstance()->Set_CurStageCube(SCENEID::SC_STAGE2);
	CCubeMgr::GetInstance()->Set_CurCubeColor(Engine::CUBE_COLOR::CUBE_COLOR_END);
	CCubeMgr::GetInstance()->ResetSaveCount();
	FAILED_CHECK_RETURN(Add_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Add_GameObject(), E_FAIL);
	FAILED_CHECK_RETURN(Add_UI(), E_FAIL);


	m_SubViewPort.X = WINCX;
	m_SubViewPort.Y = 0;
	m_SubViewPort.Width = WINCX / 6 * 2 + 10;
	m_SubViewPort.Height = WINCY / 6 * 2;
	m_SubViewPort.MinZ = 0.0f;
	m_SubViewPort.MaxZ = 1.0f;

	m_bIsRenderViewport = false;
	m_bIsResizeViewport = false;
	m_bIsMoveUp = false;
	m_bIsEnter = false;

	Engine::Get_SoundMgr()->StopAll();
	Engine::Get_SoundMgr()->PlayBGM(L"CubeFlying.wav", true);
	//Engine::Get_SoundMgr()->MyPlaySound(L"CubeRotation.wav", Engine::CHANNELID::EFFECT);
	Engine::Get_SoundMgr()->MyPlaySound(L"CubeRotation.wav", Engine::CHANNELID::EFFECT);
	Engine::Get_SoundMgr()->MyPlaySound(L"CubeRotation.wav", Engine::CHANNELID::EFFECT);
	Engine::Get_SoundMgr()->MyPlaySound(L"IntroBGM.wav", Engine::CHANNELID::RAIN);

	CCubeMgr::GetInstance()->Set_IsRender(FALSE);

	return S_OK;
}

void CStage2::Update(void)
{
	if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_RETURN))
	{
		m_pManagement->SceneChange(CSceneSelector(SC_STAGE3));
		return;
	}

	/************************ (SongSong) ************************/
	/*  빗소리 출력  */
	static float fTime = 0.f;
	fTime += 1 * GET_TIME;
	if (90.f <= fTime)
	{
		Engine::Get_SoundMgr()->StopSound(Engine::CHANNELID::RAIN);
		Engine::Get_SoundMgr()->MyPlaySound(L"IntroBGM.wav", Engine::CHANNELID::RAIN);
		fTime = 0.f;
	}


	Engine::CScene::Update();

	CCubeMgr::GetInstance()->Update();
	//CObjectMgr::GetInstance()->Update();

	if (*CCameraObserver::GetInstance()->GetNarrativeMoving() == FALSE
		&& *CCameraObserver::GetInstance()->GetNarrativeView() == NARRATIVE_VIEW::FIRST_PERSON)
	{
		if (!m_bIsEnter)
		{
			m_bIsRenderViewport = true;
		}

		m_bIsResizeViewport = true;
	
		if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_TAB))
		{
			if (m_bIsRenderViewport)
				m_bIsRenderViewport = false;
			else
				m_bIsRenderViewport = true;
		}
	}
	else
	{
		m_SubViewPort.X = WINCX;
		m_SubViewPort.Y = 0;
		m_SubViewPort.Width = WINCX / 6 * 2 + 10;
		m_SubViewPort.Height = WINCY / 6 * 2;
		m_SubViewPort.MinZ = 0.0f;
		m_SubViewPort.MaxZ = 1.0f;
		m_bIsEnter = false;
		CCubeMgr::GetInstance()->Set_IsOriginPos(true);
	}

	if (m_bIsResizeViewport)
		UpdateViewportPos();
}

void CStage2::LateUpdate(void)
{
	Engine::CScene::LateUpdate();

	CCubeMgr::GetInstance()->LateUpdate();

	D3DVIEWPORT9 wholeViewPort;

	wholeViewPort.X = 0;
	wholeViewPort.Y = 0;
	wholeViewPort.Width = WINCX;
	wholeViewPort.Height = WINCY;
	wholeViewPort.MinZ = 0.0f;
	wholeViewPort.MaxZ = 1.0f;

	Engine::CGraphicDev::GetInstance()->GetDevice()->SetViewport(&wholeViewPort);
}

void CStage2::Render(void)
{

	CCubeMgr::GetInstance()->RenderOutsideWorld();
	//CObjectMgr::GetInstance()->RenderOutsideWorld();
	Engine::CScene::Render();

	Engine::CGraphicDev::GetInstance()->GetDevice()->EndScene();

	if (*CCameraObserver::GetInstance()->GetNarrativeMoving() == FALSE
		&& *CCameraObserver::GetInstance()->GetNarrativeView() == NARRATIVE_VIEW::FIRST_PERSON)
	{
		if (m_bIsRenderViewport)
		{
			Engine::CGraphicDev::GetInstance()->GetDevice()->SetViewport(&m_SubViewPort);

			Engine::CGraphicDev::GetInstance()->GetDevice()->Clear(0, NULL,
				D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
				CCubeMgr::GetInstance()->GetBackColor(),
				1.f, // Z버퍼의 초기화 값
				0);
		
			//m_pGraphicDev->SetViewport(&leftTopViewPort);
			m_pGraphicDev->BeginScene();

			D3DXMATRIX	 mView, mProj;
			D3DXMATRIX	 mIdentity;
			D3DXMatrixIdentity(&mIdentity);

			D3DXVECTOR3 vecEye = m_pPlayer->Get_PlayerInfoCom()->m_vPos;
			vecEye.z -= 10.5f;
			D3DXVECTOR3 vecAt = m_pPlayer->Get_PlayerInfoCom()->m_vPos;
			D3DXMatrixLookAtLH(&mView, &vecEye,
				&vecAt, &D3DXVECTOR3(0.f, 1.f, 0.f));
			m_pGraphicDev->SetTransform(D3DTS_VIEW, &mView);
			m_pGraphicDev->SetTransform(D3DTS_PROJECTION, CCameraObserver::GetInstance()->GetProj());

			CCubeMgr::GetInstance()->RenderViewportCube();
			//CObjectMgr::GetInstance()->RenderViewportCube();
			m_pPlayer->ViewportRender();

		}
	}

}

void CStage2::Release(void)
{
	Engine::CScene::Release();
}



HRESULT CStage2::Add_Environment(void)
{
	/*Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	m_mapLayer.emplace(LAYER_ENVIRONMENT, pLayer);*/

	return S_OK;
}

HRESULT CStage2::Add_GameObject(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = m_pPlayer = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Player", pGameObject);

	CCubeMgr::GetInstance()->SetPlayer(m_pPlayer);

	m_mapLayer.emplace(LAYER_GAMEOBJECT, pLayer);

	return S_OK;
}

HRESULT CStage2::Add_UI(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	auto	iter = m_mapLayer.find(LAYER_GAMEOBJECT);
	if (iter == m_mapLayer.end())
		return E_FAIL;

	Engine::CGameObject* pPlayer = nullptr;
	pPlayer = iter->second->FindObject(L"Player");
	m_pPlayer = dynamic_cast<CPlayer*>(pPlayer);
	NULL_CHECK_RETURN(pPlayer, E_FAIL);

	const Engine::CComponent*	pTransform = pPlayer->FindComponent(L"Transform");
	NULL_CHECK_RETURN(pTransform, E_FAIL);

	pGameObject = CStaticCamera::Create(m_pGraphicDev, dynamic_cast<const Engine::CTransform*>(pTransform));
	dynamic_cast<CStaticCamera*>(pGameObject)->SetPlayer(m_pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"StaticCamera", pGameObject);

	pGameObject = CColorCircle::Create(m_pGraphicDev, pPlayer);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"ColorCircle", pGameObject);


	m_mapLayer.emplace(LAYER_UI, pLayer);
	return S_OK;
}

void CStage2::UpdateViewportPos(void)
{
	m_SubViewPort.X -= 300.f * GET_TIME;

	if (m_SubViewPort.X < WINCX / 6 * 4)
	{
		m_SubViewPort.X = WINCX / 6 * 4;
		m_bIsResizeViewport = false;
		m_bIsMoveUp = true;
		m_bIsEnter = true;
	}

}

CStage2* CStage2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage2*		pInstance = new CStage2(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Engine::Safe_Delete(pInstance);

	return pInstance;
}