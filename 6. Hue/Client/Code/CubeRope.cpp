#include "stdafx.h"
#include "CubeRope.h"
#include "Include.h"
#include "Export_Function.h"

#include "CollisionMgr.h"
#include "ButtonX.h"
#include "CubeMgr.h"
#include "Player.h"
USING(Engine)

CCubeRope::CCubeRope(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
	: CCube(pGraphicDev, _tCubeInfo)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CCubeRope::~CCubeRope(void)
{
	Release();
}

HRESULT CCubeRope::Initialize(void)
{
	m_pTexName = m_tCubeInfo.tTex.szTex;
	m_pTexName = L"Texture_Object_Rope";
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	/************************ (SongSong) ************************/
	/*  Transform컴포넌트에 큐브정보를 대입  */
	m_pInfoCom->m_vPos = m_tCubeInfo.vPos;
	m_pInfoCom->m_vPos.z = 0.5f;
	m_pInfoCom->m_vScale = m_tCubeInfo.vScale;

	m_pInfoCom->m_vScale.x = 0.3f;
	m_pInfoCom->m_vScale.y = 1.35f;
	/************************ (SongSong) ************************/
	/*  카메라 옵저버 생성  */
	m_pCameraObserver = CCameraObserver::GetInstance();

	return S_OK;
}

void CCubeRope::Update(void)
{
	if (m_bIsMovingCube)
		FirstMovingUpdate();

	SetTransform();
}

void CCubeRope::LateUpdate(void)
{
}

void CCubeRope::Render(void)
{
	m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 클리핑  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetTexture(m_tCubeInfo.tTex.iIndex);
	m_pBufferCom->Render_NoIndices();

}

void CCubeRope::RenderInsideWorld(void)
{
	m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 클리핑  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetTexture(m_tCubeInfo.tTex.iIndex);
	m_pBufferCom->Render_NoIndices();

}

void CCubeRope::Release(void)
{
}

void CCubeRope::SetTransform(void)
{
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 SetTransform  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
	{
		m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(0.f);
	}
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
	{
		m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(90.f);
	}

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);
}

HRESULT CCubeRope::AddComponent(void)
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
	case Engine::CUBE_CLIPPING_TYPE::CUBE_CullLR:
		m_pBufferName = L"Buffer_CubeCullLR";
		break;
	case Engine::CUBE_CLIPPING_TYPE::CUBE_CullBackAndB:
		m_pBufferName = L"Buffer_CubeCullBackAndB";
		break;
	}

	Engine::CComponent*		pComponent = nullptr;

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

CCubeRope * CCubeRope::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
{
	CCubeRope* pInstance = new CCubeRope(pGraphicDev, _tCubeInfo);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
