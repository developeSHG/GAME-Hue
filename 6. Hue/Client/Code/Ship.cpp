#include "stdafx.h"
#include "Ship.h"

#include "Include.h"
#include "Export_Function.h"
USING(Engine)

CShip::CShip(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CShip::~CShip(void)
{
	Release();
}

HRESULT CShip::Initialize(void)
{
	m_pBufferName = L"Buffer_CubeFront";
	m_pTexName = L"Texture_Ship";

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_pInfoCom->m_vPos = { -0.8f, 0.24f, 0.003f };
	m_pInfoCom->m_vScale = { 3.8f, 2.5f, 0.f };

	m_fOriginY = m_pInfoCom->m_vPos.y;

	return S_OK;
}

void CShip::Update(void)
{
	//cout << "ComX : " << m_pInfoCom->m_vPos.x << endl;
	////cout << "Y : " << m_pInfoCom->m_vPos.y << endl;
	//cout << "InfoX : " << m_tInfo.vPos.x << endl;

	if (0.7f > GET_TIME)
	{
		if (false == m_bUpDown)
		{
			m_fPowerY -= 0.4f * GET_TIME;
			m_pInfoCom->m_vPos.y -= m_fPowerY * GET_TIME;
		}
		else
		{
			m_fPowerY -= 0.4f * GET_TIME;
			m_pInfoCom->m_vPos.y += m_fPowerY * GET_TIME;
		}
	}

	if (/*m_fOriginY < m_pInfoCom->m_vPos.y || */true == m_bUpDown && 0.f >= m_fPowerY)
	{
		m_bUpDown = false;
		m_fPowerY = 0.71f;
	}
	else if (/*m_fOriginY - 0.5f > m_pInfoCom->m_vPos.y || */false == m_bUpDown && 0.f >= m_fPowerY)
	{
		m_bUpDown = true;
		m_fPowerY = 0.71f;
	}

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	SetTransform();
}


void CShip::LateUpdate(void)
{

}

void CShip::Render(void)
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

void CShip::RenderInsideWorld(void)
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

void CShip::Release(void)
{

}

void CShip::SetTransform(void)
{

}

HRESULT CShip::AddComponent(void)
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

CShip * CShip::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShip* pInstance = new CShip(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}