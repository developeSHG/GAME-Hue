#ifndef CubeBrick_h__
#define CubeBrick_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "Cube.h"

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
	virtual void		Render(void);
private:
	virtual void		Release(void);

private:
	void				SetTransform(void);

public:
	virtual HRESULT		AddComponent(void);
public:
	static CCubeBrick*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeBrick_h__