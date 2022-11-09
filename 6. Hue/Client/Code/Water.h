#ifndef Water_h__
#define Water_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "enum.h"
BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;
END

class CWater : public Engine::CGameObject
{
public:
	explicit CWater(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CWater(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void	    RenderInsideWorld(void);
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
	static CWater*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//Water_h__