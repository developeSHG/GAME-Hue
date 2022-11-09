#ifndef TutoCamera_h__
#define TutoCamera_h__

#include "Camera.h"
#include "Include.h"

BEGIN(Engine)

class CTimeMgr;
class CTransform;
class CInfoSubject;
class CKeyMgr;

END

class CCameraObserver;

class CTutoCamera : public Engine::CCamera
{
private:
	explicit CTutoCamera(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CTutoCamera(void);

public:
	void	SetTarget(const Engine::CTransform* pTargetInfo);

public:
	virtual HRESULT	Initialize(void);
	virtual void	Update(void);
	virtual void	LateUpdate(void);

public:
	const bool&		Get_NarrativeMoveing() const { return m_bNarrativeMoveing; }
	D3DXVECTOR3		Get_Eye() { return m_vEye; }
	D3DXVECTOR3		Get_At() { return m_vAt; }
	D3DXVECTOR3		Get_Up() { return m_vUp; }

public:
	void			BasicCameraDefaultInit();

private:
	void			TargetRenewal(void);

private:
	Engine::CInfoSubject*			m_pInfoSubject = nullptr;
	Engine::CKeyMgr*				m_pKeyMgr = nullptr;
	CCameraObserver*				m_pCameraObserver = nullptr;

private:
	const Engine::CTransform*		m_pTargetInfo = nullptr;
	float							m_fDistance = 10.f;
	float							m_fAngle = D3DXToRadian(45.f);
	float							m_fSpeed = 20.f;

private:
	bool							m_bNarrativeMoveing = FALSE;
	NARRATIVE_VIEW					m_eNarrativeView = NARRATIVE_VIEW::DEFAULT;

public:
	static CTutoCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const Engine::CTransform* pTargetInfo);

};

#endif // TutoCamera_h__
