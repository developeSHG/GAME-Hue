#include "stdafx.h"
#include "PlayerTuto.h"
#include "Export_Function.h"
#include "CameraObserver.h"
#include "CollisionMgr.h"
#include "Dust.h"
#include "StaticCamera.h"
#include "Outline.h"
#include "SceneSelector.h"

// CONST FLOAT
#define PLAYER_ROT 0.2f         


#define JUMP_POWER 14.0f      // 점프 파워
#define JUMP_ACC 1.5f         // 점프 가속도

//#define GRAVITY 9.8f         // 중력 파워
//#define JUMP_POWER 13.0f      // 점프 파워
//#define JUMP_ACC 2.1f         // 점프 가속도


#define MOVE_ACC 1.7f         // 움직임 가속도
#define MOVE_ACC_MAX 1.0f      // 움직임 가속도 최대
#define MOVE_ACC_MIN 0.1f      // 움직임 가속도 최소

USING(Engine)

CPlayerTuto::CPlayerTuto(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_pKeyMgr(Engine::Get_KeyMgr())
{

}

CPlayerTuto::~CPlayerTuto(void)
{
	Release();
}

void CPlayerTuto::ViewportRender()
{
}

HRESULT CPlayerTuto::Initialize(void)
{
	/************************ (SongSong) ************************/
	/*  씬체크를 위해 */
	static int iSceneNum = 0;
	m_iScene += iSceneNum;
	++iSceneNum;

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
	m_eCurState = STATE_IDLE1;
	m_eNextState = STATE_IDLE1;

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

	Engine::Get_InfoSubject()->AddData(DATA_PLAYERTUTO, this);
	if (SCENEID::SC_TUTORIAL_THREAD == *m_pCameraObserver->GetSceneID())
	{
		m_pInfoCom->m_vPos = *m_pCameraObserver->GetPlayerTutoPos();
		m_fPivotY = m_fShipY = -0.39f;
	}
	else
	{
		Engine::Get_InfoSubject()->AddData(DATA_PLAYERTUTOPOS, &m_pInfoCom->m_vPos);
		m_pInfoCom->m_vPos = { 0.f, 0.f, 0.1f };
		m_fPivotY = m_fShipY = m_pInfoCom->m_vPos.y - 0.37f;
	}

	/************************ (SongSong) ************************/
	/*  플레이어 큐브중앙쪽에 위치하게할려고 z값 1줬음  */
	//m_pInfoCom->m_vPos = { 0.f, 0.f, 0.f };
	m_pInfoCom->m_vScale = D3DXVECTOR3(15.0f, 15.0f, 1.0f);

	// Vertex Info
	m_dwVtxCnt = 6;


	/************************ (SongSong) ************************/
	/*  플레이어 옵저버 치트키  */
	//Engine::Get_InfoSubject()->AddData(DATA_PLAYER, this);

	return S_OK;
}

void CPlayerTuto::KeyInput(void)
{
	if (35.f < m_pInfoCom->m_vPos.x)
		return;

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
		{
			m_pInfoCom->m_vPos.x -= m_fSpeed * m_fLeftAccel * m_fLeftAccel* GET_TIME;
		}


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
	if (m_pKeyMgr->KeyDown(KEY_SPACE)
		&& false == m_bPlayerProperties[STATE_JUMP]
		&& false == m_bPlayerProperties[STATE_FALL])
	{
		m_bPlayerProperties[STATE_JUMP] = true;
		m_bCollSide[SIDE_UP] = false;
		m_fs_JumpPower = S_JUMP_POWER;
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

	if (m_pInfoCom->m_vPos.y <= -10.0f)
	{
		ReStart();
		//Dead();
	}
}
void CPlayerTuto::SetDirection(void)
{
}

HRESULT CPlayerTuto::AddComponent(void)
{
	Engine::CComponent*      pComponent = nullptr;

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

void CPlayerTuto::OnShip()
{
	if (0.7f > GET_TIME)
	{
		if (false == m_bUpDown)
		{
			m_fPowerY -= 0.4f * GET_TIME;
			m_fShipY -= m_fPowerY * GET_TIME;
		}
		else
		{
			m_fPowerY -= 0.4f * GET_TIME;
			m_fShipY += m_fPowerY * GET_TIME;
		}
	}
	if (/*m_fPivotY < m_fShipY || */true == m_bUpDown && 0.f >= m_fPowerY)
	{
		m_bUpDown = false;
		m_fPowerY = 0.71f;
	}
	else if (/*m_fPivotY - 0.5f >m_fShipY || */false == m_bUpDown && 0.f >= m_fPowerY)
	{
		m_bUpDown = true;
		m_fPowerY = 0.71f;
	}

	if (STATE_IDLE1 == m_eCurState
		|| STATE_WALK == m_eCurState
		|| STATE_RUN == m_eCurState)
	{
		if (SCENEID::SC_TUTORIAL_THREAD == m_iScene)
		{
			m_pInfoCom->m_vPos.y = m_fShipY;
		}
		else if (SCENEID::SC_TUTORIAL == m_iScene)
		{
			if (2.8f > m_pInfoCom->m_vPos.x)
				m_pInfoCom->m_vPos.y = m_fShipY;
		}
	}

	if (TRUE == m_bShipOn)
	{
		if (SCENEID::SC_TUTORIAL_THREAD == m_iScene)
		{
			/************************ (SongSong) ************************/
			/*  왼쪽오른쪽 최대지점  */
			if (-3.f >= m_pInfoCom->m_vPos.x)
				m_bCollSide[SIDE_RIGHT] = true;
			else
				m_bCollSide[SIDE_RIGHT] = false;

			if (2.4f <= m_pInfoCom->m_vPos.x)
				m_bCollSide[SIDE_LEFT] = true;
			else
				m_bCollSide[SIDE_LEFT] = false;
		}
		else if (SCENEID::SC_TUTORIAL == m_iScene)
		{
			/************************ (SongSong) ************************/
			/*  왼쪽 최대지점  */
			if (-3.f >= m_pInfoCom->m_vPos.x)
				m_bCollSide[SIDE_RIGHT] = true;
			else
				m_bCollSide[SIDE_RIGHT] = false;
		}
	}
}

bool CPlayerTuto::LateInit()
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

void CPlayerTuto::FrameMove()
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

void CPlayerTuto::StateChange()
{
	Engine::CComponent*      pComponent = nullptr;
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case PLAYERSTATE::STATE_IDLE1:
			m_pTexName = L"Player_Idle1";
			m_tFrame.iMaxFrame = 19;
			m_tFrame.fFrameSpeed = 0.5f;
			m_fWalkDelayTime = 0.0f;

				if (m_bOnCrate)
					Engine::Get_SoundMgr()->MyPlaySound(L"PlayerCrateLand.wav", Engine::CHANNELID::EFFECT);
				else if (m_bOnBrick)
					Engine::Get_SoundMgr()->MyPlaySound(L"PlayerLand.wav", Engine::CHANNELID::EFFECT);
			
		
			break;
		case PLAYERSTATE::STATE_IDLE2:
			m_pTexName = L"Player_Idle2";
			m_tFrame.iMaxFrame = 9;

			break;
		case PLAYERSTATE::STATE_WALK:
			m_pTexName = L"Player_Walk";
			m_tFrame.iMaxFrame = 9;
			m_tFrame.fFrameSpeed = 1.7f;
		/*	if (m_bOnCrate)
				Engine::Get_SoundMgr()->MyPlaySound(L"PlayerCrateWalk.wav", Engine::CHANNELID::EFFECT);
			else if (m_bOnBrick)
				Engine::Get_SoundMgr()->MyPlaySound(L"PlayerWalk.wav", Engine::CHANNELID::EFFECT);
*/
			break;
		case PLAYERSTATE::STATE_RUN:
			m_pTexName = L"Player_Run";
			m_tFrame.iMaxFrame = 7;
			m_tFrame.fFrameSpeed = 2.0f;

			if (45.f > m_pInfoCom->m_vPos.x)
			{
				if (m_bIsFlying)
				{
					if (m_bOnCrate)
						Engine::Get_SoundMgr()->MyPlaySound(L"PlayerCrateLand.wav", Engine::CHANNELID::EFFECT);
					else if (m_bOnBrick)
						Engine::Get_SoundMgr()->MyPlaySound(L"PlayerLand.wav", Engine::CHANNELID::EFFECT);

					m_bIsFlying = false;
				}
				else
				{
					if (m_bOnCrate)
						Engine::Get_SoundMgr()->MyPlaySound(L"PlayerCrateWalk.wav", Engine::CHANNELID::EFFECT);
					else if (m_bOnBrick)
						Engine::Get_SoundMgr()->MyPlaySound(L"PlayerWalk.wav", Engine::CHANNELID::EFFECT);
				}
			}
		
			break;
		case PLAYERSTATE::STATE_JUMP:
			m_pTexName = L"Player_Jump";
			m_tFrame.iMaxFrame = 4;
			m_tFrame.fFrameSpeed = 2.0f;
			m_fJumpAccel = JUMP_ACC;
			m_fJumpPower = JUMP_POWER;
			m_bIsFlying = true;
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

void CPlayerTuto::StateAction()
{
	if (m_eCurState == STATE_RUN)
	{
		m_fWalkDelayTime += GET_TIME;
		if (m_fWalkDelayTime >= 0.25f)
		{
			if (45.f > m_pInfoCom->m_vPos.x)
			{
				if (m_bOnCrate)
					Engine::Get_SoundMgr()->MyPlaySound(L"PlayerCrateWalk.wav", Engine::CHANNELID::EFFECT);
				else if (m_bOnBrick)
					Engine::Get_SoundMgr()->MyPlaySound(L"PlayerWalk.wav", Engine::CHANNELID::EFFECT);
				m_fWalkDelayTime = 0.0f;
			}
		}
	}
	


	if (m_bCollSide[SIDE_UP] == true)
	{
		m_bCollSide[SIDE_DOWN] = false;
		m_fJumpPower = -10.0f;
	}

	if (m_bCollSide[SIDE_DOWN] == true)
	{
		m_fJumpPower = 3.0f;
		m_fJumpAccel = JUMP_ACC;
	}

	if (m_bOnStair)
	{
		//m_fJumpPower = -5.0f;
		//m_fJumpAccel = 0.0f;
	}

	//if(STATE_JUMP == m_eCurState || STATE_FALL == m_eCurState)
	FreeFall();

	if (m_fs_JumpPower <= 0.f)
	{
		m_bPlayerProperties[STATE_JUMP] = false;
		m_bPlayerProperties[STATE_FALL] = true;
	}
	/*if (m_fFreeFall <= 0.0f)
	{
	m_bPlayerProperties[STATE_JUMP] = false;
	m_bPlayerProperties[STATE_FALL] = true;
	}*/

	// cd
	if (m_fShipY >= m_pInfoCom->m_vPos.y)
	{
		m_bOnCrate = true;
		m_bOnBrick = false;

		if (2.8f > m_pInfoCom->m_vPos.x)
		{
			m_bPlayerProperties[STATE_IDLE1] = true;
			m_bPlayerProperties[STATE_FALL] = false;
			m_bPlayerProperties[STATE_JUMP] = false;
		}
	}

	if (SCENEID::SC_TUTORIAL == m_iScene)
	{
		if (4.1f < m_pInfoCom->m_vPos.x
			&& 21.9f > m_pInfoCom->m_vPos.x)
		{
			m_bOnCrate = true;
			m_bOnBrick = false;

			if (-0.31f > m_pInfoCom->m_vPos.y)
			{
				m_pInfoCom->m_vPos.y = -0.31f;

				m_bPlayerProperties[STATE_IDLE1] = true;
				m_bPlayerProperties[STATE_FALL] = false;
				m_bPlayerProperties[STATE_JUMP] = false;
			}

		}
		else if (21.9f < m_pInfoCom->m_vPos.x)
		{
			m_bOnCrate = false;
			m_bOnBrick = true;

			if (0.1f > m_pInfoCom->m_vPos.y)
			{
				m_pInfoCom->m_vPos.y = 0.1f;

				m_bPlayerProperties[STATE_IDLE1] = true;
				m_bPlayerProperties[STATE_FALL] = false;
				m_bPlayerProperties[STATE_JUMP] = false;
			}
		}
	}

	if (SCENEID::SC_TUTORIAL == *m_pCameraObserver->GetSceneID())
	{
		if (23.5f > m_pInfoCom->m_vPos.x)
			m_bPosColor = false;
		else if (27.f < m_pInfoCom->m_vPos.x)
			m_bPosColor = true;

		if (false == m_bPosColor)
		{
			if (Engine::CUBE_COLOR::CUBE_ORANGE != CCubeMgr::GetInstance()->Get_CurCubeColor())
			{
				if (23.5f <= m_pInfoCom->m_vPos.x)
					m_pInfoCom->m_vPos.x = 23.5f;
			}
		}
		if (23.5f < m_pInfoCom->m_vPos.x
			&& 26.7f > m_pInfoCom->m_vPos.x)
			CCubeMgr::GetInstance()->SetPlayerColColor(Engine::CUBE_COLOR::CUBE_ORANGE);
		else
			CCubeMgr::GetInstance()->SetPlayerColColor(Engine::CUBE_COLOR::CUBE_COLOR_END);
		if (true == m_bPosColor)
		{
			if (Engine::CUBE_COLOR::CUBE_ORANGE != CCubeMgr::GetInstance()->Get_CurCubeColor())
			{
				if (26.7f >= m_pInfoCom->m_vPos.x)
					m_pInfoCom->m_vPos.x = 26.7f;
			}
		}

		if (35.f < m_pInfoCom->m_vPos.x)
		{
			m_pInfoCom->m_vPos.x += 5.f * GET_TIME;
			//m_pInfoCom->m_vPos.x += 3.f * GET_TIME;
		}

		if (45.f < m_pInfoCom->m_vPos.x
			&& 45.5f > m_pInfoCom->m_vPos.x)
			CCubeMgr::GetInstance()->Set_IsRender(TRUE);
	}
}

void CPlayerTuto::CollisionCheck()
{
}

void CPlayerTuto::EventCheck()
{
	// 걷는중
	if (m_bInteraction && (m_eNextState != STATE_JUMP || m_eNextState != STATE_FALL))
	{

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
		m_bFall = true;   //안쓰임
	}

	// 둘다아니면
	if (m_bPlayerProperties[STATE_JUMP] == false && m_bPlayerProperties[STATE_FALL] == false)
	{
		if (m_bPlayerProperties[STATE_WALK] == false)
		{
			m_eNextState = STATE_IDLE1;
		}
		m_fJumpAccel = 0.0f;
		m_fs_JumpPower = 0.f;
		m_fCircleActivePower = 0.f;
	}
}

void CPlayerTuto::FirstViewUpdate()
{
}

void CPlayerTuto::FreeFall()
{
	if (0.7f > GET_TIME)
	{
		//m_fFreeFall = m_fJumpPower * m_fJumpAccel * GET_TIME - GRAVITY * m_fJumpAccel * m_fJumpAccel * 0.5f * GET_TIME;
		//if (m_fJumpAccel <= 3.0f && m_fFreeFall >= -2.0f)
		//	m_fJumpAccel += 2.0f * GET_TIME;
		//m_pInfoCom->m_vPos.y += m_fFreeFall;
	}

	if (0.7f > GET_TIME)
	{
		m_fs_JumpPower -= 12.5f * GET_TIME;
		m_pInfoCom->m_vPos.y += m_fs_JumpPower * GET_TIME;
	}
}

void CPlayerTuto::ReStart()
{
}

void CPlayerTuto::Dead()
{
}

void CPlayerTuto::Update(void)
{
	static bool bSoundChange = false;
	static bool bSoundChane2 = false;
	if (TRUE == *m_pCameraObserver->GetColorCircleActive())
	{
		if (Engine::CUBE_COLOR::CUBE_COLOR_END == CCubeMgr::GetInstance()->Get_CurCubeColor())
			bSoundChange = true;
	}
	if (true == bSoundChange)
	{
		if (FALSE == *m_pCameraObserver->GetColorCircleActive() && false == bSoundChane2)
		{
			Engine::Get_SoundMgr()->StopAll();
			Engine::Get_SoundMgr()->PlayBGM(L"Derplets.wav");
			bSoundChane2 = true;
		}
	}

	KeyInput();

	/************************ (SongSong) ************************/
	/*  왼쪽 오른쪽 이미지반전  */
	if (m_bIsNextLeft)
		m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(180.f);
	else
		m_pInfoCom->m_fAngle[ANGLE_Y] = D3DXToRadian(0.f);

	/************************ (SongSong) ************************/
	/*  배에 있을때  */
	OnShip();

	EventCheck();      // 이벤트 분기
	StateChange();      // 상태전환
	FrameMove();      // 이미지 프레임 진행
	StateAction();      // 상태실행, 중력

						//EventCheck();      // 이벤트 분기
						//StateAction();      // 상태실행, 중력

						//StateChange();      // 상태전환
						//FrameMove();      // 이미지 프레임 진행

	ChangeMaterial();

	CInfoSubject::GetInstance()->Notify(DATA_PLAYERTUTOPOS);
}

void CPlayerTuto::LateUpdate(void)
{
}

void CPlayerTuto::Render(void)
{
	// 플레이어 뒷 보색 렌더
	{
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

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
		D3DXVECTOR3      vecMaskPos = m_pInfoCom->m_vPos;
		vecMaskPos.x -= 0.002f;
		vecMaskPos.y += 0.018f;
		vecMaskPos.z += 0.05f;
		m_pInfoCom->m_vScale = D3DXVECTOR3(1.13f, 1.05f, 1.f);
		Engine::CPipeline::MakeWorldMatrix(&m_pInfoCom->m_matWorld,
			&m_pInfoCom->m_vScale,
			m_pInfoCom->m_fAngle,
			&vecMaskPos);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pInfoCom->m_matWorld);

		if (Engine::CUBE_COLOR::CUBE_COLOR_END == CCubeMgr::GetInstance()->Get_CurCubeColor())
		{
			m_tMtrl.Ambient = WHITE;
			m_tMtrl.Diffuse = WHITE;
			m_tMtrl.Specular = WHITE;
			m_tMtrl.Emissive = BLACK;
			m_tMtrl.Power = 5.f;
		}

		m_pGraphicDev->SetMaterial(&m_tMtrl);      //재질★
		m_pMaskTextureCom[m_eCurState]->SetTexture((int)m_tFrame.fCurFrame);
		m_pBufferCom->Render_NoIndices();
	}

	m_tMtrl.Ambient = WHITE;
	m_tMtrl.Diffuse = WHITE;
	m_tMtrl.Specular = WHITE;
	m_tMtrl.Emissive = BLACK;
	m_tMtrl.Power = 5.f;

	m_pGraphicDev->SetMaterial(&m_tMtrl);      //재질★
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

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CPlayerTuto::Release(void)
{
	for (auto& MyDust : m_listDustEffect)
		Engine::Safe_Delete(MyDust);
}

void CPlayerTuto::MakeDustEffect()
{
}

void CPlayerTuto::ChangeMaterial()
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

CPlayerTuto * CPlayerTuto::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerTuto*   pInstance = new CPlayerTuto(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Engine::Safe_Delete(pInstance);

	return pInstance;
}