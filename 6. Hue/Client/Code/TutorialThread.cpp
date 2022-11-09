#include "stdafx.h"
#include "TutorialThread.h"

#include "Export_Function.h"
#include "Engine_Include.h"
#include "Include.h"
#include "SceneSelector.h"
#include "GraphicDev.h"
#include "ResourceMgr.h"

#include <process.h>

/************************ (SongSong) ************************/
/*  GameObject  */
#include "Cloud.h"
#include "Ship.h"
#include "PlayerTuto.h"
#include "Water.h"

/************************ (SongSong) ************************/
/*  UI  */
#include "StaticCamera.h"
#include "Loading.h"
#include "Rain.h"
#include "Cgv.h"

bool CTutorialThread::m_bLoadingComplete = false;

CTutorialThread::CTutorialThread(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
	, m_pManagement(Engine::Get_Management())
	, m_pResourceMgr(Engine::Get_ResourceMgr())
{

}

CTutorialThread::~CTutorialThread(void)
{
	Release();
}

HRESULT CTutorialThread::Initialize(void)
{
	//Engine::Get_SoundMgr()->PlayBGM(L"Home.wav");
	Engine::Get_SoundMgr()->PlayBGM(L"IntroBGM.wav");
	//Engine::Get_SoundMgr()->MyPlaySound(L"IntroBGM.wav", Engine::CHANNELID::BGM);
	Engine::Get_SoundMgr()->MyPlaySound(L"Home.wav", Engine::CHANNELID::INTERACT);
	m_hLoadingThread = (HANDLE)_beginthreadex(
		nullptr, 0, LoadingTextureByThread, this, 0, nullptr);

	if (INVALID_HANDLE_VALUE == m_hLoadingThread)
		return E_FAIL;

	InitializeCriticalSection(&m_CriticalSection);

	//FAILED_CHECK_RETURN(Add_Environment(), E_FAIL);
	FAILED_CHECK_RETURN(Add_GameObject(), E_FAIL);
	FAILED_CHECK_RETURN(Add_UI(), E_FAIL);

	return S_OK;
}

void CTutorialThread::Update(void)
{
	Engine::CScene::Update();

	//D3DXMATRIX mView;
	//D3DXMatrixLookAtLH(&mView, &CCameraObserver::GetInstance()->GetStaticCamera()->Get_Eye(),
	//	&CCameraObserver::GetInstance()->GetStaticCamera()->Get_At(), &CCameraObserver::GetInstance()->GetStaticCamera()->Get_Up());
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &mView);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, CCameraObserver::GetInstance()->GetProj());

	if (false == m_bLoadingComplete)
	{
		return;
	}
	else
	{

	}

	WaitForSingleObject(m_hLoadingThread, INFINITE);

	//if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		m_pManagement->SceneChange(CSceneSelector(SC_TUTORIAL));
		//return;
	}
}

void CTutorialThread::LateUpdate(void)
{
	Engine::CScene::LateUpdate();
}

void CTutorialThread::Render(void)
{
	Engine::CGraphicDev::GetInstance()->GetDevice()->Clear(0, NULL,
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(28, 28, 28),
		1.f, // Z버퍼의 초기화 값
		0);

	Engine::CScene::Render();
}

void CTutorialThread::Release(void)
{
	Engine::CScene::Release();

	CloseHandle(m_hLoadingThread);
	DeleteCriticalSection(&m_CriticalSection);
}

HRESULT CTutorialThread::Add_Environment(void)
{
	/*Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	m_mapLayer.emplace(LAYER_ENVIRONMENT, pLayer);*/

	return S_OK;
}

HRESULT CTutorialThread::Add_GameObject(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	/************************ (SongSong) ************************/
	/*  구름  */
	pGameObject = CCloud::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Cloud", pGameObject);

	/************************ (SongSong) ************************/
	/*  플레이어  */
	pGameObject = m_pPlayerTuto = CPlayerTuto::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Player", pGameObject);

	/************************ (SongSong) ************************/
	/*  배  */
	pGameObject = CShip::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Ship", pGameObject);

	/************************ (SongSong) ************************/
	/*  물  */
	pGameObject = CWater::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Water", pGameObject);

	m_mapLayer.emplace(LAYER_GAMEOBJECT, pLayer);

	return S_OK;
}

