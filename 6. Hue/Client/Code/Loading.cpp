#include "stdafx.h"
#include "Loading.h"

#include "Include.h"
#include "Export_Function.h"
USING(Engine)

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CLoading::~CLoading(void)
{
	Release();
}

HRESULT CLoading::Initialize(void)
{
	m_pBufferName = L"Buffer_CubeFront";
	m_pTexName = L"Texture_Loading";

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_pInfoCom->m_vPos = { 2.5f, 0.6f, -0.1f };
	m_pInfoCom->m_vScale = { 0.85f, 0.2f, 0.1f };

	return S_OK;
}

void CLoading::Update(void)
{
	if (false == m_bComplete)
	{
		if (4 >= m_fIndex)
			m_fIndex += 6.f * GET_TIME;
		else
			m_fIndex = 0.f;
	}
	else
	{
		m_pInfoCom->m_vPos = { 2.2f, 0.6f, -0.01f };
		m_fIndex = 5.f;

		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			m_bGo = true;
			return;
		}
	}

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	SetTransform();
}

void CLoading::LateUpdate(void)
{

}

void CLoading::Render(void)
{
	if (true == m_bGo)
		return;

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

	m_pGraphicDev->SetMaterial(&tMtrl);						// ÀçÁú¡Ú

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	m_pTextureCom->SetTexture(DWORD(m_fIndex));
	m_pBufferCom->Render_NoIndices();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CLoading::Release(void)
{

}

void CLoading::SetTransform(void)
{

}

HRESULT CLoading::AddComponent(void)
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

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLoading* pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
