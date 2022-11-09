#include "stdafx.h"
#include "Tutorial.h"

#include "Export_Function.h"
#include "Engine_Include.h"
#include "Include.h"
#include "SceneSelector.h"

/************************ (SongSong) ************************/
/*  GameObject  */
#include "Cloud.h"
#include "Ship.h"
#include "PlayerTuto.h"
#include "Pier.h"
#include "Rocks.h"
#include "Water.h"

/************************ (SongSong) ************************/
/*  UI  */
#include "TutoCamera.h"
#include "StaticCamera.h"
#include "ColorCircle.h"
#include "Loading.h"
#include "Rain.h"
#include "Cgv.h"

CTutorial::CTutorial(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
	, m_pManagement(Engine::Get_Management())
	, m_pResourceMgr(Engine::Get_ResourceMgr())
{

}

CTutorial::~CTutorial(void)
{
	Release();
}

HRESULT CTutorial::Initialize(void)
{
	/************************ (SongSong) ************************/
	/*  Cube 로드  */
	CCubeMgr::GetInstance()->Set_CurStageCube(SCENEID::SC_TUTORIAL);

	//FAILED_CHECK_RETURN(Add_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Add_GameObject(), E_FAIL);
	FAILED_CHECK_RETURN(Add_UI(), E_FAIL);

	Engine::Get_InfoSubject()->AddData(DATA_LISTTUTOTOBJECT, &m_listTutoObject);

	return S_OK;
}

void CTutorial::Update(void)
{
	Engine::Get_InfoSubject()->Notify(DATA_LISTTUTOTOBJECT);

	Engine::CScene::Update();
	CCubeMgr::GetInstance()->Update();

	if (GetAsyncKeyState('1') & 0x8000 
		|| 55.f < CCameraObserver::GetInstance()->GetStaticCamera()->Get_TargetPos().x)
	{
		m_pManagement->SceneChange(CSceneSelector(SC_STAGE));
		CCubeMgr::GetInstance()->Set_CurCubeColor(Engine::CUBE_COLOR::CUBE_COLOR_END);
		return;
	}
}

void CTutorial::LateUpdate(void)
{
	CCubeMgr::GetInstance()->LateUpdate();
	Engine::CScene::LateUpdate();

}

void CTutorial::Render(void)
{
	if (Engine::CUBE_COLOR::CUBE_COLOR_END == CCubeMgr::GetInstance()->Get_CurCubeColor())
	{
		Engine::CGraphicDev::GetInstance()->GetDevice()->Clear(0, NULL,
			D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB(28, 28, 28),
			1.f, // Z버퍼의 초기화 값
			0);
	}
	else
	{
		Engine::CGraphicDev::GetInstance()->GetDevice()->Clear(0, NULL,
			D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			CCubeMgr::GetInstance()->GetBackColor(),
			1.f, // Z버퍼의 초기화 값
			0);
	}
	//CCubeMgr::GetInstance()->GetBackColor(),


	CCubeMgr::GetInstance()->RenderOutsideWorld();
	Engine::CScene::Render();

}

void CTutorial::Release(void)
{
	Engine::CScene::Release();
}

HRESULT CTutorial::Add_Environment(void)
{
	/*Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	m_mapLayer.emplace(LAYER_ENVIRONMENT, pLayer);*/

	return S_OK;
}

HRESULT CTutorial::Add_GameObject(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	/************************ (SongSong) ************************/
	/*  구름  */
	pGameObject = CCloud::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Cloud", pGameObject);
	m_listTutoObject.push_back(pGameObject);

	/************************ (SongSong) ************************/
	/*  배  */
	pGameObject = CShip::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Ship", pGameObject);
	m_listTutoObject.push_back(pGameObject);

	/************************ (SongSong) ************************/
	/*  플레이어  */
	pGameObject = m_pPlayerTuto = CPlayerTuto::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Player", pGameObject);

	/************************ (SongSong) ************************/
	/*  교각  */
	pGameObject = CPier::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Pier", pGameObject);
	m_listTutoObject.push_back(pGameObject);

	/************************ (SongSong) ************************/
	/*  돌  */
	pGameObject = CRocks::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Rocks", pGameObject);
	m_listTutoObject.push_back(pGameObject);

	/************************ (SongSong) ************************/
	/*  물  */
	pGameObject = CWater::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Water", pGameObject);
	m_listTutoObject.push_back(pGameObject);

	/************************ (SongSong) ************************/
	/*  비  */
	pGameObject = CRain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Rain", pGameObject);
	m_listTutoObject.push_back(pGameObject);

	m_mapLayer.emplace(LAYER_GAMEOBJECT, pLayer);

	return S_OK;
}

HRESULT CTutorial::Add_UI(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	auto	iter = m_mapLayer.find(LAYER_GAMEOBJECT);
	if (iter == m_mapLayer.end())
		return E_FAIL;

	Engine::CGameObject* pPlayer = nullptr;
	pPlayer = iter->second->FindObject(L"Player");
	m_pPlayerTuto = dynamic_cast<CPlayerTuto*>(pPlayer);
	NULL_CHECK_RETURN(pPlayer, E_FAIL);

	const Engine::CComponent*	pTransform = pPlayer->FindComponent(L"Transform");
	NULL_CHECK_RETURN(pTransform, E_FAIL);

	//pGameObject = CTutoCamera::Create(m_pGraphicDev, dynamic_cast<const Engine::CTransform*>(pTransform));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->AddObject(L"StaticCamera", pGameObject);

	pGameObject = CStaticCamera::Create(m_pGraphicDev, dynamic_cast<const Engine::CTransform*>(pTransform));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"StaticCamera", pGameObject);

	/************************ (SongSong) ************************/
	/*  로딩  */
	pGameObject = CLoading::Create(m_pGraphicDev);
	dynamic_cast<CLoading*>(pGameObject)->SetComplete();
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Loading", pGameObject);

	//
	pGameObject = CColorCircle::Create(m_pGraphicDev, pPlayer);
	m_pColorCircle = dynamic_cast<CColorCircle*>(pGameObject);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"ColorCircle", pGameObject);

	/************************ (SongSong) ************************/
	/*  Cgv  */
	pGameObject = CCgv::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Cgv", pGameObject);
	m_listTutoObject.push_back(pGameObject);

	m_mapLayer.emplace(LAYER_UI, pLayer);
	return S_OK;
}

CTutorial* CTutorial::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTutorial*		pInstance = new CTutorial(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Engine::Safe_Delete(pInstance);

	return pInstance;
}