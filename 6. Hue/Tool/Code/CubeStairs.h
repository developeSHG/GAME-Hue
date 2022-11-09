#ifndef CubeStairs_h__
#define CubeStairs_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "Cube.h"

class CCubeStairs : public Engine::CCube
{
public:
	explicit CCubeStairs(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
public:
	virtual ~CCubeStairs(void);

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
	static CCubeStairs*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeStairs_h__