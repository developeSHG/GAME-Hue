#include "stdafx.h"
#include "Snow.h"

#include "StaticCamera.h"

USING(Engine)

CSnow::CSnow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pCameraObserver(CCameraObserver::GetInstance())
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CSnow::~CSnow(void)
{
	Release();
}

HRESULT CSnow::Initialize(void)
{
	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pBufferName = L"Buffer_CubeFront";
	m_pTexName = L"Texture_Snow";

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	ParticleInfoCreate();

	return S_OK;
}

void CSnow::Update(void)
{
	/************************ (SongSong) ************************/
	/*  빌보드  */
	D3DXMATRIX   matBill;
	D3DXMatrixIdentity(&matBill);

	matBill = *m_pCameraObserver->GetView();
	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	for (auto& tInfo : m_listParticleInfo)
	{
		tInfo.vPos.x += tInfo.vDir.x * GET_TIME;
		tInfo.vPos.y -= tInfo.vDir.y * GET_TIME;

		/************************ (SongSong) ************************/
		/*  파티클 초기화  */
		if (0.f > tInfo.vPos.x || 0.f > tInfo.vPos.y)
			ParticleInfoInit(tInfo);

		/************************ (SongSong) ************************/
		/*  카메라 시점에 따른 클리핑  */
		if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
			m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(0.f);
		else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
			m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(90.f);

		Engine::CPipeline::MakeWorldMatrix(&tInfo.matWorld,
			&tInfo.vScale,
			m_pInfoCom->m_fAngle,
			&tInfo.vPos);
	}
}

void CSnow::LateUpdate(void)
{

}

void CSnow::PreRender(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetMaterial(&m_tMtrl);                  // 재질★

														   //m_pGraphicDev->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
														   //m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);                                                
														   //m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
}

void CSnow::Render(void)
{
	PreRender();

	for (auto& tInfo : m_listParticleInfo)
	{
		/************************ (SongSong) ************************/
		/*  컬링  */
		if (false == m_pCameraObserver->GetStaticCamera()->Get_DoorGofor())
		{
			D3DXVECTOR3 vEye = m_pCameraObserver->GetStaticCamera()->Get_Eye();
			//if (-10.f <= vEye.z)
			{
				if (vEye.x - 20.f > tInfo.vPos.x || vEye.x + 20.f < tInfo.vPos.x
					|| vEye.y - 15.f > tInfo.vPos.y || vEye.y + 15.f < tInfo.vPos.y)
					continue;
			}
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &tInfo.matWorld);

		m_pTextureCom->SetTexture(0);
		m_pBufferCom->Render_NoIndices();
	}

	PostRender();
}

void CSnow::RenderInsideWorld(void)
{
	PreRender();

	D3DMATERIAL9 tMtrl;
	tMtrl.Ambient = MILDGREY;
	tMtrl.Diffuse = MILDGREY;
	tMtrl.Specular = MILDGREY;
	tMtrl.Emissive = BLACK;
	tMtrl.Power = 5.f;

	m_pGraphicDev->SetMaterial(&tMtrl);                  // 재질★

	for (auto& tInfo : m_listParticleInfo)
	{
		/************************ (SongSong) ************************/
		/*  컬링  */
		if (false == m_pCameraObserver->GetStaticCamera()->Get_DoorGofor())
		{
			D3DXVECTOR3 vEye = m_pCameraObserver->GetStaticCamera()->Get_Eye();
			//if (-10.f <= vEye.z)
			{
				if (vEye.x - 20.f > tInfo.vPos.x || vEye.x + 20.f < tInfo.vPos.x
					|| vEye.y - 15.f > tInfo.vPos.y || vEye.y + 15.f < tInfo.vPos.y)
					continue;
			}
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &tInfo.matWorld);

		m_pTextureCom->SetTexture(0);
		m_pBufferCom->Render_NoIndices();
	}

	PostRender();
}

