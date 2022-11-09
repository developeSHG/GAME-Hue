#include "stdafx.h"
#include "ColorPart.h"
#include "CameraObserver.h"
#include "ColorCircle.h"
#include "Export_Function.h"
USING(Engine)

CColorPart::CColorPart(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
{

}

CColorPart::~CColorPart()
{
	Release();
}

HRESULT CColorPart::Initialize(COLORPARTID eID)
{
	m_pBufferName = L"Buffer_RcyTex";
	m_pTexName = L"Texture_ColorCircle";


	/************************ (SongSong) ************************/
	/*  카메라 옵저버 생성  */
	m_pCameraObserver = CCameraObserver::GetInstance();

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVtx = new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->CopyToClient(Engine::RESOURCE_STATIC, m_pBufferName, m_pVertex);

	m_ePartID = eID;

	switch (m_ePartID)
	{
	case PART_RUU:
		m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(-23.f);	
		m_tMtrl.Ambient = YELLOW;
		m_tMtrl.Diffuse = YELLOW;
		m_tMtrl.Specular = YELLOW;
		m_tMtrl.Emissive = BLACK;
		m_tColor = YELLOW;
		m_tMtrl.Power = 5.f;
		break;
	case PART_RUD:
		m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(-67.f);
		m_tMtrl.Ambient = GREEN;
		m_tMtrl.Diffuse = GREEN;
		m_tMtrl.Specular = GREEN;
		m_tMtrl.Emissive = BLACK;
		m_tColor = GREEN;
		m_tMtrl.Power = 5.f;
		break;
	case PART_RDU:
		m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(-113.f);
		m_tMtrl.Ambient = PURPLE;
		m_tMtrl.Diffuse = PURPLE;
		m_tMtrl.Specular = PURPLE;
		m_tMtrl.Emissive = BLACK;
		m_tColor = PURPLE;
		m_tMtrl.Power = 5.f;
		break;
	case PART_RDD:
		m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(-157.f);
		m_tMtrl.Ambient = PINK;
		m_tMtrl.Diffuse = PINK;
		m_tMtrl.Specular = PINK;
		m_tMtrl.Emissive = BLACK;
		m_tColor = PINK;
		m_tMtrl.Power = 5.f;
		break;
	case PART_LUU:
		m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(23.f);
		m_tMtrl.Ambient = ORANGE;
		m_tMtrl.Diffuse = ORANGE;
		m_tMtrl.Specular = ORANGE;
		m_tMtrl.Emissive = BLACK;
		m_tColor = ORANGE;
		m_tMtrl.Power = 5.f;
		break;
	case PART_LUD:
		m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(67.f);
		m_tMtrl.Ambient = RED;
		m_tMtrl.Diffuse = RED;
		m_tMtrl.Specular = RED;
		m_tMtrl.Emissive = BLACK;
		m_tColor = RED;
		m_tMtrl.Power = 5.f;
		break;
	case PART_LDU:
		m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(113.f);
		m_tMtrl.Ambient = BLUE;
		m_tMtrl.Diffuse = BLUE;
		m_tMtrl.Specular = BLUE;
		m_tMtrl.Emissive = BLACK;
		m_tColor = BLUE;
		m_tMtrl.Power = 5.f;
		break;
	case PART_LDD:
		m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(157.f);
		m_tMtrl.Ambient = SKYBLUE;
		m_tMtrl.Diffuse = SKYBLUE;
		m_tMtrl.Specular = SKYBLUE;
		m_tMtrl.Emissive = BLACK;
		m_tColor = SKYBLUE;
		m_tMtrl.Power = 5.f;
		break;
	case PART_END:
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CColorPart::AddComponent(void)
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
	pComponent = m_pInfoCom = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.emplace(L"Transform", m_pInfoCom);

	return S_OK;
}

void CColorPart::SetTransform(void)
{
	D3DXMATRIX		matProj;
	D3DXMatrixOrthoLH(&matProj, (float)WINCX * 2, (float)WINCY * 2, 1.f, 10.f);

	for (size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVtx[i] = m_pVertex[i];

		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, &m_pInfoCom->m_matWorld);

		if (m_pConvertVtx[i].vPos.z < 1.f)
			m_pConvertVtx[i].vPos.z = 1.f;

		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, &matProj);
	}
} 

