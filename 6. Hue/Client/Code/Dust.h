#ifndef Dust_h__
#define Dust_h__

#include "Engine_Include.h"
#include "GameObject.h"

#include "enum.h"
BEGIN(Engine)
class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;
END

class CDust : public Engine::CGameObject
{
public:
	explicit CDust(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CDust(void);


public:
	void				SetPosAndInfo(float fX, float fY, DUSTEFFECTID eID, bool IsLeft);

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
	Engine::CResourceMgr*		m_pResourceMgr;
	Engine::VTXTEX*				m_pVertex = nullptr;
	Engine::VTXTEX*				m_pConvertVtx = nullptr;
	DWORD						m_dwVtxCnt = 0;

	Engine::CTexture*			m_pTextureCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTransform*			m_pInfoCom;

private:
	float						m_fvecAngle = 0.f;
	float						m_fRotAngle = 45.f;
	float						m_fSpeed = 0.f;
	DUSTEFFECTID				m_eDustID;
	float						m_fDieTime = 0.f;

	bool						m_bIsLeft = false;
	
private:
	void						UpdateDustOne();
	void						UpdateDustTwo();
	void						UpdateDustThree();
	void						UpdateDustFour();
	void						UpdateDustFive();
	void						UpdateDustSix();
public:
	virtual HRESULT		AddComponent(void);
public:
	static CDust*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};
#endif		//Dust_h__