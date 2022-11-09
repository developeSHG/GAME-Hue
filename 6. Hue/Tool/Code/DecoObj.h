#ifndef DecoObj_h__
#define DecoObj_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "Obj.h"

class CDecoObj : public Engine::CObj
{
public:
	explicit CDecoObj(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo);
public:
	virtual ~CDecoObj(void);

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
	static CDecoObj*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo);
};
#endif		//DecoObj_h__