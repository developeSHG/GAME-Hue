#ifndef KeyEffect_h__
#define KeyEffect_h__

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
class CKeyEffect : public Engine::CGameObject
{
public:
	explicit CKeyEffect(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CKeyEffect(void);


public:
	void				SetPosAndInfo(float fX, float fY, float fZ, bool IsKey, bool IsBattery = false);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	int					KeyEffectUpdate();
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		Release(void);
	void				SetMaterial(int iR, int iG, int iB);

private:
	D3DMATERIAL9				m_tMtrl;
	Engine::CResourceMgr*		m_pResourceMgr;
	DWORD						m_dwVtxCnt = 0;

	Engine::CTexture*			m_pTextureCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTransform*			m_pInfoCom;

private:
	float						m_fSpeed = 0.f;

private:
	CCameraObserver*			m_pCameraObserver;
public:
	virtual HRESULT				AddComponent(void);
public:
	static CKeyEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//KeyEffect_h__