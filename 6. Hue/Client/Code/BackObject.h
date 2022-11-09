#ifndef BackObject_h__
#define BackObject_h__

#include "Engine_Include.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;
END

class CBackSmoke;
class CBackDrGrey;
class CBackMum;
class CBackSeaHorse;
class CBackWolf;
class CBackBook;
class CBackHue;
class CBackKeyBoard;
class CBackGameLogo;
class CBackObject : public Engine::CGameObject
{
public:
	explicit CBackObject(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CBackObject(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		Release(void);

public:
	void SetMaterialColor(float fR, float fG, float fB);
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

private:
	CBackSmoke*					m_pBackSmoke = nullptr;
	CBackDrGrey*				m_pBackDrGrey = nullptr;
	CBackMum*					m_pBackMum = nullptr;
	CBackSeaHorse*				m_pBackSeaHorse = nullptr;
	CBackWolf*					m_pBackWolf = nullptr;
	CBackBook*					m_pBackBook = nullptr;
	CBackHue*					m_pBackHue = nullptr;
	CBackKeyBoard*				m_pBackKeyBoard = nullptr;
	CBackGameLogo*				m_pBackGameLogo = nullptr;


private:
	float			m_fColorR = 0.f;
	float			m_fColorG = 0.f;
	float			m_fColorB = 0.f;
public:
	virtual HRESULT		AddComponent(void);
public:
	static CBackObject*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//BackObject_h__