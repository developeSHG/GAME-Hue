#include "stdafx.h"
#include "Player.h"
#include "Export_Function.h"
#include "CameraObserver.h"
#include "CollisionMgr.h"
#include "Dust.h"
#include "StaticCamera.h"
#include "Outline.h"
#include <iostream>
// CONST FLOAT
#define PLAYER_ROT 0.2f			


#define JUMP_POWER 14.0f		// 점프 파워
#define JUMP_ACC 1.5f			// 점프 가속도

//#define GRAVITY 9.8f			// 중력 파워
//#define JUMP_POWER 13.0f		// 점프 파워
//#define JUMP_ACC 2.1f			// 점프 가속도


#define MOVE_ACC 1.7f			// 움직임 가속도
#define MOVE_ACC_MAX 1.0f		// 움직임 가속도 최대
#define MOVE_ACC_MIN 0.1f		// 움직임 가속도 최소

USING(Engine)

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pKeyMgr(Engine::Get_KeyMgr())
{

}

CPlayer::~CPlayer(void)
{
	Release();
}

void CPlayer::ViewportRender()
{
	D3DXMATRIX mScale, mTrans, mRot;

	// 플레이어 뒷 보색 렌더
	{
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
		//m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0X00000088);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		float fPosX = m_pInfoCom->m_vPos.x;
		float fPosY = m_pInfoCom->m_vPos.y;
		D3DXVECTOR3		vecMaskPos = m_pInfoCom->m_vPos;
		vecMaskPos.x -= 0.002f;
		vecMaskPos.y += 0.018f;
		vecMaskPos.z += 0.05f;
		m_pInfoCom->m_vScale = D3DXVECTOR3(1.13f, 1.05f, 1.f);

		if (m_bIsNextLeft)
			D3DXMatrixRotationY(&mRot, D3DXToRadian(180.f));
		else
			D3DXMatrixRotationY(&mRot, D3DXToRadian(0.f));

		D3DXMatrixScaling(&mScale, m_pInfoCom->m_vScale.x, m_pInfoCom->m_vScale.y
			, m_pInfoCom->m_vScale.z);

		D3DXMatrixTranslation(&mTrans, m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y,
			m_pInfoCom->m_vPos.z);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &(mScale * mRot * mTrans));

		m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★
		m_pMaskTextureCom[m_eCurState]->SetTexture((int)m_tFrame.fCurFrame);
		m_pBufferCom->Render_NoIndices();
	}

	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★
	m_pInfoCom->m_vScale = D3DXVECTOR3(1.f, 1.f, 1.f);

	if (m_bIsNextLeft)
		D3DXMatrixRotationY(&mRot, D3DXToRadian(180.f));
	else
		D3DXMatrixRotationY(&mRot, D3DXToRadian(0.f));

	D3DXMatrixScaling(&mScale, m_pInfoCom->m_vScale.x, m_pInfoCom->m_vScale.y
		, m_pInfoCom->m_vScale.z);

	D3DXMatrixTranslation(&mTrans, m_pInfoCom->m_vPos.x, m_pInfoCom->m_vPos.y,
		m_pInfoCom->m_vPos.z);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &(mScale * mRot * mTrans));
	m_pTextureCom[m_eCurState]->SetTexture((int)m_tFrame.fCurFrame);
	m_pBufferCom->Render_NoIndices();
	for (auto& MyDust : m_listDustEffect)
		MyDust->Render();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

HRESULT CPlayer::Initialize(void)
{
	m_fMakeLadderSound = 3.f;
	m_bHang = false;
	m_bDeadCurtain = false;
	/************************ (SongSong) ************************/
	/*  마우스 센터 초기화  */
	m_CenterPt = { WINCX / 2, WINCY / 2 };
	ClientToScreen(g_hWnd, &m_CenterPt);
	SetCursorPos(m_CenterPt.x, m_CenterPt.y);
	ShowCursor(FALSE);

	D3DXCOLOR temp(D3DCOLOR_XRGB(54, 68, 255));
	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;


	// Key
	m_pBufferName = L"Buffer_Cube_Player";
	m_pTexName = L"Player_Idle1";

	// Frame
	m_tFrame.fCurFrame = 0.0f;
	m_tFrame.iMaxFrame = 9;
	m_tFrame.fFrameSpeed = 1.0f;

	// State
	m_eCurState = STATE_FALL;
	m_eNextState = STATE_FALL;

	// Jump
	m_fJumpAccel = 0.0f;
	m_fJumpPower = 0.0f;

	// Stat
	m_fSpeed = 5.0f;

	// New Added
	ZeroMemory(m_bPlayerProperties, sizeof(m_bPlayerProperties));
	ZeroMemory(m_bCollSide, sizeof(m_bCollSide));
	m_bIsCurLeft = false;
	m_bIsNextLeft = false;
	m_bIsLeftOn = false;
	m_bIsRightOn = false;
	m_fLeftAccel = 0.1f;
	m_fRightAccel = 0.1f;
	m_bFall = false;


	/************************ (SongSong) ************************/
	/*  카메라 옵저버 생성  */
	m_pCameraObserver = CCameraObserver::GetInstance();

	// Component
	FAILED_CHECK_RETURN(AddComponent(), E_FAIL);

	//m_pInfoCom->m_vPos = D3DXVECTOR3(5.0f, 5.0f, 0.0f);

	/************************ (SongSong) ************************/
	/*  플레이어 큐브중앙쪽에 위치하게할려고 z값 1줬음  */
	//if (SCENEID::SC_LOGO != *m_pCameraObserver->GetSceneID()
	//	&& SCENEID::SC_TUTORIAL_THREAD != *m_pCameraObserver->GetSceneID()
	//	&& SCENEID::SC_TUTORIAL != *m_pCameraObserver->GetSceneID())
	m_pInfoCom->m_vPos = CCubeMgr::GetInstance()->GetDoorPos();
	m_pInfoCom->m_vScale = D3DXVECTOR3(15.0f, 15.0f, 1.0f);


	// Vertex Info
	m_dwVtxCnt = 6;


	/************************ (SongSong) ************************/
	/*  플레이어 옵저버 치트키  */
	Engine::Get_InfoSubject()->AddData(DATA_PLAYER, this);

	return S_OK;
}

