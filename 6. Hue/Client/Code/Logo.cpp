#include "stdafx.h"
#include "Logo.h"
#include "BackGround.h"
#include "Export_Function.h"
#include "SceneSelector.h"
#include "Include.h"
#include "GameObject.h"
#include "StaticCamera.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
	, m_pManagement(Engine::Get_Management())
	, m_pResourceMgr(Engine::Get_ResourceMgr())
{
}

CLogo::~CLogo(void)
{
	Release();
}

HRESULT CLogo::Initialize(void)
{
	/************************ (SongSong) ************************/
	/*  로고, 튜토리얼 리소스 생성  */
	FAILED_CHECK_RETURN(BufferForLogo(), E_FAIL);
	FAILED_CHECK_RETURN(TextureForLogo(), E_FAIL);
	FAILED_CHECK_RETURN(BufferForTutorialThread(), E_FAIL);
	FAILED_CHECK_RETURN(TextureForTutorialThread(), E_FAIL);

	FAILED_CHECK_RETURN(Add_GameObject(), E_FAIL);

	Engine::Get_SoundMgr()->PlayBGM(L"LogoBGM.wav");
	return S_OK;
}

void CLogo::Update(void)
{
	Engine::CScene::Update();

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		m_pManagement->SceneChange(CSceneSelector(SC_TUTORIAL_THREAD));
		return;
	}
}

void CLogo::LateUpdate(void)
{
	Engine::CScene::LateUpdate();
}

void CLogo::Render(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	Engine::CScene::Render();
}

void CLogo::Release(void)
{
	Engine::Get_SoundMgr()->StopAll();
	Engine::CScene::Release();
}

HRESULT CLogo::Add_GameObject(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"BackGround", pGameObject);

	pGameObject = CStaticCamera::Create(m_pGraphicDev, nullptr);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"StaticCamera", pGameObject);


	m_mapLayer.emplace(LAYER_GAMEOBJECT, pLayer);

	return S_OK;
}

HRESULT CLogo::BufferForLogo(void)
{
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Buffer(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_RCTEX,
		L"Buffer_RcTex"), E_FAIL);

	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Buffer(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_RCMOUSE,
		L"Buffer_RcMouse"), E_FAIL);

	//uvtex
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Buffer(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_UVTEX,
		L"Buffer_UVTex"), E_FAIL);

	return S_OK;
}

HRESULT CLogo::TextureForLogo(void)
{
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_BackGround",
		L"../../Tool/Bin/HueResources/Logo/BackGround/BackGround_0%d.png",
		1),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Fire",
		L"../../Tool/Bin/HueResources/Particle/Fire/Fire_0%d.png",
		1),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_BackLight",
		L"../../Tool/Bin/HueResources/BackGround/Overlay/Overlay_0%d.png",
		2),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_BackSeaHorse",
		L"../../Tool/Bin/HueResources/Logo/SeaHorse/Seahorse_0%d.png",
		1),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_BackKeyBoard",
		L"../../Tool/Bin/HueResources/Logo/KeyBoard/KeyBoard_0%d.png",
		1),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_BackGameLogo",
		L"../../Tool/Bin/HueResources/Logo/MainLogo/MainLogo_0%d.png",
		2),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_BackWolf",
		L"../../Tool/Bin/HueResources/Logo/WolfHead/Wolfhead_0%d.png",
		1),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_BackBook",
		L"../../Tool/Bin/HueResources/Logo/Book/Book_0%d.png",
		3),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_BackHue",
		L"../../Tool/Bin/HueResources/Logo/Hue/Hue_0%d.png",
		3),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_BackColorChunk",
		L"../../Tool/Bin/HueResources/Logo/Color/Color_0%d.png",
		1),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_BackDrGrey",
		L"../../Tool/Bin/HueResources/Logo/DrGrey/DrGrey_0%d.png",
		1),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_BackMum",
		L"../../Tool/Bin/HueResources/Logo/Mum/Mum_0%d.png",
		3),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_BackObject",
		L"../../Tool/Bin/HueResources/Logo/BackObject/BackObject_0%d.png",
		2),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_BackSmoke",
		L"../../Tool/Bin/HueResources/Pattern/Smoke/Smoke_0%d.png",
		5),
		E_FAIL);

	return S_OK;
}

HRESULT CLogo::BufferForTutorialThread(void)
{
	// Player 버퍼
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Buffer(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_CUBE_Player,
		L"Buffer_Cube_Player"), E_FAIL);

	// Cube 버퍼
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Buffer(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_CUBE_FRONT,
		L"Buffer_CubeFront"), E_FAIL);

	return S_OK;
}

HRESULT CLogo::TextureForTutorialThread(void)
{
	/************************ (SongSong) ************************/
	/*  플레이어  */
	FAILED_CHECK_RETURN(LoadPlayerTexture(), E_FAIL);
	FAILED_CHECK_RETURN(LoadPlayerMaskTexture(), E_FAIL);

	// Water
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Water",
		L"../Bin/HueResources/Tutorial/Water/Water_0%d.png",
		1),
		E_FAIL);

	// Loaidng
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Loading",
		L"../Bin/HueResources/Tutorial/Loading/Loading_0%d.png",
		6),
		E_FAIL);

	// Cloud
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Cloud",
		L"../Bin/HueResources/Tutorial/Cloud/Cloud_0%d.png",
		1),
		E_FAIL);

	// Ship
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Ship",
		L"../Bin/HueResources/Tutorial/Ship/Ship_0%d.png",
		1),
		E_FAIL);

	// Ship
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Rain",
		L"../Bin/HueResources/Tutorial/Rain/Rain_0%d.png",
		1),
		E_FAIL);

	// WhiteBack
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"WhiteBack",
		L"../../Tool/Bin/HueResources/BackGround/Back/WhiteBack0%d.png",
		1),
		E_FAIL);

	return S_OK;
}


