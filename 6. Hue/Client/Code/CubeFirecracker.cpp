#include "stdafx.h"
#include "CubeFirecracker.h"

#include "Include.h"
#include "Export_Function.h"
#include "CameraObserver.h"

USING(Engine)

CCubeFirecracker::CCubeFirecracker(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
	: CCube(pGraphicDev, _tCubeInfo)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CCubeFirecracker::~CCubeFirecracker(void)
{
	Release();
}

HRESULT CCubeFirecracker::Initialize(void)
{
	m_bIsKing = false;
	m_pTexName = L"Texture_CubeColor_BrickColor";
	m_tCubeInfo.tTex.iIndex = 1;
	m_tCubeInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL;

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	/************************ (SongSong) ************************/
	/*  Transform컴포넌트에 큐브정보를 대입  */
	m_pInfoCom->m_vPos = m_tCubeInfo.vPos;
	m_pInfoCom->m_vScale = m_tCubeInfo.vScale;
	m_pInfoCom->m_vScale = D3DXVECTOR3(0.25f, 0.25f, 0.25f);

	//m_pInfoCom->m_vPos.z = -15.f;

	/************************ (SongSong) ************************/
	/*  카메라 옵저버 생성  */
	m_pCameraObserver = CCameraObserver::GetInstance();

	return S_OK;
}

void CCubeFirecracker::Update(void)
{
	m_pInfoCom->m_vPos += m_pInfoCom->m_vDir * 3.f * GET_TIME;
	
	SetTransform();

}

void CCubeFirecracker::LateUpdate(void)
{
	//m_pInfoCom->m_vPos += m_pInfoCom->m_vDir * 3.f * GET_TIME;
	
	SetTransform();
}

void CCubeFirecracker::Render(void)
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

void CCubeFirecracker::RenderInsideWorld(void)
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

int CCubeFirecracker::UpdateFirework(void)
{
	m_pInfoCom->m_vPos += m_pInfoCom->m_vDir * 5.f * GET_TIME;
	m_pInfoCom->m_vScale.x -= 0.1f * GET_TIME;
	m_pInfoCom->m_vScale.y -= 0.1f * GET_TIME;
	m_pInfoCom->m_vScale.z -= 0.1f * GET_TIME;

	if (m_pInfoCom->m_vScale.x <= 0.f)
		return 0;

	SetTransform();

	return 1;
}

void CCubeFirecracker::SetPos(D3DXVECTOR3 vecPos)
{
	m_pInfoCom->m_vPos = vecPos;
}

void CCubeFirecracker::Release(void)
{
}

void CCubeFirecracker::SetTransform(void)
{
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 SetTransform  */


	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	
}

HRESULT CCubeFirecracker::AddComponent(void)
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

CCubeFirecracker * CCubeFirecracker::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
{
	CCubeFirecracker* pInstance = new CCubeFirecracker(pGraphicDev, _tCubeInfo);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