void CPlayer::KeyInput(void)
{

	//std::cout << "PlayerX : " << m_pInfoCom->m_vPos.x << "    " << "PlayerY :   " << m_pInfoCom->m_vPos.y << "    " << "PlayerZ :   " << m_pInfoCom->m_vPos.z << std::endl;

	if (SCENEID::SC_STAGE == *m_pCameraObserver->GetSceneID())
	{
		if (m_pKeyMgr->KeyDown(KEY_1))
			SetPosition(7.9f, 11.0f);
		if (m_pKeyMgr->KeyDown(KEY_2))
			SetPosition(7.7f, 32.0f);
		if (m_pKeyMgr->KeyDown(KEY_3))
			SetPosition(22.98f, 26.5f);
		if (m_pKeyMgr->KeyDown(KEY_4))
			SetPosition(46.71f, 29.8f);
		if (m_pKeyMgr->KeyDown(KEY_5))
			SetPosition(47.42f, 6.4f);
		if (m_pKeyMgr->KeyDown(KEY_6))
			SetPosition(28.3f, 19.55f);
	}
	else if (SCENEID::SC_STAGE2 == *m_pCameraObserver->GetSceneID())
	{
		if (m_pKeyMgr->KeyDown(KEY_1))
			SetPosition(27.01f, 35.43f);
		if (m_pKeyMgr->KeyDown(KEY_2))
			SetPosition(6.03f, 45.36f);
		if (m_pKeyMgr->KeyDown(KEY_3))
			SetPosition(39.96f, 18.55f);
		if (m_pKeyMgr->KeyDown(KEY_4))
			SetPosition(64.85f, 8.09f);
		if (m_pKeyMgr->KeyDown(KEY_5))
			SetPosition(66.81f, 25.11f);
		if (m_pKeyMgr->KeyDown(KEY_6))
			SetPosition(46.75f, 38.25f);
	}
	
	// *** 상호작용 *** //
	if (m_pKeyMgr->KeyDown(KEY_W))
		m_bPlayerProperties[STATE_HANG] = true;

	// ***



	// 서클 활성화 남은 움직이기
	if (m_bNoKeyInput || m_fCircleActivePower && !m_bNoKeyInput)
	{
		m_fCircleActivePower = 1.f;
		if (true == m_bCircleMoveDir[0])
			m_pInfoCom->m_vPos.x -= m_fSpeed * m_fLeftAccel * m_fLeftAccel* GET_TIME;
		if (true == m_bCircleMoveDir[1])
			m_pInfoCom->m_vPos.x += m_fSpeed * m_fRightAccel * m_fRightAccel * GET_TIME;

		return;
	}
	ZeroMemory(m_bCircleMoveDir, sizeof(bool) * 2);
	m_fCircleActivePower = 0.f;

	/************************ (SongSong) ************************/
	/*  희범아, 카메라시점이 무빙일때 키입력을 막는다!   */
	if (TRUE == m_pCameraObserver->GetStaticCamera()->Get_StageWaveMoving())
	{
		m_bInteraction = false;
		Set_NextStateWalk();
		return;
	}

	if (TRUE == *m_pCameraObserver->GetNarrativeMoving())
	{
		m_bInteraction = false;
		Set_NextStateWalk();
		m_pInfoCom->m_vPos.z = 0.f;
		for (int i = 0; i < Engine::ANGLE_END; ++i)
			m_pInfoCom->m_fAngle[i] = 0.f;
		return;
	}

	/************************ (SongSong) ************************/
	/*  카메라가 문까지 무빙 하는 상태에서 키인풋을 막는다  */
	if (false == m_pCameraObserver->GetStaticCamera()->Get_DoorGofor())
	{
		// 사다리 이동
		if (m_bHang)
		{
			m_bPlayerProperties[STATE_JUMP] = false;
			m_bPlayerProperties[STATE_FALL] = false;
			m_bHangMove = false;
			/*		m_fJumpPower = -5.0f;
			m_fJumpAccel = 0.0f;*/
			if (m_pKeyMgr->KeyPressing(KEY_W))
			{
				m_fMakeLadderSound += 1.f * GET_TIME;
				if (m_fMakeLadderSound > 0.5f)
				{
					int iRand = rand() % 2;
					if (iRand == 0)
						Engine::Get_SoundMgr()->MyPlaySound(L"LadderUp1.wav", Engine::CHANNELID::EFFECT);
					else
						Engine::Get_SoundMgr()->MyPlaySound(L"LadderUp2.wav", Engine::CHANNELID::EFFECT);

					m_fMakeLadderSound = 0.f;
				}
				m_pInfoCom->m_vPos.y += m_fSpeed* GET_TIME;
				m_bHangMove = true;
			}
			else if (m_pKeyMgr->KeyPressing(KEY_S))
			{
				m_fMakeLadderSound += 1.f * GET_TIME;
				if (m_fMakeLadderSound > 0.5f)
				{
					int iRand = rand() % 2;
					if (iRand == 0)
						Engine::Get_SoundMgr()->MyPlaySound(L"LadderUp1.wav", Engine::CHANNELID::EFFECT);
					else
						Engine::Get_SoundMgr()->MyPlaySound(L"LadderUp2.wav", Engine::CHANNELID::EFFECT);
					m_fMakeLadderSound = 0.f;
				}
				m_pInfoCom->m_vPos.y -= m_fSpeed* GET_TIME;
				m_bHangMove = true;
			}
			if (m_pKeyMgr->KeyPressing(KEY_SPACE))
			{
				m_bPlayerProperties[STATE_JUMP] = true;
				m_bCollSide[SIDE_UP] = false;
				m_fLeftAccel = MOVE_ACC_MAX;
				m_fRightAccel = MOVE_ACC_MAX;
				m_bPlayerProperties[STATE_HANG] = false;
				m_bHang = false;
				Engine::Get_SoundMgr()->MyPlaySound(L"PlayerJump.wav", Engine::CHANNELID::EFFECT);
			}
		}
		// 사다리 이동 아닐때
		else
		{
			// 상호작용
			if (m_pKeyMgr->KeyPressing(KEY_E))
			{
				m_bPlayerProperties[STATE_PUSH] = false;
				m_bPlayerProperties[STATE_PULL] = false;
				m_bInteraction = true;
			}
			// 사다리 타기
			if (m_pKeyMgr->KeyDown(KEY_W))
			{
				m_bPlayerProperties[STATE_HANG] = true;
				//사다리와 플레이어 충돌검사
				//만약 충돌이면 m_bHang을 트루로 바꿈
			}

			if (m_pKeyMgr->KeyPressing(KEY_A))
			{
				m_bPlayerProperties[STATE_WALK] = true;
				m_bIsNextLeft = true;
				m_bIsLeftOn = true;

				if (m_bInteraction == true && m_eInteractionSide == CHECK::SIDE_RIGHT)
				{
					m_eNextState = STATE_PULL;
					m_fLeftAccel = 0.5f;
					m_pInfoCom->m_vPos.x -= m_fSpeed * m_fLeftAccel * m_fLeftAccel* GET_TIME;
				}
				else if (m_bInteraction == true && m_eInteractionSide == CHECK::SIDE_LEFT)
				{
					m_eNextState = STATE_PUSH;
					m_fLeftAccel = 0.5f;
					m_pInfoCom->m_vPos.x -= m_fSpeed * m_fLeftAccel * m_fLeftAccel* GET_TIME;
				}
				else if (!m_bCollSide[SIDE_RIGHT])
					m_pInfoCom->m_vPos.x -= m_fSpeed * m_fLeftAccel * m_fLeftAccel* GET_TIME;


				m_bCircleMoveDir[0] = true;
			}
			if (m_pKeyMgr->KeyPressing(KEY_D))
			{
				m_bPlayerProperties[STATE_WALK] = true;
				m_bIsNextLeft = false;
				m_bIsRightOn = true;

				if (m_bInteraction == true && m_eInteractionSide == CHECK::SIDE_LEFT)
				{
					m_eNextState = STATE_PULL;
					m_fLeftAccel = 0.5f;
					m_pInfoCom->m_vPos.x += m_fSpeed * m_fLeftAccel * m_fLeftAccel* GET_TIME;
				}
				else if (m_bInteraction == true && m_eInteractionSide == CHECK::SIDE_RIGHT)
				{
					m_eNextState = STATE_PUSH;
					m_fLeftAccel = 0.5f;
					m_pInfoCom->m_vPos.x += m_fSpeed * m_fLeftAccel * m_fLeftAccel* GET_TIME;
				}
				else if (!m_bCollSide[SIDE_LEFT])
					m_pInfoCom->m_vPos.x += m_fSpeed * m_fRightAccel * m_fRightAccel * GET_TIME;

				m_bCircleMoveDir[1] = true;
			}
			if (m_pKeyMgr->KeyPressing(KEY_SPACE))
			{
				m_bPlayerProperties[STATE_JUMP] = true;
				m_bCollSide[SIDE_UP] = false;
			}

		
			// KEY UP
			if (m_pKeyMgr->KeyUp(KEY_W) || m_pKeyMgr->KeyUp(KEY_S))
			{
				m_bPlayerProperties[STATE_HANG] = false;
			}


			if (m_pKeyMgr->KeyUp(KEY_A))
			{
				m_bPlayerProperties[STATE_WALK] = false;
				m_bIsLeftOn = false;
			}

			if (m_pKeyMgr->KeyUp(KEY_D))
			{
				m_bPlayerProperties[STATE_WALK] = false;
				m_bIsRightOn = false;
			}
			if (m_pKeyMgr->KeyUp(KEY_E))
			{
				m_bInteraction = false;
			}
		
		}
	}


	if (m_pInfoCom->m_vPos.y <= -5.0f)
	{
		//ReStart();
		Dead();
	}
}
void CPlayer::SetDirection(void)
{
	// 호경
	// 플레이어 방향이 오른쪽일때 방향벡터가 그대로 오른쪽, 방향이 왼쪽일때 방향벡터는 반대
	if (false == m_bIsNextLeft)
		D3DXVec3TransformNormal(&m_pInfoCom->m_vDir, &D3DXVECTOR3(1.f, 0.f, 0.f), &m_pInfoCom->m_matWorld);
	else
		D3DXVec3TransformNormal(&m_pInfoCom->m_vDir, &D3DXVECTOR3(1.f, 0.f, 0.f), &m_pInfoCom->m_matWorld);
	D3DXVec3Normalize(&m_pInfoCom->m_vDir, &m_pInfoCom->m_vDir);
}

