#include "Camera.h"

USING(Engine)

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_vEye(0.f, 0.f, 0.f)
	, m_vAt(0.f, 0.f, 0.f)
	, m_vUp(0.f, 1.f, 0.f)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

Engine::CCamera::~CCamera(void)
{

}

void Engine::CCamera::SetViewSpaceMatrix(const D3DXVECTOR3* pEye,
											const D3DXVECTOR3* pAt, 
											const D3DXVECTOR3* pUp)
{
	//CPipeline::MakeViewSpaceMatrix(&m_matView, pEye, pAt, pUp);
	D3DXMatrixLookAtLH(&m_matView, pEye, pAt, pUp);
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

void Engine::CCamera::SetProjectionMatrix(const float& fFovY,
											const float& fAspect,
											const float& fNear,
											const float& fFar)
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, fFovY, fAspect, fNear, fFar);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

