#include "stdafx.h"
#include "Cgv.h"

#include "Include.h"
#include "Export_Function.h"
USING(Engine)

CCgv::CCgv(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CCgv::~CCgv(void)
{
	Release();
}

HRESULT CCgv::Initialize(void)
{
	m_pBufferName = L"Buffer_RcTex";
	m_pTexName = L"WhiteBack";

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_pInfoCom->m_vPos = { 0.f, WINCY / 2.f + 230.f, 1.f };
	m_pInfoCom->m_vScale = { float(WINCX), 75.f, 0.f };

	m_tInfo.vPos = { 0.f, -WINCY / 2.f - 230.f, 1.f };

	return S_OK;
}

void CCgv::Update(void)
{
	//cout << "ComX : " << m_pInfoCom->m_vPos.x << endl;
	////cout << "Y : " << m_pInfoCom->m_vPos.y << endl;
	//cout << "InfoX : " << m_tInfo.vPos.x << endl;

	if (Engine::CUBE_COLOR::CUBE_COLOR_END != CCubeMgr::GetInstance()->Get_CurCubeColor())
	{
		m_pInfoCom->m_vPos.y += 230.f * GET_TIME;
		m_tInfo.vPos.y -= 230.f * GET_TIME;
	}

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	Engine::CPipeline::MakeWorldMatrix(&m_tInfo.matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_tInfo.vPos);
}

void CCgv::LateUpdate(void)
{

}

void CCgv::Render(void)
{
	D3DXMATRIX    mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &mIdentity);

	D3DXMATRIX      matProj;
	D3DXMatrixOrthoLH(&matProj, (float)WINCX * 2, (float)WINCY * 2, 1.f, 10.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	D3DMATERIAL9 tMtrl;
	tMtrl.Ambient = BLACK;
	tMtrl.Diffuse = BLACK;
	tMtrl.Specular = BLACK;
	tMtrl.Emissive = BLACK;
	tMtrl.Power = 5.f;

	m_pGraphicDev->SetMaterial(&tMtrl);						// 재질★

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	m_pTextureCom->SetTexture(0);
	m_pBufferCom->Render();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_tInfo.matWorld);
	m_pTextureCom->SetTexture(0);
	m_pBufferCom->Render();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CCgv::RenderInsideWorld(void)
{
	D3DXMATRIX    mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &mIdentity);

	D3DXMATRIX      matProj;
	D3DXMatrixOrthoLH(&matProj, (float)WINCX * 2, (float)WINCY * 2, 1.f, 10.f);
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
	m_pBufferCom->Render();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_tInfo.matWorld);
	m_pTextureCom->SetTexture(0);
	m_pBufferCom->Render();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CCgv::Release(void)
{

}

void CCgv::SetTransform(void)
{

}

HRESULT CCgv::AddComponent(void)
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

CCgv * CCgv::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCgv* pInstance = new CCgv(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}