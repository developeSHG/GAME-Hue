#include "stdafx.h"
#include "CubeFireparent.h"

#include "Include.h"
#include "Export_Function.h"
#include "CameraObserver.h"
#include "CubeFirecracker.h"
USING(Engine)

CCubeFireparent::CCubeFireparent(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
	: CCube(pGraphicDev, _tCubeInfo)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CCubeFireparent::~CCubeFireparent(void)
{
	Release();
}

HRESULT CCubeFireparent::Initialize(void)
{
	m_fSpeed = 5.f;
	m_fDeadTime = (float)(rand() % 7) + 3.f;
	m_pTexName = L"Texture_CubeDeco_Aztec";
	m_tCubeInfo.tTex.iIndex = 0;
	m_tCubeInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL;

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	/************************ (SongSong) ************************/
	/*  Transform컴포넌트에 큐브정보를 대입  */
	m_pInfoCom->m_vPos = m_tCubeInfo.vPos;
	m_pInfoCom->m_vScale = m_tCubeInfo.vScale;
	m_pInfoCom->m_vScale = D3DXVECTOR3(0.125f, 0.125f, 0.125f);
	//m_pInfoCom->m_vPos.z = -15.f;

	/************************ (SongSong) ************************/
	/*  카메라 옵저버 생성  */
	m_pCameraObserver = CCameraObserver::GetInstance();

	return S_OK;
}

void CCubeFireparent::Update(void)
{


	m_pInfoCom->m_vPos += m_pInfoCom->m_vDir * 4.5f * GET_TIME;
	m_fDeadTime -= 1.f * GET_TIME;
	
	SetTransform();

}

void CCubeFireparent::LateUpdate(void)
{
	
	SetTransform();

}

void CCubeFireparent::Render(void)
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

	for (auto& MyFire : m_listFirecracker)
		MyFire->Render();
}

void CCubeFireparent::RenderInsideWorld(void)
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

int CCubeFireparent::UpdateFireparent(void)
{
	if (!m_bIsKing)
	{
		m_fSpeed -= 1.5f * GET_TIME;
		m_fDeadTime -= 2.8f * GET_TIME;
	}
	else
	{
		m_fSpeed -= 3.5f * GET_TIME;
		m_fDeadTime -= 4.6f * GET_TIME;
	}
	m_pInfoCom->m_vPos += m_pInfoCom->m_vDir * m_fSpeed * GET_TIME;



	if (m_fDeadTime <= 0.f)
		return 0;

	SetTransform();

	return 1;
}

void CCubeFireparent::SetPos(D3DXVECTOR3 vecPos)
{
	m_pInfoCom->m_vPos = vecPos;
}

void CCubeFireparent::SetIsKing()
{
	m_pInfoCom->m_vScale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	m_fDeadTime = 22.f;
	m_bIsKing = true;
	m_fSpeed = 15.f;
}

void CCubeFireparent::Release(void)
{
	for (auto& MyFire : m_listFirecracker)
		Engine::Safe_Delete(MyFire);

}

void CCubeFireparent::SetTransform(void)
{
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 SetTransform  */


	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	
}


HRESULT CCubeFireparent::AddComponent(void)
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

CCubeFireparent * CCubeFireparent::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
{
	CCubeFireparent* pInstance = new CCubeFireparent(pGraphicDev, _tCubeInfo);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
