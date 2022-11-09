#ifndef Key_h__
#define Key_h__

#include "Engine_Include.h"
#include "Export_Function.h"
#include "Include.h"
#include "ObjBasic.h"

BEGIN(Engine)

class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;

END

class CKeyEffect;
class CKey : public CObjBasic
{
public:
	explicit CKey(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo);
public:
	virtual ~CKey(void);

public:
	void				SetDead(bool bDead) {m_bIsInteraction = false;}
	void				SetIsGetKey(bool _b) { m_bIsGetKey = _b; }
	Engine::CTransform*	GetInfoCom() { return m_pInfoCom; }
	bool				GetIsGetKey() { return m_bIsGetKey; }
	virtual	void		ResetObject(void);
private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual int			ObjectUpdate(void);
	virtual void		Render(void);
	virtual void		Release(void);
	virtual void		RenderInsideWorld(void);
private:
	void				SetTransform(void);

private:
	Engine::CResourceMgr*		m_pResourceMgr;
	Engine::VTXTEX*				m_pVertex = nullptr;
	Engine::VTXTEX*				m_pConvertVtx = nullptr;
	DWORD						m_dwVtxCnt = 0;

private:
	float						m_fMakeEffectTime;
	list<CKeyEffect*>			m_listKeyEffect;
private:

	CCameraObserver*	m_pCameraObserver = nullptr;

	bool				m_bIsDead;
	bool				m_bIsGetKey;
	float				m_fMakeSoundTime;
public:
	virtual HRESULT		AddComponent(void);

private:
public:
	static CKey*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo);
};
#endif		//Key_h__