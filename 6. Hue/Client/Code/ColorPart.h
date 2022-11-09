#ifndef ColorPart_h__
#define ColorPart_h__

#include "Engine_Include.h"
#include "GameObject.h"
#include "Include.h"

BEGIN(Engine)

class CVIBuffer;
class CTexture;
class CTransform;
class CResourceMgr;
END

class CColorCircle;
class CCameraObserver;


class CColorPart : public Engine::CGameObject
{
private:
	explicit CColorPart(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CColorPart();

public:
	virtual void Update(void);
	virtual void LateUpdate(void);
	void Render(void);
	void Release(void);

public:
	void BiggerUpdate(void);
	void SmallerUpdate(void);
	void SelectColorPart(bool bSelect) { m_bIsSelect = bSelect; }
	void InitFirstPos(void);

	D3DCOLOR GetColor() { return m_tColor; }
private:
	HRESULT		Initialize(COLORPARTID eID);
	HRESULT		AddComponent(void);
	void	    SetTransform(void);


private:
	//Component
	Engine::CTexture*			m_pTextureCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTransform*			m_pInfoCom;

	//VTX(My Pipeline)
	Engine::VTXTEX*				m_pVertex = nullptr;
	Engine::VTXTEX*				m_pConvertVtx = nullptr;
	DWORD						m_dwVtxCnt = 0;

	//ÀçÁú
	D3DMATERIAL9				m_tMtrl;
	D3DCOLOR					m_tColor;
private:
	CCameraObserver*			m_pCameraObserver = nullptr;
	Engine::CResourceMgr*		m_pResourceMgr = nullptr;
private:
	COLORPARTID					m_ePartID;
	float						m_fScale = 10.f;
	CColorCircle*				m_pParent = nullptr;
	bool						m_bIsSelect = false;

public:
	void SetParent(CColorCircle* pCircle) { m_pParent = pCircle; }
public:
	static CColorPart* Create(LPDIRECT3DDEVICE9 pGraphicDev, COLORPARTID eID);

};
#endif		//ColorPart_h__