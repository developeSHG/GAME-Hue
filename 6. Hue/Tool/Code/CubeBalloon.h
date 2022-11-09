#ifndef CubeBalloon_h__
#define CubeBalloon_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "Cube.h"

class CCubeBalloon : public Engine::CCube
{
public:
	explicit CCubeBalloon(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
public:
	virtual ~CCubeBalloon(void);

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
	static CCubeBalloon*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeBalloon_h__