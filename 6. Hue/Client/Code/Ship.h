#ifndef Ship_h__
#define Ship_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "enum.h"
BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;
END

class CShip : public Engine::CGameObject
{
public:
	explicit CShip(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CShip(void);

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
	bool                  m_bUpDown = false;
	float                  m_fOriginY;
	float                  m_fPowerY = 0.5f;

public:
	virtual HRESULT      AddComponent(void);
public:
	static CShip*      Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif      //Ship_h__