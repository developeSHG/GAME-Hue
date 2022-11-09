#ifndef Hand_h__
#define Hand_h__

#include "Engine_Include.h"
#include "GameObject.h"
#include "Include.h"

BEGIN(Engine)

class CVIBuffer;
class CTexture;
class CTransform;
class CResourceMgr;

END

class CHand : public Engine::CGameObject
{
public:
	enum HAND_STATE
	{
		IDLE, LEFT, RIGHT, STATE_END
	};

private:
	explicit CHand(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CHand();

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		Release(void);

private:
	void				SetTransform(void);

private:
	void				Animation();
	void				StateCheck();

private:
	//Component
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTexture*			m_pTextureCom[HAND_STATE::STATE_END];
	Engine::CTransform*			m_pInfoCom;
private:
	//VTX(My Pipeline)
	Engine::VTXTEX*				m_pVertex = nullptr;
	Engine::VTXTEX*				m_pConvertVtx = nullptr;
	DWORD						m_dwVtxCnt = 0;

private:
	Engine::CResourceMgr*		m_pResourceMgr = nullptr;
	CCameraObserver*			m_pCameraObserver = nullptr;
	CCubeMgr*					m_pCubeMgr = nullptr;

private:
	float						m_fOriginY;
	int							m_iSelectColorActive = 0;

private:
	HAND_STATE					m_eCurState = HAND_STATE::STATE_END;
	HAND_STATE					m_eNextState = HAND_STATE::IDLE;
	FRAME_INFO					m_tFrame;


private:
	HRESULT		AddComponent(void);
public:
	static CHand* Create(LPDIRECT3DDEVICE9 pGraphicDev);

};
#endif		//Hand_h__