#ifndef Loading_h__
#define Loading_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "enum.h"
BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;
END

class CLoading : public Engine::CGameObject
{
public:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CLoading(void);
	
private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		Release(void);

private:
	void				SetTransform(void);

public:
	void				SetComplete() { m_bComplete = true; }

private:
	Engine::CResourceMgr*		m_pResourceMgr;

private:
	Engine::CTexture*			m_pTextureCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTransform*			m_pInfoCom;

private:
	bool						m_bComplete = false;
	float						m_fIndex = 0;
	bool						m_bGo = false;

public:
	virtual HRESULT		AddComponent(void);
public:
	static CLoading*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//Loading_h__