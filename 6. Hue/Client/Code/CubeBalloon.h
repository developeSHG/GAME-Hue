#ifndef CubeBalloon_h__
#define CubeBalloon_h__

#include "Export_Function.h"
#include "Engine_Include.h"
#include "Include.h"
#include "Cube.h"


class CCubeBrick;
class CCubeBalloon : public Engine::CCube
{
public:
	explicit CCubeBalloon(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
public:
	virtual ~CCubeBalloon(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	
	virtual void		RenderInsideWorld(void);

public:
	void				SetMoveUp(bool bIsUp) { m_bIsMoveUp = bIsUp; }

	BALLOON_STATE		GetState() { return m_eCurState; }
	void				SetBalloonOriginPos(void);

private:
	virtual void		Release(void);

private:
	void				SetTransform(void);
private:
	CCameraObserver*	m_pCameraObserver = nullptr;
	D3DMATERIAL9		m_tWhiteMtrl;

private:
	CCubeBrick*			m_pCubeBrick;
	bool				m_bIsMoveUp;	//중력을 받냐 안받냐.
	bool				m_bSound;
	BALLOON_STATE		m_eCurState;
public:
	virtual HRESULT		AddComponent(void);
public:
	static CCubeBalloon*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeBalloon_h__