#ifndef SkyBox_h__
#define SkyBox_h__

#include "Engine_Include.h"
#include "Export_Function.h"
#include "CameraObserver.h"
#include "Include.h"
#include "Cube.h"

class CSkyBox : public Engine::CCube
{
public:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CSkyBox(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	void				PreRender(void);
	virtual void		Render(void);
	virtual void		RenderInsideWorld(void);
	void				PostRender(void);
private:
	virtual void		Release(void);

private:
	void				SetTransform(void);

private:
	CCameraObserver*	m_pCameraObserver = nullptr;

public:
	virtual HRESULT		AddComponent(void);
public:
	static CSkyBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//SkyBox_h__