#ifndef CubeSkull_h__
#define CubeSkull_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "Cube.h"

class CCubeSkull : public Engine::CCube
{
public:
	explicit CCubeSkull(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
public:
	virtual ~CCubeSkull(void);

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
	static CCubeSkull*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeSkull_h__