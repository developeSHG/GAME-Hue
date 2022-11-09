#include "CircleCol.h"

USING(Engine)

CCircleCol::CCircleCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CCircleCol::~CCircleCol(void)
{
	Release();
}

HRESULT CCircleCol::CreateBuffer()
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 42;
	m_dwTriCnt = 40;
	m_dwVtxFVF = VTXFVF_COL;

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);


	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTXCOL*		pVtxTex = nullptr;
	
	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	pVtxTex[0].vPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	pVtxTex[1].vPos = D3DXVECTOR3(1.f, 0.f, 0.f);
	pVtxTex[2].vPos = D3DXVECTOR3(0.988f, 0.156f, 0.f);
	pVtxTex[3].vPos = D3DXVECTOR3(0.951f, 0.309f, 0.f);
	pVtxTex[4].vPos = D3DXVECTOR3(0.891f, 0.454f, 0.f);
	pVtxTex[5].vPos = D3DXVECTOR3(0.809f, 0.588f, 0.f);
	pVtxTex[6].vPos = D3DXVECTOR3(0.707f, 0.707f, 0.f);
	pVtxTex[7].vPos = D3DXVECTOR3(0.588f, 0.809f, 0.f);
	pVtxTex[8].vPos = D3DXVECTOR3(0.454f, 0.891f, 0.f);
	pVtxTex[9].vPos = D3DXVECTOR3(0.309f, 0.951f, 0.f);
	pVtxTex[10].vPos = D3DXVECTOR3(0.156f, 0.988f, 0.f);


	pVtxTex[11].vPos = D3DXVECTOR3(0.f, 1.f, 0.f);
	pVtxTex[12].vPos = D3DXVECTOR3(-0.156f, 0.988f, 0.f);
	pVtxTex[13].vPos = D3DXVECTOR3(-0.309f, 0.951f, 0.f);
	pVtxTex[14].vPos = D3DXVECTOR3(-0.454f, 0.891f, 0.f);
	pVtxTex[15].vPos = D3DXVECTOR3(-0.588f, 0.809f, 0.f);
	pVtxTex[16].vPos = D3DXVECTOR3(-0.707f, 0.707f, 0.f);
	pVtxTex[17].vPos = D3DXVECTOR3(-0.809f, 0.588f, 0.f);
	pVtxTex[18].vPos = D3DXVECTOR3(-0.891f, 0.454f, 0.f);
	pVtxTex[19].vPos = D3DXVECTOR3(-0.951f, 0.309f, 0.f);
	pVtxTex[20].vPos = D3DXVECTOR3(-0.988f, 0.156f, 0.f);

	pVtxTex[21].vPos = D3DXVECTOR3(-1.f, 0.f, 0.f);
	pVtxTex[22].vPos = D3DXVECTOR3(-0.988f, -0.156f, 0.f);
	pVtxTex[23].vPos = D3DXVECTOR3(-0.951f, -0.309f, 0.f);
	pVtxTex[24].vPos = D3DXVECTOR3(-0.891f, -0.454f, 0.f);
	pVtxTex[25].vPos = D3DXVECTOR3(-0.809f, -0.588f, 0.f);
	pVtxTex[26].vPos = D3DXVECTOR3(-0.707f, -0.707f, 0.f);
	pVtxTex[27].vPos = D3DXVECTOR3(-0.588f, -0.809f, 0.f);
	pVtxTex[28].vPos = D3DXVECTOR3(-0.454f, -0.891f, 0.f);
	pVtxTex[29].vPos = D3DXVECTOR3(-0.309f, -0.951f, 0.f);
	pVtxTex[30].vPos = D3DXVECTOR3(-0.156f, -0.988f, 0.f);

	pVtxTex[31].vPos = D3DXVECTOR3(0.f, -1.f, 0.f);
	pVtxTex[32].vPos = D3DXVECTOR3(0.156f, -0.988f, 0.f);
	pVtxTex[33].vPos = D3DXVECTOR3(0.309f, -0.951f, 0.f);
	pVtxTex[34].vPos = D3DXVECTOR3(0.454f, -0.891f, 0.f);
	pVtxTex[35].vPos = D3DXVECTOR3(0.588f, -0.809f, 0.f);
	pVtxTex[36].vPos = D3DXVECTOR3(0.707f, -0.707f, 0.f);
	pVtxTex[37].vPos = D3DXVECTOR3(0.809f, -0.588f, 0.f);
	pVtxTex[38].vPos = D3DXVECTOR3(0.891f, -0.454f, 0.f);
	pVtxTex[39].vPos = D3DXVECTOR3(0.951f, -0.309f, 0.f);
	pVtxTex[40].vPos = D3DXVECTOR3(0.988f, -0.156f, 0.f);

	pVtxTex[41].vPos = D3DXVECTOR3(1.f, 0.f, 0.f);

	for (int i = 0; i < 42; ++i)
		pVtxTex[i].dwColor = D3DCOLOR_ARGB(255, 35, 35, 35);

	m_pVB->Unlock();

	return S_OK;
}

void CCircleCol::Render(void)
{
	CVIBuffer::Render_TriFans();
}

void CCircleCol::Release(void)
{
}

CCircleCol* CCircleCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCircleCol*		pInstance = new CCircleCol(pGraphicDev);

	if (FAILED(pInstance->CreateBuffer()))
		Safe_Delete(pInstance);

	return pInstance;
}
