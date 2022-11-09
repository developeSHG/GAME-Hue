#ifndef DoorLock_h__
#define DoorLock_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "Obj.h"

class CDoorLock : public Engine::CObj
{
public:
	explicit CDoorLock(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo);
public:
	virtual ~CDoorLock(void);

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
	static CDoorLock*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo);
};
#endif		//DoorLock_h__