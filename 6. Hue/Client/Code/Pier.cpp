#include "stdafx.h"
#include "Pier.h"

#include "Include.h"
#include "Export_Function.h"
USING(Engine)

CPier::CPier(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CPier::~CPier(void)
{
	Release();
}

HRESULT CPier::Initialize(void)
{
	m_pBufferName = L"Buffer_CubeFront";
	m_pTexName = L"Texture_Pier";

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_pInfoCom->m_vPos = { 0.5f, 0.f, 0.002f };

	int s = 0;
	for (size_t i = 0; i < 6; ++i)
	{
		INFO tInfo;
		//tInfo.vPos = { (i * 3.f) + 12.f, -1.5f, 0.003f };
		tInfo.vPos = { (i * 3.f) + 12.f, -2.56f, 0.003f };

		m_listInfo.push_back(tInfo);
		s = tInfo.vPos.x;
	}

	for (size_t i = 0; i < 6; ++i)
	{
		INFO tInfo;
		//tInfo.vPos = { (i * 5.545f) + s - 2.215f, -1.8f, 0.003f };
		tInfo.vPos = { (i * 5.545f) + s - 2.215f, -2.585f, 0.003f };

		m_listBridgeInfo.push_back(tInfo);
	}

	return S_OK;
}

void CPier::Update(void)
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_bGo = true;

	int i = 0;
	for (auto& tInfo : m_listInfo)
	{
		float fX = i * 3.f;
		if (true == m_bGo)
		{
			if (fX + 5.5f < tInfo.vPos.x)
				tInfo.vPos.x -= 1.6f * GET_TIME;
		}
		++i;

		//m_pInfoCom->m_vScale = { 1.5f, 0.8f, 0.f };
		m_pInfoCom->m_vScale = { 1.5f, 1.9f, 0.f };

		Engine::CPipeline::MakeWorldMatrix(&tInfo.matWorld,
			&m_pInfoCom->m_vScale,
			m_pInfoCom->m_fAngle,
			&tInfo.vPos);
	}

	//i = 0;
	for (auto& tInfo : m_listBridgeInfo)
	{
		float fX = i * 5.545f;
		if (true == m_bGo)
		{
			if (fX + 0.f < tInfo.vPos.x)
				tInfo.vPos.x -= 1.6f * GET_TIME;
		}
		++i;

		//m_pInfoCom->m_vScale = { 2.8f, 1.3f, 0.f };
		m_pInfoCom->m_vScale = { 2.8f, 2.2f, 0.f };

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

void CPier::LateUpdate(void)
{

}

void CPier::Render(void)
{
	D3DXMATRIX matView = *CCameraObserver::GetInstance()->GetView();
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProj = *CCameraObserver::GetInstance()->GetProj();
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

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	//m_pTextureCom->SetTexture(0);
	//m_pBufferCom->Render_NoIndices();

	if (D3DCOLOR_XRGB(128, 128, 128) != CCubeMgr::GetInstance()->GetBackColor())
	{
		tMtrl.Ambient = PIER;
		tMtrl.Diffuse = PIER;
		tMtrl.Specular = PIER;
		m_pGraphicDev->SetMaterial(&tMtrl);						// 재질★
	}

	for (auto& tInfo : m_listInfo)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &tInfo.matWorld);
		m_pTextureCom->SetTexture(0);
		m_pBufferCom->Render_NoIndices();
	}

	int i = 0;
	for (auto& tInfo : m_listBridgeInfo)
	{
		if (0 == i)
		{
			D3DXVECTOR3 vPos = tInfo.vPos;
			vPos.x += 4.f;
			vPos.y = -1.2f;
			vPos.y += 3.29f;
			m_pInfoCom->m_vScale = { 1.f, 2.5f, 0.f };

			Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
				&m_pInfoCom->m_vScale,
				m_pInfoCom->m_fAngle,
				&vPos);

			if (D3DCOLOR_XRGB(128, 128, 128) != CCubeMgr::GetInstance()->GetBackColor())
			{
				tMtrl.Ambient = WHITE;
				tMtrl.Diffuse = WHITE;
				tMtrl.Specular = WHITE;
				m_pGraphicDev->SetMaterial(&tMtrl);						// 재질★
			}

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
			m_pTextureCom->SetTexture(2);
			m_pBufferCom->Render_NoIndices();

			++i;
		}

		if (D3DCOLOR_XRGB(128, 128, 128) != CCubeMgr::GetInstance()->GetBackColor())
		{
			tMtrl.Ambient = MILDGREY;
			tMtrl.Diffuse = MILDGREY;
			tMtrl.Specular = MILDGREY;
			m_pGraphicDev->SetMaterial(&tMtrl);						// 재질★
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &tInfo.matWorld);
		m_pTextureCom->SetTexture(1);
		m_pBufferCom->Render_NoIndices();
	}

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CPier::RenderInsideWorld(void)
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

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	//m_pTextureCom->SetTexture(0);
	//m_pBufferCom->Render_NoIndices();

	for (auto& tInfo : m_listInfo)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &tInfo.matWorld);
		m_pTextureCom->SetTexture(0);
		m_pBufferCom->Render_NoIndices();
	}
	int i = 0;
	for (auto& tInfo : m_listBridgeInfo)
	{
		if (0 == i)
		{
			D3DXVECTOR3 vPos = tInfo.vPos;
			vPos.x += 4.f;
			vPos.y = -1.2f;
			vPos.y += 3.29f;
			m_pInfoCom->m_vScale = { 1.f, 2.5f, 0.f };

			Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
				&m_pInfoCom->m_vScale,
				m_pInfoCom->m_fAngle,
				&vPos);

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
			m_pTextureCom->SetTexture(2);
			m_pBufferCom->Render_NoIndices();

			++i;
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &tInfo.matWorld);
		m_pTextureCom->SetTexture(1);
		m_pBufferCom->Render_NoIndices();
	}

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CPier::Release(void)
{

}

void CPier::SetTransform(void)
{

}

HRESULT CPier::AddComponent(void)
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

CPier * CPier::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPier* pInstance = new CPier(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
