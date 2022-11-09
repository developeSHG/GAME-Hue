#include "Resources.h"

USING(Engine)

CResources::CResources(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pwRefCnt(new WORD(0))
{

}

CResources::CResources(const CResources& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_pwRefCnt(rhs.m_pwRefCnt)
{
	m_pGraphicDev->AddRef();
}

CResources::~CResources(void)
{
	//Release();
}

void CResources::Update(void)
{
}

void CResources::Release(void)
{
	if (0 == (*m_pwRefCnt))
	{
		Safe_Delete(m_pwRefCnt);
	}
	else
		(*m_pwRefCnt)--;
}

