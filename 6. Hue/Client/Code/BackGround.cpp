#include "stdafx.h"

#include "BackGround.h"
#include "Export_Function.h"
#include "Include.h"
#include "BackObject.h"
#include "BackLight.h"
#include "Fire.h"
CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	ZeroMemory(&m_tInfo, sizeof(Engine::INFO));
}

CBackGround::~CBackGround(void)
{
	Release();
}

HRESULT CBackGround::Initialize(void)
{
	m_pBufferName = L"Buffer_RcTex";
	m_pTexName = L"Texture_BackGround";

	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVtx = new Engine::VTXTEX[m_dwVtxCnt];

	m_tMtrl.Ambient = D3DXCOLOR(D3DCOLOR_XRGB(255, 0, 0));
	m_tMtrl.Diffuse = D3DXCOLOR(D3DCOLOR_XRGB(255, 0, 0));
	m_tMtrl.Specular = D3DXCOLOR(D3DCOLOR_XRGB(255, 0, 0));
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	Engine::Get_ResourceMgr()->CopyToClient(Engine::RESOURCE_STATIC, m_pBufferName, m_pVertex);

	m_pBackObject = CBackObject::Create(m_pGraphicDev);
	m_pBackLight = CBackLight::Create(m_pGraphicDev);

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);
	//CFire* pFire = CFire::Create(m_pGraphicDev);
	//pFire->SetSpawnPos(FIRESPAWNPOS::LEFT_DOWN);
	//m_ListFire.emplace_back(pFire);

	//pFire = CFire::Create(m_pGraphicDev);
	//pFire->SetSpawnPos(FIRESPAWNPOS::LEFT_UP);
	//m_ListFire.emplace_back(pFire);


	return S_OK;
}

HRESULT CBackGround::AddComponent(void)
{
	Engine::CComponent*		pComponent = nullptr;
	// Buffer
	pComponent = Engine::CResourceMgr::GetInstance()->Clone_Buffer(Engine::RESOURCE_STATIC, m_pBufferName);
	m_pBufferCom = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	m_mapComponent.emplace(L"Buffer", m_pBufferCom);
	// Texture
	pComponent = Engine::CResourceMgr::GetInstance()->Clone_Tex(Engine::RESOURCE_STATIC, m_pTexName);
	m_pTextureCom = dynamic_cast<Engine::CTexture*>(pComponent);
	m_mapComponent.emplace(L"Texture", m_pTextureCom);

	return S_OK;
}

void CBackGround::MakeFireParticle(void)
{
	if (GET_TIME < 0.7f)
	{
		m_fMakeTime += 10.f * GET_TIME;

		if (m_fMakeTime > 1.f)
		{
			CFire* pFire = CFire::Create(m_pGraphicDev);
			pFire->SetSpawnPos(FIRESPAWNPOS::LEFT_UP);
			m_ListFire.emplace_back(pFire);
			
			pFire = CFire::Create(m_pGraphicDev);
			pFire->SetSpawnPos(FIRESPAWNPOS::LEFT_DOWN);
			m_ListFire.emplace_back(pFire);

			pFire = CFire::Create(m_pGraphicDev);
			pFire->SetSpawnPos(FIRESPAWNPOS::RIGHT_DOWN);
			m_ListFire.emplace_back(pFire);

			pFire = CFire::Create(m_pGraphicDev);
			pFire->SetSpawnPos(FIRESPAWNPOS::RIGHT_UP);
			m_ListFire.emplace_back(pFire);

			m_fMakeTime = 0.f;
		}
	}
}

void CBackGround::ColorToYellow()
{
	if (m_fColorG - 255 > 0.f && m_fColorG != 0.f)
	{
		m_fColorG = 255.f;
		m_eCurColorID = CURCOLORID::CURCOLORID_YELLOW;
		m_fColorR = 255.f;
		return;
	}
	m_fColorG += GET_TIME* 100.f;


	m_tMtrl.Ambient = D3DXCOLOR(D3DCOLOR_XRGB(255, (int)m_fColorG, 0));
	m_tMtrl.Diffuse = D3DXCOLOR(D3DCOLOR_XRGB(255, (int)m_fColorG, 0));
	m_tMtrl.Specular = D3DXCOLOR(D3DCOLOR_XRGB(255, (int)m_fColorG, 0));
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;
}

void CBackGround::ColorToGreen()
{
	if ((int)m_fColorR <= 0 && m_fColorG != 0.f)
	{
		m_fColorG = 255.f;
		m_fColorR = 0.f;
		m_eCurColorID = CURCOLORID::CURCOLORID_GREEN;
		return;
	}
	m_fColorR -= GET_TIME* 100.f;

	m_tMtrl.Ambient = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, 255, 0));
	m_tMtrl.Diffuse = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, 255, 0));
	m_tMtrl.Specular = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, 255, 0));
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;
}

