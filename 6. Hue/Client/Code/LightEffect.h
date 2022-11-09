#ifndef LightEffect_h__
#define LightEffect_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "enum.h"
BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;
END

class CCameraObserver;
class CLightEffect : public Engine::CGameObject
{
public:
	explicit CLightEffect(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CLightEffect(void);


public:
	void				SetPosAndInfo(float fX, float fY, Engine::OBJDECOID eID);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	int					LightEffectUpdate();
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	void				RenderInside(void);
	virtual void		Release(void);

private:
	D3DMATERIAL9				m_tMtrl;
	Engine::CResourceMgr*		m_pResourceMgr;
	DWORD						m_dwVtxCnt = 0;
	D3DMATERIAL9				m_tBlackMtrl;
	Engine::CTexture*			m_pTextureCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTransform*			m_pInfoCom;

private:
	float						m_fSpeed = 0.f;

	Engine::OBJDECOID			m_eID;

private:
	CCameraObserver*			m_pCameraObserver;
public:
	virtual HRESULT				AddComponent(void);
public:
	static CLightEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//LightEffect_h__