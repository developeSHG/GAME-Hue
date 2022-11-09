#include "stdafx.h"
#include "Cloud.h"

#include "Include.h"
#include "Export_Function.h"
#include "PlayerTuto.h"
USING(Engine)

CCloud::CCloud(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CCloud::~CCloud(void)
{
	Release();
}

HRESULT CCloud::Initialize(void)
{
	m_pBufferName = L"Buffer_CubeFront";
	m_pTexName = L"Texture_Cloud";

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	//m_pInfoCom->m_vPos = { 0.f, 3.3f, 0.004f };
	m_pInfoCom->m_vScale = { 7.37f, 3.1f, 0.f };

	for (size_t i = 0; i < 7; ++i)
	{
		INFO tInfo;
		tInfo.vPos = { (i * 14.7f) - 14.7f, 4.2f, 0.5f };

		m_listInfo.push_back(tInfo);
	}

	return S_OK;
}

void CCloud::Update(void)
{
	//cout << "ComX : " << m_pInfoCom->m_vPos.x << endl;
	////cout << "Y : " << m_pInfoCom->m_vPos.y << endl;
	//cout << "InfoX : " << m_tInfo.vPos.x << endl;

	for (auto& tInfo : m_listInfo)
	{
		if (0.7f > GET_TIME)
			tInfo.vPos.x -= 0.35f * GET_TIME;

		if (-14.7f * 2 >= tInfo.vPos.x)
			tInfo.vPos.x = (5 * 14.7f);

		if (21.9f < CCameraObserver::GetInstance()->GetPlayerTuto()->Get_PlayerInfoCom()->m_vPos.x)
		{
			if (4.7f > tInfo.vPos.y)
			{
				tInfo.vPos.y += 1.f * GET_TIME;
			}
			else
				tInfo.vPos.y = 4.7f;
		}
		else
		{
			if (4.2f < tInfo.vPos.y)
			{
				tInfo.vPos.y -= 1.f * GET_TIME;
			}
			else
				tInfo.vPos.y = 4.2f;
		}

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


void CCloud::LateUpdate(void)
{

}

void CCloud::Render(void)
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

void CCloud::RenderInsideWorld(void)
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

void CCloud::Release(void)
{

}

void CCloud::SetTransform(void)
{

}

HRESULT CCloud::AddComponent(void)
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

CCloud * CCloud::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCloud* pInstance = new CCloud(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
