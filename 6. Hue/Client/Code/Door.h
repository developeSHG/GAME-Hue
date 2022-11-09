#ifndef Door_h__
#define Door_h__

#include "Engine_Include.h"
#include "Export_Function.h"
#include "Include.h"
#include "ObjBasic.h"

BEGIN(Engine)

class CTexture;
class CVIBuffer;
class CTransform;
class CResourceMgr;
class CInfoSubject;
END

class CKeyEffect;
class CDoor : public CObjBasic
{
public:
	explicit CDoor(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo);
public:
	virtual ~CDoor(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual int			ObjectUpdate(void);
	virtual void		Render(void);
	virtual void		Release(void);
	virtual void		RenderInsideWorld(void);
	void				Set_Lock() { m_bIsLock = true; }
	virtual	void		ResetObject(void);

public:
	void				SetEndDoor() { m_bIsEndDoor = true; }
private:
	void				SetTransform(void);

private:
	Engine::CResourceMgr*		m_pResourceMgr;
	Engine::VTXTEX*				m_pVertex = nullptr;
	Engine::VTXTEX*				m_pConvertVtx = nullptr;
	Engine::CInfoSubject*		m_pInfoSubject = nullptr;
	CCameraObserver*			m_pCameraObserver = nullptr;
	DWORD						m_dwVtxCnt = 0;

	bool						m_bIsLock;

	bool						m_bIsEndDoor;
private:
	Engine::CTexture*			m_pLockTextureCom = nullptr;

public:
	void	SetDoorLock(bool bLock) { m_bIsLock = bLock; }
private:
	float						m_fMakeEffectTime;
private:


public:
	virtual HRESULT		AddComponent(void);

private:
	list<CKeyEffect*>	m_listKeyEffect;
public:
	static CDoor*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::OBJ_INFO& _tCubeInfo);
};
#endif		//Door_h__