void CPlayer::SetTransform(void)
{
	const D3DXMATRIX*		matView = m_pCameraObserver->GetView();
	const D3DXMATRIX*		matProj = m_pCameraObserver->GetProj();

	//for (size_t i = 0; i < m_dwVtxCnt; ++i)
	//{
	//	m_pConvertVtx[i] = m_pVertex[i];

	//	if (!m_bIsNextLeft)
	//		Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, &m_pInfoCom->m_matWorld);
	//	else
	//		Engine::CPipeline::MyTransformCoordReverseX(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, &m_pInfoCom->m_matWorld);

	//	Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, matView);

	//	if (m_pConvertVtx[i].vPos.z < 1.f)
	//		m_pConvertVtx[i].vPos.z = 1.f;

	//	Engine::CPipeline::MyTransformCoord(&m_pConvertVtx[i].vPos, &m_pConvertVtx[i].vPos, matProj);
	//}
}

HRESULT CPlayer::AddComponent(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Buffer
	pComponent = m_pResourceMgr->Clone_Buffer(Engine::RESOURCE_STATIC, m_pBufferName);
	m_pBufferCom = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	m_mapComponent.emplace(L"Buffer", m_pBufferCom);

	// Texture
	for (int i = 0; i < PLAYERSTATE::STATE_END; ++i)
	{
		switch (i)
		{
		case STATE_IDLE1:
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"Player_Idle1");
			m_pTextureCom[i] = dynamic_cast<Engine::CTexture*>(pComponent);
			m_pTexName = L"Player_Idle1";
			m_mapComponent.emplace(m_pTexName, m_pTextureCom[i]);
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"PlayerMask_Idle1");
			m_pTexName = L"Player_MaskIdle1";
			break;
		case STATE_IDLE2:
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"Player_Idle2");
			m_pTextureCom[i] = dynamic_cast<Engine::CTexture*>(pComponent);
			m_pTexName = L"Player_Idle2";
			m_mapComponent.emplace(m_pTexName, m_pTextureCom[i]);
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"PlayerMask_Idle2");
			m_pTexName = L"Player_MaskIdle2";
			break;
		case STATE_WALK:
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"Player_Walk");
			m_pTextureCom[i] = dynamic_cast<Engine::CTexture*>(pComponent);
			m_pTexName = L"Player_Walk";
			m_mapComponent.emplace(m_pTexName, m_pTextureCom[i]);
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"PlayerMask_Walk");
			m_pTexName = L"Player_MaskWalk";
			break;
		case STATE_RUN:
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"Player_Run");
			m_pTextureCom[i] = dynamic_cast<Engine::CTexture*>(pComponent);
			m_pTexName = L"Player_Run";
			m_mapComponent.emplace(m_pTexName, m_pTextureCom[i]);
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"PlayerMask_Run");
			m_pTexName = L"Player_MaskRun";
			break;
		case STATE_JUMP:
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"Player_Jump");
			m_pTextureCom[i] = dynamic_cast<Engine::CTexture*>(pComponent);
			m_pTexName = L"Player_Jump";
			m_mapComponent.emplace(m_pTexName, m_pTextureCom[i]);
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"PlayerMask_Jump");
			m_pTexName = L"Player_MaskJump";
			break;
		case STATE_FALL:
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"Player_Fall");
			m_pTextureCom[i] = dynamic_cast<Engine::CTexture*>(pComponent);
			m_pTexName = L"Player_Fall";
			m_mapComponent.emplace(m_pTexName, m_pTextureCom[i]);
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"PlayerMask_Fall");
			m_pTexName = L"Player_MaskFall";
			break;
		case STATE_PULL:
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"Player_Pull");
			m_pTextureCom[i] = dynamic_cast<Engine::CTexture*>(pComponent);
			m_pTexName = L"Player_Pull";
			m_mapComponent.emplace(m_pTexName, m_pTextureCom[i]);
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"PlayerMask_Pull");
			m_pTexName = L"Player_MaskPull";
			break;
		case STATE_PUSH:
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"Player_Push");
			m_pTextureCom[i] = dynamic_cast<Engine::CTexture*>(pComponent);
			m_pTexName = L"Player_Push";
			m_mapComponent.emplace(m_pTexName, m_pTextureCom[i]);
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"PlayerMask_Push");
			m_pTexName = L"Player_MaskPush";
			break;
		case STATE_HANG:
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"Player_Hang");
			m_pTextureCom[i] = dynamic_cast<Engine::CTexture*>(pComponent);
			m_pTexName = L"Player_Hang";
			m_mapComponent.emplace(m_pTexName, m_pTextureCom[i]);
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"PlayerMask_Hang");
			m_pTexName = L"Player_MaskHang";
			break;
		case STATE_SHOOT:
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"Player_Shoot");
			m_pTextureCom[i] = dynamic_cast<Engine::CTexture*>(pComponent);
			m_pTexName = L"Player_Shoot";
			m_mapComponent.emplace(m_pTexName, m_pTextureCom[i]);
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"PlayerMask_Shoot");
			m_pTexName = L"Player_MaskShoot";
			break;
		case STATE_SLIDE:
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"Player_Slide");
			m_pTextureCom[i] = dynamic_cast<Engine::CTexture*>(pComponent);
			m_pTexName = L"Player_Slide";
			m_mapComponent.emplace(m_pTexName, m_pTextureCom[i]);
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"PlayerMask_Slide");
			m_pTexName = L"Player_MaskSlide";
			break;
		case STATE_DIE:
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"Player_Die");
			m_pTextureCom[i] = dynamic_cast<Engine::CTexture*>(pComponent);
			m_pTexName = L"Player_Die";
			m_mapComponent.emplace(m_pTexName, m_pTextureCom[i]);
			pComponent = m_pResourceMgr->Clone_Tex(Engine::RESOURCE_STATIC, L"PlayerMask_Die");
			m_pTexName = L"Player_MaskDie";
			break;
		case STATE_END:
			break;
		default:
			break;
		}
		m_pMaskTextureCom[i] = dynamic_cast<Engine::CTexture*>(pComponent);
		m_mapComponent.emplace(m_pTexName, m_pMaskTextureCom[i]);
	}
	m_pTexName = L"Player_Idle1";

	// Transform															
	pComponent = m_pInfoCom = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.emplace(L"Transform", m_pInfoCom);

	return S_OK;
}

