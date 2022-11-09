#include "stdafx.h"
#include "LaserEffect.h"
#include "Include.h"
#include "Export_Function.h"

USING(Engine)

CLaserEffect::CLaserEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pCameraObserver(CCameraObserver::GetInstance())
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CLaserEffect::~CLaserEffect(void)
{
	Release();
}


void CLaserEffect::SetPosAndInfo(float fX, float fY, float fZ, bool IsDown)
{
	m_bIsDown = IsDown;
	m_pInfoCom->m_vPos.x = fX;
	m_pInfoCom->m_vPos.y = fY;
	m_pInfoCom->m_vPos.z = fZ;

	D3DXVECTOR3 vecDir;

	if (m_bIsDown)
	{
		int iRot = rand() % 150;
		vecDir = D3DXVECTOR3(-3.f, 1.f, 0.f);
		D3DXMATRIX mRot;
		D3DXMatrixRotationZ(&mRot, D3DXToRadian(-iRot));
		D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);
	}
	else
	{
		int iRot = rand() % 150;
		vecDir = D3DXVECTOR3(-1.f, 3.f, 0.f);
		D3DXMATRIX mRot;
		D3DXMatrixRotationZ(&mRot, D3DXToRadian(iRot));
		D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);
	}
	D3DXMATRIX mRot;
	int iRot = rand() % 90;
	int iReverseRand = rand() % 2;
	if (iReverseRand == 0)
		iRot *= -1;
	D3DXMatrixRotationX(&mRot, D3DXToRadian(iRot));
	D3DXVec3TransformNormal(&vecDir, &vecDir, &mRot);

	m_pInfoCom->m_vDir = vecDir;
}

HRESULT CLaserEffect::Initialize(void)
{
	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pBufferName = L"Buffer_Cube_Player";
	m_pTexName = L"Texture_Snow";
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_pInfoCom->m_vScale.x = 0.125f;
	m_pInfoCom->m_vScale.y = 0.125f;
	m_pInfoCom->m_vScale.z = 0.125f;

	return S_OK;
}

void CLaserEffect::Update(void)
{
	

}

int CLaserEffect::LaserEffectUpdate()
{
	if (GET_TIME < 0.7f)
	{
		if (m_bIsDown)
		{
			m_pInfoCom->m_vPos += m_pInfoCom->m_vDir * 0.75f * GET_TIME;
			m_pInfoCom->m_vScale.x -= 0.35f * GET_TIME;
			m_pInfoCom->m_vScale.y -= 0.35f * GET_TIME;
		}
		else
		{
			m_pInfoCom->m_vPos += m_pInfoCom->m_vDir * 0.75f * GET_TIME;
			m_pInfoCom->m_vScale.x -= 0.35f * GET_TIME;
			m_pInfoCom->m_vScale.y -= 0.35f * GET_TIME;
		}
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

void CLaserEffect::LateUpdate(void)
{

}

void CLaserEffect::Render(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetMaterial(&m_tMtrl);						//재질★
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	//m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	m_pTextureCom->SetTexture(0);
	//m_pBufferCom->Render();
	m_pBufferCom->Render_NoIndices();
}

void CLaserEffect::Release(void)
{

}

void CLaserEffect::SetMaterial(Engine::CUBE_COLOR eColor)
{
	switch (eColor)
	{
	case Engine::CUBE_RED:
		m_tMtrl.Ambient = RED;
		m_tMtrl.Diffuse = RED;
		m_tMtrl.Specular = RED;
		m_tMtrl.Emissive = BLACK;
		m_tMtrl.Power = 5.f;
		break;
	case Engine::CUBE_ORANGE:
		m_tMtrl.Ambient = ORANGE;
		m_tMtrl.Diffuse = ORANGE;
		m_tMtrl.Specular = ORANGE;
		m_tMtrl.Emissive = BLACK;
		m_tMtrl.Power = 5.f;
		break;
	case Engine::CUBE_GREEN:
		m_tMtrl.Ambient = GREEN;
		m_tMtrl.Diffuse = GREEN;
		m_tMtrl.Specular = GREEN;
		m_tMtrl.Emissive = BLACK;
		m_tMtrl.Power = 5.f;
		break;
	case Engine::CUBE_YELLOW:
		m_tMtrl.Ambient = YELLOW;
		m_tMtrl.Diffuse = YELLOW;
		m_tMtrl.Specular = YELLOW;
		m_tMtrl.Emissive = BLACK;
		m_tMtrl.Power = 5.f;
		break;
	case Engine::CUBE_BLUE:
		m_tMtrl.Ambient = BLUE;
		m_tMtrl.Diffuse = BLUE;
		m_tMtrl.Specular = BLUE;
		m_tMtrl.Emissive = BLACK;
		m_tMtrl.Power = 5.f;
		break;
	case Engine::CUBE_SKYBLUE:
		m_tMtrl.Ambient = SKYBLUE;
		m_tMtrl.Diffuse = SKYBLUE;
		m_tMtrl.Specular = SKYBLUE;
		m_tMtrl.Emissive = BLACK;
		m_tMtrl.Power = 5.f;
		break;
	case Engine::CUBE_PINK:
		m_tMtrl.Ambient = PINK;
		m_tMtrl.Diffuse = PINK;
		m_tMtrl.Specular = PINK;
		m_tMtrl.Emissive = BLACK;
		m_tMtrl.Power = 5.f;
		break;
	case Engine::CUBE_PURPLE:
		m_tMtrl.Ambient = PURPLE;
		m_tMtrl.Diffuse = PURPLE;
		m_tMtrl.Specular = PURPLE;
		m_tMtrl.Emissive = BLACK;
		m_tMtrl.Power = 5.f;
		break;
	case Engine::CUBE_COLOR_END:
		m_tMtrl.Ambient = BLACK;
		m_tMtrl.Diffuse = BLACK;
		m_tMtrl.Specular = BLACK;
		m_tMtrl.Emissive = BLACK;
		m_tMtrl.Power = 5.f;
		break;
	default:
		break;
	}
}


HRESULT CLaserEffect::AddComponent(void)
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

CLaserEffect * CLaserEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLaserEffect* pInstance = new CLaserEffect(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
