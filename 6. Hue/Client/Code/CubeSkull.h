#ifndef CubeSkull_h__
#define CubeSkull_h__

#include "Engine_Include.h"
#include "GameObject.h"
#include "CameraObserver.h"
#include "Cube.h"
#include "Include.h"

class CCameraObserver;


class CSkullDust;
class CCubeSkull : public Engine::CCube
{
public:
	explicit CCubeSkull(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
public:
	virtual ~CCubeSkull(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		RenderInsideWorld(void);
	void				RenderWhite(void);
private:
	virtual void		Release(void);
	void				StateChange();

private:
	void				SetTransform(void);

public:
	// Ãß°¡µÊ
	SKULLSTATE&			Get_SkullNextState() { return m_eNextState; }

private:
	CCameraObserver*	m_pCameraObserver = nullptr;
	D3DMATERIAL9		m_tWhiteMtrl;

private:
	SKULLSTATE			m_eCurState;
	SKULLSTATE			m_eNextState;

private:
	D3DXVECTOR3			m_vecOriginPos;
	bool				m_bIsReadyToFall;
	bool				m_bToRight;
	int					m_iRotTime;
	float				m_fCrashTime;
private:
	void				ReadyUpdate();
	void				FallUpdate();
	void				CrashUpdate();
	void				UpUpdate();

	Engine::CVIBuffer*		m_pFirstBufferCom = nullptr;
public:
	virtual HRESULT		AddComponent(void);

	void				MakeSkullDust(void);

private:
	list<CSkullDust*>		m_plistSkullDust;
public:
	static CCubeSkull*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeDeco_h__