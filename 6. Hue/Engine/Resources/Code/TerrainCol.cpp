#include "TerrainCol.h"

USING(Engine)

Engine::CTerrainCol::CTerrainCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CTerrainCol::~CTerrainCol(void)
{
	Release();
}

HRESULT Engine::CTerrainCol::CreateBuffer(const WORD& wCntX,
										const WORD& wCntZ,
										const WORD& wItv)
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = wCntX * wCntZ;
	m_dwTriCnt = (wCntX - 1) * (wCntZ - 1) * 2;
	m_dwVtxFVF = VTXFVF_COL;

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTXCOL*		pVtxCol = nullptr;

	int iIndex = 0;

	m_pVB->Lock(0, 0, (void**)&pVtxCol, 0);

	for (int z = 0; z < wCntZ; ++z)
	{
		for (int x = 0; x < wCntX; ++x)
		{
			iIndex = z * wCntX + x;
			pVtxCol[iIndex].vPos = D3DXVECTOR3(float(x) * 1.7f, float(z) * 1.7f,
				0.f);
			pVtxCol[iIndex].dwColor= D3DCOLOR_ARGB(255, 255, 255, 255);
		}
	}

	m_pVB->Unlock();

	INDEX16*		pIndex = nullptr;
	int				iTriCnt = 0;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (int z = 0; z < wCntZ - 1; ++z)
	{
		for (int x = 0; x < wCntX - 1; ++x)
		{
			iIndex = z * wCntX + x;

			// ¿À¸¥ÂÊ À§ »ï°¢Çü
			pIndex[iTriCnt]._0 = iIndex + wCntX;
			pIndex[iTriCnt]._1 = iIndex + wCntX + 1;
			pIndex[iTriCnt]._2 = iIndex + 1;
			iTriCnt++;

			// ¿ÞÂÊ ¾Æ·¡ »ï°¢Çü
			pIndex[iTriCnt]._0 = iIndex + wCntX;
			pIndex[iTriCnt]._1 = iIndex + 1;
			pIndex[iTriCnt]._2 = iIndex;
			iTriCnt++;
		}
	}

	m_pIB->Unlock();


	return S_OK;
}

void Engine::CTerrainCol::Render(void)
{
	CVIBuffer::Render();
}

void Engine::CTerrainCol::Release(void)
{

}

CTerrainCol* Engine::CTerrainCol::Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const WORD& wCntX,
										const WORD& wCntZ,
										const WORD& wItv)
{
	CTerrainCol*		pInstance = new CTerrainCol(pGraphicDev);

	if (FAILED(pInstance->CreateBuffer(wCntX, wCntZ, wItv)))
		Safe_Delete(pInstance);

	return pInstance;
}
