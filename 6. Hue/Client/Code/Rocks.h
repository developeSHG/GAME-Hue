#ifndef Rocks_h__
#define Rocks_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "enum.h"
BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;
END

class CRocks : public Engine::CGameObject
{
public:
	explicit CRocks(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CRocks(void);

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

public:
	virtual HRESULT      AddComponent(void);
public:
	static CRocks*      Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif      //Rocks_h__