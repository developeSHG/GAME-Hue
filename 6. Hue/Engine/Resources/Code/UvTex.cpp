#include "UvTex.h"
#include "Export_Function.h"
USING(Engine)

Engine::CUvTex::CUvTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

CUvTex::CUvTex(const CUvTex & rhs)
	: CVIBuffer(rhs)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

Engine::CUvTex::~CUvTex(void)
{
	Release();
}

HRESULT Engine::CUvTex::CreateBuffer(void)
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

	pVtxTex[0].vPos = D3DXVECTOR3(-1.f, 1.f, 0.f);
	pVtxTex[0].vTex = D3DXVECTOR2(0.f, 0.f);

	pVtxTex[1].vPos = D3DXVECTOR3(1.f, 1.f, 0.f);
	pVtxTex[1].vTex = D3DXVECTOR2(1.f, 0.f);

	pVtxTex[2].vPos = D3DXVECTOR3(1.f, -1.f, 0.f);
	pVtxTex[2].vTex = D3DXVECTOR2(1.f, 1.f);

	pVtxTex[3].vPos = D3DXVECTOR3(-1.f, -1.f, 0.f);
	pVtxTex[3].vTex = D3DXVECTOR2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// ������ ��
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	// ���� �Ʒ�
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	m_fUvTex = 0.f;
	return S_OK;
}

void Engine::CUvTex::Render(void)
{
	CVIBuffer::Render();
}

void Engine::CUvTex::Release(void)
{

}

void CUvTex::Update(void)
{
	m_fUvTex += 1.15f * Engine::Get_TimeMgr()->GetTime();
	VTXTEX*		pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	pVtxTex[0].vPos = D3DXVECTOR3(-1.f, 1.f, 0.f);
	pVtxTex[0].vTex = D3DXVECTOR2(0.f + m_fUvTex, 0.f);

	pVtxTex[1].vPos = D3DXVECTOR3(1.f, 1.f, 0.f);
	pVtxTex[1].vTex = D3DXVECTOR2(1.f + m_fUvTex, 0.f);

	pVtxTex[2].vPos = D3DXVECTOR3(1.f, -1.f, 0.f);
	pVtxTex[2].vTex = D3DXVECTOR2(1.f + m_fUvTex, 1.f);

	pVtxTex[3].vPos = D3DXVECTOR3(-1.f, -1.f, 0.f);
	pVtxTex[3].vTex = D3DXVECTOR2(0.f + m_fUvTex, 1.f);

	m_pVB->Unlock();
}

CResources * CUvTex::CloneUV(void)
{
	(*m_pwRefCnt)++;

	return new CUvTex(*this);
}

CUvTex* Engine::CUvTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUvTex*		pInstance = new CUvTex(pGraphicDev);

	if (FAILED(pInstance->CreateBuffer()))
		Safe_Delete(pInstance);

	return pInstance;
}
