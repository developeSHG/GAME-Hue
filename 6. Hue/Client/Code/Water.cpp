#include "stdafx.h"
#include "Water.h"

#include "Include.h"
#include "Export_Function.h"
#include "StaticCamera.h"
USING(Engine)

CWater::CWater(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CWater::~CWater(void)
{
	Release();
}

HRESULT CWater::Initialize(void)
{
	m_pBufferName = L"Buffer_CubeFront";
	m_pTexName = L"Texture_Water";

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_pInfoCom->m_vPos = { -2.f, -3.f, -1.f };
	m_pInfoCom->m_vScale = { 7.5f, 2.f, 0.f };

	for (size_t i = 0; i < 7; ++i)
	{
		INFO tInfo;
		tInfo.vPos = { (i * 15.f) - 15.f - 2.f, -3.f, -1.f };

		m_listInfo.push_back(tInfo);
	}

	return S_OK;
}

void CWater::Update(void)
{
	//cout << "ComX : " << m_pInfoCom->m_vPos.x << endl;
	////cout << "Y : " << m_pInfoCom->m_vPos.y << endl;
	//cout << "InfoX : " << m_tInfo.vPos.x << endl;

	for (auto& tInfo : m_listInfo)
	{
		if (0.7f > GET_TIME)
			tInfo.vPos.x -= 1.6f * GET_TIME;

		if (-15.f * 2 >= tInfo.vPos.x)
			tInfo.vPos.x = (5 * 15.f);

		Engine::CPipeline::MakeWorldMatrix(&tInfo.matWorld,
			&m_pInfoCom->m_vScale,
			m_pInfoCom->m_fAngle,
			&tInfo.vPos);
	}

	//Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
	//	&m_pInfoCom->m_vScale,
	//	m_pInfoCom->m_fAngle,
	//	&m_pInfoCom->m_vPos);

	SetTransform();
}


void CWater::LateUpdate(void)
{

}

void CWater::Render(void)
{
	D3DXMATRIX matView = *CCameraObserver::GetInstance()->GetView();
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProj = *CCameraObserver::GetInstance()->GetProj();
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	//D3DXMatrixOrthoLH(&matProj, (float)WINCX * 2, (float)WINCY * 2, 1.f, 10.f);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	D3DMATERIAL9 tMtrl;
	tMtrl.Ambient = WHITE;
	tMtrl.Diffuse = WHITE;
	tMtrl.Specular = WHITE;
	tMtrl.Emissive = BLACK;
	tMtrl.Power = 5.f;

	if (D3DCOLOR_XRGB(128, 128, 128) != CCubeMgr::GetInstance()->GetBackColor())
	{
		tMtrl.Ambient = SKYBLUE;
		tMtrl.Diffuse = SKYBLUE;
		tMtrl.Specular = SKYBLUE;
	}

	m_pGraphicDev->SetMaterial(&tMtrl);						// 재질★

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	for (auto& tInfo : m_listInfo)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &tInfo.matWorld);
		m_pTextureCom->SetTexture(0);
		m_pBufferCom->Render_NoIndices();
	}

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CWater::RenderInsideWorld(void)
{
	D3DXMATRIX matView = *CCameraObserver::GetInstance()->GetView();
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProj = *CCameraObserver::GetInstance()->GetProj();
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	//D3DXMatrixOrthoLH(&matProj, (float)WINCX * 2, (float)WINCY * 2, 1.f, 10.f);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

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

	for (auto& tInfo : m_listInfo)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &tInfo.matWorld);
		m_pTextureCom->SetTexture(0);
		m_pBufferCom->Render_NoIndices();
	}

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CWater::Release(void)
{

}

void CWater::SetTransform(void)
{

}

HRESULT CWater::AddComponent(void)
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

CWater * CWater::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWater* pInstance = new CWater(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
