#include "stdafx.h"
#include "CubeRock.h"
#include "CollisionMgr.h"
#include "Player.h"
static float		m_fImpactSoundWaitTime = 0.0f;

USING(Engine)

CCubeRock::CCubeRock(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
	: CCube(pGraphicDev, _tCubeInfo)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CCubeRock::~CCubeRock(void)
{
	Release();
}

HRESULT CCubeRock::Initialize(void)
{

	D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));

	m_tWhiteMtrl.Ambient = WHITE;
	m_tWhiteMtrl.Diffuse = WHITE;
	m_tWhiteMtrl.Specular = WHITE;
	m_tWhiteMtrl.Emissive = BLACK;
	m_tWhiteMtrl.Power = 5.f;

	m_pTexName = m_tCubeInfo.tTex.szTex;
	m_vStartPosition = m_tCubeInfo.vPos;
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

void CCubeRock::Update(void)
{
	if (m_fImpactSoundWaitTime <= 1.0f)
		m_fImpactSoundWaitTime += GET_TIME;
	if (m_fImpactSoundWaitTime >= 1.0f)
		m_bImpactSoundPlay = true;

	Rollin();

	if (m_bReSetTrigger)
	{
		if (WatiForSeconds(1.0f))
		{
			if (m_fDist <= 15.0f)
				Engine::Get_SoundMgr()->MyPlaySound(L"RockStartFall.wav", Engine::CHANNELID::EFFECT);
			m_bReSetTrigger = false;
		}
	}
	else
	{
		float m_fFreeFall = m_fResponsePower * m_fAccel * GET_TIME - GRAVITY * m_fAccel * m_fAccel * 0.5f * GET_TIME;
		if (m_fAccel <= 1.5f)
			m_fAccel += 0.5f * GET_TIME;
		m_pInfoCom->m_vPos.y += m_fFreeFall;
		m_pInfoCom->m_vPos.x += m_fSpeedX * GET_TIME;
		if (CCollisionMgr::GetInstance()->IsCollisionRockToCube(this)) {	}
	}

	SetTransform();
	CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);
}

void CCubeRock::LateUpdate(void)
{

}

void CCubeRock::Render(void)
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

void CCubeRock::RenderInsideWorld(void)
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

void CCubeRock::Release(void)
{

}
void CCubeRock::ReSetPosition()
{
	m_pInfoCom->m_vPos = m_vStartPosition;
	m_fAccel = 0.0f;
	m_bReSetTrigger = true;
	SetSpeed(0.0f, 0.0f);
}

bool CCubeRock::WatiForSeconds(float fTime)
{
	m_fWaitTime += GET_TIME;
	if (m_fWaitTime >= fTime)
	{
		m_fWaitTime = 0.0f;
		return true;
	}
	return false;
}

void CCubeRock::SetImpactSoundPlay(bool bOn)
{
	if (bOn)
	{
		if (m_bImpactSoundPlay)
		{
			Engine::Get_SoundMgr()->MyPlaySound(L"RockImpact.wav", Engine::CHANNELID::EFFECT);
			m_fImpactSoundWaitTime = 0.0f;
			m_bImpactSoundPlay = false;
		}
	}
}

void CCubeRock::SetFallSoundPlay(bool bOn)
{
	Engine::Get_SoundMgr()->MyPlaySound(L"RockFallEnd.wav", Engine::CHANNELID::EFFECT);
}

void CCubeRock::SetTransform(void)
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
}

void CCubeRock::Rollin()
{
	m_pInfoCom->m_fAngle[ANGLE_Z] += D3DXToRadian(180.0f)*GET_TIME;
	if (m_pInfoCom->m_vPos.y <= 5.0f)
		ReSetPosition();
}

HRESULT CCubeRock::AddComponent(void)
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

CCubeRock * CCubeRock::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
{
	CCubeRock* pInstance = new CCubeRock(pGraphicDev, _tCubeInfo);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
