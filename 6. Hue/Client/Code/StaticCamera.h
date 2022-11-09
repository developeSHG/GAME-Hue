#ifndef StaticCamera_h__
#define StaticCamera_h__

#include "Camera.h"
#include "Include.h"

BEGIN(Engine)

class CTimeMgr;
class CTransform;
class CInfoSubject;
class CKeyMgr;

END

class CCameraObserver;

class CPlayer;

class CStaticCamera : public Engine::CCamera
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CStaticCamera(void);

public:
	void   SetTarget(const Engine::CTransform* pTargetInfo);

public:
	virtual HRESULT   Initialize(void);
	virtual void   Update(void);
	virtual void   LateUpdate(void);

public:
	const bool&      Get_NarrativeMoveing() const { return m_bNarrativeMoveing; }
	D3DXVECTOR3      Get_Eye() { return m_vEye; }
	D3DXVECTOR3      Get_At() { return m_vAt; }
	D3DXVECTOR3      Get_Up() { return m_vUp; }
	bool          Get_DoorGofor() { return m_bDoorGofor; }
	bool          Get_DoorPoint() { return m_bDoorPoint; }
	D3DXVECTOR3       Get_TargetPos() { return m_pTargetInfo->m_vPos; }
	bool          Get_StageWaveMoving() { return m_bStageWaveMoving; }

public:
	void         SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	void         SetDoorGofor(bool _b) { m_bDoorGofor = _b; }
	void         SetDoorPoint(bool _b) { m_bDoorPoint = _b; }


public:
	// Àç¿ìÇü
	void         UpdateLogoCamera();

public:
	void         BasicCameraDefaultInit();
	void         StageDoorGofor();
	void         StageAddKeyUpdate();

private:
	void         KeyInput(void);
	void         TargetRenewal(void);
	void         TutoTargetRenewal(void);
	void         NarrativeMoveing(void);
public:
	void         CameraDefaultDir(BYTE _byDir);
	void         CameraFirstPersonDir(BYTE _byDir);

public:
	void       StagePlayerWaveCheck();
	void       CameraStageWaveMoving();

private:
	Engine::CInfoSubject*         m_pInfoSubject = nullptr;
	Engine::CKeyMgr*              m_pKeyMgr = nullptr;
	CCameraObserver*              m_pCameraObserver = nullptr;

private:
	const Engine::CTransform*        m_pTargetInfo = nullptr;
	float                      m_fDistance = 10.f;
	float                      m_fAngle = D3DXToRadian(45.f);
	float                      m_fSpeed = 20.f;

private:
	bool                     m_bNarrativeMoveing = FALSE;
	NARRATIVE_VIEW           m_eNarrativeView = NARRATIVE_VIEW::DEFAULT;
	bool                     m_bCubPointMove = false;
	D3DXVECTOR3              m_vCubPointPower[2] = { { 0.f, 0.f, 1.f },{ 20.f, 0.f, 25.f } };

private:
	int                      m_iScene = SCENEID::SC_LOGO;
	bool                     m_bDoorGofor = true;
	bool                     m_bDoorPoint = false;

private:
	float                    m_fPowerY = 0.5f;
	bool                     m_bUpDown = false;
	float                    m_fOriginY;

private:
	bool                m_bStageWaveMoving = false;
	bool                m_bStageWavePoint = false;
	int                   m_iWave = 1;
	bool                m_bStage1MovingPoint[4] = { false, false, false, false };
	bool                m_bStage2MovingPoint[8] = { false, false, false, false, false, false, false };
	float                m_fMovingZ = 0.f;
	float                m_fZoomZ = 0.f;
	CAMERA_ZOOM_TYPE       m_eCameraZoomType = CAMERA_ZOOM_TYPE::ZOOM_DEFAULT;

private:
	CPlayer*                  m_pPlayer = nullptr;

public:
	static CStaticCamera*   Create(LPDIRECT3DDEVICE9 pGraphicDev, const Engine::CTransform* pTargetInfo);

};

#endif // StaticCamera_h__