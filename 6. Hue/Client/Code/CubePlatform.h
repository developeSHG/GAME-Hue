#ifndef CubePlatform_h__
#define CubePlatform_h__

#include "Engine_Include.h"
#include "GameObject.h"
#include "CameraObserver.h"
#include "Cube.h"

class CCubePlatform : public Engine::CCube
{
public:
	explicit CCubePlatform(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
public:
	virtual ~CCubePlatform(void);

private:
	virtual HRESULT		Initialize(void);
public:
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Render(void);
	virtual void		RenderInsideWorld(void);
private:
	virtual void		Release(void);

private:
	void				SetTransform(void);

private:
	void				PosMoving(void);
	// Ãß°¡µÊ
	void				StateChange();
public:
	// Ãß°¡µÊ
	bool					Get_IsStop() { return m_bStop; }
	Engine::CUBE_MOVING_DIR Get_State() { return m_eCurState; }
private:
	CCameraObserver*	m_pCameraObserver = nullptr;

private:
	D3DXVECTOR3			m_vOriginPos;
	D3DXVECTOR3			m_vDistPos;
	bool				m_bStop = true;
	// Ãß°¡µÊ
	float				m_fChangeTime = 0.0f;
	Engine::CUBE_MOVING_DIR	m_eNextState;
	Engine::CUBE_MOVING_DIR	m_eCurState;
public:
	virtual HRESULT		AddComponent(void);
public:
	static CCubePlatform*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubePlatform_h__

