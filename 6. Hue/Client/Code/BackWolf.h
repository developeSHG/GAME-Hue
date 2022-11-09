#ifndef BackWolf_h__
#define BackWolf_h__

#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;
END

class CBackWolf : public Engine::CGameObject
{
public:
	explicit CBackWolf(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CBackWolf(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		Release(void);
private:

private:
	void				SetTransform(void);

private:
	D3DMATERIAL9				m_tMtrl;
	Engine::CResourceMgr*		m_pResourceMgr;
	Engine::VTXTEX*				m_pVertex = nullptr;
	Engine::VTXTEX*				m_pConvertVtx = nullptr;
	DWORD						m_dwVtxCnt = 0;

	Engine::CTexture*			m_pTextureCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTransform*			m_pInfoCom;

public:
	virtual HRESULT		AddComponent(void);
public:
	static CBackWolf*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//BackWolf_h__