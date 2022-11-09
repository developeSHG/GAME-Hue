#include "stdafx.h"
#include "CubePlatform.h"

#include "Include.h"
#include "Export_Function.h"
#include "CameraObserver.h"

USING(Engine)

CCubePlatform::CCubePlatform(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
	: CCube(pGraphicDev, _tCubeInfo)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CCubePlatform::~CCubePlatform(void)
{
	Release();
}

HRESULT CCubePlatform::Initialize(void)
{
	m_pTexName = m_tCubeInfo.tTex.szTex;

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	/************************ (SongSong) ************************/
	/*  Transform컴포넌트에 큐브정보를 대입  */
	m_pInfoCom->m_vPos = m_tCubeInfo.vPos;
	m_pInfoCom->m_vScale = m_tCubeInfo.vScale;

	/************************ (SongSong) ************************/
	/*  원점좌표에 처음 생성좌표를 대입, 목적지 좌표에 Dist좌표대입  */
	m_vOriginPos = m_tCubeInfo.vPos;
	m_vDistPos = m_tCubeInfo.vPos;
	switch (m_tCubeInfo.tPlatformInfo.eDir)
	{
	case Engine::CUBE_MOVING_DIR::CUBE_UP:
		m_vDistPos.y += m_tCubeInfo.tPlatformInfo.fDist;
		m_eNextState = CUBE_UP;
		break;
	case Engine::CUBE_MOVING_DIR::CUBE_DOWN:
		m_vDistPos.y -= m_tCubeInfo.tPlatformInfo.fDist;
		m_eNextState = CUBE_DOWN;
		break;
	case Engine::CUBE_MOVING_DIR::CUBE_LEFT:
		m_vDistPos.x -= m_tCubeInfo.tPlatformInfo.fDist;
		m_eNextState = CUBE_LEFT;
		break;
	case Engine::CUBE_MOVING_DIR::CUBE_RIGHT:
		m_vDistPos.x += m_tCubeInfo.tPlatformInfo.fDist;
		m_eNextState = CUBE_RIGHT;
		break;
	}
	// 추가됨
	m_eCurState = CUBE_MOVING_DIR_END;

	/************************ (SongSong) ************************/
	/*  카메라 옵저버 생성  */
	m_pCameraObserver = CCameraObserver::GetInstance();

	return S_OK;
}

void CCubePlatform::Update(void)
{
	if (!m_bIsMovingCube)
	{/************************ (SongSong) ************************/
	/*  위치이동  */
		PosMoving();
		StateChange();
	}
	SetTransform();

	if (m_bIsMovingCube)
		FirstMovingUpdate();
}

void CCubePlatform::LateUpdate(void)
{

}

void CCubePlatform::Render(void)
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

void CCubePlatform::RenderInsideWorld(void)
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

void CCubePlatform::Release(void)
{

}

void CCubePlatform::SetTransform(void)
{
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

void CCubePlatform::PosMoving(void)
{
	if (GET_TIME < 0.7f)
	{
		switch (m_tCubeInfo.tPlatformInfo.eDir)
		{
		case Engine::CUBE_MOVING_DIR::CUBE_UP:
			if (m_vDistPos.y <= m_pInfoCom->m_vPos.y)
				m_eNextState = CUBE_DOWN;
			else if (m_vOriginPos.y >= m_pInfoCom->m_vPos.y)
				m_eNextState = CUBE_UP;
			break;
		case Engine::CUBE_MOVING_DIR::CUBE_DOWN:
			if (m_vDistPos.y >= m_pInfoCom->m_vPos.y)
				m_eNextState = CUBE_UP;
			else if (m_vOriginPos.y <= m_pInfoCom->m_vPos.y)
				m_eNextState = CUBE_DOWN;
			break;
		case Engine::CUBE_MOVING_DIR::CUBE_LEFT:
			if (m_vDistPos.x >= m_pInfoCom->m_vPos.x)
				m_eNextState = CUBE_RIGHT;
			else if (m_vOriginPos.x <= m_pInfoCom->m_vPos.x)
				m_eNextState = CUBE_LEFT;
			break;
		case Engine::CUBE_MOVING_DIR::CUBE_RIGHT:
			if (m_vDistPos.x <= m_pInfoCom->m_vPos.x)
				m_eNextState = CUBE_LEFT;
			else if (m_vOriginPos.x >= m_pInfoCom->m_vPos.x)
				m_eNextState = CUBE_RIGHT;
			break;
		}
	}
}

void CCubePlatform::StateChange()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case Engine::CUBE_UP:
			break;
		case Engine::CUBE_DOWN:
			break;
		case Engine::CUBE_LEFT:
			break;
		case Engine::CUBE_RIGHT:
			break;
		case Engine::CUBE_MOVING_DIR_END:
			break;
		default:
			break;
		}
		m_bStop = true;
		m_eCurState = m_eNextState;
	}


	if (!m_bStop)
	{
		switch (m_eCurState)
		{
		case Engine::CUBE_UP:

			m_pInfoCom->m_vPos.y += m_tCubeInfo.tPlatformInfo.fSpeed * GET_TIME;
			break;
		case Engine::CUBE_DOWN:
			m_pInfoCom->m_vPos.y -= m_tCubeInfo.tPlatformInfo.fSpeed * GET_TIME;
			break;
		case Engine::CUBE_LEFT:
			m_pInfoCom->m_vPos.x -= m_tCubeInfo.tPlatformInfo.fSpeed * GET_TIME;
			break;
		case Engine::CUBE_RIGHT:
			m_pInfoCom->m_vPos.x += m_tCubeInfo.tPlatformInfo.fSpeed * GET_TIME;
			break;
		case Engine::CUBE_MOVING_DIR_END:
			break;
		default:
			break;
		}
	}
	
	if (m_bStop)
	{
		m_fChangeTime += GET_TIME;
		if (m_fChangeTime >= 1.0f)
		{
			m_fChangeTime = 0.0f;
			m_bStop = false;
		}
	}
}

HRESULT CCubePlatform::AddComponent(void)
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

CCubePlatform * CCubePlatform::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
{
	CCubePlatform* pInstance = new CCubePlatform(pGraphicDev, _tCubeInfo);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
