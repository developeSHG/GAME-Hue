#ifndef Skulldust_h__
#define Skulldust_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "enum.h"
BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;
END

class CSkullDust : public Engine::CGameObject
{
public:
	explicit CSkullDust(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CSkullDust(void);


public:
	void				SetPosAndInfo(float fX, float fY, SKULLDUSTEFFECTID eID, bool IsLeft);

	void				SetMaterial(Engine::CUBE_COLOR eColor);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	int					DustUpdate();
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		Release(void);
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
	Engine::CTexture*			m_pMaskTextureCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTransform*			m_pInfoCom;

private:
	float						m_fvecAngle = 0.f;
	float						m_fRotAngle = 45.f;
	float						m_fSpeed = 0.f;
	SKULLDUSTEFFECTID			m_eSkullDustID;
	float						m_fDieTime = 0.f;
	int							m_iTexIndex;

	bool						m_bSwitch = false;
	bool						m_bIsLeft = false;
	
private:
	void						UpdateSkullDustOne();
	void						UpdateSkullDustTwo();
	void						UpdateSkullDustThree();
	void						UpdateSkullDustFour();
	void						UpdateSkullDustFive();
	void						UpdateSkullDustSix();
public:
	virtual HRESULT		AddComponent(void);
public:
	static CSkullDust*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//Skulldust_h__