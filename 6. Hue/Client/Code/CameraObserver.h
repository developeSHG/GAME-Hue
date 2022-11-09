#ifndef CameraObserver_h__
#define CameraObserver_h__

#include "Observer.h"
#include "Include.h"

namespace Engine
{
	class CInfoSubject;
	class CGameObject;
}

class CPlayer;
class CPlayerTuto;
class CStaticCamera;
class CTutoCamera;
class CDoor;
class CColorCircle;
class CKey;
class COutline;

class CCameraObserver : public Engine::CObserver
{
	DECLARE_SINGLETON(CCameraObserver)

private:
	CCameraObserver(void);
public:
	virtual ~CCameraObserver(void);

public:
	const D3DXMATRIX*		GetView(void) { return &m_matView; }
	const D3DXMATRIX*		GetProj(void) { return &m_matProj; }
	const bool*				GetNarrativeMoving(void) { return &m_bNarrativeMoveing; }
	const NARRATIVE_VIEW*	GetNarrativeView(void) { return &m_eNarrativeView; }
	CPlayer*				GetPlayer(void) { return m_pPlayer; }
	CStaticCamera*			GetStaticCamera(void) { return m_pStaticCamera; }
	bool*					GetColorCircleActive(void) { return &m_bColorCircleActive; }
	CDoor*					GetDoor(void) { return m_pDoor; }
	SCENEID*				GetSceneID(void) { return &m_eSceneID; }
	CColorCircle*			GetColorCircle(void) { return m_pColorCircle; }
	CKey*					GetKey(void) { return m_pKey; }
	COutline*				GetOutlint(void) { return m_pOutline; }
	CPlayerTuto*			GetPlayerTuto(void) { return m_pPlayerTuto; }
	D3DXVECTOR3*			GetPlayerTutoPos(void) { return &m_vPlayerTutoPos; }
	CTutoCamera*			GetTutoCamera(void) { return m_pTutoCamera; }
	list<Engine::CGameObject*>*		GetlistTutoObject(void) { return &m_listTutoObject; }

public:
	virtual void Update(int iMessage);

private:
	Engine::CInfoSubject*			m_pInfoSubject = nullptr;
	D3DXMATRIX						m_matView;
	D3DXMATRIX						m_matProj;
	bool							m_bNarrativeMoveing;
	NARRATIVE_VIEW					m_eNarrativeView;
	CPlayer*						m_pPlayer;
	CStaticCamera*					m_pStaticCamera;
	bool							m_bColorCircleActive;
	CDoor*							m_pDoor;
	SCENEID							m_eSceneID;
	CColorCircle*					m_pColorCircle;
	CKey*							m_pKey;
	COutline*						m_pOutline;
	CPlayerTuto*					m_pPlayerTuto;
	D3DXVECTOR3						m_vPlayerTutoPos;
	CTutoCamera*					m_pTutoCamera = nullptr;
	list<Engine::CGameObject*>		m_listTutoObject;
};



#endif // CameraObserver_h__
