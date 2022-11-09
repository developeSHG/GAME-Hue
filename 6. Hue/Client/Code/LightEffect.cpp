#include "stdafx.h"
#include "LightEffect.h"
#include "Include.h"
#include "Export_Function.h"
USING(Engine)

CLightEffect::CLightEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pCameraObserver(CCameraObserver::GetInstance())
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CLightEffect::~CLightEffect(void)
{
	Release();
}


void CLightEffect::SetPosAndInfo(float fX, float fY, Engine::OBJDECOID eID)
{
	m_pInfoCom->m_vPos.x = fX;
	m_pInfoCom->m_vPos.y = fY;
	m_pInfoCom->m_vPos.z = 0.5f;

	m_eID = eID;

	m_tBlackMtrl.Ambient = BLACK;
	m_tBlackMtrl.Diffuse = BLACK;
	m_tBlackMtrl.Specular = BLACK;
	m_tBlackMtrl.Emissive = BLACK;
	m_tBlackMtrl.Power = 5.f;

	int iRot;
	D3DXVECTOR3 vecDir;
	D3DXMATRIX mRot;

	switch (eID)
	{
	case Engine::OBJDECO_FIREPOT:
		iRot = rand() % 115;
		vecDir = D3DXVECTOR3(3.5f, 2.f, 0.f);

		D3DXMatrixRotationZ(&mRot, D3DXToRadian(iRot));
		D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);

		m_pInfoCom->m_vPos += vecDir * 0.08f;
		m_pInfoCom->m_vScale.x = 0.255f;
		m_pInfoCom->m_vScale.y = 0.255f;
		m_pInfoCom->m_vScale.z = 0.155f;
		m_pInfoCom->m_vPos.z = 0.3f;
		break;
	case Engine::OBJDECO_TOPFIRE:
		iRot = rand() % 135;
		vecDir = D3DXVECTOR3(3.5f, 1.f, 0.f);

		D3DXMatrixRotationZ(&mRot, D3DXToRadian(iRot));
		D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);

		m_pInfoCom->m_vPos += vecDir * 0.08f;
		m_pInfoCom->m_vScale.x = 0.255f;
		m_pInfoCom->m_vScale.y = 0.255f;
		m_pInfoCom->m_vScale.z = 0.155f;
		m_pInfoCom->m_vPos.z = 0.3f;
		break;
	case Engine::OBJDECO_LEFTLANTERN:

		iRot = rand() % 30;
		vecDir = D3DXVECTOR3(1.f, 5.f, 0.f);

		D3DXMatrixRotationZ(&mRot, D3DXToRadian(iRot));
		D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);

		m_pInfoCom->m_vPos += vecDir * 0.08f;
		m_pInfoCom->m_vScale.x = 0.225f;
		m_pInfoCom->m_vScale.y = 0.225f;
		m_pInfoCom->m_vScale.z = 0.155f;
		m_pInfoCom->m_vPos.z = -0.3f;
		break;
	case Engine::OBJDECO_RIGHTLANTERN:
		iRot = rand() % 45;
		vecDir = D3DXVECTOR3(2.f, 5.f, 0.f);

		D3DXMatrixRotationZ(&mRot, D3DXToRadian(iRot));
		D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);

		m_pInfoCom->m_vPos += vecDir * 0.08f;
		m_pInfoCom->m_vScale.x = 0.225f;
		m_pInfoCom->m_vScale.y = 0.225f;
		m_pInfoCom->m_vScale.z = 0.155f;
		m_pInfoCom->m_vPos.z = -0.3f;
		break;
	case Engine::OBJDECOID_END:
		break;
	default:
		break;
	}

	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;

	//D3DXMatrixRotationX(&mRot, D3DXToRadian(iRot));
	//D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);


	m_pInfoCom->m_vDir = vecDir;

	m_pInfoCom->m_vScale.z = 0.f;
}

HRESULT CLightEffect::Initialize(void)
{
	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pBufferName = L"Buffer_CubeFront";
	m_pTexName = L"Texture_Fire";
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_pInfoCom->m_vScale.x = 0.125f;
	m_pInfoCom->m_vScale.y = 0.125f;
	m_pInfoCom->m_vScale.z = 0.125f;

	return S_OK;
}

void CLightEffect::Update(void)
{
	

}

int CLightEffect::LightEffectUpdate()
{
	switch (m_eID)
	{
	case Engine::OBJDECO_FIREPOT:
		if (GET_TIME < 0.7f)
		{
			m_pInfoCom->m_vPos += m_pInfoCom->m_vDir * 1.f * GET_TIME;
			m_pInfoCom->m_vScale.x -= 0.28f * GET_TIME;
			m_pInfoCom->m_vScale.y -= 0.28f * GET_TIME;
		}
		break;
	case Engine::OBJDECO_TOPFIRE:
		if (GET_TIME < 0.7f)
		{
			m_pInfoCom->m_vPos += m_pInfoCom->m_vDir * 0.45f * GET_TIME;
			m_pInfoCom->m_vScale.x -= 0.42f * GET_TIME;
			m_pInfoCom->m_vScale.y -= 0.42f * GET_TIME;
		}
		break;
	case Engine::OBJDECO_LEFTLANTERN:
		if (GET_TIME < 0.7f)
		{
			m_pInfoCom->m_vPos += m_pInfoCom->m_vDir * 0.4f * GET_TIME;
			m_pInfoCom->m_vScale.x -= 0.45f * GET_TIME;
			m_pInfoCom->m_vScale.y -= 0.45f * GET_TIME;
		}
		break;
	case Engine::OBJDECO_RIGHTLANTERN:
		if (GET_TIME < 0.7f)
		{
			m_pInfoCom->m_vPos += m_pInfoCom->m_vDir * 0.4f * GET_TIME;
			m_pInfoCom->m_vScale.x -= 0.35f * GET_TIME;
			m_pInfoCom->m_vScale.y -= 0.35f * GET_TIME;
		}
		break;
	case Engine::OBJDECOID_END:
		break;
	default:
		break;
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

void CLightEffect::LateUpdate(void)
{

}

void CLightEffect::Render(void)
{
	/*if (m_eID == Engine::OBJDECO_LEFTLANTERN)
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, false);*/
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetMaterial(&m_tMtrl);						//재질★
	
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 클리핑  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	//m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	m_pTextureCom->SetTexture(0);
	//m_pBufferCom->Render();
	m_pBufferCom->Render_NoIndices();

	if (m_eID == Engine::OBJDECO_LEFTLANTERN)
		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, true);
}

void CLightEffect::RenderInside(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, false);	//★★★★★
	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetMaterial(&m_tBlackMtrl);						//재질★

																/************************ (SongSong) ************************/
																/*  카메라 시점에 따른 클리핑  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	//m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	m_pTextureCom->SetTexture(0);
	//m_pBufferCom->Render();
	m_pBufferCom->Render_NoIndices();
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, true);	//★★★★★
}

void CLightEffect::Release(void)
{

}


HRESULT CLightEffect::AddComponent(void)
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

CLightEffect * CLightEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLightEffect* pInstance = new CLightEffect(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