void CPlayer::FirstNarrativeKeyInput()
{
	if (*CCameraObserver::GetInstance()->GetSceneID() != SC_STAGE3)
	{/************************ (SongSong) ************************/
	 /*  희범아, 카메라시점이 무빙일때 키입력을 막는다!   */
		if (TRUE == *m_pCameraObserver->GetNarrativeMoving())
		{
			Set_NextStateWalk();
			m_pInfoCom->m_vPos.z = 0.f;

			m_pInfoCom->m_fAngle[ANGLE_X] = m_pInfoCom->m_fAngle[ANGLE_Z] = 0.f;
			return;
		}

		// KEY DOWN
		if (m_pKeyMgr->KeyPressing(KEY_W))
		{
			m_bPlayerProperties[STATE_WALK] = true;
			m_eNextState = PLAYERSTATE::STATE_RUN;

			if (!m_bCollSide[SIDE_LEFT] && !m_bCollSide[SIDE_RIGHT])
			{
				m_pInfoCom->m_vPos.x += m_pInfoCom->m_vDir.x * 5.f * GET_TIME;
				m_pInfoCom->m_vPos.z += m_pInfoCom->m_vDir.z * 5.f * GET_TIME;
			}
		}

		/************************ (SongSong) ************************/
		/*  앞으로 가는 치트키  */
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			m_bPlayerProperties[STATE_WALK] = true;
			m_eNextState = PLAYERSTATE::STATE_RUN;

			m_pInfoCom->m_vPos.x += m_pInfoCom->m_vDir.x * 5.f * GET_TIME;
			m_pInfoCom->m_vPos.z += m_pInfoCom->m_vDir.z * 5.f * GET_TIME;
		}

		if (m_pKeyMgr->KeyPressing(KEY_S))
		{
			m_bPlayerProperties[STATE_WALK] = true;
			m_eNextState = PLAYERSTATE::STATE_RUN;

			m_pInfoCom->m_vPos.x -= m_pInfoCom->m_vDir.x * 5.f * GET_TIME;
			m_pInfoCom->m_vPos.z -= m_pInfoCom->m_vDir.z * 5.f * GET_TIME;
		}
		if (m_pKeyMgr->KeyPressing(KEY_A))
		{
			m_bPlayerProperties[STATE_WALK] = true;
			m_eNextState = PLAYERSTATE::STATE_RUN;

			D3DXVECTOR3 vLeft;
			memcpy(&vLeft, &m_pInfoCom->m_matWorld.m[2][0], sizeof(float) * 3);
			m_pInfoCom->m_vPos.x += vLeft.x * 3.f * GET_TIME;
			m_pInfoCom->m_vPos.z += vLeft.z * 3.f * GET_TIME;
		}
		if (m_pKeyMgr->KeyPressing(KEY_D))
		{
			m_bPlayerProperties[STATE_WALK] = true;
			m_eNextState = PLAYERSTATE::STATE_RUN;

			D3DXVECTOR3 vRight;
			memcpy(&vRight, &m_pInfoCom->m_matWorld.m[2][0], sizeof(float) * 3);
			m_pInfoCom->m_vPos.x -= vRight.x * 3.f * GET_TIME;
			m_pInfoCom->m_vPos.z -= vRight.z * 3.f * GET_TIME;
		}
	}
	else               //스테이지3용
	{
		if (TRUE == *m_pCameraObserver->GetNarrativeMoving())
		{
			Set_NextStateWalk();
			m_pInfoCom->m_vPos.z = 0.f;

			m_pInfoCom->m_fAngle[ANGLE_X] = m_pInfoCom->m_fAngle[ANGLE_Z] = 0.f;
			return;
		}

		// KEY DOWN
		if (m_pKeyMgr->KeyPressing(KEY_W))
		{
			m_bPlayerProperties[STATE_WALK] = true;
			m_eNextState = PLAYERSTATE::STATE_RUN;

			if (!m_bCollSide[SIDE_LEFT] && !m_bCollSide[SIDE_RIGHT])
			{
				m_pInfoCom->m_vPos.x += m_pInfoCom->m_vDir.x * 10.f * GET_TIME;
				m_pInfoCom->m_vPos.z += m_pInfoCom->m_vDir.z * 10.f * GET_TIME;
			}
		}
		if (m_pKeyMgr->KeyPressing(KEY_S))
		{
			m_bPlayerProperties[STATE_WALK] = true;
			m_eNextState = PLAYERSTATE::STATE_RUN;

			m_pInfoCom->m_vPos.x -= m_pInfoCom->m_vDir.x * 7.5f * GET_TIME;
			m_pInfoCom->m_vPos.z -= m_pInfoCom->m_vDir.z * 7.5f * GET_TIME;
		}
		if (m_pKeyMgr->KeyPressing(KEY_A))
		{
			m_bPlayerProperties[STATE_WALK] = true;
			m_eNextState = PLAYERSTATE::STATE_RUN;

			D3DXVECTOR3 vLeft;
			memcpy(&vLeft, &m_pInfoCom->m_matWorld.m[2][0], sizeof(float) * 3);
			m_pInfoCom->m_vPos.x += vLeft.x * 5.f * GET_TIME;
			m_pInfoCom->m_vPos.z += vLeft.z * 5.f * GET_TIME;
		}
		if (m_pKeyMgr->KeyPressing(KEY_D))
		{
			m_bPlayerProperties[STATE_WALK] = true;
			m_eNextState = PLAYERSTATE::STATE_RUN;

			D3DXVECTOR3 vRight;
			memcpy(&vRight, &m_pInfoCom->m_matWorld.m[2][0], sizeof(float) * 3);
			m_pInfoCom->m_vPos.x -= vRight.x * 5.f * GET_TIME;
			m_pInfoCom->m_vPos.z -= vRight.z * 5.f * GET_TIME;
		}
	}
	if (m_pKeyMgr->KeyDown(KEY_SPACE))
	{
		m_bPlayerProperties[STATE_JUMP] = true;
		m_bCollSide[SIDE_UP] = false;
		Engine::Get_SoundMgr()->MyPlaySound(L"PlayerJump.wav", Engine::CHANNELID::EFFECT);
	}

	/*if (m_pKeyMgr->KeyDown(KEY_Q) || m_pInfoCom->m_vPos.y <= -3.0f)
	{
	ReStart();
	}*/

	// KEY UP
	if (m_pKeyMgr->KeyUp(KEY_W) || m_pKeyMgr->KeyUp(KEY_S)
		|| m_pKeyMgr->KeyUp(KEY_A) || m_pKeyMgr->KeyUp(KEY_D)
		|| m_pKeyMgr->KeyUp(KEY_SPACE))
	{
		if (m_eCurState != STATE_FALL)
		{
			m_eNextState = PLAYERSTATE::STATE_IDLE1;
			m_bPlayerProperties[STATE_WALK] = false;
		}
	}
	// 1인칭이 되고 나서. 마우스좌표따라 이동
	if (false == *m_pCameraObserver->GetColorCircleActive())
	{
		POINT pt = {};
		GetCursorPos(&pt);

		// 좌우 이동
		m_pInfoCom->m_fAngle[Engine::ANGLE_Y] += D3DXToRadian((float)(pt.x - m_CenterPt.x) * 20.f) * GET_TIME;

		float fRotY = fabsf(m_pInfoCom->m_fAngle[Engine::ANGLE_Y]);
		fRotY = D3DXToDegree(fRotY);
		float fRatio = (float)((int)fRotY % 360);
		// 상하 이동
		if (0.8f >= m_pInfoCom->m_fAngle[Engine::ANGLE_Z] && -0.8f <= m_pInfoCom->m_fAngle[Engine::ANGLE_Z])
		{
			// 왼쪽방향
			if (fRatio > 90 && fRatio < 270)
			{
				m_pInfoCom->m_fAngle[Engine::ANGLE_Z] += D3DXToRadian((float)(pt.y - m_CenterPt.y) * 10.f) * GET_TIME;
				m_bIsNextLeft = true;
			}
			// 오른쪽방향
			else
			{
				m_pInfoCom->m_fAngle[Engine::ANGLE_Z] -= D3DXToRadian((float)(pt.y - m_CenterPt.y) * 10.f) * GET_TIME;
				m_bIsNextLeft = false;
			}
		}
		else if (0.8f < m_pInfoCom->m_fAngle[Engine::ANGLE_Z])
			m_pInfoCom->m_fAngle[Engine::ANGLE_Z] = 0.8f;
		else if (-0.8f > m_pInfoCom->m_fAngle[Engine::ANGLE_Z])
			m_pInfoCom->m_fAngle[Engine::ANGLE_Z] = -0.8f;

		SetCursorPos(m_CenterPt.x, m_CenterPt.y);
		ShowCursor(FALSE);

		m_fMouseAngle = m_pInfoCom->m_fAngle[Engine::ANGLE_Y];
	}
	else if (TRUE == *m_pCameraObserver->GetColorCircleActive())
	{
		//ShowCursor(TRUE);
		m_pInfoCom->m_fAngle[Engine::ANGLE_Y] = m_fMouseAngle;
	}


	if (m_pInfoCom->m_vPos.y <= -50.0f)
	{
		//ReStart();
		Dead();
	}
}