HRESULT CTutorialThread::Add_UI(void)
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

	pGameObject = CStaticCamera::Create(m_pGraphicDev, dynamic_cast<const Engine::CTransform*>(pTransform));
	//dynamic_cast<CStaticCamera*>(pGameObject)->SetPlayer(m_pPlayerTuto);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"StaticCamera", pGameObject);

	/************************ (SongSong) ************************/
	/*  비  */
	pGameObject = CRain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Rain", pGameObject);

	/************************ (SongSong) ************************/
	/*  로딩  */
	pGameObject = CLoading::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Loading", pGameObject);

	/************************ (SongSong) ************************/
	/*  Cgv  */
	pGameObject = CCgv::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Cgv", pGameObject);

	m_mapLayer.emplace(LAYER_UI, pLayer);
	return S_OK;
}

HRESULT CTutorialThread::BufferForStage(void)
{
	// CircleCol 버퍼
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Buffer(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_RCCOL,
		L"Buffer_RcCol"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Buffer(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_CIRCLECOL,
		L"Buffer_CircleCol"), E_FAIL);

	// Rcy 버퍼(ColorCircle)
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Buffer(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_RCYTEX,
		L"Buffer_RcyTex"), E_FAIL);

	// SkyBox 버퍼
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Buffer(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_CUBE_SkyBox,
		L"Buffer_Cube_SkyBox"), E_FAIL);

	// Cube 버퍼
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Buffer(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_CUBE_NORMAL,
		L"Buffer_CubeNormal"), E_FAIL);

	// Cube 버퍼
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Buffer(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_CUBE_CullB,
		L"Buffer_CubeCullB"), E_FAIL);

	// Cube 버퍼
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Buffer(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_CUBE_CullL,
		L"Buffer_CubeCullL"), E_FAIL);

	// Cube 버퍼
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Buffer(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_CUBE_CullR,
		L"Buffer_CubeCullR"), E_FAIL);

	// Cube 버퍼
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Buffer(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_CUBE_CullLR,
		L"Buffer_CubeCullLR"), E_FAIL);

	//CullBackAndB  -> SKULL
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Buffer(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_CUBE_CullBackAndB,
		L"Buffer_CubeCullBackAndB"), E_FAIL);

	return S_OK;
}

HRESULT CTutorialThread::TextureForTutorial(void)
{
	// Pier
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Pier",
		L"../Bin/HueResources/Tutorial/Pier/Pier_0%d.png",
		3),
		E_FAIL);

	// Pier
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Rocks",
		L"../Bin/HueResources/Tutorial/Rocks/Rocks_0%d.png",
		1),
		E_FAIL);

	return S_OK;
}

