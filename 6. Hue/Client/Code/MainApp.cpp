#include "stdafx.h"
#include "MainApp.h"

#include "CubeMgr.h"
#include "Export_Function.h"
#include "SceneSelector.h"
#include "CollisionMgr.h"
/************************ (SongSong) ************************/
/*  CameraObserver Singleton  */
#include "CameraObserver.h"

CMainApp::CMainApp(void)
	: m_pGraphicDev(Engine::Get_GraphicDev())
	, m_pManagement(Engine::Get_Management())
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pKeyMgr(Engine::Get_KeyMgr())
	, m_pTimeMgr(Engine::Get_TimeMgr())
	, m_pInfoSubject(Engine::Get_InfoSubject())
	, m_pSoundMgr(Engine::Get_SoundMgr())
	, m_pCubeMgr(CCubeMgr::GetInstance())
	//, m_pObjectMgr(CObjectMgr::GetInstance())
	, m_pCollisionMgr(CCollisionMgr::GetInstance())
	, m_pDevice(nullptr)
{

}

CMainApp::~CMainApp(void)
{
	Release();
}

HRESULT CMainApp::Initialize(void)
{
	ShowCursor(false);
	FAILED_CHECK_RETURN(m_pGraphicDev->InitGraphicDev(Engine::CGraphicDev::MODE_FULL,
		g_hWnd,

		WINCX,
		WINCY),
		E_FAIL);

	/************************ (SongSong) ************************/
	/*  옵저버연결  */
	Engine::Get_InfoSubject()->Subscribe(CCameraObserver::GetInstance());

	m_pDevice = m_pGraphicDev->GetDevice();
	NULL_CHECK_RETURN(m_pDevice, E_FAIL);

	FAILED_CHECK_RETURN(m_pManagement->Initialize(m_pDevice), E_FAIL);
	FAILED_CHECK_RETURN(m_pManagement->SceneChange(CSceneSelector(SC_LOGO)), E_FAIL);

	//Setup_Camera();
	//Setup_Proj();
	Setup_Light();


	/************************ (SongSong) ************************/
	/*  Cube 로드  */
	//FAILED_CHECK_RETURN(CCubeMgr::GetInstance()->Load_Stage(m_pDevice), E_FAIL);
	//FAILED_CHECK_RETURN(CObjectMgr::GetInstance()->Load_Stage1(m_pDevice), E_FAIL);

	return S_OK;
}

void CMainApp::Update(void)
{
	m_pTimeMgr->Update();
	m_pKeyMgr->Update();


	if (g_bIsStageChange)
	{
		switch (*CCameraObserver::GetInstance()->GetSceneID())
		{
		case SCENEID::SC_LOGO:
			break;
		case SCENEID::SC_TUTORIAL:
			CCubeMgr::GetInstance()->Set_CurStageCube(SCENEID::SC_STAGE);
			Engine::Get_Management()->SceneChange(CSceneSelector(SC_STAGE));
			break;
		case SCENEID::SC_TUTORIAL_THREAD:
			break;
		case SCENEID::SC_STAGE:
			CCubeMgr::GetInstance()->Set_CurStageCube(SCENEID::SC_STAGE2);
			Engine::Get_Management()->SceneChange(CSceneSelector(SC_STAGE2));
			break;
		case SCENEID::SC_STAGE2:
			CCubeMgr::GetInstance()->Set_CurStageCube(SCENEID::SC_STAGE3);
			Engine::Get_Management()->SceneChange(CSceneSelector(SC_STAGE3));
			break;
		case SCENEID::SC_STAGE3:
			break;
		case SCENEID::SC_END:
			break;
		default:
			break;
		}
		g_bIsStageChange = false;
		CCubeMgr::GetInstance()->Set_IsStageChange(false);
	}

	if (nullptr != m_pManagement)
		m_pManagement->Update();


}

void CMainApp::LateUpdate(void)
{
	if (nullptr != m_pManagement)
		m_pManagement->LateUpdate();
}

void CMainApp::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if (nullptr != m_pManagement)
		m_pManagement->Render();

}

void CMainApp::Setup_Camera(void)
{
	/*D3DXVECTOR3 vecCameraPos(0.f, 20.f, -50.f);
	D3DXVECTOR3 vecLookAt(0.f, 0.f, 0.f);
	D3DXVECTOR3 vecUp(0.f, 1.f, 0.f);
	D3DXMATRIX v;
	D3DXMatrixLookAtLH(&v, &vecCameraPos, &vecLookAt, &vecUp);
	Engine::CGraphicDev::GetInstance()->GetDevice()->SetTransform(D3DTS_VIEW, &v);*/
}

void CMainApp::Setup_Proj(void)
{
	//D3DXMATRIX proj;
	//D3DXMatrixPerspectiveFovLH(&proj,
	//	D3DX_PI * 0.5f,
	//	(float)WINCX / WINCY,
	//	0.3f,
	//	1000.f);
	//Engine::CGraphicDev::GetInstance()->GetDevice()->SetTransform(D3DTS_PROJECTION, &proj);

}

void CMainApp::Setup_Light(void)
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
}

void CMainApp::Release(void)
{
	CCameraObserver::GetInstance()->DestroyInstance();
	Engine::Safe_Single_Destroy(m_pCollisionMgr);
	Engine::Safe_Single_Destroy(m_pCubeMgr);
	//Engine::Safe_Single_Destroy(m_pObjectMgr);
	Engine::Safe_Single_Destroy(m_pInfoSubject);
	Engine::Safe_Single_Destroy(m_pTimeMgr);
	Engine::Safe_Single_Destroy(m_pKeyMgr);
	Engine::Safe_Single_Destroy(m_pResourceMgr);
	Engine::Safe_Single_Destroy(m_pManagement);
	Engine::Safe_Single_Destroy(m_pGraphicDev);
	Engine::Safe_Single_Destroy(m_pSoundMgr);
}

CMainApp* CMainApp::Create(void)
{
	CMainApp*	pInstance = new CMainApp;

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;

}