void CBackGround::ColorToBlue()
{
	if (m_fColorB - 255 > 0.f && m_fColorB != 0.f)
	{
		m_fColorG = 255.f;
		m_fColorB = 255.f;
		m_eCurColorID = CURCOLORID::CURCOLORID_BLUE;
		m_fColorR = 0.f;
		m_tMtrl.Ambient = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, (int)m_fColorG, (int)m_fColorB));
		m_tMtrl.Diffuse = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, (int)m_fColorG, (int)m_fColorB));
		m_tMtrl.Specular = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, (int)m_fColorG, (int)m_fColorB));
		return;
	}
	m_fColorB += GET_TIME* 100.f;

	m_tMtrl.Ambient = D3DXCOLOR(D3DCOLOR_XRGB(0, 255, (int)m_fColorB));
	m_tMtrl.Diffuse = D3DXCOLOR(D3DCOLOR_XRGB(0, 255, (int)m_fColorB));
	m_tMtrl.Specular = D3DXCOLOR(D3DCOLOR_XRGB(0, 255, (int)m_fColorB));
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;
}

void CBackGround::ColorToRed()
{
	if (m_fColorR - 255 > 0.f && m_fColorR != 0.f)
	{
		m_fColorG = 0.f;
		m_fColorB = 0.f;
		m_eCurColorID = CURCOLORID::CURCOLORID_RED;
		m_fColorR = 255.f;
		m_tMtrl.Ambient = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, (int)m_fColorG, (int)m_fColorB));
		m_tMtrl.Diffuse = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, (int)m_fColorG, (int)m_fColorB));
		m_tMtrl.Specular = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, (int)m_fColorG, (int)m_fColorB));
		return;
	}
	m_fColorR += GET_TIME* 100.f;
	m_fColorG -= GET_TIME* 100.f;
	m_fColorB -= GET_TIME* 100.f;

	m_tMtrl.Ambient = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, (int)m_fColorG, (int)m_fColorB));
	m_tMtrl.Diffuse = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, (int)m_fColorG, (int)m_fColorB));
	m_tMtrl.Specular = D3DXCOLOR(D3DCOLOR_XRGB((int)m_fColorR, (int)m_fColorG, (int)m_fColorB));
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;
}

void CBackGround::Update(void)
{
	MakeFireParticle();

	const D3DXMATRIX*		matView = CCameraObserver::GetInstance()->GetView();
	D3DXMATRIX				matProj;
	D3DXMatrixOrthoLH(&matProj, (float)WINCX * 2, (float)WINCY * 2, 1.f, 1000.f);
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, 1280.f, 720.f, 1.f);
	for (size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVtx[i] = m_pVertex[i];

		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, &mScale);

		if (m_pConvertVtx[i].vPos.z < 1.f)
			m_pConvertVtx[i].vPos.z = 1.f;

		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, &matProj);
	}


	switch (m_eCurColorID)
	{
	case CURCOLORID_RED:
		ColorToYellow();
		break;
	case CURCOLORID_YELLOW:
		ColorToGreen();
		break;
	case CURCOLORID_GREEN:
		ColorToBlue();
		break;
	case CURCOLORID_BLUE:
		ColorToRed();
		break;
	case CURCOLOR_END:
		break;
	default:
		break;
	}
	m_pBackLight->SetMaterialColor(m_fColorR, m_fColorG, m_fColorB);
	m_pBackLight->Update();

	auto iter_begin = m_ListFire.begin();
	auto iter_end = m_ListFire.end();


	for (; iter_begin != iter_end; )
	{
		int iEvent = (*iter_begin)->FireUpdate();

		if (0 == iEvent)
		{
			delete (*iter_begin);
			iter_begin = m_ListFire.erase(iter_begin);
		}
		else
			++iter_begin;
	}

	m_pBackObject->SetMaterialColor(m_fColorR, m_fColorG, m_fColorB);
	m_pBackObject->Update();
}

void CBackGround::LateUpdate(void)
{
	m_pBackLight->LateUpdate();
	for (auto& MyFire : m_ListFire)
		MyFire->LateUpdate();
	m_pBackObject->LateUpdate();
}

void CBackGround::Render(void)
{
	Engine::Get_ResourceMgr()->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);
	m_pGraphicDev->SetMaterial(&m_tMtrl);		//ÀçÁú¡Ú
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0X00000088);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTextureCom->SetTexture(WORD(0));
	m_pBufferCom->Render();
	m_pBackLight->Render();
	/*m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);*/

	m_pBackObject->Render();
	for (auto& MyFire : m_ListFire)
		MyFire->Render();
	m_pBackLight->Render();
}

void CBackGround::SetTransform(void)
{
}

void CBackGround::Release(void)
{
	for (auto& MyFire : m_ListFire)
		Engine::Safe_Delete(MyFire);
	Engine::Safe_Delete(m_pBackLight);
	Engine::Safe_Delete(m_pBackObject);

	if (m_pVertex)
		Engine::Safe_Delete_Array(m_pVertex);
	if (m_pConvertVtx)
		Engine::Safe_Delete_Array(m_pConvertVtx);
}

CBackGround * CBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBackGround*	pInstance = new CBackGround(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Engine::Safe_Delete(pInstance);

	return pInstance;
}
