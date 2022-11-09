#include "stdafx.h"
#include "ButtonX.h"
#include "Player.h"
USING(Engine)

CButtonX::CButtonX(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CButtonX::~CButtonX(void)
{
	Release();
}

HRESULT CButtonX::Initialize(void)
{
	m_bIsRender = false;
	m_pCameraObserver = CCameraObserver::GetInstance();

	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pBufferName = L"Buffer_Cube_Player";
	m_pTexName = L"Texture_ButtonX";
	//m_pBufferName = L"Buffer_RcTex";

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	m_pInfoCom->m_vPos.x = 20.f;
	m_pInfoCom->m_vPos.y = 18.f;
	m_pInfoCom->m_vPos.z = -1.5f;

	m_pInfoCom->m_vScale.x = 0.f;
	m_pInfoCom->m_vScale.y = 0.f;
	m_pInfoCom->m_vScale.z = 0.f;


	return S_OK;
}

void CButtonX::Update(void)
{
	if (!m_bIsRender)
	{
		m_pInfoCom->m_vScale.x -= 0.4f * GET_TIME;
		m_pInfoCom->m_vScale.y -= 0.4f * GET_TIME;

		if (m_pInfoCom->m_vScale.x <= 0.f || m_pInfoCom->m_vScale.y <= 0.f)
			m_pInfoCom->m_vScale.x = m_pInfoCom->m_vScale.y = 0.f;
	}
	else
	{
		m_pInfoCom->m_vScale.x += 0.4f * GET_TIME;
		m_pInfoCom->m_vScale.y += 0.4f * GET_TIME;

		if (m_pInfoCom->m_vScale.x >= 0.25f || m_pInfoCom->m_vScale.y >= 0.25f)
			m_pInfoCom->m_vScale.x = m_pInfoCom->m_vScale.y = 0.25f;
	}

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);
}

void CButtonX::LateUpdate(void)
{

}



void CButtonX::Render(void)
{
	m_pGraphicDev->SetMaterial(&m_tMtrl);

	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	m_pTextureCom->SetTexture(2);
	m_pBufferCom->Render_NoIndices();

}


void CButtonX::Release(void)
{

}

void CButtonX::RenderInsideWorld(void)
{
	m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★
												//m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 클리핑  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetTexture(2);
	m_pBufferCom->Render_NoIndices();

}

void CButtonX::SetPos(float fX, float fY, bool bIsLeft)
{
	m_pInfoCom->m_vPos.x = fX; 
	m_pInfoCom->m_vPos.y = fY; 
	m_bIsRender = true; 
	m_bIsLeft = bIsLeft;

	if (bIsLeft)
		m_pInfoCom->m_vPos.x -= 0.6f;
	else
		m_pInfoCom->m_vPos.x += 0.6f;

	m_pInfoCom->m_vPos.y -= 0.1f;
}

void CButtonX::SetTransform(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
}

HRESULT CButtonX::AddComponent(void)
{
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

CButtonX* CButtonX::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CButtonX* pInstance = new CButtonX(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}

