#ifndef ColorCircle_h__
#define ColorCircle_h__

#include "Engine_Include.h"
#include "GameObject.h"
#include "ColorPart.h"
#include "InsideCircle.h"
#include "Player.h"
BEGIN(Engine)

class CVIBuffer;
class CTexture;
class CTransform;
class CResourceMgr;
class CInfoSubject;

END
#include "PlayerTuto.h"

class CColorHP;
class CMouse;
class CCameraObserver;
class CPlayer;
class CSnow;
class CRain;
class COutline;

class CColorCircle : public Engine::CGameObject
{
private:
	explicit CColorCircle(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CColorCircle();

public:
	void Update(void);
	void LateUpdate(void);
	void Render(void);
	void Release(void);

private:
	HRESULT      Initialize(CGameObject* pPlayer);
	void      KeyInput(void);
	HRESULT      AddComponent(void);

private:
	void      CreateColorPart(void);

private:
	bool                  m_bIsReady = false;
	bool                  m_bIsSizeMax = false;
	COLORPARTID               m_eSelectID = COLORPARTID::PART_END;

public:
	CInsideCircle*            Get_InsideCircle() { return m_pInsideCircle; }
	CSnow*                  Get_Snow() { return m_pSnow; }

public:
	void SetIsReady(bool bReady) { m_bIsReady = bReady; }
	void SetIsSizeMax(bool bSize) { m_bIsSizeMax = bSize; }
	void SetPlayer(CGameObject* pPlayer) { m_pPlayer = dynamic_cast<CPlayer*>(pPlayer); }
	void SetPlayerTuto(CGameObject* pPlayerTuto) { m_pPlayerTuto = dynamic_cast<CPlayerTuto*>(pPlayerTuto); }

private:
	CCameraObserver*         m_pCameraObserver = nullptr;
	Engine::CResourceMgr*      m_pResourceMgr = nullptr;
	CPlayer*               m_pPlayer = nullptr;
	CPlayerTuto*            m_pPlayerTuto = nullptr;
	CSnow*                  m_pSnow = nullptr;
	CRain*                  m_pRain = nullptr;

	bool                  m_bForSound;
public:
	static CColorCircle* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pPlayer);

private:
	CColorPart*         m_pColorPartArr[COLORPARTID::PART_END];
	CInsideCircle*      m_pInsideCircle = nullptr;
	CColorHP*         m_pColorHP = nullptr;
private:
	/************************ (SongSong) ************************/
	/*  컬러서클 활성화 되있는지 체크  */
	Engine::CInfoSubject*         m_pInfoSubject = nullptr;
	bool                     m_bActive = false;
	Engine::CGameObject*         m_pHand = nullptr;

private:
	bool                     m_bCanSelectColor;

	COutline*                  m_pOutline;
private:
	Engine::CUBE_COLOR            m_eCurBufferColor;
private:
	CMouse*                     m_pMouse = nullptr;
};
#endif      //ColorCircle_h__