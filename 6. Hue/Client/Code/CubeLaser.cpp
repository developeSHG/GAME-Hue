#include "stdafx.h"
#include "CubeLaser.h"
#include "Include.h"
#include "Export_Function.h"

#include "CollisionMgr.h"
#include "ButtonX.h"
#include "CubeMgr.h"
#include "Player.h"
#include "Laser.h"
USING(Engine)

CCubeLaser::CCubeLaser(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
	: CCube(pGraphicDev, _tCubeInfo)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CCubeLaser::~CCubeLaser(void)
{
	Release();
}

HRESULT CCubeLaser::Initialize(void)
{
	if (m_tCubeInfo.tPlatformInfo.eDir == Engine::CUBE_MOVING_DIR::CUBE_LEFT)
		m_bIsToDown = false;
	else
		m_bIsToDown = true;

	m_pTexName = m_tCubeInfo.tTex.szTex;
	m_pTexName = L"Texture_Object_Laser";
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	/************************ (SongSong) ************************/
	/*  Transform컴포넌트에 큐브정보를 대입  */
	m_pInfoCom->m_vPos = m_tCubeInfo.vPos;
	m_pInfoCom->m_vScale = m_tCubeInfo.vScale;
	m_pInfoCom->m_vScale.x = 0.2f;
	m_pInfoCom->m_vScale.y = 0.2f;
	m_pInfoCom->m_vScale.z = 0.2f;

	if (!m_bIsToDown)
	{
		m_pInfoCom->m_vPos.x -= 0.7f;
		m_tCubeInfo.vPos.x -= 0.7f;

	}
	else
	{
		m_pInfoCom->m_vPos.y += 0.7f;
		m_tCubeInfo.vPos.y += 0.7f;
		m_pInfoCom->m_fAngle[ANGLE_Z] = D3DXToRadian(-90.f);
	}
	m_pCameraObserver = CCameraObserver::GetInstance();

	m_pLaser = CLaser::Create(m_pGraphicDev, m_tCubeInfo);
	switch (m_tCubeInfo.tColor)
	{
	case Engine::CUBE_COLOR::CUBE_RED:
		m_pLaser->SetMaterial(RED);
		break;
	case Engine::CUBE_COLOR::CUBE_ORANGE:
		m_pLaser->SetMaterial(ORANGE);
		break;
	case Engine::CUBE_COLOR::CUBE_GREEN:
		m_pLaser->SetMaterial(GREEN);
		break;
	case Engine::CUBE_COLOR::CUBE_YELLOW:
		m_pLaser->SetMaterial(YELLOW);
		break;
	case Engine::CUBE_COLOR::CUBE_BLUE:
		m_pLaser->SetMaterial(BLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_SKYBLUE:
		m_pLaser->SetMaterial(SKYBLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_PINK:
		m_pLaser->SetMaterial(PINK);
		break;
	case Engine::CUBE_COLOR::CUBE_PURPLE:
		m_pLaser->SetMaterial(PURPLE);
		break;
	case Engine::CUBE_COLOR::CUBE_COLOR_END:
		m_pLaser->SetMaterial(BLACK);
		break;
	}
	m_pLaser->SetParentLaser(this);

	m_tCubeInfo.tColor = Engine::CUBE_COLOR::CUBE_COLOR_END;

	/************************ (SongSong) ************************/
	/*  카메라 옵저버 생성  */
	return S_OK;
}

void CCubeLaser::Update(void)
{
	SetTransform();
	m_pLaser->Update();

	if (m_bIsMovingCube)
		FirstMovingUpdate();

	if (m_tCubeInfo.tPlatformInfo.eDir != Engine::CUBE_MOVING_DIR::CUBE_LEFT)
	{
		/************************ (SongSong) ************************/
		/*  카메라 시점에 따른 각도  */
		if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
			m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(0.f);
		else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
			m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(90.f);
	}
}

void CCubeLaser::LateUpdate(void)
{
	m_pLaser->LateUpdate();
}

void CCubeLaser::Render(void)
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

	if (CCubeMgr::GetInstance()->Get_CurCubeColor() != m_pLaser->Get_CubeInfo().tColor)
		m_pLaser->Render();
}

void CCubeLaser::RenderInsideWorld(void)
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

	m_pLaser->RenderInsideWorld();
}

void CCubeLaser::Release(void)
{
	Engine::Safe_Delete(m_pLaser);
}

void CCubeLaser::SetTransform(void)
{
	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);
}

HRESULT CCubeLaser::AddComponent(void)
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

	m_pBufferName = L"Buffer_Cube_Player";

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

CCubeLaser * CCubeLaser::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
{
	CCubeLaser* pInstance = new CCubeLaser(pGraphicDev, _tCubeInfo);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
