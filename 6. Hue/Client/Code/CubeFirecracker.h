#ifndef Cubecracker_h__
#define Cubecracker_h__

#include "Engine_Include.h"
#include "GameObject.h"
#include "CameraObserver.h"
#include "Cube.h"

class CCameraObserver;

class CCubeFirecracker : public Engine::CCube
{
public:
	explicit CCubeFirecracker(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
public:
	virtual ~CCubeFirecracker(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		RenderInsideWorld(void);

	bool				GetIsKing() { return m_bIsKing; }
	int					UpdateFirework(void);
	void				SetIsKing() { m_bIsKing = true;  m_pInfoCom->m_vScale = D3DXVECTOR3(0.4f, 0.4f, 0.4f);	}
public:
	void				SetPos(D3DXVECTOR3 vecPos);

	void				SetDir(D3DXVECTOR3 vecDir) { m_pInfoCom->m_vDir = vecDir; }
private:
	virtual void		Release(void);

	bool				m_bIsKing;
private:
	void				SetTransform(void);

private:
	CCameraObserver*	m_pCameraObserver = nullptr;

public:
	virtual HRESULT		AddComponent(void);
public:
	static CCubeFirecracker*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//Cubecracker_h__