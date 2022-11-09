#include "stdafx.h"
#include "Laser.h"
#include "Include.h"
#include "Export_Function.h"

#include "CollisionMgr.h"
#include "ButtonX.h"
#include "CubeMgr.h"
#include "UvTex.h"
#include "Player.h"
#include "LaserEffect.h"
USING(Engine)

CLaser::CLaser(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
	: CCube(pGraphicDev, _tCubeInfo)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CLaser::~CLaser(void)
{
	Release();
}

HRESULT CLaser::Initialize(void)
{
	if (m_tCubeInfo.tPlatformInfo.eDir == Engine::CUBE_MOVING_DIR::CUBE_LEFT)
		m_bIsToDown = false;
	else
		m_bIsToDown = true;

	m_bIsMoving = true;
	m_pTexName = m_tCubeInfo.tTex.szTex;
	m_pTexName = L"Texture_Object_Laser";
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	/************************ (SongSong) ************************/
	/*  Transform컴포넌트에 큐브정보를 대입  */
	m_pInfoCom->m_vPos = m_tCubeInfo.vPos;
	m_pInfoCom->m_vScale = m_tCubeInfo.vScale;

	m_pInfoCom->m_vScale.x = 0.2f;
	m_pInfoCom->m_vScale.y = 0.2f;
	m_pInfoCom->m_vScale.z = 0.5f;

	if (!m_bIsToDown)
		m_pInfoCom->m_vPos.x -= 0.35f;
	else
	{
		m_pInfoCom->m_vPos.y += 0.3f;
		m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(-90.f);
	}

	/************************ (SongSong) ************************/
	/*  카메라 옵저버 생성  */
	m_pCameraObserver = CCameraObserver::GetInstance();

	m_fMakeEffectTime = 0.f;

	return S_OK;
}

void CLaser::Update(void)
{
	if (m_bIsMoving)
	{
		if (GET_TIME < 0.7f)
		{
			if (!m_bIsToDown)
			{
				m_pInfoCom->m_vScale.x += 7.5f * GET_TIME;
				m_pInfoCom->m_vPos.x += 7.5f * GET_TIME;
			}
			else
			{
				m_pInfoCom->m_vScale.x += 7.5f * GET_TIME;
				m_pInfoCom->m_vPos.y -= 7.5f * GET_TIME;
			}
		}
	}
	SetTransform();
	m_pBufferUvTex->Update();

	if (!m_bIsToDown)

		CCollisionMgr::IsCollisionRightLeftLaserToCube(this);
	else
		CCollisionMgr::IsCollisionUpDownLaserToCube(this);

	if (*CCameraObserver::GetInstance()->GetSceneID() != SCENEID::SC_STAGE3)
		CCollisionMgr::IsCollisionLaserToPlayer(this, CCubeMgr::GetInstance()->GetPlayer(), m_bIsToDown);
	else
		CCollisionMgr::IsCollisionLaserToPlayerStage3(this, CCubeMgr::GetInstance()->GetPlayer(), m_bIsToDown);

	m_fMakeEffectTime += 1.85f * GET_TIME;

	//충돌시에만 이펙트 만들기
	if (m_fMakeEffectTime > 0.05f && !m_bIsMoving)
	{
		CLaserEffect* pEffect = CLaserEffect::Create(m_pGraphicDev);
		if(!m_bIsToDown)
			pEffect->SetPosAndInfo(m_pInfoCom->m_vPos.x + m_pInfoCom->m_vScale.x, m_pInfoCom->m_vPos.y, m_pInfoCom->m_vPos.z, m_bIsToDown);
		else
			pEffect->SetPosAndInfo(m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y - m_pInfoCom->m_vScale.x, m_pInfoCom->m_vPos.z, m_bIsToDown);
		       
		pEffect->SetMaterial(m_tCubeInfo.tColor);          

		m_listEffect.push_back(pEffect);

		m_fMakeEffectTime = 0.f;
	}


	auto iter_begin = m_listEffect.begin();
	auto iter_end = m_listEffect.end();

	for (; iter_begin != iter_end; )
	{
		int iEvent = (*iter_begin)->LaserEffectUpdate();

		if (0 == iEvent)
		{
			delete (*iter_begin);
			iter_begin = m_listEffect.erase(iter_begin);
		}
		else
			++iter_begin;
	}

	if (m_bIsMovingCube)
		FirstMovingUpdate();

	if (m_tCubeInfo.tPlatformInfo.eDir != Engine::CUBE_MOVING_DIR::CUBE_LEFT)
	{
		/************************ (SongSong) ************************/
		/*  카메라 시점에 따른 각도  */
		if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
			m_pInfoCom->m_fAngle[ANGLE_X] = D3DXToRadian(0.f);
		else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
			m_pInfoCom->m_fAngle[ANGLE_X] = D3DXToRadian(90.f);
	}

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);
}

void CLaser::LateUpdate(void)
{
}

void CLaser::Render(void)
{
	m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 클리핑  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pTextureCom->SetTexture(0);
	m_pBufferUvTex->Render();

	for (auto& MyEffect : m_listEffect)
		MyEffect->Render();

	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CLaser::RenderInsideWorld(void)
{
	m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 클리핑  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetTexture(0);
	m_pBufferUvTex->Render();

	for (auto& MyEffect : m_listEffect)
		MyEffect->Render();
}

void CLaser::Release(void)
{
	Engine::Safe_Delete(m_pBufferUvTex);

	for (auto& MyEffect : m_listEffect)
		Engine::Safe_Delete(MyEffect);

}

void CLaser::SetTransform(void)
{
}

HRESULT CLaser::AddComponent(void)
{
	switch (m_tCubeInfo.eType)
	{
	case Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL:
		m_pBufferName = L"Buffer_CubeNormal";
		break;
	case Engine::CUBE_CLIPPING_TYPE::CUBE_FRONT:
		m_pBufferName = L"Buffer_CubeFront";
		break;
	case Engine::CUBE_CLIPPING_TYPE::CUBE_CullB:
		m_pBufferName = L"Buffer_CubeCullB";
		break;
	case Engine::CUBE_CLIPPING_TYPE::CUBE_CullL:
		m_pBufferName = L"Buffer_CubeCullL";
		break;
	case Engine::CUBE_CLIPPING_TYPE::CUBE_CullR:
		m_pBufferName = L"Buffer_CubeCullR";
		break;
	}

	m_pBufferName = L"Buffer_UVTex";
	Engine::CComponent*		pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->Clone_UVBuffer();
	//m_pBufferCom = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	m_pBufferUvTex = dynamic_cast<Engine::CUvTex*>(pComponent);
	//m_mapComponent.emplace(L"Buffer", m_pBufferUvTex);

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

CLaser * CLaser::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
{
	CLaser* pInstance = new CLaser(pGraphicDev, _tCubeInfo);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
