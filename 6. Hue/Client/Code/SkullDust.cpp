#include "stdafx.h"
#include "SkullDust.h"
#include "Include.h"
#include "Export_Function.h"

USING(Engine)

CSkullDust::CSkullDust(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CSkullDust::~CSkullDust(void)
{
	Release();
}


void CSkullDust::SetPosAndInfo(float fX, float fY, SKULLDUSTEFFECTID eID, bool IsLeft)
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

	m_eSkullDustID = eID;
	switch (m_eSkullDustID)
	{
	case SKULLDUST_ONE:
		if (!m_bIsLeft)
		{
			m_pInfoCom->m_vPos.x = fX + 0.15f;
			m_pInfoCom->m_vPos.y = fY - 1.f;
			m_pInfoCom->m_vPos.z = -1.8f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.20f, 0.20f, 0.20f);
			vecDir = D3DXVECTOR3(1.f, 0.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 3.f;
		}
		else
		{
			m_pInfoCom->m_vPos.x = fX - 0.15f;
			m_pInfoCom->m_vPos.y = fY - 1.f;
			m_pInfoCom->m_vPos.z = -1.8f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.20f, 0.20f, 0.20f);
			m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(180.f);
			vecDir = D3DXVECTOR3(-1.f, 0.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 3.f;
		}
		m_iTexIndex = 0;
		break;
	case SKULLDUST_TWO:
		if (!m_bIsLeft)
		{
			m_pInfoCom->m_vPos.x = fX + 0.1f;
			m_pInfoCom->m_vPos.y = fY - 1.f;
			m_pInfoCom->m_vPos.z = -1.3f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.25f, 0.25f, 0.25f);
			vecDir = D3DXVECTOR3(1.f, 0.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 3.f;
		}
		else
		{
			m_pInfoCom->m_vPos.x = fX - 0.1f;
			m_pInfoCom->m_vPos.y = fY - 1.f;
			m_pInfoCom->m_vPos.z = -1.3f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.25f, 0.25f, 0.25f);
			m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(180.f);
			vecDir = D3DXVECTOR3(-1.f, 0.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 3.f;
		}
		m_iTexIndex = 1;
		break;
	case SKULLDUST_THREE:
		if (!m_bIsLeft)
		{
			m_pInfoCom->m_vPos.x = fX + 0.05f;
			m_pInfoCom->m_vPos.y = fY - 1.f;
			m_pInfoCom->m_vPos.z = -0.8f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.18f, 0.18f, 0.18f);
			vecDir = D3DXVECTOR3(1.f, 0.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 3.f;
		}
		else
		{
			m_pInfoCom->m_vPos.x = fX - 0.05f;
			m_pInfoCom->m_vPos.y = fY - 1.f;
			m_pInfoCom->m_vPos.z = -0.8f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.18f, 0.18f, 0.18f);
			m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(180.f);
			vecDir = D3DXVECTOR3(-1.f, 0.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 3.f;
		}
		m_iTexIndex = 2;
		break;
	case SKULLDUST_FOUR:
		if (!m_bIsLeft)
		{
			m_pInfoCom->m_vPos.x = fX + 0.05f;
			m_pInfoCom->m_vPos.y = fY - 1.f;
			m_pInfoCom->m_vPos.z = -0.8f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
			vecDir = D3DXVECTOR3(1.f, 0.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 3.f;
		}
		else
		{
			m_pInfoCom->m_vPos.x = fX - 0.05f;
			m_pInfoCom->m_vPos.y = fY - 1.f;
			m_pInfoCom->m_vPos.z = -0.8f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(0.22f, 0.22f, 0.22f);
			m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(180.f);
			vecDir = D3DXVECTOR3(-1.f, 0.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 3.f;
		}
		m_iTexIndex = 3;
		break;
	case SKULLDUST_FIVE:
		if (!m_bIsLeft)
		{
			m_pInfoCom->m_vPos.x = fX - 0.12f;
			m_pInfoCom->m_vPos.y = fY - 0.3f;
			m_pInfoCom->m_vPos.z = -0.3f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(1.025f, 1.025f, 1.f);
			vecDir = D3DXVECTOR3(1.f, 3.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 1.f;
		}
		else
		{
			m_pInfoCom->m_vPos.x = fX -+0.12f;
			m_pInfoCom->m_vPos.y = fY - 0.3f;
			m_pInfoCom->m_vPos.z = -0.3f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(1.025f, 1.025f, 1.f);
			vecDir = D3DXVECTOR3(-1.f, 3.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 1.f;
		}
		break;
	case SKULLDUST_SIX:
		if (!m_bIsLeft)
		{
			m_pInfoCom->m_vPos.x = fX - 0.18f;
			m_pInfoCom->m_vPos.y = fY - 0.3f;
			m_pInfoCom->m_vPos.z = -0.3f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(1.025f, 1.025f, 1.f);
			vecDir = D3DXVECTOR3(0.f, 1.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 1.f;
		}
		else
		{
			m_pInfoCom->m_vPos.x = fX + 0.18f;
			m_pInfoCom->m_vPos.y = fY - 0.3f;
			m_pInfoCom->m_vPos.z = -0.3f;
			m_pInfoCom->m_vScale = D3DXVECTOR3(1.025f, 1.025f, 1.f);
			vecDir = D3DXVECTOR3(0.f, 1.f, 0.f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_fDieTime = 1.f;
		}
		break;
	case SKULLDUSTID_END:
		break;
	default:
		break;
	}

	m_pInfoCom->m_vScale *= 1.35f;
	m_pInfoCom->m_vPos.y += 0.45f;
	

	m_bSwitch = false;

	m_pInfoCom->m_vDir = vecDir;
}

void CSkullDust::SetMaterial(Engine::CUBE_COLOR eColor)
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
		break;
	default:
		break;
	}
}

HRESULT CSkullDust::Initialize(void)
{
	m_tMtrl.Ambient = PINK;
	m_tMtrl.Diffuse = PINK;
	m_tMtrl.Specular = PINK;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_tWhiteMtrl.Ambient = WHITE;
	m_tWhiteMtrl.Diffuse = WHITE;
	m_tWhiteMtrl.Specular = WHITE;
	m_tWhiteMtrl.Emissive = BLACK;
	m_tWhiteMtrl.Power = 5.f;

	m_pBufferName = L"Buffer_Cube_Player";
	m_pTexName = L"Particle_Dust";
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	/*m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVtx = new Engine::VTXTEX[m_dwVtxCnt];*/

	//m_pResourceMgr->CopyToClient(Engine::RESOURCE_STATIC, m_pBufferName, m_pVertex);

	//m_pVertex[0].vPos = D3DXVECTOR3(-1.f, 1.f, 0.f);
	//m_pVertex[0].vTex = D3DXVECTOR2(0.f, 0.f);

	//m_pVertex[1].vPos = D3DXVECTOR3(1.f, 1.f, 0.f);
	//m_pVertex[1].vTex = D3DXVECTOR2(1.f, 0.f);

	//m_pVertex[2].vPos = D3DXVECTOR3(1.f, -1.f, 0.f);
	//m_pVertex[2].vTex = D3DXVECTOR2(1.f, 1.f);

	//m_pVertex[3].vPos = D3DXVECTOR3(-1.f, -1.f, 0.f);
	//m_pVertex[3].vTex = D3DXVECTOR2(0.f, 1.f);

	return S_OK;
}

void CSkullDust::Update(void)
{

}

int CSkullDust::DustUpdate()
{
	switch (m_eSkullDustID)
	{
	case SKULLDUST_ONE:
		UpdateSkullDustOne();
		break;
	case SKULLDUST_TWO:
		UpdateSkullDustTwo();
		break;
	case SKULLDUST_THREE:
		UpdateSkullDustThree();
		break;
	case SKULLDUST_FOUR:
		UpdateSkullDustFour();
		break;
	case SKULLDUST_FIVE:
		UpdateSkullDustFive();
		break;
	case SKULLDUST_SIX:
		UpdateSkullDustSix();
		break;
	case SKULLDUSTID_END:
		break;
	default:
		break;
	}

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	if (m_pInfoCom->m_vScale.x <= 0.f || m_pInfoCom->m_vScale.y <= 0.f)
		return 0;

	return 1;
}

void CSkullDust::LateUpdate(void)
{

}

void CSkullDust::Render(void)
{

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetMaterial(&m_tWhiteMtrl);						//ÀçÁú¡Ú
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	//m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	m_pTextureCom->SetTexture(m_iTexIndex);
	m_pBufferCom->Render_NoIndices();

	m_pGraphicDev->SetMaterial(&m_tMtrl);

	m_pMaskTextureCom->SetTexture(m_iTexIndex);
	m_pBufferCom->Render_NoIndices();
}

void CSkullDust::Release(void)
{
	if (m_pVertex)
		Engine::Safe_Delete_Array(m_pVertex);
	if (m_pConvertVtx)
		Engine::Safe_Delete_Array(m_pConvertVtx);
}

void CSkullDust::SetTransform(void)
{


}

void CSkullDust::UpdateSkullDustOne()
{
	if (GET_TIME < 0.7f)
	{

		if (!m_bSwitch)
		{
			m_pInfoCom->m_vPos += 0.25f * m_pInfoCom->m_vDir * GET_TIME;
			
			m_fDieTime -= 3.f * GET_TIME;
		}
		else
		{
			m_pInfoCom->m_vPos += 0.1f * m_pInfoCom->m_vDir * GET_TIME;
			m_pInfoCom->m_vScale.x -= 0.25f * GET_TIME;
			m_pInfoCom->m_vScale.y -= 0.25f * GET_TIME;
			m_fDieTime -= 2.5f * GET_TIME;
		}

		if (m_fDieTime < 0.f && !m_bSwitch)
		{
			m_fDieTime = 1.27f;
			m_bSwitch = true;
		}

	}
}

void CSkullDust::UpdateSkullDustTwo()
{
	if (GET_TIME < 0.7f)
	{

		if (!m_bSwitch)
		{
			m_pInfoCom->m_vPos += 0.75f * m_pInfoCom->m_vDir * GET_TIME;

			m_fDieTime -= 3.f * GET_TIME;
		}
		else
		{
			m_pInfoCom->m_vPos += 0.35f * m_pInfoCom->m_vDir * GET_TIME;
			m_pInfoCom->m_vScale.x -= 0.25f * GET_TIME;
			m_pInfoCom->m_vScale.y -= 0.25f * GET_TIME;
			m_fDieTime -= 2.5f * GET_TIME;
		}

		if (m_fDieTime < 0.f && !m_bSwitch)
		{
			m_fDieTime = 1.37f;
			m_bSwitch = true;
		}

	}
}

void CSkullDust::UpdateSkullDustThree()
{
	if (GET_TIME < 0.7f)
	{

		if (!m_bSwitch)
		{
			m_pInfoCom->m_vPos += 0.65f * m_pInfoCom->m_vDir * GET_TIME;

			m_fDieTime -= 3.f * GET_TIME;
		}
		else
		{
			m_pInfoCom->m_vPos += 0.2f * m_pInfoCom->m_vDir * GET_TIME;
			m_pInfoCom->m_vScale.x -= 0.25f * GET_TIME;
			m_pInfoCom->m_vScale.y -= 0.25f * GET_TIME;
			m_fDieTime -= 2.5f * GET_TIME;
		}

		if (m_fDieTime < 0.f && !m_bSwitch)
		{
			m_fDieTime = 1.34f;
			m_bSwitch = true;
		}

	}
}

void CSkullDust::UpdateSkullDustFour()
{
	if (GET_TIME < 0.7f)
	{

		if (!m_bSwitch)
		{
			m_pInfoCom->m_vPos += 0.25f * m_pInfoCom->m_vDir * GET_TIME;

			m_fDieTime -= 3.f * GET_TIME;
		}
		else
		{
			m_pInfoCom->m_vPos += 0.1f * m_pInfoCom->m_vDir * GET_TIME;
			m_pInfoCom->m_vScale.x -= 0.35f * GET_TIME;
			m_pInfoCom->m_vScale.y -= 0.35f * GET_TIME;
			m_fDieTime -= 2.5f * GET_TIME;
		}

		if (m_fDieTime < 0.f && !m_bSwitch)
		{
			m_fDieTime = 1.27f;
			m_bSwitch = true;
		}

	}
}

void CSkullDust::UpdateSkullDustFive()
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

void CSkullDust::UpdateSkullDustSix()
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

HRESULT CSkullDust::AddComponent(void)
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


	m_pTexName = L"Particle_DustMask";
	pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, m_pTexName);
	m_pMaskTextureCom = dynamic_cast<Engine::CTexture*>(pComponent);
	m_mapComponent.emplace(L"TextureMask", m_pMaskTextureCom);

	// Transform
	D3DXVECTOR3	vecDir(-1.f, 1.f, 0.f);
	D3DXVec3Normalize(&vecDir, &vecDir);
	pComponent = Engine::CTransform::Create(vecDir);
	m_pInfoCom = dynamic_cast<Engine::CTransform*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.emplace(L"Transform", m_pInfoCom);

	return S_OK;
}

CSkullDust * CSkullDust::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkullDust* pInstance = new CSkullDust(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
