#include "Renderer.h"
#include "FrameMgr.h"
USING(Engine)

Engine::CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_tBackColor = D3DXCOLOR(D3DCOLOR_XRGB(0, 0, 0));
}

Engine::CRenderer::~CRenderer(void)
{
}

void CRenderer::SetScene(CScene * pScene)
{
	m_pScene = pScene;
}

HRESULT Engine::CRenderer::Initialize(void)
{

	return S_OK;
}

void Engine::CRenderer::Render(void)
{
	m_pGraphicDev->Clear(0, NULL,
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		m_tBackColor,
		1.f, // Z버퍼의 초기화 값
		0);

	m_pGraphicDev->BeginScene();

	if (nullptr != m_pScene)
		m_pScene->Render();

	CFrameMgr::GetInstance()->RenderFPS();

	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(NULL, NULL, NULL, NULL);
}

CRenderer * CRenderer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRenderer*	pInstance = new CRenderer(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Safe_Delete(pInstance);

	return pInstance;
}

