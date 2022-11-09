#include "stdafx.h"
#include "Dust.h"
#include "Include.h"
#include "Export_Function.h"

USING(Engine)

CDust::CDust(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CDust::~CDust(void)
{
	Release();
}


void CDust::SetPosAndInfo(float fX, float fY, DUSTEFFECTID eID, bool IsLeft)
{
	//int iRand = rand() % 3;
	//if (iRand == 0)
	//	m_pInfoCom->m_vScale = D3DXVECTOR3(0.025f, 0.025f, 1.f);
	//else if (iRand == 1)
	//	m_pInfoCom->m_vScale = D3DXVECTOR3(0.03f, 0.03f, 1.f);
	//else
	//	m_pInfoCom->m_vScale = D3DXVECTOR3(0.02f, 0.02f, 1.f);

	D3DXVECTOR3	vecDir;
	
	m_bIsLeft = IsLeft;

	m_eDustID = eID;
	switch (m_eDustID)
	{
	case DUST_ONE:
		if (!m_bIsLeft)
		{
			m_pInfoCom->m_vPos.x = fX - 0.2f;
			m_pInfoCom->m_vPos.y = fY - 0.4f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.025f, 0.025f, 1.f);
			vecDir = D3DXVECTOR3(-1.f, 1.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 1.f;
		}
		else
		{
			m_pInfoCom->m_vPos.x = fX + 0.2f;
			m_pInfoCom->m_vPos.y = fY - 0.4f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.025f, 0.025f, 1.f);
			vecDir = D3DXVECTOR3(1.f, 1.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 1.f;
		}
		break;
	case DUST_TWO:
		if (!m_bIsLeft)
		{
			m_pInfoCom->m_vPos.x = fX - 0.25f;
			m_pInfoCom->m_vPos.y = fY - 0.4f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.03f, 0.03f, 1.f);
			vecDir = D3DXVECTOR3(-0.5f, 2.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 1.f;
		}
		else
		{
			m_pInfoCom->m_vPos.x = fX + 0.25f;
			m_pInfoCom->m_vPos.y = fY - 0.4f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.03f, 0.03f, 1.f);
			vecDir = D3DXVECTOR3(0.5f, 2.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 1.f;
		}
		break;
	case DUST_THREE:
		if (!m_bIsLeft)
		{
			m_pInfoCom->m_vPos.x = fX - 0.225f;
			m_pInfoCom->m_vPos.y = fY - 0.35f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.02f, 0.02f, 1.f);
			vecDir = D3DXVECTOR3(-0.5f, 100.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 1.f;
		}
		else
		{
			m_pInfoCom->m_vPos.x = fX + 0.225f;
			m_pInfoCom->m_vPos.y = fY - 0.35f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.02f, 0.02f, 1.f);
			vecDir = D3DXVECTOR3(0.5f, 100.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 1.f;
		}
		break;
	case DUST_FOUR:
		if (!m_bIsLeft)
		{
			m_pInfoCom->m_vPos.x = fX - 0.17f;
			m_pInfoCom->m_vPos.y = fY - 0.3f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.02f, 0.02f, 1.f);
			vecDir = D3DXVECTOR3(-1.f, 3.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 1.f;
		}
		else
		{
			m_pInfoCom->m_vPos.x = fX + 0.17f;
			m_pInfoCom->m_vPos.y = fY - 0.3f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.02f, 0.02f, 1.f);
			vecDir = D3DXVECTOR3(1.f, 3.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 1.f;
		}
		break;
	case DUST_FIVE:
		if (!m_bIsLeft)
		{
			m_pInfoCom->m_vPos.x = fX - 0.12f;
			m_pInfoCom->m_vPos.y = fY - 0.3f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.035f, 0.035f, 1.f);
			vecDir = D3DXVECTOR3(1.f, 3.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 1.f;
		}
		else
		{
			m_pInfoCom->m_vPos.x = fX -+0.12f;
			m_pInfoCom->m_vPos.y = fY - 0.3f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.035f, 0.035f, 1.f);
			vecDir = D3DXVECTOR3(-1.f, 3.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 1.f;
		}
		break;
	case DUST_SIX:
		if (!m_bIsLeft)
		{
			m_pInfoCom->m_vPos.x = fX - 0.18f;
			m_pInfoCom->m_vPos.y = fY - 0.3f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.035f, 0.035f, 1.f);
			vecDir = D3DXVECTOR3(0.f, 1.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 1.f;
		}
		else
		{
			m_pInfoCom->m_vPos.x = fX + 0.18f;
			m_pInfoCom->m_vPos.y = fY - 0.3f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.035f, 0.035f, 1.f);
			vecDir = D3DXVECTOR3(0.f, 1.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 1.f;
		}
		break;
	case DUSTID_END:
		break;
	default:
		break;
	}


	m_pInfoCom->m_vDir = vecDir;
}

HRESULT CDust::Initialize(void)
{
	m_tMtrl.Ambient = BLACK;
	m_tMtrl.Diffuse = BLACK;
	m_tMtrl.Specular = BLACK;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pBufferName = L"Buffer_Cube_Player";
	m_pTexName = L"Particle_Dust";
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	//m_pResourceMgr->CopyToClient(Engine::RESOURCE_STATIC, m_pBufferName, m_pVertex);

	return S_OK;
}

void CDust::Update(void)
{

}

int CDust::DustUpdate()
{
	switch (m_eDustID)
	{
	case DUST_ONE:
		UpdateDustOne();
		break;
	case DUST_TWO:
		UpdateDustTwo();
		break;
	case DUST_THREE:
		UpdateDustThree();
		break;
	case DUST_FOUR:
		UpdateDustFour();
		break;
	case DUST_FIVE:
		UpdateDustFive();
		break;
	case DUST_SIX:
		UpdateDustSix();
		break;
	case DUSTID_END:
		break;
	default:
		break;
	}

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	//SetTransform();
	
	if (m_fDieTime < 0.f)
		return 0;

	return 1;
}

void CDust::LateUpdate(void)
{

}

void CDust::Render(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetMaterial(&m_tMtrl);						//ÀçÁú¡Ú
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	//m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	m_pTextureCom->SetTexture(3);
	m_pBufferCom->Render_NoIndices();
}

void CDust::Release(void)
{
	
}

void CDust::SetTransform(void)
{

}

void CDust::UpdateDustOne()
{
	if (GET_TIME < 0.7f)
	{
		m_fvecAngle += 325.f * GET_TIME;

		D3DXVECTOR3 vecMove;
		D3DXMATRIX mRot;
		D3DXMatrixRotationZ(&mRot, D3DXToRadian(m_fvecAngle));
		D3DXVec3TransformNormal(&vecMove, &m_pInfoCom->m_vDir, &mRot);
		m_pInfoCom->m_vPos += 0.5f * vecMove * GET_TIME;

		m_fDieTime -= 3.5f * GET_TIME;
	}
}

void CDust::UpdateDustTwo()
{
	if (GET_TIME < 0.7f)
	{
		m_fvecAngle += 435.f * GET_TIME;

		D3DXVECTOR3 vecMove;
		D3DXMATRIX mRot;
		D3DXMatrixRotationZ(&mRot, D3DXToRadian(m_fvecAngle));
		D3DXVec3TransformNormal(&vecMove, &m_pInfoCom->m_vDir, &mRot);
		m_pInfoCom->m_vPos += 1.f * vecMove * GET_TIME;

		m_fDieTime -= 8.f * GET_TIME;
	}
}

void CDust::UpdateDustThree()
{
	if (GET_TIME < 0.7f)
	{
		m_fvecAngle += 255.f * GET_TIME;

		D3DXVECTOR3 vecMove;
		D3DXMATRIX mRot;
		D3DXMatrixRotationZ(&mRot, D3DXToRadian(m_fvecAngle));
		D3DXVec3TransformNormal(&vecMove, &m_pInfoCom->m_vDir, &mRot);
		m_pInfoCom->m_vPos += 1.f * vecMove * GET_TIME;

		m_fDieTime -= 8.f * GET_TIME;
	}
}

void CDust::UpdateDustFour()
{
	if (GET_TIME < 0.7f)
	{
		m_fvecAngle += 205.f * GET_TIME;

		D3DXVECTOR3 vecMove;
		D3DXMATRIX mRot;
		D3DXMatrixRotationZ(&mRot, D3DXToRadian(m_fvecAngle));
		D3DXVec3TransformNormal(&vecMove, &m_pInfoCom->m_vDir, &mRot);
		m_pInfoCom->m_vPos += 1.f * vecMove * GET_TIME;

		m_fDieTime -= 7.f * GET_TIME;
	}
}

void CDust::UpdateDustFive()
{
	if (GET_TIME < 0.7f)
	{
		m_fvecAngle += 105.f * GET_TIME;

		D3DXVECTOR3 vecMove;
		D3DXMATRIX mRot;
		D3DXMatrixRotationZ(&mRot, D3DXToRadian(m_fvecAngle));
		D3DXVec3TransformNormal(&vecMove, &m_pInfoCom->m_vDir, &mRot);
		m_pInfoCom->m_vPos += 2.f * vecMove * GET_TIME;

		m_fDieTime -= 8.f * GET_TIME;
	}
}

void CDust::UpdateDustSix()
{
	if (GET_TIME < 0.7f)
	{
		m_fvecAngle += 5.f * GET_TIME;

		D3DXVECTOR3 vecMove;
		D3DXMATRIX mRot;
		D3DXMatrixRotationZ(&mRot, D3DXToRadian(m_fvecAngle));
		D3DXVec3TransformNormal(&vecMove, &m_pInfoCom->m_vDir, &mRot);
		m_pInfoCom->m_vPos += 2.f * vecMove * GET_TIME;

		m_fDieTime -= 8.f * GET_TIME;
	}
}

HRESULT CDust::AddComponent(void)
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

CDust * CDust::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDust* pInstance = new CDust(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
