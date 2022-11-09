#ifndef CubeLaser_h__
#define CubeLaser_h__

#include "Engine_Include.h"
#include "GameObject.h"
#include "CameraObserver.h"
#include "Cube.h"


class CLaser;
class CCubeLaser : public Engine::CCube
{
public:
	explicit CCubeLaser(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
public:
	virtual ~CCubeLaser(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		RenderInsideWorld(void);
private:
	virtual void		Release(void);

private:
	void				SetTransform(void);
public:
	// Ãß°¡µÊ
	Engine::CTransform*	Get_LaserInfoCom() { return m_pInfoCom; }
private:
	CCameraObserver*	m_pCameraObserver = nullptr;
	CLaser*				m_pLaser = nullptr;
	bool				m_bIsToDown;
	
public:
	virtual HRESULT		AddComponent(void);
public:
	static CCubeLaser*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeLaser_h__