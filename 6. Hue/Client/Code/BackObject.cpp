#include "stdafx.h"
#include "BackObject.h"
#include "Include.h"
#include "Export_Function.h"
#include "BackSmoke.h"
#include "BackDrGrey.h"
#include "BackMum.h"
#include "BackSeaHorse.h"
#include "BackWolf.h"
#include "BackBook.h"
#include "BackHue.h"
#include "BackKeyBoard.h"
#include "BackGameLogo.h"
USING(Engine)

CBackObject::CBackObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CBackObject::~CBackObject(void)
{
	Release();
}

HRESULT CBackObject::Initialize(void)
{
	m_pBackSmoke = CBackSmoke::Create(m_pGraphicDev);
	m_pBackDrGrey = CBackDrGrey::Create(m_pGraphicDev);
	m_pBackMum = CBackMum::Create(m_pGraphicDev);
	m_pBackSeaHorse = CBackSeaHorse::Create(m_pGraphicDev);
	m_pBackWolf = CBackWolf::Create(m_pGraphicDev);
	m_pBackBook = CBackBook::Create(m_pGraphicDev);
	m_pBackHue = CBackHue::Create(m_pGraphicDev);
	m_pBackKeyBoard = CBackKeyBoard::Create(m_pGraphicDev);
	m_pBackGameLogo = CBackGameLogo::Create(m_pGraphicDev);

	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pBufferName = L"Buffer_RcTex";
	m_pTexName = L"Texture_BackObject";
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVtx = new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->CopyToClient(Engine::RESOURCE_STATIC, m_pBufferName, m_pVertex);

	return S_OK;
}

void CBackObject::Update(void)
{
	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;
	
	m_pBackSeaHorse->Update();
	m_pBackSmoke->Update();
	m_pBackDrGrey->Update();
	m_pBackWolf->Update();
	m_pBackBook->SetMaterial((int)m_fColorR, (int)m_fColorG, (int)m_fColorB);
	m_pBackBook->Update();//
	m_pBackMum->SetMaterial((int)m_fColorR, (int)m_fColorG, (int)m_fColorB);
	m_pBackMum->Update();//
	m_pBackHue->SetMaterial((int)m_fColorR, (int)m_fColorG, (int)m_fColorB);
	m_pBackHue->Update();//
	m_pBackKeyBoard->Update();
	m_pBackGameLogo->SetMaterial((int)m_fColorR, (int)m_fColorG, (int)m_fColorB);
	m_pBackGameLogo->Update();//
}

void CBackObject::LateUpdate(void)
{
	m_pBackSeaHorse->LateUpdate();
	m_pBackSmoke->LateUpdate();
	m_pBackDrGrey->LateUpdate();
	m_pBackWolf->LateUpdate();
	m_pBackBook->LateUpdate();
	m_pBackMum->LateUpdate();
	m_pBackHue->LateUpdate();
	m_pBackKeyBoard->LateUpdate();
	m_pBackGameLogo->LateUpdate();
}

void CBackObject::Render(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetMaterial(&m_tMtrl);		//ÀçÁú¡Ú
	//m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pInfoCom->m_vScale = D3DXVECTOR3(3.2f, 3.2f, 1.f);
	m_pInfoCom->m_vPos.y = 0.f;
	m_pInfoCom->m_vPos.z = 3.f;
	SetTransform();
	m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	m_pTextureCom->SetTexture(1);
	m_pBufferCom->Render();

	m_pInfoCom->m_vPos.y = 1.2f;
	m_pInfoCom->m_vScale = D3DXVECTOR3(3.3f, 2.f, 1.f);
	m_pInfoCom->m_vPos.z = 2.5f;
	SetTransform();
	m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	m_pTextureCom->SetTexture(0);
	m_pBufferCom->Render();

	m_pBackSeaHorse->Render();
	m_pBackSmoke->Render();
	m_pBackDrGrey->Render();
	m_pBackWolf->Render();
	m_pBackBook->Render();
	m_pBackMum->Render();
	m_pBackHue->Render();
	m_pBackKeyBoard->Render();
	m_pBackGameLogo->Render();

}

void CBackObject::Release(void)
{
	//m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pVertex);

	Engine::Safe_Delete(m_pBackGameLogo);
	Engine::Safe_Delete(m_pBackKeyBoard);
	Engine::Safe_Delete(m_pBackHue);
	Engine::Safe_Delete(m_pBackBook);
	Engine::Safe_Delete(m_pBackWolf);
	Engine::Safe_Delete(m_pBackSeaHorse);
	Engine::Safe_Delete(m_pBackMum);
	Engine::Safe_Delete(m_pBackDrGrey);
	Engine::Safe_Delete(m_pBackSmoke);

	if (m_pVertex)
		Engine::Safe_Delete_Array(m_pVertex);
	if (m_pConvertVtx)
		Engine::Safe_Delete_Array(m_pConvertVtx);
}

void CBackObject::SetMaterialColor(float fR, float fG, float fB)
{
	m_fColorR = fR;
	m_fColorG = fG;
	m_fColorB = fB;
}

void CBackObject::SetTransform(void)
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

HRESULT CBackObject::AddComponent(void)
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

CBackObject * CBackObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBackObject* pInstance = new CBackObject(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
