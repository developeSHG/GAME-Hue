#include "stdafx.h"
#include "BackHue.h"
#include "Include.h"
#include "Export_Function.h"
#include "BackColorChunk.h"
USING(Engine)

CBackHue::CBackHue(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CBackHue::~CBackHue(void)
{
	Release();
}

HRESULT CBackHue::Initialize(void)
{

	m_pBackColorChunk = CBackColorChunk::Create(m_pGraphicDev);

	m_tWhiteMtrl.Ambient = WHITE;
	m_tWhiteMtrl.Diffuse = WHITE;
	m_tWhiteMtrl.Specular = WHITE;
	m_tWhiteMtrl.Emissive = BLACK;
	m_tWhiteMtrl.Power = 5.f;

	m_tMtrl.Ambient = ORANGE;
	m_tMtrl.Diffuse = ORANGE;
	m_tMtrl.Specular = ORANGE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pBufferName = L"Buffer_RcTex";
	m_pTexName = L"Texture_BackHue";
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVtx = new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->CopyToClient(Engine::RESOURCE_STATIC, m_pBufferName, m_pVertex);


	m_pInfoCom->m_vScale = D3DXVECTOR3(1.25f, 1.35f, 1.f);

	m_pInfoCom->m_vPos.x = 0.4f;
	m_pInfoCom->m_vPos.y = -0.8f;
	m_pInfoCom->m_vPos.z = 0.5f;

	return S_OK;
}

void CBackHue::Update(void)
{
	//m_pBackColorChunk->SetMaterial()
	m_pBackColorChunk->Update();
	SetTransform();
}

void CBackHue::LateUpdate(void)
{
	m_pBackColorChunk->LateUpdate();
}

void CBackHue::Render(void)
{


	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetMaterial(&m_tWhiteMtrl);		//재질★

	SetTransform();
	m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	m_pInfoCom->m_vPos.z = 0.7f;
	SetTransform();
	m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	//문제
	m_pTextureCom->SetTexture(1);
	m_pBufferCom->Render();

	m_pInfoCom->m_vPos.z = 0.5f;
	SetTransform();
	m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	m_pTextureCom->SetTexture(2);
	m_pBufferCom->Render();


	m_pBackColorChunk->Render();
	m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★
	m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);

	m_pTextureCom->SetTexture(0);
	m_pBufferCom->Render();
}

void CBackHue::Release(void)
{
	Engine::Safe_Delete(m_pBackColorChunk);

	if (m_pVertex)
		Engine::Safe_Delete_Array(m_pVertex);
	if (m_pConvertVtx)
		Engine::Safe_Delete_Array(m_pConvertVtx);
}

void CBackHue::SetMaterial(int fR, int fG, int fB)
{
	D3DXCOLOR newMaterial(D3DCOLOR_XRGB(fR, fG, fB));
	m_tMtrl.Ambient = newMaterial;
	m_tMtrl.Diffuse = newMaterial;
	m_tMtrl.Specular = newMaterial;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;
	m_pBackColorChunk->SetMaterial(fR, fG, fB);
}

void CBackHue::SetTransform(void)
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

HRESULT CBackHue::AddComponent(void)
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

CBackHue * CBackHue::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBackHue* pInstance = new CBackHue(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