HRESULT CTutorialThread::TextureForStage(void)
{
	// SavePoint
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_SavePoint",
		L"../Bin/HueResources/Object/Bulb/SavePoint_0%d.png",
		2),
		E_FAIL);

	// UI ColorHP
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_ColorHP",
		L"../Bin/HueResources/UI/ColorHpBar/ColorHpBar_0%d.png",
		3),
		E_FAIL);

	// ColorCircle 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_ColorCircle",
		L"../Bin/HueResources/UI/ColorChunk/ColorChunk_0%d.png",
		1),
		E_FAIL);

	//dust
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Particle_Dust",
		L"../../Tool/Bin/HueResources/Particle/Dust/Dust_0%d.png",
		6),
		E_FAIL);

	//dustmask
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Particle_DustMask",
		L"../../Tool/Bin/HueResources/Particle/Dust_mask/Dust_mask_0%d.png",
		6),
		E_FAIL);

	// Hand (Idle)
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_HandIdle",
		L"../Bin/HueResources/UI/Hand/Idle/Hand%d.png",
		21),
		E_FAIL);

	// Hand (Left)
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_HandLeft",
		L"../Bin/HueResources/UI/Hand/Left/Left%d.png",
		42),
		E_FAIL);

	// Hand (Right)
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_HandRight",
		L"../Bin/HueResources/UI/Hand/Right/Right%d.png",
		28),
		E_FAIL);

	//Mouse
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Mouse",
		L"../Bin/HueResources/UI/Cursor/Cursor_0%d.png",
		3),
		E_FAIL);

	//Viewport
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Outline",
		L"../Bin/HueResources/Background/Back/Viewport_0%d.png",
		3),
		E_FAIL);

	// SkyBox 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_SkyBox",
		L"../Bin/HueResources/Background/Back/SkyBox_0%d.png",
		9),
		E_FAIL);

	// SKULL
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeObstacle_Skull",
		L"../Bin/HueResources/Obstacle/Skull/Skull_0%d.png",
		1),
		E_FAIL);

	// Spikes
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeObstacle_Spikes",
		L"../Bin/HueResources/Obstacle/Spikes/Spikes_0%d.png",
		2),
		E_FAIL);

	// Spikes
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeObstacle_Wire",
		L"../Bin/HueResources/Obstacle/Wire/Wire_0%d.png",
		1),
		E_FAIL);

	// Stairs
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeObstacle_Stairs",
		L"../Bin/HueResources/Background/Stairs/Stairs_0%d.png",
		2),
		E_FAIL);

	// Rock
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeObstacle_Rock",
		L"../Bin/HueResources/Obstacle/ColorRock/ColorRock_0%d.png",
		3),
		E_FAIL);

	// Balloon
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeObstacle_Balloon",
		L"../Bin/HueResources/Obstacle/Balloon/Balloon_0%d.png",
		1),
		E_FAIL);

	// Basic, Deco 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeDeco_Aztec",
		L"../Bin/HueResources/Box/Aztec/Aztec_0%d.png",
		4),
		E_FAIL);

	// Basic, Deco 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeDeco_Brick",
		L"../Bin/HueResources/Box/Brick/Brick_0%d.png",
		4),
		E_FAIL);

	// Basic, Deco 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeDeco_Bricks",
		L"../Bin/HueResources/Box/Bricks/Bricks_0%d.png",
		4),
		E_FAIL);

	// Basic, Deco 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeDeco_Cobbles",
		L"../Bin/HueResources/Box/Cobbles/Cobbles_0%d.png",
		6),
		E_FAIL);

	// Basic, Deco 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeDeco_RockPool",
		L"../Bin/HueResources/Box/RockPool/RockPool_0%d.png",
		5),
		E_FAIL);

	// Basic, Deco 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeDeco_Square",
		L"../Bin/HueResources/Box/Square/Square_0%d.png",
		9),
		E_FAIL);

	// Basic, Deco 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeDeco_Vine",
		L"../Bin/HueResources/Box/Vine/Vine_0%d.png",
		5),
		E_FAIL);

	// Basic, Deco 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeDeco_WireScenery",
		L"../Bin/HueResources/Box/WireScenery/WireScenery_0%d.png",
		5),
		E_FAIL);

	// Color 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeColor_BrickColor",
		L"../Bin/HueResources/Box/BrickColor/BrickColor_0%d.png",
		4),
		E_FAIL);

	// Color 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeColor_Crate",
		L"../Bin/HueResources/Box/Crate/Crate_0%d.png",
		2),
		E_FAIL);

	// Color 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeColor_CrateLarge",
		L"../Bin/HueResources/Box/CrateLarge/CrateLarge_0%d.png",
		1),
		E_FAIL);

	// Color 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeColor_Platform",
		L"../Bin/HueResources/Obstacle/MovingPlatform/MovingPlatform_0%d.png",
		1),
		E_FAIL);

	// Color 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeColor_CrateTall",
		L"../Bin/HueResources/Box/CrateTall/CrateTall_0%d.png",
		2),
		E_FAIL);

	// *** Object *** 
	// Axe 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Axe",
		L"../Bin/HueResources/Object/Axe/Axe_0%d.png",
		1),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Battery",
		L"../Bin/HueResources/Obstacle/Battery/Battery_0%d.png",
		1),
		E_FAIL);

	// Boat 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Boat",
		L"../Bin/HueResources/Object/Boat/Boat_0%d.png",
		2),
		E_FAIL);

	// Boss 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Boss",
		L"../Bin/HueResources/Object/Boss/Boss_0%d.png",
		1),
		E_FAIL);

	// Candle 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Candle",
		L"../Bin/HueResources/Object/Candle/Candle_0%d.png",
		4),
		E_FAIL);

	// Chandlier 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Chandlier",
		L"../Bin/HueResources/Object/Chandlier/Chandlier_0%d.png",
		1),
		E_FAIL);

	// Door 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Door",
		L"../Bin/HueResources/Object/Door/Door_0%d.png",
		1),
		E_FAIL);

	// DoorLock 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_DoorLock",
		L"../Bin/HueResources/Object/DoorLock/DoorLock_0%d.png",
		1),
		E_FAIL);

	// Etc 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Etc",
		L"../Bin/HueResources/Object/Etc/Etc_0%d.png",
		6),
		E_FAIL);

	// FirePot 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_FirePot",
		L"../Bin/HueResources/Object/FirePot/FirePot_0%d.png",
		7),
		E_FAIL);

	// Grass 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Grass",
		L"../Bin/HueResources/Object/Grass/Grass_0%d.png",
		8),
		E_FAIL);

	// Key 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Key",
		L"../Bin/HueResources/Object/Key/Key_0%d.png",
		1),
		E_FAIL);

	// Lantern 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Lantern",
		L"../Bin/HueResources/Object/Lantern/Lantern_0%d.png",
		3),
		E_FAIL);

	// Mushroom 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Mushroom",
		L"../Bin/HueResources/Object/Mushroom/Mushroom_0%d.png",
		18),
		E_FAIL);

	// Pipe 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Pipe",
		L"../Bin/HueResources/Object/Pipe/Pipe_0%d.png",
		9),
		E_FAIL);

	// PotPlant 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_PotPlant",
		L"../Bin/HueResources/Object/PotPlant/PotPlant_0%d.png",
		2),
		E_FAIL);

	// Rope 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Rope",
		L"../Bin/HueResources/Box/Rope/Rope_0%d.png",
		1),
		E_FAIL);

	// Laser 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Laser",
		L"../Bin/HueResources/Box/Laser/Laser_0%d.png",
		2),
		E_FAIL);

	// Statue 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Statue",
		L"../Bin/HueResources/Object/Statue/Statue_0%d.png",
		8),
		E_FAIL);

	// Topiary 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Topiary",
		L"../Bin/HueResources/Object/Topiary/Topiary_0%d.png",
		6),
		E_FAIL);

	// Tree 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Tree",
		L"../Bin/HueResources/Object/Tree/Tree_0%d.png",
		6),
		E_FAIL);

	// Trinket 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Trinket",
		L"../Bin/HueResources/Object/Trinket/Trinket_0%d.png",
		6),
		E_FAIL);

	// Trophie 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Trophie",
		L"../Bin/HueResources/Object/Trophie/Trophie_0%d.png",
		13),
		E_FAIL);

	// Vase 텍스쳐
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Vase",
		L"../Bin/HueResources/Object/Vase/Vase_0%d.png",
		14),
		E_FAIL);

	// Snow
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Snow",
		L"../Bin/HueResources/Particle/Glow/Glow_0%d.png",
		2),
		E_FAIL);

	// Snow
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_ButtonX",
		L"../Bin/HueResources/UI/Button/Button_0%d.png",
		3),
		E_FAIL);

	return S_OK;
}

