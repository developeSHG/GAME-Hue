#ifndef Laser_h__
#define Laser_h__

#include "Engine_Include.h"
#include "GameObject.h"
#include "CameraObserver.h"
#include "Cube.h"

BEGIN(Engine)

class CUvTex;
END;

class CCubeLaser;
class CLaserEffect;
class CLaser : public Engine::CCube
{
public:
	explicit CLaser(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
public:
	virtual ~CLaser(void);

public:
	void	SetMoving(bool bMove) { m_bIsMoving = bMove; }

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		RenderInsideWorld(void);

public:
	CCubeLaser*			GetParentLaser(void) { return m_pParent; }
	void				SetParentLaser(CCubeLaser* pParent) { m_pParent = pParent; }
private:
	virtual void		Release(void);


private:
	void				SetTransform(void);
public:
	// Ãß°¡µÊ
	Engine::CTransform*	Get_InfoCom() { return m_pInfoCom; }
private:
	CCameraObserver*	m_pCameraObserver = nullptr;
	Engine::CUvTex*		m_pBufferUvTex = nullptr;
	bool				m_bIsMoving;
	CCubeLaser*			m_pParent = nullptr;
	bool				m_bIsToDown;

	float				m_fMakeEffectTime;
private:
	list<CLaserEffect*> m_listEffect;
public:
	virtual HRESULT		AddComponent(void);
public:
	static CLaser*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeLaser_h__