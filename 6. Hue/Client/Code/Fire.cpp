#include "stdafx.h"
#include "Fire.h"
#include "Include.h"
#include "Export_Function.h"

USING(Engine)

CFire::CFire(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CFire::~CFire(void)
{
	Release();
}

void CFire::SetSpawnPos(FIRESPAWNPOS ePos)
{
	m_eSpawnPos = ePos;

	D3DXVECTOR3	vecDir;

	int iRand = rand() % 3;


	switch (m_eSpawnPos)
	{
	case LEFT_DOWN:
		m_pInfoCom->m_vScale = D3DXVECTOR3(0.5f, 0.4f, 1.f);
		m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(45.f);
		m_pInfoCom->m_vPos.x = -3.2f;
		m_pInfoCom->m_vPos.y = 2.3f;
		m_pInfoCom->m_vPos.z = 2.2f;
		vecDir = D3DXVECTOR3(-1.f, 1.f, 0.f);
		D3DXVec3Normalize(&vecDir, &vecDir);
		m_pInfoCom->m_vDir = vecDir;

		if (iRand == 0)
		{
			m_pInfoCom->m_vPos.x -= 0.2f;
			m_pInfoCom->m_vPos.y -= 0.2f;
			m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(50.f);
		}
		else if (iRand == 1)
		{
			m_pInfoCom->m_vPos.x -= 0.f;
			m_pInfoCom->m_vPos.y -= 0.f;
			m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(45.f);
		}
		else if (iRand == 2)
		{
			m_pInfoCom->m_vPos.x += 0.2f;
			m_pInfoCom->m_vPos.y += 0.2f;
			m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(40.f);
		}

		break;
	case LEFT_UP:
		m_pInfoCom->m_vScale = D3DXVECTOR3(0.6f, 0.5f, 1.f);
		m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(35.f);
		m_pInfoCom->m_vPos.x = -2.8f;
		m_pInfoCom->m_vPos.y = 2.2f;
		m_pInfoCom->m_vPos.z = 2.3f;
		vecDir = D3DXVECTOR3(-1.f, 2.f, 0.f);
		D3DXVec3Normalize(&vecDir, &vecDir);

		if (iRand == 0)
		{
			m_pInfoCom->m_vPos.x -= 0.2f;
			m_pInfoCom->m_vPos.y -= 0.2f;
			m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(40.f);
		}
		else if (iRand == 1)
		{
			m_pInfoCom->m_vPos.x -= 0.f;
			m_pInfoCom->m_vPos.y -= 0.f;
			m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(35.f);
		}
		else if (iRand == 2)
		{
			m_pInfoCom->m_vPos.x += 0.2f;
			m_pInfoCom->m_vPos.y += 0.2f;
			m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(30.f);
		}
		break;
	case RIGHT_DOWN:
		m_pInfoCom->m_vScale = D3DXVECTOR3(0.6f, 0.5f, 1.f);
		m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(-35.f);
		m_pInfoCom->m_vPos.x = 3.f;
		m_pInfoCom->m_vPos.y = 2.5f;
		m_pInfoCom->m_vPos.z = 2.3f;
		vecDir = D3DXVECTOR3(1.f, 1.f, 0.f);
		D3DXVec3Normalize(&vecDir, &vecDir);
		m_pInfoCom->m_vDir = vecDir;

		if (iRand == 0)
		{
			m_pInfoCom->m_vPos.x += 0.2f;
			m_pInfoCom->m_vPos.y -= 0.2f;
			m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(-40.f);
		}
		else if (iRand == 1)
		{
			m_pInfoCom->m_vPos.x += 0.f;
			m_pInfoCom->m_vPos.y -= 0.f;
			m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(-35.f);
		}
		else if (iRand == 2)
		{
			m_pInfoCom->m_vPos.x -= 0.2f;
			m_pInfoCom->m_vPos.y += 0.2f;
			m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(-30.f);
		}
		break;
	case RIGHT_UP:
		m_pInfoCom->m_vScale = D3DXVECTOR3(0.6f, 0.5f, 1.f);
		m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(-55.f);
		m_pInfoCom->m_vPos.x = 2.3f;
		m_pInfoCom->m_vPos.y = 2.7f;
		m_pInfoCom->m_vPos.z = 2.3f;
		vecDir = D3DXVECTOR3(1.8f, 1.f, 0.f);
		D3DXVec3Normalize(&vecDir, &vecDir);
		m_pInfoCom->m_vDir = vecDir;

		if (iRand == 0)
		{
			m_pInfoCom->m_vPos.x += 0.2f;
			m_pInfoCom->m_vPos.y -= 0.2f;
			m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(-60.f);
		}
		else if (iRand == 1)
		{
			m_pInfoCom->m_vPos.x += 0.f;
			m_pInfoCom->m_vPos.y -= 0.f;
			m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(-55.f);
		}
		else if (iRand == 2)
		{
			m_pInfoCom->m_vPos.x -= 0.2f;
			m_pInfoCom->m_vPos.y += 0.2f;
			m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(-50.f);
		}
		break;
	case FIRESPAWN_END:
		break;
	default:
		break;
	}

	iRand = rand() % 4;

	if (iRand == 0)
	{
		m_tMtrl.Ambient = WHITE;
		m_tMtrl.Diffuse = WHITE;
		m_tMtrl.Specular = WHITE;
		m_tMtrl.Emissive = BLACK;
		m_tMtrl.Power = 5.f;
	}
	else if (iRand == 1)
	{
		m_tMtrl.Ambient = WHITEGREY;
		m_tMtrl.Diffuse = WHITEGREY;
		m_tMtrl.Specular = WHITEGREY;
		m_tMtrl.Emissive = BLACK;
		m_tMtrl.Power = 5.f;
	}
	else if (iRand == 2)
	{
		m_tMtrl.Ambient = MILDDARKGREY;
		m_tMtrl.Diffuse = MILDDARKGREY;
		m_tMtrl.Specular = MILDDARKGREY;
		m_tMtrl.Emissive = BLACK;
		m_tMtrl.Power = 5.f;
	}
	else
	{
		m_tMtrl.Ambient = MILDGREY;
		m_tMtrl.Diffuse = MILDGREY;
		m_tMtrl.Specular = MILDGREY;
		m_tMtrl.Emissive = BLACK;
		m_tMtrl.Power = 5.f;
	}

}

HRESULT CFire::Initialize(void)
{
	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pBufferName = L"Buffer_RcTex";
	m_pTexName = L"Texture_Fire";
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVtx = new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->CopyToClient(Engine::RESOURCE_STATIC, m_pBufferName, m_pVertex);

	return S_OK;
}

void CFire::Update(void)
{
	/*switch (m_eSpawnPos)
	{
	case LEFT_DOWN:
		UpdateLeftDown();
		break;
	case LEFT_UP:
		UpdateLeftUp();
		break;
	case RIGHT_DOWN:
		UpdateRightDown();
		break;
	case RIGHT_UP:
		UpdateRightUp();
		break;
	case FIRESPAWN_END:
		break;
	default:
		break;
	}
	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);


	SetTransform();*/
}

int CFire::FireUpdate()
{
	switch (m_eSpawnPos)
	{
	case LEFT_DOWN:
		UpdateLeftDown();
		break;
	case LEFT_UP:
		UpdateLeftUp();
		break;
	case RIGHT_DOWN:
		UpdateRightDown();
		break;
	case RIGHT_UP:
		UpdateRightUp();
		break;
	case FIRESPAWN_END:
		break;
	default:
		break;
	}
	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	SetTransform();

	if (m_pInfoCom->m_vScale.x < 0.f || m_pInfoCom->m_vScale.y < 0.f)
		return 0;
	
	return 1;
}

void CFire::LateUpdate(void)
{

}

void CFire::Render(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetMaterial(&m_tMtrl);						//ÀçÁú¡Ú

	m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	m_pTextureCom->SetTexture(0);
	m_pBufferCom->Render();
	//m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pVertex);
}

void CFire::Release(void)
{
	if (m_pVertex)
		Engine::Safe_Delete_Array(m_pVertex);
	if (m_pConvertVtx)
		Engine::Safe_Delete_Array(m_pConvertVtx);
}

void CFire::UpdateLeftDown()
{
	if (GET_TIME < 0.7f)
	{
		if (!m_bSwitch)
		{
			m_fvecAngle += 185.f * GET_TIME;
			m_pInfoCom->m_fAngle[ANGLE_Z] += D3DXToRadian(60.f) * GET_TIME;
		}
		else
		{
			m_fvecAngle -= 135.f * GET_TIME;
			m_pInfoCom->m_fAngle[ANGLE_Z] -= D3DXToRadian(55.f) * GET_TIME;
		}

		m_pInfoCom->m_vScale.x -= 0.4f * GET_TIME;
		m_pInfoCom->m_vScale.y -= 0.37f * GET_TIME;

		if (!m_bSwitch && m_fvecAngle > 60.f)
			m_bSwitch = true;

		D3DXVECTOR3 vecMove;
		D3DXMATRIX mRot;
		D3DXMatrixRotationZ(&mRot, D3DXToRadian(m_fvecAngle));
		D3DXVec3TransformNormal(&vecMove, &m_pInfoCom->m_vDir, &mRot);
		m_pInfoCom->m_vPos += 5.5f * vecMove * GET_TIME;
	}
}

void CFire::UpdateLeftUp()
{
	if (GET_TIME < 0.7f)
	{
		if (!m_bSwitch)
		{
			m_fvecAngle += 105.f * GET_TIME;
			m_pInfoCom->m_fAngle[ANGLE_Z] += D3DXToRadian(60.f) * GET_TIME;
		}
		else
		{
			m_fvecAngle -= 125.f * GET_TIME;
			m_pInfoCom->m_fAngle[ANGLE_Z] -= D3DXToRadian(65.f) * GET_TIME;
		}

		m_pInfoCom->m_vScale.x -= 0.55f * GET_TIME;
		m_pInfoCom->m_vScale.y -= 0.5f * GET_TIME;

		if (!m_bSwitch && m_fvecAngle > 35.f)
			m_bSwitch = true;

		D3DXVECTOR3 vecMove;
		D3DXMATRIX mRot;
		D3DXMatrixRotationZ(&mRot, D3DXToRadian(m_fvecAngle));
		D3DXVec3TransformNormal(&vecMove, &m_pInfoCom->m_vDir, &mRot);
		m_pInfoCom->m_vPos += 5.5f * vecMove * GET_TIME;
	}
}

void CFire::UpdateRightDown()
{
	if (GET_TIME < 0.7f)
	{
		if (!m_bSwitch)
		{
			m_fvecAngle -= 165.f * GET_TIME;
			m_pInfoCom->m_fAngle[ANGLE_Z] -= D3DXToRadian(90.f) * GET_TIME;
		}
		else
		{
			m_fvecAngle += 175.f * GET_TIME;
			m_pInfoCom->m_fAngle[ANGLE_Z] += D3DXToRadian(55.f) * GET_TIME;
		}

		m_pInfoCom->m_vScale.x -= 0.5f * GET_TIME;
		m_pInfoCom->m_vScale.y -= 0.47f * GET_TIME;

		if (!m_bSwitch && m_fvecAngle < -75.f)
			m_bSwitch = true;

		D3DXVECTOR3 vecMove;
		D3DXMATRIX mRot;
		D3DXMatrixRotationZ(&mRot, D3DXToRadian(m_fvecAngle));
		D3DXVec3TransformNormal(&vecMove, &m_pInfoCom->m_vDir, &mRot);
		m_pInfoCom->m_vPos += 5.5f * vecMove * GET_TIME;
	}
}

void CFire::UpdateRightUp()
{
	if (GET_TIME < 0.7f)
	{
		if (!m_bSwitch)
		{
			m_fvecAngle -= 165.f * GET_TIME;
			m_pInfoCom->m_fAngle[ANGLE_Z] -= D3DXToRadian(90.f) * GET_TIME;
		}
		else
		{
			m_fvecAngle += 175.f * GET_TIME;
			m_pInfoCom->m_fAngle[ANGLE_Z] += D3DXToRadian(105.f) * GET_TIME;
		}

		m_pInfoCom->m_vScale.x -= 0.5f * GET_TIME;
		m_pInfoCom->m_vScale.y -= 0.47f * GET_TIME;

		if (!m_bSwitch && m_fvecAngle < -65.f)
			m_bSwitch = true;

		D3DXVECTOR3 vecMove;
		D3DXMATRIX mRot;
		D3DXMatrixRotationZ(&mRot, D3DXToRadian(m_fvecAngle));
		D3DXVec3TransformNormal(&vecMove, &m_pInfoCom->m_vDir, &mRot);
		m_pInfoCom->m_vPos += 5.5f * vecMove * GET_TIME;
	}
}

void CFire::SetTransform(void)
{
	const D3DXMATRIX*		matView = CCameraObserver::GetInstance()->GetView();
	const D3DXMATRIX*		matProj = CCameraObserver::GetInstance()->GetProj();


	for (size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVtx[i] = m_pVertex[i];

		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, &m_pInfoCom->m_matWorld);
		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, matView);
		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, matProj);
	}

}

HRESULT CFire::AddComponent(void)
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

CFire * CFire::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFire* pInstance = new CFire(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