unsigned __stdcall CTutorialThread::LoadingTextureByThread(void* pParam)
{
	srand(unsigned int(time(NULL)));
	CTutorialThread* pTutorialThread = reinterpret_cast<CTutorialThread*>(pParam);

	EnterCriticalSection(&pTutorialThread->m_CriticalSection);

	FAILED_CHECK_RETURN(BufferForStage(), E_FAIL);
	FAILED_CHECK_RETURN(TextureForTutorial(), E_FAIL);
	FAILED_CHECK_RETURN(TextureForStage(), E_FAIL);

	/************************ (SongSong) ************************/
	/*  Cube 로드  */
	FAILED_CHECK_RETURN(CCubeMgr::GetInstance()->Load_Tutorial(Engine::Get_GraphicDev()->GetDevice()), E_FAIL);
	FAILED_CHECK_RETURN(CCubeMgr::GetInstance()->Load_Stage(Engine::Get_GraphicDev()->GetDevice()), E_FAIL);

	m_bLoadingComplete = true;

	LeaveCriticalSection(&pTutorialThread->m_CriticalSection);

	return 0;
}

CTutorialThread* CTutorialThread::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTutorialThread*		pInstance = new CTutorialThread(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Engine::Safe_Delete(pInstance);

	return pInstance;
}