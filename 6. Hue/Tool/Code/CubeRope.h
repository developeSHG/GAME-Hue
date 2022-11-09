#ifndef CubeRope_h__
#define CubeRope_h__

#include "Engine_Include.h"
#include "GameObject.h"

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
	virtual void		Render(void);
private:
	virtual void		Release(void);

private:
	void				SetTransform(void);

public:
	virtual HRESULT		AddComponent(void);
public:
	static CCubeRope*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeRope_h__