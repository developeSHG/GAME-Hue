#ifndef CubeFireparent_h__
#define CubeFireparent_h__

#include "Engine_Include.h"
#include "GameObject.h"
#include "CameraObserver.h"
#include "Cube.h"

class CCameraObserver;
class CCubeFirecracker;
class CCubeFireparent : public Engine::CCube
{
public:
	explicit CCubeFireparent(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
public:
	virtual ~CCubeFireparent(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		RenderInsideWorld(void);

	int					UpdateFireparent(void);
public:
	void				SetPos(D3DXVECTOR3 vecPos);

	void				SetDir(D3DXVECTOR3 vecDir) { m_pInfoCom->m_vDir = vecDir; }
	void				SetIsKing();
	bool				GetIsKing() { return m_bIsKing; }
private:
	virtual void		Release(void);


private:
	void				SetTransform(void);
	float				m_fSpeed;
	bool				m_bIsKing;
private:
	CCameraObserver*	m_pCameraObserver = nullptr;
	list<CCubeFirecracker*>			m_listFirecracker;
	float				m_fDeadTime;
public:
	virtual HRESULT		AddComponent(void);
public:
	static CCubeFireparent*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubeFireparent_h__