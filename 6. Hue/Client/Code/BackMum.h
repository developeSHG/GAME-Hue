#ifndef BackMum_h__
#define BackMum_h__

#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;
END

class CBackMum : public Engine::CGameObject
{
public:
	explicit CBackMum(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CBackMum(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		Release(void);

	void				SetMaterial(int fR, int fG, int fB);

private:

private:
	void				SetTransform(void);

private:
	D3DMATERIAL9				m_tMtrl;
	D3DMATERIAL9				m_tWhiteMtrl;
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
	static CBackMum*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//BackMum_h__