HRESULT CLogo::LoadPlayerTexture(void)
{
	// ************************************** Player 텍스쳐 ************************************** //
	// Idle 1st
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Player_Idle1",
		L"../../Tool/Bin/HueResources/Player/Idle1st/Idle1st_%d.png",	// 엔진 경로에 있는 텍스쳐 가져옴
		20),
		E_FAIL);

	// Idle 2nd
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Player_Idle2",
		L"../../Tool/Bin/HueResources/Player/Idle2nd/Idle2nd_0%d.png",
		10),
		E_FAIL);

	// Walk
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Player_Walk",
		L"../../Tool/Bin/HueResources/Player/Walk/Walk_0%d.png",
		10),
		E_FAIL);

	// Run
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Player_Run",
		L"../../Tool/Bin/HueResources/Player/Run/Run_0%d.png",
		8),
		E_FAIL);

	// Jump
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Player_Jump",
		L"../../Tool/Bin/HueResources/Player/Jump/Jump_0%d.png",
		5),
		E_FAIL);

	// Fall
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Player_Fall",
		L"../../Tool/Bin/HueResources/Player/Fall/Fall_0%d.png",
		5),
		E_FAIL);

	// Pull
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Player_Pull",
		L"../../Tool/Bin/HueResources/Player/Pull/Pull_0%d.png",
		10),
		E_FAIL);

	// Push
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Player_Push",
		L"../../Tool/Bin/HueResources/Player/Push/Push_0%d.png",
		10),
		E_FAIL);

	// Hang
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Player_Hang",
		L"../../Tool/Bin/HueResources/Player/Hang/Hang_0%d.png",
		7),
		E_FAIL);

	// Shoot
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Player_Shoot",
		L"../../Tool/Bin/HueResources/Player/Shoot/Shoot_0%d.png",
		10),
		E_FAIL);

	// Slide
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Player_Slide",
		L"../../Tool/Bin/HueResources/Player/Slide/Slide_0%d.png",
		5),
		E_FAIL);

	// Die
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Player_Die",
		L"../../Tool/Bin/HueResources/Player/Die/Die_0%d.png",
		9),
		E_FAIL);

	return S_OK;
}

HRESULT CLogo::LoadPlayerMaskTexture(void)
{
	// ************************************** Player 텍스쳐 ************************************** //
	// Idle 1st
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"PlayerMask_Idle1",
		L"../../Tool/Bin/HueResources/PlayerMask/Idle1st/Idle1st_%d.png",	// 엔진 경로에 있는 텍스쳐 가져옴
		20),
		E_FAIL);

	// Idle 2nd
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"PlayerMask_Idle2",
		L"../../Tool/Bin/HueResources/PlayerMask/Idle2nd/Idle2nd_0%d.png",
		10),
		E_FAIL);

	// Walk
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"PlayerMask_Walk",
		L"../../Tool/Bin/HueResources/PlayerMask/Walk/Walk_0%d.png",
		10),
		E_FAIL);

	// Run
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"PlayerMask_Run",
		L"../../Tool/Bin/HueResources/PlayerMask/Run/Run_0%d.png",
		8),
		E_FAIL);

	// Jump
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"PlayerMask_Jump",
		L"../../Tool/Bin/HueResources/PlayerMask/Jump/Jump_0%d.png",
		5),
		E_FAIL);

	// Fall
	FAILED_CHECK_RETURN(m_pResourceMgr->Add_Texture(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"PlayerMask_Fall",
		L"../../Tool/Bin/HueResources/PlayerMask/Fall/Fall_0%d.png",
		5),
		E_FAIL);

	// Pull
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"PlayerMask_Pull",
		L"../../Tool/Bin/HueResources/PlayerMask/Pull/Pull_0%d.png",
		10),
		E_FAIL);

	// Push
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"PlayerMask_Push",
		L"../../Tool/Bin/HueResources/PlayerMask/Push/Push_0%d.png",
		10),
		E_FAIL);

	// Hang
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"PlayerMask_Hang",
		L"../../Tool/Bin/HueResources/PlayerMask/Hang/Hang_0%d.png",
		7),
		E_FAIL);

	// Shoot
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"PlayerMask_Shoot",
		L"../../Tool/Bin/HueResources/PlayerMask/Shoot/Shoot_0%d.png",
		10),
		E_FAIL);

	// Slide
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"PlayerMask_Slide",
		L"../../Tool/Bin/HueResources/PlayerMask/Slide/Slide_0%d.png",
		5),
		E_FAIL);

	// Die
	FAILED_CHECK_RETURN(Engine::Get_ResourceMgr()->Add_Texture(Engine::Get_GraphicDev()->GetDevice(),
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"PlayerMask_Die",
		L"../../Tool/Bin/HueResources/PlayerMask/Die/Die_0%d.png",
		9),
		E_FAIL);

	return S_OK;
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*		pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Engine::Safe_Delete(pInstance);

	return pInstance;
}

