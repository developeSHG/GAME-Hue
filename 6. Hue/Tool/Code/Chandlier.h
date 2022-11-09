#ifndef Chandlier_h__
#define Chandlier_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "Obj.h"

class CChandlier : public Engine::CObj
{
public:
	explicit CChandlier(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo);
public:
	virtual ~CChandlier(void);

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
	static CChandlier*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo);
};
#endif		//Chandlier_h__