bool CPlayer::LateInit()
{
	if (!m_bLateInit)
	{
		if (m_fWaitTime >= 5.0f)
		{
			m_bLateInit = true;
			m_fWaitTime = 0.f;
		}
		m_fWaitTime += GET_TIME;
	}

	return m_bLateInit;
}

void CPlayer::FrameMove()
{
	// 1초에 m_iMaxFrame만큼 애니메이션 재생.

	if (m_bHang)
	{
		if (m_bHangMove)
		{
			m_tFrame.fCurFrame += m_tFrame.iMaxFrame * GET_TIME * m_tFrame.fFrameSpeed;
			if ((int)m_tFrame.fCurFrame >= (int)m_tFrame.iMaxFrame + 1)
			{
				if (m_eCurState == STATE_WALK)
					m_eNextState = STATE_RUN;
				m_tFrame.fCurFrame = 0.f;
			}
		}
		else
		{

		}
	}
	else
	{
		if (m_bIsDead && m_eNextState == STATE_DIE)
		{
			m_tFrame.fCurFrame += m_tFrame.iMaxFrame * GET_TIME * m_tFrame.fFrameSpeed;
			if ((int)m_tFrame.fCurFrame >= (int)m_tFrame.iMaxFrame + 1)
			{
				m_tFrame.fCurFrame = m_tFrame.iMaxFrame;
			}
		}
		else
		{
			m_tFrame.fCurFrame += m_tFrame.iMaxFrame * GET_TIME * m_tFrame.fFrameSpeed;
			if ((int)m_tFrame.fCurFrame >= (int)m_tFrame.iMaxFrame + 1)
			{
				if (m_eCurState == STATE_WALK)
					m_eNextState = STATE_RUN;
				m_tFrame.fCurFrame = 0.f;
			}
		}
	}
}

