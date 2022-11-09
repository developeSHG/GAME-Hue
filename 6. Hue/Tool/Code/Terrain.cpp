#include "stdafx.h"
#include "Terrain.h"

#include "Export_Function.h"

USING(Engine)

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CTerrain::~CTerrain()
{
	Release();
}

HRESULT CTerrain::Initialize(int vtxX, int vtxZ, int vtxItv)
{
	FAILED_CHECK_RETURN(Engine::CResourceMgr::GetInstance()->Add_Buffer(m_pGraphicDev,
		Engine::RESOURCE_STATIC,
		Engine::BUFFER_TERRAINCOL,
		L"Terrain", vtxX, vtxZ, vtxItv), E_FAIL);

	return S_OK;
}

void CTerrain::Update(void)
{
	
}

void CTerrain::Render(void)
{
	Engine::CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity(&mWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mWorld);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//Engine::CResourceMgr::GetInstance()->SetTexture(Engine::RESOURCE_STATIC, L"Texture_CubeNormal", 0);
	//Engine::CResourceMgr::GetInstance()->SetTexture(Engine::RESOURCE_STATIC, L"Texture_Terrain", 0);
	Engine::CResourceMgr::GetInstance()->Render(L"Terrain");
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	Engine::CGraphicDev::GetInstance()->GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
}

void CTerrain::Release(void)
{
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev, int vtxX, int vtxZ, int vtxItv)
{
	CTerrain*	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Initialize(vtxX, vtxZ, vtxItv)))
		Engine::Safe_Delete(pInstance);

	return pInstance;
}
