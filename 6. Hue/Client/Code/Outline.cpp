#include "stdafx.h"
#include "Outline.h"
#include "SceneSelector.h"
USING(Engine)

COutline::COutline(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

COutline::~COutline(void)
{
	Release();
}

HRESULT COutline::Initialize(void)
{
	m_pCameraObserver = CCameraObserver::GetInstance();

	m_fWaitTime = 0.f;
	m_bSwitch = false;
	m_bIsDown = true;
	m_bIsUp = false;

	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;
	m_bSound = true;

	m_bIsWait = false;

	m_pBufferName = L"Buffer_Cube_Player";
	m_pTexName = L"Texture_Outline";
	m_pBufferName = L"Buffer_RcTex";

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVtx = new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->CopyToClient(Engine::RESOURCE_STATIC, m_pBufferName, m_pVertex);


	m_pInfoCom->m_vPos = { 0.f, (float)WINCY + 1350.f, 0.f };
	m_pInfoCom->m_vScale = { (float)WINCX + 100, (float)WINCY, 10.f };

	CInfoSubject::GetInstance()->AddData(DATA_OUTLINE, this);

	return S_OK;
}

void COutline::Update(void)
{
	if (CCubeMgr::GetInstance()->Get_IsRender() && !m_bSwitch)
	{
		m_bSwitch = true;
		m_bIsDown = true;
	}

	//아래로갈때
	if (m_bSwitch && m_bIsDown)
	{
		m_pInfoCom->m_vPos.y -= (float)2500.f * GET_TIME;
		if (m_bSound)
		{
			Engine::Get_SoundMgr()->StopAll();
			Engine::Get_SoundMgr()->MyPlaySound(L"CurtianClose.wav", Engine::CHANNELID::EFFECT);
			m_bSound = false;
		}
	}

	if (m_pInfoCom->m_vPos.y <= (float)WINCY - 700.f)
	{
		m_pInfoCom->m_vPos.y = (float)WINCY - 700.f;
		m_bIsDown = false;
	}

	//대기시간
	if (!m_bIsUp && !m_bIsDown)
	{
		m_bIsWait = true;
		m_fWaitTime += 0.75f * GET_TIME;
		if (m_fWaitTime > 1.f)
		{
			if (CCubeMgr::GetInstance()->GetPlayerDie())
			{
				CCubeMgr::GetInstance()->SetPlayerDie(false);
				CCubeMgr::GetInstance()->ResetCubePos();
			}

			//전역변수로 이동
			if (CCubeMgr::GetInstance()->Get_IsStageChange())
			{
				g_bIsStageChange = true;
				//CCubeMgr::GetInstance()->Set_IsStageChange(false);
			}


			m_bIsUp = true;
			m_fWaitTime = 0.f;
		}
	}
	else
		m_bIsWait = false;


	//위로갈때
	if (m_bSwitch && m_bIsUp)
		m_pInfoCom->m_vPos.y += (float)4000.f * GET_TIME;

	if (m_pInfoCom->m_vPos.y >= (float)WINCY + 1350.f && m_bSwitch)
	{
		m_pInfoCom->m_vPos.y = (float)WINCY + 1350.f;
		m_bIsDown = true;
		m_bIsUp = false;
		m_bSwitch = false;
		CCubeMgr::GetInstance()->Set_IsRender(false);
		m_bSound = true;
		switch (*CCameraObserver::GetInstance()->GetSceneID())
		{
		case SCENEID::SC_LOGO:
			Engine::Get_SoundMgr()->PlayBGM(L"LogoBGM.wav");
			break;
		case SCENEID::SC_TUTORIAL:
			Engine::Get_SoundMgr()->PlayBGM(L"Stage1BGM.wav");
			break;
		case SCENEID::SC_TUTORIAL_THREAD:
			//Engine::Get_SoundMgr()->PlayBGM(L"Stage1BGM.wav");
			break;
		case SCENEID::SC_STAGE:
			Engine::Get_SoundMgr()->PlayBGM(L"Stage1BGM.wav");
			break;
		case SCENEID::SC_STAGE2:
			Engine::Get_SoundMgr()->PlayBGM(L"Stage2BGM.wav");
			break;
		case SCENEID::SC_STAGE3:
			Engine::Get_SoundMgr()->PlayBGM(L"Stage3BGM.wav");
			break;
		case SCENEID::SC_END:
			//Engine::Get_SoundMgr()->PlayBGM(L"Stage1BGM.wav");
			break;
		default:
			break;
		}
	}

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);


	D3DXMATRIX  matProj;
	D3DXMatrixOrthoLH(&matProj, (float)WINCX * 2, (float)WINCY * 2, 0.f, 10.f);

	for (size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVtx[i] = m_pVertex[i];

		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, &m_pInfoCom->m_matWorld);

		if (m_pConvertVtx[i].vPos.z < 1.f)
			m_pConvertVtx[i].vPos.z = 1.f;

		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, &matProj);
	}
}

void COutline::LateUpdate(void)
{

}


void COutline::Render(void)
{
	if (CCubeMgr::GetInstance()->Get_IsRender())
	{
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, false); //★★★★★
		m_pGraphicDev->SetMaterial(&m_tMtrl);

		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);

		m_pTextureCom->SetTexture(1);
		m_pBufferCom->Render();
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, true); //★★★★★
	}
}


void COutline::Release(void)
{
	if (m_pVertex)
		Engine::Safe_Delete_Array(m_pVertex);
	if (m_pConvertVtx)
		Engine::Safe_Delete_Array(m_pConvertVtx);
}

void COutline::RenderInsideWorld(void)
{
	m_pGraphicDev->SetMaterial(&m_tMtrl);  //재질★
										   //m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 클리핑  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetTexture(1);
	m_pBufferCom->Render_NoIndices();
}

void COutline::SetTransform(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
}

HRESULT COutline::AddComponent(void)
{
	Engine::CComponent*  pComponent = nullptr;

	// Buffer

	pComponent = m_pResourceMgr->Clone_Buffer(Engine::RESOURCE_STATIC, m_pBufferName);
	m_pBufferCom = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	m_mapComponent.emplace(L"Buffer", m_pBufferCom);

	// Texture
	pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, m_pTexName);
	m_pTextureCom = dynamic_cast<Engine::CTexture*>(pComponent);
	m_mapComponent.emplace(L"Texture", m_pTextureCom);

	// Transform
	pComponent = Engine::CTransform::Create(g_vLook);
	m_pInfoCom = dynamic_cast<Engine::CTransform*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.emplace(L"Transform", m_pInfoCom);

	return S_OK;
}

COutline* COutline::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	COutline* pInstance = new COutline(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}