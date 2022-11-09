#ifndef SavePoint_h__
#define SavePoint_h__

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

class CCameraObserver;
class CSavePoint : public CObjBasic
{
public:
	explicit CSavePoint(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo);
public:
	virtual ~CSavePoint(void);

public:
	void				SetDead(bool bDead) { m_bIsDead = bDead; }
	void				SetIsGetKey(bool _b) { m_bIsGetSavePoint = _b; }
	Engine::CTransform*	GetInfoCom() { return m_pInfoCom; }
	bool				GetIsGetKey() { return m_bIsGetSavePoint; }

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual int			ObjectUpdate(void);
	virtual void		Render(void);
	virtual void		Release(void);
	virtual void		RenderInsideWorld(void);
	virtual	void		ResetObject(void);
private:
	void				SetTransform(void);

private:
	Engine::CResourceMgr*		m_pResourceMgr;
	Engine::VTXTEX*				m_pVertex = nullptr;
	Engine::VTXTEX*				m_pConvertVtx = nullptr;
	DWORD						m_dwVtxCnt = 0;

private:
	float						m_fMakeEffectTime;
private:

	CCameraObserver*	m_pCameraObserver = nullptr;

	bool				m_bIsDead;
	bool				m_bIsGetSavePoint;

public:
	virtual HRESULT		AddComponent(void);

public:
	static CSavePoint*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo);
};
#endif		//SavePoint_h__