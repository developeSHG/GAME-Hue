#include "Transform.h"

USING(Engine)

Engine::CTransform::CTransform(const D3DXVECTOR3& vLook)
	: m_vDir(vLook)
	, m_vPos(0.f, 0.f, 0.f)
	, m_vScale(0.f, 0.f, 0.f)
{
	ZeroMemory(m_fAngle, sizeof(float) * ANGLE_END);
	D3DXMatrixIdentity(&m_matWorld);
}

Engine::CTransform::~CTransform(void)
{

}

void Engine::CTransform::Update(void)
{

}

CTransform* Engine::CTransform::Create(const D3DXVECTOR3& vLook)
{
	return new CTransform(vLook);
}