void CColorPart::Update(void)
{

}

void CColorPart::LateUpdate(void)
{
}

void CColorPart::BiggerUpdate(void)
{
	if (!m_bIsSelect)
	{
		for (int i = 0; i < (int)m_dwVtxCnt; ++i)
		{
			if (m_fScale > 120.f)
			{
				m_fScale = 123.f;
				m_pVertex[0].vPos.y = 2.54f;
				m_pVertex[1].vPos.y = 2.54f;
				m_pVertex[2].vPos.y = 1.488f;
				m_pVertex[3].vPos.y = 1.488f;
				m_pParent->SetIsSizeMax(true);
				break;
			}

			m_pVertex[i].vPos.y += 6.6f * GET_CONSTANTTIME;
			m_fScale += 90.f * GET_CONSTANTTIME;
		}
	}
	else
	{
		for (int i = 0; i < (int)m_dwVtxCnt; ++i)
		{
			if (m_fScale > 150.f)
			{
				m_fScale = 170.f;
				m_pVertex[0].vPos.y = 2.7f;
				m_pVertex[1].vPos.y = 2.7f;
				m_pVertex[2].vPos.y = 1.7f;
				m_pVertex[3].vPos.y = 1.7f;
				break;
			}

			m_pVertex[i].vPos.y += 3.6f * GET_CONSTANTTIME;
			m_fScale += 30.f * GET_CONSTANTTIME;
		}
	}

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&D3DXVECTOR3(m_fScale, m_fScale, m_fScale),
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	SetTransform();
}

void CColorPart::SmallerUpdate(void)
{
	for (int i = 0; i < (int)m_dwVtxCnt; ++i)
	{
		if (m_fScale < 12.f)
		{
			m_fScale = 10.f;
			m_pVertex[0].vPos = D3DXVECTOR3(-1.f, 0.516f, 0.f);
			m_pVertex[1].vPos = D3DXVECTOR3(1.f, 0.516f, 0.f);
			m_pVertex[2].vPos = D3DXVECTOR3(1.f, -0.516f, 0.f);
			m_pVertex[3].vPos = D3DXVECTOR3(-1.f, -0.516f, 0.f);
			m_pParent->SetIsReady(true);
			break;
		}
		m_pVertex[i].vPos.y -= 4.f * GET_CONSTANTTIME;
		m_fScale -= 110.f * GET_CONSTANTTIME;
		m_pParent->SetIsReady(false);
		m_pParent->SetIsSizeMax(false);
	}

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&D3DXVECTOR3(m_fScale, m_fScale, m_fScale),
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	SetTransform();

}

void CColorPart::InitFirstPos(void)
{
	m_fScale = 123.f;
	m_pVertex[0].vPos.y = 2.50f;
	m_pVertex[1].vPos.y = 2.50f;
	m_pVertex[2].vPos.y = 1.448f;
	m_pVertex[3].vPos.y = 1.448f;

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&D3DXVECTOR3(m_fScale, m_fScale, m_fScale),
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	SetTransform();
}

void CColorPart::Render(void)
{
	if (m_fScale >= 45.f)
	{
		m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0X00000088);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★
		m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
		m_pTextureCom->SetTexture(0);
		m_pBufferCom->Render();

	}

}

void CColorPart::Release(void)
{
	if (m_pVertex)
		Engine::Safe_Delete_Array(m_pVertex);
	if (m_pConvertVtx)
		Engine::Safe_Delete_Array(m_pConvertVtx);
}

CColorPart* CColorPart::Create(LPDIRECT3DDEVICE9 pGraphicDev, COLORPARTID eID)
{
	CColorPart*	pInstance = new CColorPart(pGraphicDev);

	if (FAILED(pInstance->Initialize(eID)))
		Engine::Safe_Delete(pInstance);

	return pInstance;
}
