#include "stdafx.h"
#include "KeyEffect.h"
#include "Include.h"
#include "Export_Function.h"
USING(Engine)

CKeyEffect::CKeyEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pCameraObserver(CCameraObserver::GetInstance())
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CKeyEffect::~CKeyEffect(void)
{
	Release();
}


void CKeyEffect::SetPosAndInfo(float fX, float fY, float fZ, bool IsKey, bool IsBattery/* = false*/)
{
	m_pInfoCom->m_vPos.x = fX;
	m_pInfoCom->m_vPos.y = fY;
	m_pInfoCom->m_vPos.z = fZ + 0.5f;

	int iRot = rand() % 360;

	D3DXVECTOR3 vecDir = D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXMATRIX mRot;
	D3DXMatrixRotationZ(&mRot, D3DXToRadian(iRot));
	D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);

	iRot = rand() % 90;
	int iReverseRand = rand() % 2;
	if (iReverseRand == 0)
		iRot *= -1;
	D3DXMatrixRotationX(&mRot, D3DXToRadian(iRot));
	D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);
	if (IsKey)
	{
		m_pInfoCom->m_vPos += vecDir * 0.12f;
		m_pInfoCom->m_vPos.z = fZ  - 0.2f;
	}
	else
	{
		m_pInfoCom->m_vPos += vecDir * 0.18f;
		m_pInfoCom->m_vScale.x = 0.155f;
		m_pInfoCom->m_vScale.y = 0.155f;
		m_pInfoCom->m_vScale.z = 0.155f;
		m_pInfoCom->m_vPos.z = fZ + 0.3f;
	}
	if (IsBattery)
	{
		m_pInfoCom->m_vScale.x = 0.15f;
		m_pInfoCom->m_vScale.y = 0.15f;
		m_pInfoCom->m_vScale.z = 0.15f;

		m_pInfoCom->m_vPos.z = fZ + 0.2f;
	}
	else
		m_pInfoCom->m_vScale *= 1.5f;

	m_pInfoCom->m_vDir = vecDir;

	m_pInfoCom->m_vScale.z = 0.f;
}

HRESULT CKeyEffect::Initialize(void)
{

	m_pBufferName = L"Buffer_CubeFront";
	m_pTexName = L"Texture_Snow";
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_pInfoCom->m_vScale.x = 0.125f;
	m_pInfoCom->m_vScale.y = 0.125f;
	m_pInfoCom->m_vScale.z = 0.125f;

	return S_OK;
}

void CKeyEffect::Update(void)
{
	

}

int CKeyEffect::KeyEffectUpdate()
{
	if (GET_TIME < 0.7f)
	{
		m_pInfoCom->m_vPos += m_pInfoCom->m_vDir * 1.f * GET_TIME;
		m_pInfoCom->m_vScale.x -= 0.18f * GET_TIME;
		m_pInfoCom->m_vScale.y -= 0.18f * GET_TIME;
	}


	if (m_pInfoCom->m_vScale.x <= 0.f || m_pInfoCom->m_vScale.y <= 0.f)
		return 0;

	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 각도  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(0.f);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(90.f);

 	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);


		
	return 1;
}

void CKeyEffect::LateUpdate(void)
{

}

void CKeyEffect::Render(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetMaterial(&m_tMtrl);						//재질★
	
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 클리핑  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	//m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	m_pTextureCom->SetTexture(0);
	//m_pBufferCom->Render();
	m_pBufferCom->Render_NoIndices();
}

void CKeyEffect::Release(void)
{

}

void CKeyEffect::SetMaterial(int iR, int iG, int iB)
{
	D3DXCOLOR randomColor = (D3DCOLOR_XRGB(iR, iG, iB));
	m_tMtrl.Ambient = randomColor;
	m_tMtrl.Diffuse = randomColor;
	m_tMtrl.Specular = randomColor;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;
}


HRESULT CKeyEffect::AddComponent(void)
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
	D3DXVECTOR3	vecDir(-1.f, 1.f, 0.f);
	D3DXVec3Normalize(&vecDir, &vecDir);
	pComponent = Engine::CTransform::Create(vecDir);
	m_pInfoCom = dynamic_cast<Engine::CTransform*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.emplace(L"Transform", m_pInfoCom);

	return S_OK;
}

CKeyEffect * CKeyEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CKeyEffect* pInstance = new CKeyEffect(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
