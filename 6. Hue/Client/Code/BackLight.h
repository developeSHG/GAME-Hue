#ifndef BackLight_h__
#define BackLight_h__

#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;
END

class CBackLight : public Engine::CGameObject
{
public:
	explicit CBackLight(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CBackLight(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		Release(void);
	void				SetMaterialColor(float fR, float fG, float fB);
private:

private:
	void				SetTransform(void);
	void				SetMaterial(int fR, int fG, int fB);
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

	float						m_fColorR = 0.f;
	float						m_fColorG = 0.f;
	float						m_fColorB = 0.f;
public:
	virtual HRESULT		AddComponent(void);
public:
	static CBackLight*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//BackLight_h__