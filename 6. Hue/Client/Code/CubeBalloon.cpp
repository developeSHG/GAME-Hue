#include "stdafx.h"
#include "CubeBalloon.h"
#include "CubeBrick.h"
#include "CollisionMgr.h"
#include "CubeMgr.h"
#include "Player.h"
USING(Engine)

CCubeBalloon::CCubeBalloon(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
	: CCube(pGraphicDev, _tCubeInfo)
{
	m_pResourceMgr = Engine::Get_ResourceMgr();
}

CCubeBalloon::~CCubeBalloon(void)
{
	Release();
}

HRESULT CCubeBalloon::Initialize(void)
{
	m_bSound = true;
	D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));

	m_tWhiteMtrl.Ambient = BLACK;
	m_tWhiteMtrl.Diffuse = BLACK;
	m_tWhiteMtrl.Specular = BLACK;
	m_tWhiteMtrl.Emissive = BLACK;
	m_tWhiteMtrl.Power = 5.f;

	m_pTexName = m_tCubeInfo.tTex.szTex;

	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	/************************ (SongSong) ************************/
	/*  Transform컴포넌트에 큐브정보를 대입  */
	m_pInfoCom->m_vScale = m_tCubeInfo.vScale;
	
	m_pInfoCom->m_vScale.x = 1.5f;
	m_pInfoCom->m_vScale.y = 1.5f;


	m_pInfoCom->m_vPos = m_tCubeInfo.vPos;
	m_pInfoCom->m_vPos.z = 0.5f;
	/************************ (SongSong) ************************/
	/*  카메라 옵저버 생성  */
	m_pCameraObserver = CCameraObserver::GetInstance();

	Engine::CUBE_INFO eBrickInfo = m_tCubeInfo;
	eBrickInfo.eType = Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL;
	lstrcpy(eBrickInfo.tTex.szTex, L"Texture_CubeColor_BrickColor");

	m_pCubeBrick = CCubeBrick::Create(m_pGraphicDev, eBrickInfo);

	int iRandomColor = rand() % (int)(Engine::CUBE_COLOR::CUBE_COLOR_END);

	while(iRandomColor == (int)m_tCubeInfo.tColor)
		iRandomColor = rand() % (int)(Engine::CUBE_COLOR::CUBE_COLOR_END);

	switch (iRandomColor)
	{
	case Engine::CUBE_COLOR::CUBE_RED:
		m_pCubeBrick->Set_CubeColor(Engine::CUBE_RED);
		m_pCubeBrick->SetMaterial(RED);
		break;
	case Engine::CUBE_COLOR::CUBE_ORANGE:
		m_pCubeBrick->Set_CubeColor(Engine::CUBE_ORANGE);
		m_pCubeBrick->SetMaterial(ORANGE);
		break;
	case Engine::CUBE_COLOR::CUBE_GREEN:
		m_pCubeBrick->Set_CubeColor(Engine::CUBE_GREEN);
		m_pCubeBrick->SetMaterial(GREEN);
		break;
	case Engine::CUBE_COLOR::CUBE_YELLOW:
		m_pCubeBrick->Set_CubeColor(Engine::CUBE_YELLOW);
		m_pCubeBrick->SetMaterial(YELLOW);
		break;
	case Engine::CUBE_COLOR::CUBE_BLUE:
		m_pCubeBrick->Set_CubeColor(Engine::CUBE_BLUE);
		m_pCubeBrick->SetMaterial(BLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_SKYBLUE:
		m_pCubeBrick->Set_CubeColor(Engine::CUBE_SKYBLUE);
		m_pCubeBrick->SetMaterial(SKYBLUE);
		break;
	case Engine::CUBE_COLOR::CUBE_PINK:
		m_pCubeBrick->Set_CubeColor(Engine::CUBE_PINK);
		m_pCubeBrick->SetMaterial(PINK);
		break;
	case Engine::CUBE_COLOR::CUBE_PURPLE:
		m_pCubeBrick->Set_CubeColor(Engine::CUBE_PURPLE);
		m_pCubeBrick->SetMaterial(PURPLE);
		break;
	case Engine::CUBE_COLOR::CUBE_COLOR_END:
		m_pCubeBrick->SetMaterial(BLACK);
		break;
	}

	m_pCubeBrick->Get_InfoCom()->m_vPos.y -= 2.25f;
	//m_pCubeBrick->Get_InfoCom()->m_vPos.y -= 2.25f * 2.0f;
	return S_OK;
}

