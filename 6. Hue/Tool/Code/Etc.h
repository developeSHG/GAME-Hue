#ifndef Etc_h__
#define Etc_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "Obj.h"

class CEtc : public Engine::CObj
{
public:
	explicit CEtc(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo);
public:
	virtual ~CEtc(void);

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
	static CEtc*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo);
};
#endif		//Etc_h__