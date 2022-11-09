#ifndef OrthoCamera_h__
#define OrthoCamera_h__

#include "Camera.h"

BEGIN(Engine)

class CTimeMgr;
class CTransform;

END

class COrthoCamera : public Engine::CCamera
{
private:
	explicit COrthoCamera(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~COrthoCamera(void);


public:
	virtual HRESULT		Initialize(void);
	virtual void		Update(void);

public:
	void				KeyInput(void);
	
public:
	const D3DXVECTOR3&		Get_CameraPos() const { return m_vecCameraPos; }

public:
	float							m_fDistance		= 10.f;
	float							m_fAngle		= D3DXToRadian(45.f);
	float							m_fSpeed = 20.f;

	D3DXVECTOR3						m_vecCameraPos;
	D3DXVECTOR3						m_vecLookAt;
	D3DXVECTOR3						m_vecUp;
public:
	static COrthoCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

};

#endif // OrthoCamera_h__