void CPlayer::StateChange()
{
	Engine::CComponent*		pComponent = nullptr;
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case PLAYERSTATE::STATE_IDLE1:
			m_pTexName = L"Player_Idle1";
			m_tFrame.iMaxFrame = 19;
			m_tFrame.fFrameSpeed = 0.5f;

			break;
		case PLAYERSTATE::STATE_IDLE2:
			m_pTexName = L"Player_Idle2";
			m_tFrame.iMaxFrame = 9;

			break;
		case PLAYERSTATE::STATE_WALK:
			m_pTexName = L"Player_Walk";
			m_tFrame.iMaxFrame = 9;
			m_tFrame.fFrameSpeed = 1.7f;

			break;
		case PLAYERSTATE::STATE_RUN:
			m_pTexName = L"Player_Run";
			m_tFrame.iMaxFrame = 7;
			m_tFrame.fFrameSpeed = 2.0f;


			break;
		case PLAYERSTATE::STATE_JUMP:
			m_pTexName = L"Player_Jump";
			m_tFrame.iMaxFrame = 4;
			m_tFrame.fFrameSpeed = 2.0f;
			m_fJumpAccel = JUMP_ACC;
			m_fJumpPower = JUMP_POWER;
			Engine::Get_SoundMgr()->MyPlaySound(L"PlayerJump.wav", Engine::CHANNELID::EFFECT);
			break;
		case PLAYERSTATE::STATE_FALL:
			m_pTexName = L"Player_Fall";
			m_tFrame.iMaxFrame = 4;
			m_tFrame.fFrameSpeed = 2.0f;

			break;
		case PLAYERSTATE::STATE_PULL:
			m_pTexName = L"Player_Pull";
			m_tFrame.iMaxFrame = 9;
			m_tFrame.fFrameSpeed = 1.0f;

			break;
		case PLAYERSTATE::STATE_PUSH:
			m_pTexName = L"Player_Push";
			m_tFrame.iMaxFrame = 9;
			m_tFrame.fFrameSpeed = 1.0f;

			break;
		case PLAYERSTATE::STATE_HANG:
			m_pTexName = L"Player_Hang";
			m_tFrame.iMaxFrame = 6;
			m_tFrame.fFrameSpeed = 2.5f;

			break;
		case PLAYERSTATE::STATE_SHOOT:
			m_pTexName = L"Player_Shoot";
			m_tFrame.iMaxFrame = 9;

			break;
		case PLAYERSTATE::STATE_SLIDE:
			m_pTexName = L"Player_Slide";
			m_tFrame.iMaxFrame = 4;

			break;
		case PLAYERSTATE::STATE_DIE:
			m_pTexName = L"Player_Die";
			m_tFrame.iMaxFrame = 8;
			m_tFrame.fFrameSpeed = 1.0f;

			break;
		case PLAYERSTATE::STATE_END:
			break;
		default:
			break;
		}
		m_tFrame.fCurFrame = 0.f;
		m_bIsCurLeft = m_bIsNextLeft;
		m_eCurState = m_eNextState;
	}
}

void CPlayer::StateAction()
{
	switch (m_eCurState)
	{
	case STATE_IDLE1:
		break;
	case STATE_IDLE2:
		break;
	case STATE_WALK:
		break;
	case STATE_RUN:
		break;
	case STATE_JUMP:
		break;
	case STATE_FALL:
		break;
	case STATE_PULL:
		break;
	case STATE_PUSH:
		break;
	case STATE_HANG:
		break;
	case STATE_SHOOT:
		break;
	case STATE_SLIDE:
		break;
	case STATE_DIE:
		m_fJumpAccel = 0.1f;
		m_fJumpPower = 0.0f;
		break;
	case STATE_END:
		break;
	default:
		break;
	}



	if (m_bCollSide[SIDE_UP] == true)
	{
		m_bCollSide[SIDE_DOWN] = false;
		m_fJumpPower = -5.0f;
	}

	if (m_bCollSide[SIDE_DOWN] == true)
	{
		m_fJumpPower = 5.0f;
		m_fJumpAccel = JUMP_ACC;
	}

	if (m_bOnStair)
	{
		//m_fJumpPower = -5.0f;
		//m_fJumpAccel = 0.0f;
	}

	if (true == m_pCameraObserver->GetStaticCamera()->Get_DoorPoint())
		FreeFall();

	if (m_fFreeFall <= 0.0f)
	{
		m_bPlayerProperties[STATE_JUMP] = false;
		m_bPlayerProperties[STATE_FALL] = true;
	}
}

void CPlayer::CollisionCheck()
{
	m_eInteractionSide = CHECK::SIDE_END;

	// 큐브지형충돌
	if (CCollisionMgr::GetInstance()->IsCollisionPlayerToCube(this, m_bPlayerProperties, m_bCollSide, m_bInteraction, m_bIsLeftOn, m_bIsRightOn, m_bHang, m_bRopeEnd, m_eInteractionSide, m_eNextState, m_bIsOnGrass))

	{
		// 호경
		m_fCircleActivePower = 0.f;
	}
	else
	{
		if (m_bHang)
		{

			{
				m_fJumpAccel = 0.0f;
				m_fJumpPower = -10.0f;
				m_bPlayerProperties[STATE_IDLE1] = false;
				m_bPlayerProperties[STATE_IDLE2] = false;
				m_bPlayerProperties[STATE_WALK] = false;
				m_bPlayerProperties[STATE_RUN] = false;
				m_bPlayerProperties[STATE_PULL] = false;
				m_bPlayerProperties[STATE_PUSH] = false;
				m_bPlayerProperties[STATE_FALL] = true;
				m_eNextState = PLAYERSTATE::STATE_FALL;
				m_bHang = false;
			}
		}
	}
}

