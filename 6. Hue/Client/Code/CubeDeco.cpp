#include "stdafx.h"
#include "CubeDeco.h"

USING(Engine)

CCubeDeco::CCubeDeco(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
	: CCube(pGraphicDev, _tCubeInfo)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CCubeDeco::~CCubeDeco(void)
{
	Release();
}

HRESULT CCubeDeco::Initialize(void)
{
	D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));

	m_tMtrl.Ambient = BLACK;
	m_tMtrl.Diffuse = BLACK;
	m_tMtrl.Specular = BLACK;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_tWhiteMtrl.Ambient = WHITE;
	m_tWhiteMtrl.Diffuse = WHITE;
	m_tWhiteMtrl.Specular = WHITE;
	m_tWhiteMtrl.Emissive = BLACK;
	m_tWhiteMtrl.Power = 5.f;

	m_pTexName = m_tCubeInfo.tTex.szTex;

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	/************************ (SongSong) ************************/
	/*  Transform컴포넌트에 큐브정보를 대입  */
	m_pInfoCom->m_vPos = m_tCubeInfo.vPos;
	m_pInfoCom->m_vScale = m_tCubeInfo.vScale;

	/************************ (SongSong) ************************/
	/*  카메라 옵저버 생성  */
	m_pCameraObserver = CCameraObserver::GetInstance();

	return S_OK;
}

void CCubeDeco::Update(void)
{
	SetTransform();

	if (m_bIsMovingCube)
	{
		FirstMovingUpdate();
	}

	if (m_bIsEndingCube && g_bIsRenderEnding)
	{
		m_tMtrl.Ambient = SKYBLUE;
		m_tMtrl.Diffuse = SKYBLUE;
		m_tMtrl.Specular = SKYBLUE;
		m_tMtrl.Emissive = BLACK;
		m_tMtrl.Power = 5.f;


		m_tWhiteMtrl.Ambient = SKYBLUE;
		m_tWhiteMtrl.Diffuse = SKYBLUE;
		m_tWhiteMtrl.Specular = SKYBLUE;
		m_tWhiteMtrl.Emissive = BLACK;
		m_tWhiteMtrl.Power = 5.f;

		m_bIsEndingCube = false;
	}
}

void CCubeDeco::LateUpdate(void)
{
}

void CCubeDeco::Render(void)
{
	if (m_bIsEndingCube && g_bIsRenderEnding)
	{
		m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

		/************************ (SongSong) ************************/
		/*  카메라 시점에 따른 클리핑  */
		if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


		m_pTextureCom->SetTexture(m_tCubeInfo.tTex.iIndex);
		m_pBufferCom->Render_NoIndices();
	}
	else if (!m_bIsEndingCube)
	{
		m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

		/************************ (SongSong) ************************/
		/*  카메라 시점에 따른 클리핑  */
		if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


		m_pTextureCom->SetTexture(m_tCubeInfo.tTex.iIndex);
		m_pBufferCom->Render_NoIndices();
	}
}

void CCubeDeco::RenderInsideWorld(void)
{
	if (m_bIsEndingCube && g_bIsRenderEnding)
	{
		m_pGraphicDev->SetMaterial(&m_tWhiteMtrl);		//재질★

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

		/************************ (SongSong) ************************/
		/*  카메라 시점에 따른 클리핑  */
		if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


		m_pTextureCom->SetTexture(m_tCubeInfo.tTex.iIndex);
		m_pBufferCom->Render_NoIndices();
	}
	else if (!m_bIsEndingCube)
	{
		m_pGraphicDev->SetMaterial(&m_tWhiteMtrl);		//재질★

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

		/************************ (SongSong) ************************/
		/*  카메라 시점에 따른 클리핑  */
		if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


		m_pTextureCom->SetTexture(m_tCubeInfo.tTex.iIndex);
		m_pBufferCom->Render_NoIndices();
	}
}

void CCubeDeco::RenderWhite(void)
{
	m_pGraphicDev->SetMaterial(&m_tWhiteMtrl);		//재질★

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 클리핑  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetTexture(m_tCubeInfo.tTex.iIndex);
	m_pBufferCom->Render_NoIndices();
}

void CCubeDeco::Release(void)
{

}

void CCubeDeco::SetTransform(void)
{
	///************************ (SongSong) ************************/
	///*  카메라 시점에 따른 SetTransform  */
	//if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
	//{
	//	if (L"Buffer_CubeFront" == m_pBufferName)
	//	{
	//		if (0.f < m_pInfoCom->m_vScale.z)
	//			m_pInfoCom->m_vScale.z -= 0.8f * GET_TIME;
	//		else
	//			m_pInfoCom->m_vScale.z = 0.f;
	//	}
	//}
	//else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
	//{
	//	if (1.5f > m_pInfoCom->m_vScale.z)
	//		m_pInfoCom->m_vScale.z += 0.85f * GET_TIME;
	//	else
	//		m_pInfoCom->m_vScale.z = 1.5f;
	//}
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 SetTransform  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
	{
		if (0.6f < m_pInfoCom->m_vScale.z)
			m_pInfoCom->m_vScale.z -= 0.8f * GET_TIME;
		else
			m_pInfoCom->m_vScale.z = 0.6f;
	}
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
	{
		if (1.5f > m_pInfoCom->m_vScale.z)
			m_pInfoCom->m_vScale.z += 0.85f * GET_TIME;
		else
			m_pInfoCom->m_vScale.z = 1.5f;
	}

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);
}

HRESULT CCubeDeco::AddComponent(void)
{
	switch (m_tCubeInfo.eType)
	{
	case Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL:
		m_pBufferName = L"Buffer_CubeNormal";
		break;
	case Engine::CUBE_CLIPPING_TYPE::CUBE_FRONT:
		m_pBufferName = L"Buffer_CubeFront";
		break;
	case Engine::CUBE_CLIPPING_TYPE::CUBE_CullB:
		m_pBufferName = L"Buffer_CubeCullB";
		break;
	case Engine::CUBE_CLIPPING_TYPE::CUBE_CullL:
		m_pBufferName = L"Buffer_CubeCullL";
		break;
	case Engine::CUBE_CLIPPING_TYPE::CUBE_CullR:
		m_pBufferName = L"Buffer_CubeCullR";
		break;
	case Engine::CUBE_CLIPPING_TYPE::CUBE_CullLR:
		m_pBufferName = L"Buffer_CubeCullLR";
		break;
	case Engine::CUBE_CLIPPING_TYPE::CUBE_CullBackAndB:
		m_pBufferName = L"Buffer_CubeCullBackAndB";
		break;
	}

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

CCubeDeco * CCubeDeco::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
{
	CCubeDeco* pInstance = new CCubeDeco(pGraphicDev, _tCubeInfo);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