void CCubeBalloon::Update(void)
{
	float fPlayerX = CCubeMgr::GetInstance()->GetPlayer()->Get_Transform()->m_vPos.x;
	float fPlayerY = CCubeMgr::GetInstance()->GetPlayer()->Get_Transform()->m_vPos.y;

	float fDistX = fPlayerX - m_pInfoCom->m_vPos.x;
	float fDistY = fPlayerY - m_pInfoCom->m_vPos.y;
	float fDistance = sqrtf(fDistX * fDistX + fDistY * fDistY);

	if (!m_bIsMovingCube)
	{
		BALLOON_COLCHECK eCheck = CCollisionMgr::IsCollisionBalloonToCube(this, m_pCubeBrick);

		if (m_tCubeInfo.tColor != CCubeMgr::GetInstance()->Get_CurCubeColor())
			m_bIsMoveUp = true;
		else
			m_bIsMoveUp = false;

		if (eCheck == BALLOON_COLCHECK::COL_BALLOON ||
			eCheck == BALLOON_COLCHECK::COL_BALLOONCUBE)
			m_eCurState = BALLOON_STATE::BALLOON_STOP;

		if (m_bIsMoveUp && GET_TIME < 0.7f)
		{
			if (m_bSound && fDistance<3.f)
			{
				Engine::Get_SoundMgr()->MyPlaySound(L"BalloonUp.wav", Engine::CHANNELID::BALLOON);
				m_bSound = false;
			}
			//만약 풍선이 블록과 충돌 안됫으면 밑에 구문 타게하기
			if (eCheck != BALLOON_COLCHECK::COL_BALLOON)
			{
				//m_pInfoCom->m_vPos.y += 1.45f * GET_TIME;
				//m_pCubeBrick->Get_InfoCom()->m_vPos.y += 1.45f * GET_TIME;
				m_pInfoCom->m_vPos.y += 1.45f *2.0f* GET_TIME;
				m_pCubeBrick->Get_InfoCom()->m_vPos.y += 1.45f*2.0f * GET_TIME;
				m_eCurState = BALLOON_STATE::BALLOON_UP;
			}
		}
		else if (!m_bIsMoveUp && GET_TIME < 0.7f)
		{
			//만약 밑에 블록이 충돌 되지 않았으면. 밑에 구문 타게 하기

			if (eCheck != BALLOON_COLCHECK::COL_BALLOONCUBE)
			{
				m_bSound = true;
				//m_pInfoCom->m_vPos.y -= 2.5 * GET_TIME;
				//m_pCubeBrick->Get_InfoCom()->m_vPos.y -= 2.5 * GET_TIME;
				m_pInfoCom->m_vPos.y -= 2.5 *2.0f* GET_TIME;
				m_pCubeBrick->Get_InfoCom()->m_vPos.y -= 2.5*2.0f * GET_TIME;
				m_eCurState = BALLOON_STATE::BALLOON_DOWN;
			}
		}


		CCollisionMgr::IsCollisionBalloonCubeToPlayer(m_pCubeBrick, CCubeMgr::GetInstance()->GetPlayer(), m_eCurState);
	}
	SetTransform();

	CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);

	m_pCubeBrick->Update();

	if (m_bIsMovingCube)
		FirstMovingUpdate();
}

void CCubeBalloon::LateUpdate(void)
{

}

void CCubeBalloon::Render(void)
{
	if (CCubeMgr::GetInstance()->Get_CurCubeColor() != m_tCubeInfo.tColor)
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

	if (m_pCubeBrick->Get_CubeColor() != CCubeMgr::GetInstance()->Get_CurCubeColor())
		m_pCubeBrick->Render();
}

void CCubeBalloon::RenderInsideWorld(void)
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

	m_pCubeBrick->RenderInsideWorld();
}

void CCubeBalloon::Release(void)
{
	Engine::Safe_Delete(m_pCubeBrick);
}

void CCubeBalloon::SetTransform(void)
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

void CCubeBalloon::SetBalloonOriginPos(void)
{
	m_pInfoCom->m_vPos = m_vecOriginPos;
	m_pCubeBrick->SetOriginPos();
	m_pCubeBrick->Get_InfoCom()->m_vPos.y -= 2.25f;
}

HRESULT CCubeBalloon::AddComponent(void)
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

CCubeBalloon * CCubeBalloon::Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo)
{
	CCubeBalloon* pInstance = new CCubeBalloon(pGraphicDev, _tCubeInfo);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}
