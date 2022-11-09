#ifndef CubeDeco_h__
#define CubeDeco_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "Cube.h"

class CCubeDeco : public Engine::CCube
{
public:
	explicit CCubeDeco(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
public:
	virtual ~CCubeDeco(void);

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
	static CCubeDeco*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeDeco_h__