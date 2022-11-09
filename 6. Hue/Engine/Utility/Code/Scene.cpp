#include "Scene.h"

USING(Engine)

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{

}

Engine::CScene::~CScene(void)
{
	Release();
}

HRESULT Engine::CScene::Initialize(void)
{
	return S_OK;
}

void Engine::CScene::Update(void)
{
	for (auto& iter : m_mapLayer)
		iter.second->Update();
}

void CScene::LateUpdate(void)
{
	for (auto& iter : m_mapLayer)
		iter.second->LateUpdate();
}

void Engine::CScene::Render(void)
{
	for (auto& iter : m_mapLayer)
		iter.second->Render();
}

void Engine::CScene::Release(void)
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();
}

