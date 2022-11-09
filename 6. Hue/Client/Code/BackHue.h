#ifndef BackHue_h__
#define BackHue_h__

#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;
END


class CBackColorChunk;
class CBackHue : public Engine::CGameObject
{
public:
	explicit CBackHue(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CBackHue(void);

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

	CBackColorChunk*			m_pBackColorChunk = nullptr;

public:
	virtual HRESULT		AddComponent(void);
public:
	static CBackHue*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//BackHue_h__