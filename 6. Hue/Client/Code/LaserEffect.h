#ifndef LaserEffect_h__
#define LaserEffect_h__

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
class CLaserEffect : public Engine::CGameObject
{
public:
	explicit CLaserEffect(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CLaserEffect(void);


public:
	void				SetPosAndInfo(float fX, float fY, float fZ, bool IsKey);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	int					LaserEffectUpdate();
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		Release(void);
	
	void				SetMaterial(Engine::CUBE_COLOR eColor);
private:
	D3DMATERIAL9				m_tMtrl;
	Engine::CResourceMgr*		m_pResourceMgr;
	DWORD						m_dwVtxCnt = 0;

	Engine::CTexture*			m_pTextureCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTransform*			m_pInfoCom;

private:
	float						m_fSpeed = 0.f;
	bool						m_bIsDown;
	CCameraObserver*			m_pCameraObserver;
public:
	virtual HRESULT				AddComponent(void);
public:
	static CLaserEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//LaserEffect_h__