#include "RcyTex.h"

USING(Engine)

Engine::CRcyTex::CRcyTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CRcyTex::~CRcyTex(void)
{
	Release();
}

HRESULT Engine::CRcyTex::CreateBuffer(void)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 4;
	m_dwTriCnt = 2;
	m_dwVtxFVF = VTXFVF_TEX;

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);


	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTXTEX*		pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	pVtxTex[0].vPos = D3DXVECTOR3(-1.f, 0.516f, 0.f);
	pVtxTex[0].vTex = D3DXVECTOR2(0.f, 0.f);
	
	pVtxTex[1].vPos = D3DXVECTOR3(1.f, 0.516f, 0.f);
	pVtxTex[1].vTex = D3DXVECTOR2(1.f, 0.f);

	pVtxTex[2].vPos = D3DXVECTOR3(1.f, -0.516f, 0.f);
	pVtxTex[2].vTex = D3DXVECTOR2(1.f, 1.f);

	pVtxTex[3].vPos = D3DXVECTOR3(-1.f, -0.516f, 0.f);
	pVtxTex[3].vTex = D3DXVECTOR2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// 오른쪽 위
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	// 왼쪽 아래
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;
	
	m_pIB->Unlock();
	
	return S_OK;
}

void Engine::CRcyTex::Render(void)
{
	CVIBuffer::Render();
}

void Engine::CRcyTex::Release(void)
{

}

CRcyTex* Engine::CRcyTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcyTex*		pInstance = new CRcyTex(pGraphicDev);

	if (FAILED(pInstance->CreateBuffer()))
		Safe_Delete(pInstance);

	return pInstance;
}
