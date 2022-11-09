#ifndef Mouse_h__
#define Mouse_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "enum.h"
BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;
END

class CMouse : public Engine::CGameObject
{
public:
	explicit CMouse(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CMouse(void);

public:
	
private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	void				FirstPersonRender(void);
	virtual void		Release(void);

	void				Render_FirstMouse();

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
	static CMouse*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//Mouse_h__