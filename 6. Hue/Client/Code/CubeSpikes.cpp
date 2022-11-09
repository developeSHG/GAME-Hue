#include "stdafx.h"
#include "CubeSpikes.h"

USING(Engine)

CCubeSpikes::CCubeSpikes(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
	: CCube(pGraphicDev, _tCubeInfo)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CCubeSpikes::~CCubeSpikes(void)
{
	Release();
}

HRESULT CCubeSpikes::Initialize(void)
{
	D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));

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

	m_tMtrl.Ambient = BLACK;
	m_tMtrl.Diffuse = BLACK;
	m_tMtrl.Specular = BLACK;
	m_tWhiteMtrl.Emissive = BLACK;
	m_tWhiteMtrl.Power = 5.f;

	m_pInfoCom->m_vScale.x = 1.f;
	m_pInfoCom->m_vScale.y = 0.3f;
	m_pInfoCom->m_vPos.y = m_tCubeInfo.vPos.y - 0.55f;
	m_fOriginPosY = m_pInfoCom->m_vPos.y;

	m_bIsSpike = true;
	return S_OK;
}

void CCubeSpikes::Update(void)
{
	SetTransform();

	if (m_bIsMovingCube)
		FirstMovingUpdate();
}

void CCubeSpikes::LateUpdate(void)
{

}

void CCubeSpikes::Render(void)
{

	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 클리핑  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	RenderFirst();
	RenderSecond();
	RenderThird();

}

void CCubeSpikes::RenderInsideWorld(void)
{
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	RenderFirst();
	RenderSecond();
	RenderThird();
}

void CCubeSpikes::Release(void)
{

}

void CCubeSpikes::RenderFirst(void)
{
	m_pInfoCom->m_vScale.y = 0.3f;
	m_pInfoCom->m_vPos.y = m_fOriginPosY;
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 z값변환 */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_vPos.z = 0.1f - 0.2f;
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_vPos.z = -0.4f;

	CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);
	m_pGraphicDev->SetMaterial(&m_tWhiteMtrl);		//재질★

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	m_pTextureCom->SetTexture(0);
	m_pBufferCom->Render_NoIndices();

	m_pInfoCom->m_vScale.y = 0.275f;
	m_pInfoCom->m_vPos.y -= 0.05f;
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 z값변환 */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_vPos.z = 0.f - 0.2f;
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_vPos.z = -0.5f;

	CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	m_pGraphicDev->SetMaterial(&m_tMtrl);
	m_pTextureCom->SetTexture(1);
	m_pBufferCom->Render_NoIndices();
}

void CCubeSpikes::RenderSecond(void)
{
	m_pInfoCom->m_vScale.y = 0.3f;
	m_pInfoCom->m_vPos.y = m_fOriginPosY;
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 z값변환 */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_vPos.z = 0.6f - 0.2f;
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_vPos.z = 0.1f;

	CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);
	m_pGraphicDev->SetMaterial(&m_tWhiteMtrl);		//재질★

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	m_pTextureCom->SetTexture(0);
	m_pBufferCom->Render_NoIndices();

	m_pInfoCom->m_vScale.y = 0.275f;
	m_pInfoCom->m_vPos.y -= 0.05f;
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 z값변환 */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_vPos.z = 0.5f - 0.2f;
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_vPos.z = 0.f;

	CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	m_pGraphicDev->SetMaterial(&m_tMtrl);
	m_pTextureCom->SetTexture(1);
	m_pBufferCom->Render_NoIndices();
}

void CCubeSpikes::RenderThird(void)
{
	m_pInfoCom->m_vScale.y = 0.3f;
	m_pInfoCom->m_vPos.y = m_fOriginPosY;
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 z값변환 */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_vPos.z = 1.1f - 0.2f;
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_vPos.z = 0.4f;

	CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);
	m_pGraphicDev->SetMaterial(&m_tWhiteMtrl);		//재질★

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	m_pTextureCom->SetTexture(0);
	m_pBufferCom->Render_NoIndices();

	m_pInfoCom->m_vScale.y = 0.275f;
	m_pInfoCom->m_vPos.y -= 0.05f;
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 z값변환 */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_vPos.z = 1.f - 0.2f;
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_vPos.z = 0.5f;

	CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	m_pGraphicDev->SetMaterial(&m_tMtrl);
	m_pTextureCom->SetTexture(1);
	m_pBufferCom->Render_NoIndices();
}

void CCubeSpikes::SetTransform(void)
{
	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 z값변환 */
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

	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 각도변환 */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(0.f);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(90.f);

	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);
}

HRESULT CCubeSpikes::AddComponent(void)
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

	m_pBufferName = L"Buffer_CubeFront";

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

CCubeSpikes * CCubeSpikes::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
{
	CCubeSpikes* pInstance = new CCubeSpikes(pGraphicDev, _tCubeInfo);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
