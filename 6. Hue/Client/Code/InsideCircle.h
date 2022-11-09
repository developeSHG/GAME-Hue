#ifndef InsideCircle_h__
#define InsideCircle_h__

#include "Engine_Include.h"
#include "GameObject.h"
#include "Include.h"

BEGIN(Engine)

class CVIBuffer;
class CTexture;
class CTransform;
class CResourceMgr;
END

#include "PlayerTuto.h"

class CMouse;
class CSnow;
class CRain;
class CInsideCircle : public Engine::CGameObject
{
private:
	explicit CInsideCircle(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CInsideCircle();

public:
	virtual void Update(void);
	virtual void LateUpdate(void);
	void Render(void);
	void Release(void);

public:
	void BiggerUpdate(void);
	void SmallerUpdate(void);

public:
	float      Get_Scale(void) { return m_fScale; }

public:
	void      SetPlayer(CGameObject* pPlayer) { m_pPlayer = pPlayer; }
	void      SetHand(CGameObject* pHand) { m_pHand = pHand; }
	void      SetSnow(CGameObject* pSnow) { m_pSnow = pSnow; }
	void      SetRain(CGameObject* pRain) { m_pRain = pRain; }

private:
	HRESULT      Initialize();
	HRESULT      AddComponent(void);
	void       SetTransform(void);
	D3DMATERIAL9            m_tMtrl;
private:
	//Component
	Engine::CVIBuffer*         m_pBufferCom;
	Engine::CTexture*         m_pTextureCom;
	Engine::CTransform*         m_pInfoCom;

	//VTX(My Pipeline)
	Engine::VTXCOL*            m_pVertex = nullptr;
	Engine::VTXCOL*            m_pConvertVtx = nullptr;
	DWORD                  m_dwVtxCnt = 0;

	CGameObject*            m_pPlayer = nullptr;
	CGameObject*            m_pSnow = nullptr;
	CGameObject*            m_pRain = nullptr;
	CGameObject*            m_pHand = nullptr;
private:
	Engine::CResourceMgr*      m_pResourceMgr = nullptr;
private:
	float                  m_fScale = 10.f;
public:
	static CInsideCircle* Create(LPDIRECT3DDEVICE9 pGraphicDev);

};
#endif      //ColorPart_h__