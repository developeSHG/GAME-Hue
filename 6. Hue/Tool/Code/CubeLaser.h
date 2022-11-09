#ifndef CubeLaser_h__
#define CubeLaser_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "Cube.h"

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
	virtual void		Render(void);
private:
	virtual void		Release(void);

private:
	void				SetTransform(void);

public:
	virtual HRESULT		AddComponent(void);
public:
	static CCubeLaser*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeLaser_h__