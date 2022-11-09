#include "Management.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

Engine::CManagement::CManagement(void)
{

}

Engine::CManagement::~CManagement(void)
{
	Release();
}

HRESULT Engine::CManagement::Initialize(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;

	m_pRenderer = CRenderer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN_MSG(m_pRenderer, E_FAIL, L"Renderer Create False");


	return S_OK;
}

void Engine::CManagement::Update(void)
{
	if (nullptr != m_pScene)
		m_pScene->Update();
}

void CManagement::LateUpdate(void)
{
	if (nullptr != m_pScene)
		m_pScene->LateUpdate();
}

void Engine::CManagement::Render(void)
{
	if (nullptr != m_pRenderer)
		m_pRenderer->Render();
}

void Engine::CManagement::Release(void)
{
	Safe_Delete(m_pScene);
	Safe_Delete(m_pRenderer);
}

