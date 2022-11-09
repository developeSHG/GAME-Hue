#include "GameObject.h"

USING(Engine)


CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pBufferName(L"")
	, m_pTexName(L"")
{
}

CGameObject::~CGameObject(void)
{
	Release();
}

HRESULT CGameObject::Initialize(void)
{
	return S_OK;
}

void CGameObject::Update(void)
{
	for (auto iter : m_mapComponent)
		iter.second->Update();
}

void CGameObject::Render(void)
{

}

void CGameObject::RenderInsideWorld(void)
{

}

void CGameObject::LateUpdate(void)
{
	for (auto iter : m_mapComponent)
		iter.second->LateUpdate();
}

CComponent* CGameObject::FindComponent(const wstring & wstrCompKey)
{
	auto& iter = m_mapComponent.find(wstrCompKey);
	if (iter == m_mapComponent.end())
		return nullptr;

	return iter->second;
}


void CGameObject::Release(void)
{
	for_each(m_mapComponent.begin(), m_mapComponent.end(), CDeleteMap());

	m_mapComponent.clear();
}