void CSnow::PostRender(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CSnow::Release(void)
{

}

void CSnow::ParticleInfoCreate()
{
	SCENEID eScene = *m_pCameraObserver->GetSceneID();

	switch (eScene)
	{
	case SCENEID::SC_STAGE:
	{
		float fWidth = 65.f;
		int iHeight = 45;
		size_t iNumX = 26;
		size_t iNumY = 26;

		for (size_t i = 0; i < iNumY; ++i)
		{
			for (size_t j = 0; j < iNumX; ++j)
			{
				PARTICLE_INFO tInfo;

				float fX = j * fWidth / iNumX;
				float fY = float(rand() % iHeight);
				//float fY = i * fHeight / iNumY;
				float fScale = float(rand() % 2000 + 3500) / 60000.f;

				tInfo.vPos = { fX, fY, rand() % 250 / 100.f - 2.f };
				tInfo.vScale = { fScale, fScale, 0.f };
				tInfo.vDir = { float(rand() % 20 - 10.f), float(rand() % 95 + 5), 0.f };
				D3DXVec3Normalize(&tInfo.vDir, &tInfo.vDir);
				tInfo.fVelocity = (rand() % 150 + 5.f) / 10.f;

				m_listParticleInfo.push_back(tInfo);
			}
		}
	}
	break;
	case SCENEID::SC_STAGE2:
	{
		float fWidth = 91.f;
		int iHeight = 55;
		size_t iNumX = 32;
		size_t iNumY = 32;

		for (size_t i = 0; i < iNumY; ++i)
		{
			for (size_t j = 0; j < iNumX; ++j)
			{
				PARTICLE_INFO tInfo;

				float fX = j * fWidth / iNumX;
				float fY = float(rand() % iHeight);
				//float fY = i * fHeight / iNumY;
				float fScale = float(rand() % 2000 + 3500) / 60000.f;

				tInfo.vPos = { fX, fY, rand() % 250 / 100.f - 2.f };
				tInfo.vScale = { fScale, fScale, 0.f };
				tInfo.vDir = { float(rand() % 20 - 10.f), float(rand() % 95 + 5), 0.f };
				D3DXVec3Normalize(&tInfo.vDir, &tInfo.vDir);
				tInfo.fVelocity = (rand() % 150 + 5.f) / 10.f;

				m_listParticleInfo.push_back(tInfo);
			}
		}
	}
	break;
	}
}

void CSnow::ParticleInfoInit(PARTICLE_INFO& tInfo)
{
	SCENEID eScene = *m_pCameraObserver->GetSceneID();

	switch (eScene)
	{
	case SCENEID::SC_STAGE:
	{
		int iWidth = 65;
		int iHeight = 45;
		float fScale = float(rand() % 2000 + 3500) / 60000.f;

		tInfo.vPos = { float(rand() % iWidth), float(rand() % iHeight + 2), rand() % 250 / 100.f - 2.f };
		tInfo.vScale = { fScale, fScale, 0.f };
		tInfo.vDir = { float(rand() % 20 - 10.f), float(rand() % 95 + 5), 0.f };
		D3DXVec3Normalize(&tInfo.vDir, &tInfo.vDir);
		tInfo.fVelocity = (rand() % 150 + 5.f) / 10.f;
	}
	break;
	case SCENEID::SC_STAGE2:
	{
		int iWidth = 91;
		int iHeight = 55;
		float fScale = float(rand() % 2000 + 3500) / 60000.f;

		tInfo.vPos = { float(rand() % iWidth), float(rand() % iHeight + 2), rand() % 250 / 100.f - 2.f };
		tInfo.vScale = { fScale, fScale, 0.f };
		tInfo.vDir = { float(rand() % 20 - 10.f), float(rand() % 95 + 5), 0.f };
		D3DXVec3Normalize(&tInfo.vDir, &tInfo.vDir);
		tInfo.fVelocity = (rand() % 150 + 5.f) / 10.f;
		break;
	}
	}
}

HRESULT CSnow::AddComponent(void)
{
	Engine::CComponent*      pComponent = nullptr;

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

CSnow* CSnow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSnow* pInstance = new CSnow(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
