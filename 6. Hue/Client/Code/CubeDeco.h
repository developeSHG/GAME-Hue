#ifndef CubeDeco_h__
#define CubeDeco_h__

#include "Include.h"
#include "Export_Function.h"
#include "Engine_Include.h"
#include "Cube.h"

class CCameraObserver;

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
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		RenderInsideWorld(void);
	void				RenderWhite(void);
	void				SetEndingCube(void) { m_bIsEndingCube = true; }
private:
	virtual void		Release(void);

private:
	void				SetTransform(void);

private:
	CCameraObserver*	m_pCameraObserver = nullptr;
	D3DMATERIAL9		m_tWhiteMtrl;
	bool				m_bIsEndingCube;
public:
	virtual HRESULT		AddComponent(void);
public:
	static CCubeDeco*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeDeco_h__