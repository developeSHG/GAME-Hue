#include "stdafx.h"
#include "Rain.h"

#include "StaticCamera.h"

USING(Engine)

CRain::CRain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pCameraObserver(CCameraObserver::GetInstance())
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CRain::~CRain(void)
{
	Release();
}

HRESULT CRain::Initialize(void)
{
	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pBufferName = L"Buffer_Cube_Player";
	m_pTexName = L"Texture_Rain";

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	ParticleInfoCreate();

	return S_OK;
}

void CRain::Update(void)
{
	for (auto& tInfo : m_listParticleInfo)
	{
		tInfo.vPos.x += tInfo.vDir.x * ((rand() % 110 + 110) / 100.f) * GET_TIME;
		tInfo.vPos.y -= tInfo.vDir.y * tInfo.fVelocity * GET_TIME;

		/************************ (SongSong) ************************/
		/*  파티클 초기화  */
		if (-4.3f > tInfo.vPos.y)
			ParticleInfoInit(tInfo);

		/************************ (SongSong) ************************/
		/*  카메라 시점에 따른 클리핑  */
		if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
			m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(0.f);
		else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
			m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(90.f);

		tInfo.fAngle[PARTI_ANGLE_X] = 0.f;
		tInfo.fAngle[PARTI_ANGLE_Y] = 0.f;

		Engine::CPipeline::MakeWorldMatrix(&tInfo.matWorld,
			&tInfo.vScale,
			tInfo.fAngle,
			&tInfo.vPos);
	}
}

void CRain::LateUpdate(void)
{

}

void CRain::PreRender(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	D3DMATERIAL9 tMtrl;
	tMtrl.Ambient = WHITE;
	tMtrl.Diffuse = WHITE;
	tMtrl.Specular = WHITE;
	tMtrl.Emissive = BLACK;
	tMtrl.Power = 5.f;
	m_pGraphicDev->SetMaterial(&tMtrl);						// 재질★

															//m_pGraphicDev->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
															//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);																
															//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
}

