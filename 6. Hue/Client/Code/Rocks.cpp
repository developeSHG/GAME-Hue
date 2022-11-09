#include "stdafx.h"
#include "Rocks.h"

#include "Include.h"
#include "Export_Function.h"
USING(Engine)

CRocks::CRocks(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CRocks::~CRocks(void)
{
	Release();
}

HRESULT CRocks::Initialize(void)
{
	m_pBufferName = L"Buffer_CubeFront";
	m_pTexName = L"Texture_Rocks";

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_pInfoCom->m_vPos = { 25.f, 0.78f, 0.002f };
	m_pInfoCom->m_vScale = { 2.6f, 1.2f, 0.f };

	return S_OK;
}

void CRocks::Update(void)
{
	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);
}


void CRocks::LateUpdate(void)
{

}

void CRocks::Render(void)
{
	D3DMATERIAL9 tMtrl;

	if (Engine::CUBE_COLOR::CUBE_COLOR_END == CCubeMgr::GetInstance()->Get_CurCubeColor())
	{
		tMtrl.Ambient = WHITE;
		tMtrl.Diffuse = WHITE;
		tMtrl.Specular = WHITE;
		tMtrl.Emissive = BLACK;
		tMtrl.Power = 5.f;
	}
	else
	{
		tMtrl.Ambient = ORANGE;
		tMtrl.Diffuse = ORANGE;
		tMtrl.Specular = ORANGE;
		tMtrl.Emissive = RED;
		tMtrl.Power = 5.f;
	}
	if (Engine::CUBE_COLOR::CUBE_ORANGE == CCubeMgr::GetInstance()->Get_CurCubeColor())
		return;

	D3DXMATRIX matView = *CCameraObserver::GetInstance()->GetView();
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProj = *CCameraObserver::GetInstance()->GetProj();
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetMaterial(&tMtrl);						// 재질★

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	m_pTextureCom->SetTexture(0);
	m_pBufferCom->Render_NoIndices();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRocks::RenderInsideWorld(void)
{
	D3DXMATRIX matView = *CCameraObserver::GetInstance()->GetView();
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProj = *CCameraObserver::GetInstance()->GetProj();
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	D3DMATERIAL9 tMtrl;
	tMtrl.Ambient = WHITE;
	tMtrl.Diffuse = WHITE;
	tMtrl.Specular = WHITE;
	tMtrl.Emissive = BLACK;
	tMtrl.Power = 5.f;

	m_pGraphicDev->SetMaterial(&tMtrl);						// 재질★

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	m_pTextureCom->SetTexture(0);
	m_pBufferCom->Render_NoIndices();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRocks::Release(void)
{

}

void CRocks::SetTransform(void)
{

}

HRESULT CRocks::AddComponent(void)
{
	Engine::CComponent*      pComponent = nullptr;

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

CRocks * CRocks::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRocks* pInstance = new CRocks(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}