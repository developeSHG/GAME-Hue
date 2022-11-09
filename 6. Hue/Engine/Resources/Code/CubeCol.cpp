#include "CubeCol.h"

USING(Engine)

CCubeCol::CCubeCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CCubeCol::~CCubeCol(void)
{
	Release();
}

HRESULT CCubeCol::CreateBuffer()
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 8;
	m_dwTriCnt = 12;
	m_dwVtxFVF = VTXFVF_COL;

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);


	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTXCOL*		pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	pVtxTex[0].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[1].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[2].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[3].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);

	pVtxTex[4].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[5].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[6].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[7].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);


	for(int i = 0; i< (int)m_dwVtxCnt; ++i)
		pVtxTex[i].dwColor= D3DCOLOR_ARGB(255, 255, 255, 255);

	m_pVB->Unlock();

	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// 오른쪽 위
	pIndex[0]._0  = 0; pIndex[0]._1  = 1; pIndex[0]._2  = 2;
	pIndex[1]._0  = 0; pIndex[1]._1  = 2; pIndex[1]._2  = 3;
	pIndex[2]._0  = 4; pIndex[2]._1  = 6; pIndex[2]._2  = 5;
	pIndex[3]._0  = 4; pIndex[3]._1  = 7; pIndex[3]._2  = 6;
	pIndex[4]._0  = 0; pIndex[4]._1  = 3; pIndex[4]._2  = 7;
	pIndex[5]._0  = 0; pIndex[5]._1  = 7; pIndex[5]._2  = 4;
	pIndex[6]._0  = 1; pIndex[6]._1  = 5; pIndex[6]._2  = 6;
	pIndex[7]._0  = 1; pIndex[7]._1  = 6; pIndex[7]._2  = 2;
	pIndex[8]._0  = 3; pIndex[8]._1  = 2; pIndex[8]._2  = 6;
	pIndex[9]._0  = 3; pIndex[9]._1  = 6; pIndex[9]._2  = 7;
	pIndex[10]._0 = 0; pIndex[10]._1 = 4; pIndex[10]._2 = 5;
	pIndex[11]._0 = 0; pIndex[11]._1 = 5; pIndex[11]._2 = 1;

	m_pIB->Unlock();

	return S_OK;
}

void CCubeCol::Render(void)
{
	CVIBuffer::Render();
}

void CCubeCol::Release(void)
{
}

CCubeCol* CCubeCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCubeCol*		pInstance = new CCubeCol(pGraphicDev);

	if (FAILED(pInstance->CreateBuffer()))
		Safe_Delete(pInstance);

	return pInstance;
}
