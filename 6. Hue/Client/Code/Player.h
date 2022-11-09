#ifndef Player_h__
#define Player_h__

#include "Engine_Include.h"
#include "GameObject.h"
#include "Include.h"
BEGIN(Engine)

class CVIBuffer;
class CTexture;
class CTransform;
class CResourceMgr;
class CInfoSubject;
class CKeyMgr;
END

class CCameraObserver;
class CDust;
class CPlayer : public Engine::CGameObject
{
	enum COLL_SIDE
	{
		SIDE_NONE, SIDE_UP, SIDE_DOWN, SIDE_LEFT, SIDE_RIGHT, SIDE_END
	};
	enum INTERACTION
	{
		INTERACT_LEFT, INTERACT_RIGHT, INTERACT_END
	};
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CPlayer(void);
public:
	virtual	void		Update(void);
	virtual	void		LateUpdate(void);
	virtual	void		Render(void);
	virtual void		Release(void);

public:
	void		ViewportRender();
private:
	HRESULT		Initialize(void);
	void		KeyInput(void);
	void		SetDirection(void);
	void	    SetTransform(void);
	HRESULT		AddComponent(void);

public:
	Engine::CTransform*	Get_PlayerInfoCom() { return m_pInfoCom; }

public:
	// ȣ��
	PLAYERSTATE		Get_CurState() { return m_eCurState; }
	void			Set_NextStateWalk() {
		m_eNextState = PLAYERSTATE::STATE_IDLE1; m_bPlayerProperties[STATE_WALK] = false; m_bPlayerProperties[STATE_IDLE1] = true;

		/*	if (true == m_bIsNextLeft)
		{
		m_bIsLeftOn = true;
		m_bIsRightOn = true;
		}
		else
		{
		m_bIsRightOn = true;
		m_bIsLeftOn = true;
		}*/
	}
	bool			Get_ForCameraFirstDir() { return m_bIsNextLeft; }
	void			FirstNarrativeKeyInput();
	void			Set_CenterPointInit() { m_CenterPt = { WINCX / 2, WINCY / 2 }; }
	bool			Get_Respawn() { return m_bRespawn; }
	void			Set_Respawn(bool _b) { m_bRespawn = _b; }


public:/**** Added *****/
	bool LateInit();
	void FrameMove();
	void StateChange();
	void StateAction();
	void CollisionCheck();
	void EventCheck();
	void FirstViewUpdate();
	void FreeFall();
	Engine::CVIBuffer*			Get_BufferCom() { return m_pBufferCom; }//
	Engine::CTransform*			Get_Transform() { return m_pInfoCom; }//
public:/* Added */
	void Setplayerproperties(PLAYERSTATE eState, bool bSwitch) { m_bPlayerProperties[eState] = bSwitch; }	// ����
	void ReStart(/*üũ����Ʈ*/);
	void Dead();
	void SetDead(bool bDead) { m_bIsDead = bDead; m_eNextState = STATE_DIE; }
	bool GetDead() { return m_bIsDead; }
	void SetOnStair(bool bChange) { m_bOnStair = bChange; }
	bool GetIsOnStair() { return m_bOnStair; }
	void SetJumpPower(float fPower) { m_fJumpPower = fPower; }
	bool GetPlayerIsNextLeft() { return m_bIsNextLeft; }
	void SetPosition(float fX, float fY);
	void SetRegisterInteraction(bool bOn) { m_bRegisterInteraction = bOn; }
	bool GetIsRegisterInteraction() { return m_bRegisterInteraction; }
private:
	//Component
	Engine::CTexture*			m_pTextureCom[PLAYERSTATE::STATE_END];
	Engine::CTexture*			m_pMaskTextureCom[PLAYERSTATE::STATE_END];
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTransform*			m_pInfoCom;
	Engine::CKeyMgr*			m_pKeyMgr;

	Engine::VTXTEX*				m_pVertex = nullptr;
	Engine::VTXTEX*				m_pConvertVtx = nullptr;
	DWORD						m_dwVtxCnt = 0;
	bool						m_bIsOnGrass;
	void						GrassCheck();
private:
	CCameraObserver*			m_pCameraObserver = nullptr;
	Engine::CResourceMgr*		m_pResourceMgr = nullptr;

private:/**** Added *****/
		// Frame
	FRAME_INFO					m_tFrame;	// GameObject�� �ű��.
											// State
											// State
	PLAYERSTATE					m_eCurState;					// ���� ����
	PLAYERSTATE					m_eNextState;					// ���� ����

																// New Added
	bool						m_bPlayerProperties[STATE_END];	// ���� �Ӽ�
	bool						m_bCollSide[SIDE_END];			// �浹 ����

public:
	bool*						GetPlayerProper() { return m_bPlayerProperties; }
	bool*						GetPlayerCollside() { return m_bCollSide; }
	float						m_fCircleActivePower = 0.f;
	float						m_fMakeLadderSound = 0.f;
private:

	bool						m_bIsNextLeft;
	bool						m_bIsCurLeft;
	bool						m_bIsLeftOn;
	bool						m_bIsRightOn;
	float						m_fLeftAccel;
	float						m_fRightAccel;
	float						m_fFreeFall = 0.0f;
	bool						m_bFall;
	bool						m_bHang;
	bool						m_bHangMove = false;
	// Stat
	float						m_fSpeed;
	float						m_fJumpAccel;
	float						m_fJumpPower;
	float						m_fWaitTime = 0.0f;				// ���ð� (������)
	D3DMATERIAL9				m_tMtrl;
	bool						m_bIsDead = false;				// �׾����� üũ

	bool						m_bInteraction = false;			// ��ȣ�ۿ� �Ⱦ�
	CHECK::SIDE					m_eInteractionSide = CHECK::SIDE_END;

	bool						m_bDeadCurtain;
	bool						m_bLateInit = false;
	bool						m_bRopeEnd = false;
	bool						m_bOnStair = false;
	bool						m_bRegisterInteraction = false;
private:
	/************************ (SongSong) ************************/
	/*  ȣ��  */
	POINT						m_CenterPt;
	float						m_fMouseAngle;

	bool						m_bCircleMoveDir[2] = { false };
	bool						m_bRespawn = false;


private:		//�����
	list<CDust*>				m_listDustEffect;
	float						m_fMakeDustTime = 0;
	void						MakeDustEffect();
	bool						m_bNoKeyInput = false;
	void						ChangeMaterial();
public:
	void						SetPlayerInput(bool bCheck) { m_bNoKeyInput = bCheck; }

public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

#endif		//Player_h__