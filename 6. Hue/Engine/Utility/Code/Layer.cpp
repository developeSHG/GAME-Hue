#include "Layer.h"

USING(Engine)

CLayer::CLayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
{
}

CLayer::~CLayer(void)
{
	Release();
}

HRESULT CLayer::AddObject(const wstring& wstrObjKey, CGameObject * pGameObject)
{
	if (pGameObject != nullptr)
	{
		auto iter = m_mapObjList.find(wstrObjKey);

		if (iter == m_mapObjList.end())
		{
			m_mapObjList[wstrObjKey] = list<CGameObject*>();		//이해아직안됨.
		}

		m_mapObjList[wstrObjKey].push_back(pGameObject);
	}


	return S_OK;
}

void CLayer::Update(void)
{
	for (auto mapiter : m_mapObjList)
	{
		for (auto iter : mapiter.second)
		{
			iter->Update();
		}
	}
}

void CLayer::LateUpdate(void)
{
	for (auto mapiter : m_mapObjList)
	{
		for (auto iter : mapiter.second)
		{
			iter->LateUpdate();
		}
	}
}

void CLayer::Render(void)
{
	for (auto mapiter : m_mapObjList)
	{
		for (auto iter : mapiter.second)
		{
			iter->Render();
		}
	}
}

CGameObject * CLayer::FindObject(const wstring & wstrObjKey)
{
	//현재 플레이어에 한해서만.
	auto& iter = m_mapObjList.find(wstrObjKey);
	if (iter == m_mapObjList.end())
		return nullptr;

	return iter->second.front();
}

void CLayer::Release(void)
{
	for (auto mapiter : m_mapObjList)
	{
		for (auto iter : mapiter.second)
		{
			Safe_Delete(iter);
		}
		mapiter.second.clear();
	}
	m_mapObjList.clear();
}

CLayer * CLayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return new CLayer(pGraphicDev);
}
