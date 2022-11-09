#include "stdafx.h"
#include "OrthoCamera.h"

#include "Export_Function.h"


COrthoCamera::COrthoCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev)
{
	m_vecCameraPos = D3DXVECTOR3(0.f, 0.f, -20.f);
	m_vecLookAt = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vecUp = D3DXVECTOR3(0.f, 1.f, 0.f);
}

COrthoCamera::~COrthoCamera(void)
{
}

HRESULT COrthoCamera::Initialize(void)
{
	return S_OK;
}

void COrthoCamera::Update(void)
{
	KeyInput();

	D3DXMATRIX v;
	D3DXMatrixLookAtLH(&v, &m_vecCameraPos, &m_vecLookAt, &m_vecUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &v);
}

void COrthoCamera::KeyInput(void)
{
	if (Engine::Get_KeyMgr()->KeyPressing(Engine::KEY_W))
	{
		m_vecCameraPos.y += 20.f * GET_TIME;
		m_vecLookAt.y += 20.f * GET_TIME;
	}
	if (Engine::Get_KeyMgr()->KeyPressing(Engine::KEY_S))
	{
		m_vecCameraPos.y += -20.f * GET_TIME;
		m_vecLookAt.y += -20.f * GET_TIME;
	}
	if (Engine::Get_KeyMgr()->KeyPressing(Engine::KEY_A))
	{
		m_vecCameraPos.x += -20.f * GET_TIME;
		m_vecLookAt.x += -20.f * GET_TIME;
	}
	if (Engine::Get_KeyMgr()->KeyPressing(Engine::KEY_D))
	{
		m_vecCameraPos.x += 20.f * GET_TIME;
		m_vecLookAt.x += 20.f * GET_TIME;
	}

	if (Engine::Get_KeyMgr()->KeyPressing(Engine::KEY_Q))
	{
		m_vecCameraPos.z += 30.f * GET_TIME;
	}
	if (Engine::Get_KeyMgr()->KeyPressing(Engine::KEY_E))
	{
		m_vecCameraPos.z += -30.f * GET_TIME;
	}
}

COrthoCamera * COrthoCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	COrthoCamera*	pInstance = new COrthoCamera(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Engine::Safe_Delete(pInstance);

	return pInstance;
}
