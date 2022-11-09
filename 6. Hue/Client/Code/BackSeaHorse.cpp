#include "stdafx.h"
#include "BackSeaHorse.h"
#include "Include.h"
#include "Export_Function.h"

USING(Engine)

CBackSeaHorse::CBackSeaHorse(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CBackSeaHorse::~CBackSeaHorse(void)
{
	Release();
}

HRESULT CBackSeaHorse::Initialize(void)
{
	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pBufferName = L"Buffer_RcTex";
	m_pTexName = L"Texture_BackSeaHorse";
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVtx = new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->CopyToClient(Engine::RESOURCE_STATIC, m_pBufferName, m_pVertex);


	m_pInfoCom->m_vScale = D3DXVECTOR3(0.7f, 1.1f, 1.f);

	m_pInfoCom->m_vPos.x = -1.8f;
	m_pInfoCom->m_vPos.y = -0.7f;
	m_pInfoCom->m_vPos.z = 2.3f;

	return S_OK;
}

void CBackSeaHorse::Update(void)
{
	SetTransform();
}

void CBackSeaHorse::LateUpdate(void)
{
}

void CBackSeaHorse::Render(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetMaterial(&m_tMtrl);		//ÀçÁú¡Ú

	m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);

	m_pTextureCom->SetTexture(0);
	m_pBufferCom->Render();

}

void CBackSeaHorse::Release(void)
{
	if (m_pVertex)
		Engine::Safe_Delete_Array(m_pVertex);
	if (m_pConvertVtx)
		Engine::Safe_Delete_Array(m_pConvertVtx);
}

void CBackSeaHorse::SetTransform(void)
{
	const D3DXMATRIX*		matView = CCameraObserver::GetInstance()->GetView();
	const D3DXMATRIX*		matProj = CCameraObserver::GetInstance()->GetProj();

	D3DXMATRIX matTrans, matScale;
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixTranslation(&matTrans, m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y, m_pInfoCom->m_vPos.z);
	D3DXMatrixScaling(&matScale, m_pInfoCom->m_vScale.x, m_pInfoCom->m_vScale.y, 1.f);
	m_pInfoCom->m_matWorld = matScale * matTrans;

	for (size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVtx[i] = m_pVertex[i];

		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, &m_pInfoCom->m_matWorld);
		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, matView);
		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, matProj);
	}
}


HRESULT CBackSeaHorse::AddComponent(void)
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

CBackSeaHorse * CBackSeaHorse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBackSeaHorse* pInstance = new CBackSeaHorse(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
