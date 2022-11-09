#ifndef CubeBrick_h__
#define CubeBrick_h__

#include "Engine_Include.h"
#include "GameObject.h"
#include "CameraObserver.h"
#include "Cube.h"

class CCameraObserver;

class CCubeBrick : public Engine::CCube
{
public:
	explicit CCubeBrick(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
public:
	virtual ~CCubeBrick(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		RenderInsideWorld(void);

public:
	void				SetPos(D3DXVECTOR3 vecPos);

private:
	virtual void		Release(void);
	

private:
	void				SetTransform(void);

private:
	CCameraObserver*	m_pCameraObserver = nullptr;

public:
	virtual HRESULT		AddComponent(void);
public:
	static CCubeBrick*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeBrick_h__