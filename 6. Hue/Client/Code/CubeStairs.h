#ifndef CubeStairs_h__
#define CubeStairs_h__

#include "Export_Function.h"
#include "Engine_Include.h"
#include "Include.h"
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
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		RenderInsideWorld(void);
private:
	virtual void		Release(void);

private:
	void				SetTransform(void);

public:
	float		Get_fB() { return m_fB; }
	float		Get_fA() { return m_fA; }

private:
	CCameraObserver*	m_pCameraObserver = nullptr;
	D3DMATERIAL9		m_tWhiteMtrl;

private:
	float				m_fA;
	float				m_fB;

public:
	virtual HRESULT		AddComponent(void);
public:
	static CCubeStairs*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeStairs_h__