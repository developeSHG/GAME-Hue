#ifndef Pier_h__
#define Pier_h__

#include "Engine_Include.h"
#include "GameObject.h"
#include "Include.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;
END

class CPier : public Engine::CGameObject
{
public:
	explicit CPier(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CPier(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		RenderInsideWorld(void);
	virtual void		Release(void);

private:
	void				SetTransform(void);

private:
	Engine::CResourceMgr*		m_pResourceMgr;

private:
	Engine::CTexture*			m_pTextureCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTransform*			m_pInfoCom;

private:
	list<Engine::INFO>			m_listInfo;
	list<Engine::INFO>			m_listBridgeInfo;
	bool						m_bGo = false;

public:
	virtual HRESULT		AddComponent(void);
public:
	static CPier*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//Pier_h__