void CPlayer::EventCheck()
{
	// 걷는중
	if (m_bInteraction && (m_eNextState != STATE_JUMP || m_eNextState != STATE_FALL))
	{
		////m_eNextState = STATE_PUSH;
		//if (m_bPlayerProperties[STATE_PUSH] == true)
		//{
		//	m_eNextState = STATE_PUSH;

		//}
		//else if (m_bPlayerProperties[STATE_PULL] == true)
		//{
		//	m_eNextState = STATE_PULL;

		//}
	}
	else
	{
		if (m_bPlayerProperties[STATE_WALK] == true/* || m_bOnStair*/)
		{
			m_eNextState = STATE_WALK;

			// 왼쪽으로
			if (m_bIsLeftOn)
			{
				if (m_fLeftAccel <= MOVE_ACC_MAX)
					m_fLeftAccel += MOVE_ACC * GET_TIME;
				else
					m_eNextState = STATE_RUN;
			}
			else
			{
				if (m_fLeftAccel >= MOVE_ACC_MIN)
					m_fLeftAccel -= MOVE_ACC * GET_TIME;
			}
			//오른쪽으로
			if (m_bIsRightOn)
			{
				if (m_fRightAccel <= MOVE_ACC_MAX)
					m_fRightAccel += MOVE_ACC * GET_TIME;
				else
					m_eNextState = STATE_RUN;
			}
			else
			{
				if (m_fRightAccel >= MOVE_ACC_MIN)
					m_fRightAccel -= MOVE_ACC * GET_TIME;
			}
		}
	}



	// 점프 올라가는중
	if (m_bPlayerProperties[STATE_JUMP] == true)
	{
		m_eNextState = STATE_JUMP;
	}

	// 점프 내려오는중
	if (m_bPlayerProperties[STATE_FALL] == true && m_bOnStair == false)
	{
		m_eNextState = STATE_FALL;
		m_bFall = true;	//안쓰임

	}

	// 둘다아니면
	if (m_bPlayerProperties[STATE_JUMP] == false && m_bPlayerProperties[STATE_FALL] == false)
	{
		if (m_bPlayerProperties[STATE_WALK] == false)
		{
			m_eNextState = STATE_IDLE1;
		}
		m_fJumpAccel = 0.0f;
	}

	// 밀기
	if (m_bPlayerProperties[STATE_PUSH] == true)
	{

	}

	// 사다리 타기
	if (m_bHang)
	{
		m_eNextState = STATE_HANG;
	}



	// 플레이어 사망
	if (m_bPlayerProperties[STATE_DIE] == true)
	{
		SetDead(true);
		m_fJumpAccel = JUMP_ACC;
		m_fJumpPower = JUMP_POWER;
	}
}

void CPlayer::FirstViewUpdate()
{

	m_fMakeDustTime -= 3.f * GET_TIME;
	if (m_fMakeDustTime < 0.f && m_eCurState == PLAYERSTATE::STATE_RUN)
	{
		MakeDustEffect();
		if (!m_bIsOnGrass)
			Engine::Get_SoundMgr()->MyPlaySound(L"PlayerWalk.wav", Engine::CHANNELID::EFFECT);
		else
			Engine::Get_SoundMgr()->MyPlaySound(L"PlayerLandGrass.wav", Engine::CHANNELID::EFFECT);
		m_fMakeDustTime = rand() % 3 + 3.f;
	}

	/************************ (SongSong) ************************/
	/*  카메라 시점에 따른 키 업데이트  */
	if (NARRATIVE_VIEW::DEFAULT == *m_pCameraObserver->GetNarrativeView())
	{
		//m_pInfoCom->m_fAngle[Engine::ANGLE_Y] = 0.f;
		KeyInput();

		// 상호작용
		if (m_bInteraction)
		{
			if (m_eInteractionSide == CHECK::SIDE_LEFT)
			{
				if (m_bIsNextLeft)
				{
					m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(180.f);
				}
				else
				{
					m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(180.f);
				}
			}
			else if (m_eInteractionSide == CHECK::SIDE_RIGHT)
			{
				if (m_bIsNextLeft)
				{
					m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(0.f);
				}
				else
				{
					m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(0.f);
				}
			}
		}
		//평상
		else
		{
			if (m_bIsNextLeft)
				m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(180.f);
			else
				m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(0.f);
		}
	}
	else if (NARRATIVE_VIEW::FIRST_PERSON == *m_pCameraObserver->GetNarrativeView())
		FirstNarrativeKeyInput();
}

void CPlayer::FreeFall()
{
	if (!m_bHang)
	{
		if (0.7f > GET_TIME)
		{
			m_fFreeFall = m_fJumpPower * m_fJumpAccel - GRAVITY * m_fJumpAccel * m_fJumpAccel * 0.5f;
			if (m_fFreeFall >= -20.0f)
				m_fJumpAccel += 2.0f * GET_TIME;
			m_pInfoCom->m_vPos.y += m_fFreeFall*GET_TIME;
		}
	}
}

void CPlayer::ReStart()
{
	/************************ (SongSong) ************************/
	/*  플레이어 큐브중앙쪽에 위치하게할려고 z값 1줬음  */
	CCubeMgr::GetInstance()->SetResetBattery(true);
	m_pInfoCom->m_vPos = CCubeMgr::GetInstance()->GetDoorPos();
	//m_pInfoCom->m_vPos = D3DXVECTOR3(15.0f, 15.f, 1.f);

	// Frame
	m_tFrame.fCurFrame = 0.0f;
	m_tFrame.iMaxFrame = 4;
	m_tFrame.fFrameSpeed = 1.0f;

	// State
	m_eNextState = STATE_IDLE1;

	// Jump
	m_fJumpAccel = 0.0f;
	m_fJumpPower = 0.0f;

	// Stat
	m_fSpeed = 5.0f;

	m_bIsCurLeft = false;
	m_bIsNextLeft = false;
	m_bIsLeftOn = false;
	m_bIsRightOn = false;
	m_fLeftAccel = 0.1f;
	m_fRightAccel = 0.1f;
	m_bFall = false;
	m_bIsDead = false;

	/*if(0.7f > GET_TIME && FALSE == m_pCameraObserver->GetStaticCamera()->Get_DoorGofor())
	m_bRespawn = true;*/

	m_bPlayerProperties[STATE_DIE] = false;
	m_bPlayerProperties[STATE_IDLE1] = true;
}

void CPlayer::Dead()
{
	m_bPlayerProperties[STATE_IDLE1];
	m_fWaitTime += GET_TIME;

	FrameMove();

	if (m_fWaitTime >= 1.0f)
	{
		m_fWaitTime = 0.0f;
		if (TRUE == m_pCameraObserver->GetOutlint()->GetIsWait())
			ReStart();
		CCubeMgr::GetInstance()->Set_IsRender(true);
		CCubeMgr::GetInstance()->SetPlayerDie(true);
		return;
	}
}

void CPlayer::SetPosition(float fX, float fY)
{
	m_pInfoCom->m_vPos = D3DXVECTOR3(fX, fY, 1.f);
	// Frame
	m_tFrame.fCurFrame = 0.0f;
	m_tFrame.iMaxFrame = 4;
	m_tFrame.fFrameSpeed = 1.0f;

	// State
	m_eNextState = STATE_IDLE1;

	// Jump
	m_fJumpAccel = 0.0f;
	m_fJumpPower = 0.0f;

	// Stat
	m_fSpeed = 5.0f;

	m_bIsCurLeft = false;
	m_bIsNextLeft = false;
	m_bIsLeftOn = false;
	m_bIsRightOn = false;
	m_fLeftAccel = 0.1f;
	m_fRightAccel = 0.1f;
	m_bFall = false;
	m_bIsDead = false;

	m_bPlayerProperties[STATE_DIE] = false;
	m_bPlayerProperties[STATE_IDLE1] = true;
}