void CRain::Render(void)
{
	if (SCENEID::SC_TUTORIAL == *m_pCameraObserver->GetSceneID())
	{
		if (D3DCOLOR_XRGB(128, 128, 128) != CCubeMgr::GetInstance()->GetBackColor())
			return;
	}

	D3DXMATRIX matView = *CCameraObserver::GetInstance()->GetView();
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProj = *CCameraObserver::GetInstance()->GetProj();
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	PreRender();

	for (auto& tInfo : m_listParticleInfo)
	{
		/************************ (SongSong) ************************/
		/*  컬링  */
		if (true == m_pCameraObserver->GetStaticCamera()->Get_DoorGofor())
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

void CRain::RenderInsideWorld(void)
{
	D3DXMATRIX matView = *CCameraObserver::GetInstance()->GetView();
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProj = *CCameraObserver::GetInstance()->GetProj();
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

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

void CRain::PostRender(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRain::Release(void)
{

}

void CRain::ParticleInfoCreate()
{
	SCENEID eScene = *m_pCameraObserver->GetSceneID();

	size_t iNumX;
	size_t iNumY;

	switch (eScene)
	{
	case SCENEID::SC_TUTORIAL_THREAD:
	case SCENEID::SC_TUTORIAL:
		iNumX = 65;
		iNumY = 65;

		for (size_t i = 0; i < iNumY; ++i)
		{
			for (size_t j = 0; j < iNumX; ++j)
			{
				PARTICLE_INFO tInfo;

				int iWidth = 15000;
				int iHeight = 9000;
				//	float fScale = float(rand() % 2000 + 3500) / 4000000.f;

				tInfo.vPos = { float(rand() % iWidth / 250.f) - 13.5f , float(rand() % iHeight) / 1000.f - 4.f, -1.f };
				float fScale = float(rand() % 2000 + 3500) / 400000.f;

				//tInfo.vPos = { fX - 1.f, fY, 0.001f };
				tInfo.vScale = { 0.005f, fScale + 0.02f, 0.f };
				tInfo.vDir = { float(rand() % 4000 - 2000.f) / 1200.f, float(rand() % 95 + 5), 0.f };
				D3DXVec3Normalize(&tInfo.vDir, &tInfo.vDir);
				//tInfo.fVelocity = (rand() % 500 + 40.f) / 10.f;
				tInfo.fVelocity = (rand() % 450 + 250) / 100.f;
				tInfo.fAngle[PARTI_ANGLE_Z] = D3DXToRadian(float(rand() % 15 - 8.f));

				m_listParticleInfo.push_back(tInfo);
			}
		}
		break;

	case SCENEID::SC_STAGE2:
		iNumX = 90;
		iNumY = 90;

		for (size_t i = 0; i < iNumY; ++i)
		{
			for (size_t j = 0; j < iNumX; ++j)
			{
				PARTICLE_INFO tInfo;

				int iWidth = 23000;
				int iHeight = 300000;
				//	float fScale = float(rand() % 2000 + 3500) / 4000000.f;

				tInfo.vPos = { float(rand() % iWidth / 250.f) - 13.5f , float(rand() % iHeight) / 1000.f + 25.f, -1.f };
				float fScale = float(rand() % 2000 + 3500) / 400000.f;

				//tInfo.vPos = { fX - 1.f, fY, 0.001f };
				tInfo.vScale = { 0.005f, fScale + 0.02f, 0.f };
				tInfo.vDir = { float(rand() % 4000 - 2000.f) / 1200.f, float(rand() % 95 + 5), 0.f };
				D3DXVec3Normalize(&tInfo.vDir, &tInfo.vDir);
				//tInfo.fVelocity = (rand() % 500 + 40.f) / 10.f;
				tInfo.fVelocity = (rand() % 450 + 250) / 100.f;
				tInfo.fAngle[PARTI_ANGLE_Z] = D3DXToRadian(float(rand() % 15 - 8.f));

				m_listParticleInfo.push_back(tInfo);
			}
		}
		break;
	}

}

void CRain::ParticleInfoInit(PARTICLE_INFO& tInfo)
{
	SCENEID eScene = *m_pCameraObserver->GetSceneID();

	int iWidth;
	int iHeight;
	float fScale;

	switch (eScene)
	{
	case SCENEID::SC_TUTORIAL_THREAD:
	case SCENEID::SC_TUTORIAL:
		iWidth = 15000;
		iHeight = 2000;
		fScale = float(rand() % 2000 + 3500) / 400000.f;

		tInfo.vPos = { float(rand() % iWidth / 250.f) - 13.5f , float(rand() % iHeight) / 1000.f + 4.f, -1.f };
		tInfo.vScale = { 0.005f, fScale + 0.02f, 0.f };
		tInfo.vDir = { float(rand() % 4000 - 2000.f) / 1200.f, float(rand() % 95 + 5), 0.f };
		D3DXVec3Normalize(&tInfo.vDir, &tInfo.vDir);
		tInfo.fVelocity = (rand() % 450 + 250) / 100.f;
		tInfo.fAngle[PARTI_ANGLE_Z] = D3DXToRadian(float(rand() % 15 - 8.f));
		break;

	case SCENEID::SC_STAGE2:
		iWidth = 23000;
		iHeight = 300000;
		fScale = float(rand() % 2000 + 3500) / 400000.f;

		tInfo.vPos = { float(rand() % iWidth / 250.f) - 13.5f , float(rand() % iHeight) / 1000.f + 25.f, -1.f };
		tInfo.vScale = { 0.005f, fScale + 0.02f, 0.f };
		tInfo.vDir = { float(rand() % 4000 - 2000.f) / 1200.f, float(rand() % 95 + 5), 0.f };
		D3DXVec3Normalize(&tInfo.vDir, &tInfo.vDir);
		tInfo.fVelocity = (rand() % 450 + 250) / 100.f;
		tInfo.fAngle[PARTI_ANGLE_Z] = D3DXToRadian(float(rand() % 15 - 8.f));
		break;
	}
}

HRESULT CRain::AddComponent(void)
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

CRain* CRain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRain* pInstance = new CRain(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}