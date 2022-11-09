#ifndef Cgv_h__
#define Cgv_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "enum.h"
BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;
END

class CCgv : public Engine::CGameObject
{
public:
	explicit CCgv(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CCgv(void);

private:
	virtual HRESULT      Initialize(void);
public:
	virtual void      Update(void);
	virtual void      LateUpdate(void);
	virtual void      Render(void);
	virtual void	  RenderInsideWorld(void);
	virtual void      Release(void);

private:
	void            SetTransform(void);

private:
	Engine::CResourceMgr*      m_pResourceMgr;

private:
	Engine::CTexture*         m_pTextureCom;
	Engine::CVIBuffer*         m_pBufferCom;
	Engine::CTransform*         m_pInfoCom;

private:
	Engine::INFO						m_tInfo;

public:
	virtual HRESULT      AddComponent(void);
public:
	static CCgv*      Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif      //Cgv_h__