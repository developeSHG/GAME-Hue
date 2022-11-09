#ifndef CubeCrate_h__
#define CubeCrate_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "Cube.h"

class CCubeCrate : public Engine::CCube
{
public:
	explicit CCubeCrate(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
public:
	virtual ~CCubeCrate(void);

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
	static CCubeCrate*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeCrate_h__