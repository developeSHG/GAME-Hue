#include "stdafx.h"
#include "CubeStairs.h"

USING(Engine)

CCubeStairs::CCubeStairs(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
	: CCube(pGraphicDev, _tCubeInfo)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CCubeStairs::~CCubeStairs(void)
{
	Release();
}

HRESULT CCubeStairs::Initialize(void)
{
	D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));

	m_tWhiteMtrl.Ambient = WHITE;
	m_tWhiteMtrl.Diffuse = WHITE;
	m_tWhiteMtrl.Specular = WHITE;
	m_tWhiteMtrl.Emissive = BLACK;
	m_tWhiteMtrl.Power = 5.f;

	m_pTexName = m_tCubeInfo.tTex.szTex;

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	// 계단 반전 추가
	if (m_tCubeInfo.tPlatformInfo.eDir == CUBE_MOVING_DIR::CUBE_LEFT)
	{
		m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(180.0f);
	}
	else
	{
		m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(0.0f);
	}

	/************************ (SongSong) ************************/
	/*  Transform컴포넌트에 큐브정보를 대입  */
	m_pInfoCom->m_vPos = m_tCubeInfo.vPos;
	int iRand = rand() % 3;
	if (iRand == 0)
		m_pInfoCom->m_vPos.z = 0.1f;
	else if (iRand == 1)
		m_pInfoCom->m_vPos.z = 0.25f;
	else if (iRand == 2)
		m_pInfoCom->m_vPos.z = 0.4f;

	if (m_tCubeInfo.tTex.iIndex == 1)
	{
		m_pInfoCom->m_vPos.z = 0.f;
		m_pInfoCom->m_vPos.y -= 0.2f;
	}

	m_pInfoCom->m_vScale = m_tCubeInfo.vScale;
	m_pInfoCom->m_vScale.x = 1.f;
	m_pInfoCom->m_vScale.y = 1.f;

	/************************ (SongSong) ************************/
	/*  카메라 옵저버 생성  */
	m_pCameraObserver = CCameraObserver::GetInstance();


	return S_OK;
}

void CCubeStairs::Update(void)
{
	float fXScaleHalf = m_pInfoCom->m_vScale.x / 2.f;
	float fYScaleHalf = m_pInfoCom->m_vScale.y / 2.f;
	//m_fA = ((m_pInfoCom->m_vPos.y + fYScaleHalf) - (m_pInfoCom->m_vPos.y - fYScaleHalf)) / ((m_pInfoCom->m_vPos.x + fXScaleHalf) - (m_pInfoCom->m_vPos.x - fXScaleHalf));
	//m_fB = m_pInfoCom->m_vPos.y - (m_fA * m_pInfoCom->m_vPos.x);

	//m_fA = ((m_pInfoCom->m_vPos.y + fYScaleHalf) - (m_pInfoCom->m_vPos.y - fYScaleHalf)) / ((m_pInfoCom->m_vPos.x + fXScaleHalf) - (m_pInfoCom->m_vPos.x - fXScaleHalf));

	if (m_tCubeInfo.tPlatformInfo.eDir == CUBE_MOVING_DIR::CUBE_RIGHT)
		m_fA = ((m_pInfoCom->m_vPos.y + fYScaleHalf) - (m_pInfoCom->m_vPos.y - fYScaleHalf)) / ((m_pInfoCom->m_vPos.x + fXScaleHalf) - (m_pInfoCom->m_vPos.x - fXScaleHalf));
	else
		m_fA = ((m_pInfoCom->m_vPos.y + fYScaleHalf) - (m_pInfoCom->m_vPos.y - fYScaleHalf)) / ((m_pInfoCom->m_vPos.x + fXScaleHalf) - (m_pInfoCom->m_vPos.x - fXScaleHalf)) * (-1.0f);

	m_fB = m_pInfoCom->m_vPos.y - (m_fA * m_pInfoCom->m_vPos.x);




	SetTransform();

	CPipeline::MakeWorldMatrix(
		&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	if (m_bIsMovingCube)
		FirstMovingUpdate();
}

void CCubeStairs::LateUpdate(void)
{

}

void CCubeStairs::Render(void)
{
	m_pGraphicDev->SetMaterial(&m_tMtrl);      //재질★

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 클리핑  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetTexture(m_tCubeInfo.tTex.iIndex);
	m_pBufferCom->Render_NoIndices();
}

void CCubeStairs::RenderInsideWorld(void)
{
	m_pGraphicDev->SetMaterial(&m_tWhiteMtrl);      //재질★

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 클리핑  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->SetTexture(m_tCubeInfo.tTex.iIndex);
	m_pBufferCom->Render_NoIndices();
}

void CCubeStairs::Release(void)
{

}

void CCubeStairs::SetTransform(void)
{

}

HRESULT CCubeStairs::AddComponent(void)
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

CCubeStairs * CCubeStairs::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
{
	CCubeStairs* pInstance = new CCubeStairs(pGraphicDev, _tCubeInfo);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}