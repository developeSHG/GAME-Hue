#ifndef CubeRock_h__
#define CubeRock_h__

#include "Export_Function.h"
#include "Engine_Include.h"
#include "Include.h"
#include "Cube.h"

class CCubeRock : public Engine::CCube
{
public:
	explicit CCubeRock(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
public:
	virtual ~CCubeRock(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		RenderInsideWorld(void);
private:
	virtual void		Release(void);

	void				StateChange();
public:
	void				SetSpeed(float fX, float fY) { m_fSpeedX = fX, m_fSpeedY = fY; }
	void				SetAcc(float fAcc) { m_fAccel = fAcc; }
	void				ReSetPosition();
	bool				WatiForSeconds(float fTime);
	void				SetTrigger(bool bSet) { m_bReSetTrigger = bSet; }
	bool				GetImpactSoundPlay() { return m_bImpactSoundPlay; }
	void				SetImpactSoundPlay(bool bOn);
	void				SetFallSoundPlay(bool bOn);
	void				SetDist(float& fDist) { m_fDist = fDist; }
private:
	void				SetTransform(void);
	void				Rollin();
private:
	CCameraObserver*	m_pCameraObserver = nullptr;
	D3DMATERIAL9		m_tWhiteMtrl;

public:
	virtual HRESULT		AddComponent(void);
public:
	static CCubeRock*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
private:
	D3DXVECTOR3			m_vStartPosition;

	float				m_fSpeedX = 0.0f;
	float				m_fSpeedY = 0.0f;
	float				m_fAccel = 0.0f;
	float				m_fResponsePower = 2.0f;
	float				m_fWaitTime = 0.0f;
	float				m_fDist = 0.0f;
	bool				m_bReSetTrigger = false;
	bool				m_bImpactSoundPlay = true;


};
#endif		//CubeRock_h__