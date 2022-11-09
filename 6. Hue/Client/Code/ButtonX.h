#ifndef ButtonX_h__
#define ButtonX_h__

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
class CButtonX : public Engine::CGameObject
{
public:
	explicit CButtonX(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CButtonX(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		Release(void);
	virtual void		RenderInsideWorld(void);

public:
	void				SetPos(float fX, float fY, bool bIsLeft);
	bool				GetRender() { return m_bIsRender; }
	void				SetRender(bool b) { m_bIsRender = b; }
private:
	void				SetTransform(void);

private:
	Engine::CVIBuffer*		m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pInfoCom = nullptr;

private:
	D3DMATERIAL9				m_tMtrl;
	Engine::CResourceMgr*		m_pResourceMgr;

private:
	bool				m_bIsRender;
	bool				m_bIsLeft;

	CCameraObserver*	m_pCameraObserver = nullptr;

public:
	virtual HRESULT		AddComponent(void);

public:
	static CButtonX*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//Key_h__