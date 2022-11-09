#ifndef CubePlatform_h__
#define CubePlatform_h__

#include "Engine_Include.h"
#include "GameObject.h"

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
	virtual void		Render(void);
private:
	virtual void		Release(void);

private:
	void				SetTransform(void);

private:
	void				PosMoving(void);

private:
	D3DXVECTOR3			m_vOriginPos;
	D3DXVECTOR3			m_vDistPos;
	bool				m_bMovePatern = true;

public:
	virtual HRESULT		AddComponent(void);
public:
	static CCubePlatform*		Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CUBE_INFO& _tCubeInfo);
};
#endif		//CubePlatform_h__