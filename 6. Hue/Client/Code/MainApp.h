#ifndef MainApp_h__
#define MainApp_h__

#include "Engine_Include.h"

BEGIN(Engine)

class  CGraphicDev;
class  CManagement;
class  CInfoSubject;
class  CResourceMgr;
class  CKeyMgr;
class  CTimeMgr;
class  CSoundMgr;
END
class CCollisionMgr;

class CCubeMgr;
//class CObjectMgr;
class CMainApp
{
	// 생성자
private:
	explicit CMainApp(void);
	// 소멸자
public:
	~CMainApp(void);

public:
	HRESULT		Initialize(void);
	void		Update(void);
	void		LateUpdate(void);
	void		Render(void);

	void		Setup_Camera(void);
	void		Setup_Proj(void);
	void		Setup_Light(void);
	// Public 함수
	// Public 변수

	// Protected 함수
	// Protected 변수

	// Private 함수
private:
	void		Release(void);
	// Private 변수

private:
	Engine::CGraphicDev*		m_pGraphicDev = nullptr;
	Engine::CManagement*		m_pManagement = nullptr;
	Engine::CResourceMgr*		m_pResourceMgr = nullptr;
	Engine::CKeyMgr*			m_pKeyMgr = nullptr;
	Engine::CTimeMgr*			m_pTimeMgr = nullptr;
	Engine::CInfoSubject*		m_pInfoSubject = nullptr;
	Engine::CSoundMgr*			m_pSoundMgr = nullptr;
	CCubeMgr*					m_pCubeMgr = nullptr;
	//CObjectMgr*				m_pObjectMgr = nullptr;
	CCollisionMgr*				m_pCollisionMgr = nullptr;
	
	LPDIRECT3DDEVICE9			m_pDevice = nullptr;


	// 생성 함수
public:
	static CMainApp*		Create(void);
	// 소멸 함수

};

#endif // MainApp_h__

