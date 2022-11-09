#ifndef Cloud_h__
#define Cloud_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "enum.h"
BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;
END

class CCloud : public Engine::CGameObject
{
public:
	explicit CCloud(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CCloud(void);

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

public:
	virtual HRESULT		AddComponent(void);
public:
	static CCloud*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//Cloud_h__