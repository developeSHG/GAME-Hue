#include "TerrainTex.h"
#include <fstream>
USING(Engine)

Engine::CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CTerrainTex::~CTerrainTex(void)
{
	Release();
	
}

HRESULT Engine::CTerrainTex::CreateBuffer(const WORD& wCntX,
										const WORD& wCntZ,
										const WORD& wItv)
{
	std::vector<int> heightmap;
	TCHAR* heightFilePath = L"../Bin/Resources/Texture/Terrain/Height.bmp";
	
	std::vector<BYTE> in(wCntX * wCntZ * 4 + 54);			//32비트 비트맵이므로.
	std::ifstream inFile(heightFilePath, std::ios_base::binary);
	inFile.read((char*)&in[0], in.size());
	inFile.close();

	heightmap.resize(wCntX * wCntZ * 4 + 54);
	for (int i = 0; i < wCntX * wCntZ * 4 + 54; ++i)
		heightmap[i] = in[i];
		
	in.clear();
	in.shrink_to_fit();

	// [54] [58] [62]


	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = wCntX * wCntZ;
	m_dwTriCnt = (wCntX - 1) * (wCntZ - 1) * 2;
	m_dwVtxFVF = VTXFVF_TEX;
	m_vecPlane.resize(m_dwTriCnt);

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTXTEX*		pVtxTex = nullptr;

	int iIndex = 0;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	for (int z = 0; z < wCntZ; ++z)
	{
		for (int x = 0; x < wCntX; ++x)
		{
			iIndex = z * wCntX + x;
			pVtxTex[iIndex].vPos = D3DXVECTOR3(float(x) * wItv, heightmap[(z * wCntX + x) * 4 + 55] / 100.f, 
				float(z) * wItv);
			pVtxTex[iIndex].vTex = D3DXVECTOR2(x / (wCntX - 1.f), z / (wCntZ - 1.f));
		}
	}

	m_pVB->Unlock();

	INDEX16*		pIndex = nullptr;
	int				iTriCnt = 0;
	int				iPlaneIndex = 0;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (int z = 0; z < wCntZ - 1; ++z)
	{
		for (int x = 0; x < wCntX - 1; ++x)
		{
			iIndex = z * wCntX + x;

			// 오른쪽 위 삼각형
			pIndex[iTriCnt]._0 = iIndex + wCntX;
			pIndex[iTriCnt]._1 = iIndex;
			pIndex[iTriCnt]._2 = iIndex + 1;
			D3DXPlaneFromPoints(&m_vecPlane[iPlaneIndex], &pVtxTex[pIndex[iTriCnt]._0].vPos, &pVtxTex[pIndex[iTriCnt]._1].vPos, &pVtxTex[pIndex[iTriCnt]._2].vPos);
			iTriCnt++;
			iPlaneIndex++;

			// 왼쪽 아래 삼각형
			pIndex[iTriCnt]._0 = iIndex + 1;
			pIndex[iTriCnt]._1 = iIndex + 1 + wCntX;
			pIndex[iTriCnt]._2 = iIndex + wCntX;
			D3DXPlaneFromPoints(&m_vecPlane[iPlaneIndex], &pVtxTex[pIndex[iTriCnt]._0].vPos, &pVtxTex[pIndex[iTriCnt]._1].vPos, &pVtxTex[pIndex[iTriCnt]._2].vPos);
			iTriCnt++;
			iPlaneIndex++;
		}
	}

	m_pIB->Unlock();


	return S_OK;
}

void Engine::CTerrainTex::Render(void)
{
	CVIBuffer::Render();
}

void Engine::CTerrainTex::Release(void)
{
}

CTerrainTex* Engine::CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const WORD& wCntX, 
										const WORD& wCntZ,
										const WORD& wItv)	
{
	CTerrainTex*		pInstance = new CTerrainTex(pGraphicDev);

	if (FAILED(pInstance->CreateBuffer(wCntX, wCntZ, wItv)))
		Safe_Delete(pInstance);

	return pInstance;
}

