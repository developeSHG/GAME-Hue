#ifndef Outline_h__
#define Outline_h__

#include "Engine_Include.h"
#include "Export_Function.h"
#include "Include.h"
#include "ObjBasic.h"

BEGIN(Engine)

class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;

END

class CKeyEffect;
class COutline : public Engine::CGameObject
{
public:
	explicit COutline(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~COutline(void);


public:
	bool				GetIsWait() { return m_bIsWait; }
private:
	virtual HRESULT		Initialize(void);

public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		Release(void);
	virtual void		RenderInsideWorld(void);
	void				SetTransform(void);
private:
	Engine::CVIBuffer*		m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pInfoCom = nullptr;

private:
	D3DMATERIAL9				m_tMtrl;
	Engine::CResourceMgr*		m_pResourceMgr;
	Engine::VTXTEX*				m_pVertex = nullptr;
	Engine::VTXTEX*				m_pConvertVtx = nullptr;
	DWORD						m_dwVtxCnt = 0;
	bool						m_bSound;

private:
	CCameraObserver*	m_pCameraObserver = nullptr;

	bool				m_bSwitch;
	bool				m_bIsDown;
	bool				m_bIsUp;
	float				m_fWaitTime;
	bool				m_bIsWait;
public:
	virtual HRESULT		AddComponent(void);

	static COutline*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
};
#endif		//Outline_h__