void CPlayer::GrassCheck()
{
	SCENEID eID = *CCameraObserver::GetInstance()->GetSceneID();
	if (eID != SCENEID::SC_STAGE2)
		return;

	float fX = m_pInfoCom->m_vPos.x;
	float fY = m_pInfoCom->m_vPos.y;
	//at fZ = m_pInfoCom->m_vPos.z;

	if ((fX > 23.f && fX < 50.f) && (fY > 6.f && fY < 8.f) || (fX > 30.f && fX < 43.f) && (fY > 11.f && fY < 13.f) || (fX > 45.f && fX < 65.f) && (fY > 36.f && fY < 38.f))
		m_bIsOnGrass = true;
}

void CPlayer::Update(void)
{
	m_bIsOnGrass = false;
	GrassCheck();
	//std::cout << "PlayerX : " << m_pInfoCom->m_vPos.x << "    " << "PlayerY :   " << m_pInfoCom->m_vPos.y << "    " << "PlayerZ :   " << m_pInfoCom->m_vPos.z << std::endl;

	if (LateInit())
		if (GetDead())
			Dead();
		else
		{
			/* 호경이 */
			FirstViewUpdate();

			EventCheck();		// 이벤트 분기
			StateChange();		// 상태전환
			FrameMove();		// 이미지 프레임 진행
			StateAction();		// 상태실행
			CollisionCheck();	// 충돌체크
								//Interaction();
			SetDirection();
			Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
				&m_pInfoCom->m_vScale,
				m_pInfoCom->m_fAngle,
				&m_pInfoCom->m_vPos);
			SetTransform();


			auto iter_begin = m_listDustEffect.begin();
			auto iter_end = m_listDustEffect.end();

			for (; iter_begin != iter_end; )
			{
				int iEvent = (*iter_begin)->DustUpdate();

				if (0 == iEvent)
				{
					delete (*iter_begin);
					iter_begin = m_listDustEffect.erase(iter_begin);
				}
				else
					++iter_begin;
			}


			ChangeMaterial();
		}
}

void CPlayer::LateUpdate(void)
{
	for (auto& MyDust : m_listDustEffect)
		MyDust->LateUpdate();
}

void CPlayer::Render(void)
{
	// 플레이어 뒷 보색 렌더
	{
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
		//m_pResourceMgr->SendToEngine(Engine::RESOURCE_STATIC, m_pBufferName, m_pConvertVtx);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0X00000088);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		float fPosX = m_pInfoCom->m_vPos.x;
		float fPosY = m_pInfoCom->m_vPos.y;
		D3DXVECTOR3		vecMaskPos = m_pInfoCom->m_vPos;
		vecMaskPos.x -= 0.002f;
		vecMaskPos.y += 0.018f;
		vecMaskPos.z += 0.05f;
		m_pInfoCom->m_vScale = D3DXVECTOR3(1.13f, 1.05f, 1.f);
		Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
			&m_pInfoCom->m_vScale,
			m_pInfoCom->m_fAngle,
			&vecMaskPos);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

		m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★
		m_pMaskTextureCom[m_eCurState]->SetTexture((int)m_tFrame.fCurFrame);
		m_pBufferCom->Render_NoIndices();
	}

	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pGraphicDev->SetMaterial(&m_tMtrl);		//재질★
	m_pInfoCom->m_vScale = D3DXVECTOR3(1.f, 1.f, 1.f);
	Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
		&m_pInfoCom->m_vScale,
		m_pInfoCom->m_fAngle,
		&m_pInfoCom->m_vPos);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);
	m_pTextureCom[m_eCurState]->SetTexture((int)m_tFrame.fCurFrame);
	m_pBufferCom->Render_NoIndices();
	for (auto& MyDust : m_listDustEffect)
		MyDust->Render();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CPlayer::Release(void)
{
	for (auto& MyDust : m_listDustEffect)
		Engine::Safe_Delete(MyDust);
}

void CPlayer::MakeDustEffect()
{
	int iRand[6] = { 0,1,2,3,4,5 };
	for (int i = 0; i < 20; ++i)
	{
		int iIdx1 = rand() % 6;
		int iIdx2 = rand() % 6;
		int iTemp = iRand[iIdx1];
		iRand[iIdx1] = iRand[iIdx2];
		iRand[iIdx2] = iTemp;
	}

	float fPlayerPosX = m_pInfoCom->m_vPos.x;
	float fPlayerPosY = m_pInfoCom->m_vPos.y;


	for (int i = 0; i < 4; ++i)
	{
		CDust* pDust = CDust::Create(m_pGraphicDev);
		pDust->SetPosAndInfo(fPlayerPosX, fPlayerPosY, (DUSTEFFECTID)(iRand[i]), m_bIsCurLeft);
		m_listDustEffect.push_back(pDust);
	}
}

void CPlayer::ChangeMaterial()
{
	Engine::CUBE_COLOR eCurCubeColor = CCubeMgr::GetInstance()->Get_CurCubeColor();

	D3DXCOLOR maskColor;
	switch (eCurCubeColor)
	{
	case Engine::CUBE_RED:
		maskColor = D3DXCOLOR(D3DCOLOR_XRGB(19, 123, 98));
		break;
	case Engine::CUBE_ORANGE:
		maskColor = D3DXCOLOR(D3DCOLOR_XRGB(54, 68, 255));
		break;
	case Engine::CUBE_GREEN:
		maskColor = D3DXCOLOR(D3DCOLOR_XRGB(255, 54, 220));
		break;
	case Engine::CUBE_YELLOW:
		maskColor = D3DXCOLOR(D3DCOLOR_XRGB(64, 45, 171));
		break;
	case Engine::CUBE_BLUE:
		maskColor = D3DXCOLOR(D3DCOLOR_XRGB(255, 111, 48));
		break;
	case Engine::CUBE_SKYBLUE:
		maskColor = D3DXCOLOR(D3DCOLOR_XRGB(255, 71, 47));
		break;
	case Engine::CUBE_PINK:
		maskColor = D3DXCOLOR(D3DCOLOR_XRGB(61, 240, 37));
		break;
	case Engine::CUBE_PURPLE:
		maskColor = D3DXCOLOR(D3DCOLOR_XRGB(179, 255, 103));
		break;
	case Engine::CUBE_COLOR_END:
		maskColor = D3DXCOLOR(D3DCOLOR_XRGB(54, 68, 255));
		break;
	default:
		break;
	}

	m_tMtrl.Ambient = maskColor;
	m_tMtrl.Diffuse = maskColor;
	m_tMtrl.Specular = maskColor;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Engine::Safe_Delete(pInstance);

	return pInstance;
}
