#ifndef CubeRope_h__
#define CubeRope_h__

#include "Engine_Include.h"
#include "GameObject.h"
#include "CameraObserver.h"
#include "Cube.h"


class CCubeRope : public Engine::CCube
{
public:
	explicit CCubeRope(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
public:
	virtual ~CCubeRope(void);

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
	Engine::CTransform*	Get_CrateInfoCom() { return m_pInfoCom; }
private:
	CCameraObserver*	m_pCameraObserver = nullptr;

public:
	virtual HRESULT		AddComponent(void);
public:
	static CCubeRope*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeCrate_h__