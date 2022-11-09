#ifndef Rain_h__
#define Rain_h__

#include "Engine_Include.h"
#include "Export_Function.h"
#include "GameObject.h"
#include "Include.h"

BEGIN(Engine)

class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;

END

class CRain : public Engine::CGameObject
{
public:
	explicit CRain(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CRain(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	void				PreRender(void);
	virtual void		Render(void);
	virtual void		RenderInsideWorld(void);
	void				PostRender(void);
	virtual void		Release(void);

private:
	void				ParticleInfoCreate();
	void				ParticleInfoInit(PARTICLE_INFO& tInfo);

public:
	void				SetType(int _i) { m_iType = _i; }

private:
	Engine::CResourceMgr*		m_pResourceMgr;
	CCameraObserver*			m_pCameraObserver;

private:
	Engine::CTexture*			m_pTextureCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTransform*			m_pInfoCom;

private:
	D3DMATERIAL9				m_tMtrl;

private:
	list<PARTICLE_INFO>			m_listParticleInfo;
	int							m_iType = 0;

public:
	virtual HRESULT		AddComponent(void);
public:
	static CRain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//Rain_h__