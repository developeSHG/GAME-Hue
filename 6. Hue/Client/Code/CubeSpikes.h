#ifndef CubeSpikes_h__
#define CubeSpikes_h__

#include "Export_Function.h"
#include "Engine_Include.h"
#include "Include.h"
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
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		RenderInsideWorld(void);
private:
	virtual void		Release(void);

private:
	void				RenderFirst(void);
	void				RenderSecond(void);
	void				RenderThird(void);
private:
	void				SetTransform(void);

private:
	CCameraObserver*	m_pCameraObserver = nullptr;
	D3DMATERIAL9		m_tWhiteMtrl;
	float				m_fOriginPosY;
public:
	virtual HRESULT		AddComponent(void);
public:
	static CCubeSpikes*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeSpikes_h__