#ifndef CubeSpikes_h__
#define CubeSpikes_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "Cube.h"

class CCubeSpikes : public Engine::CCube
{
public:
	explicit CCubeSpikes(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
public:
	virtual ~CCubeSpikes(void);

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
	static CCubeSpikes*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeSpikes_h__