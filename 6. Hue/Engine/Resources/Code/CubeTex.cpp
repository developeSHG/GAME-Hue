#include "CubeTex.h"

USING(Engine)

CCubeTex::CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
	, m_eCubeType(CUBE_CLIPPING_TYPE::CUBE_TYPE_END)
{
}

CCubeTex::~CCubeTex(void)
{
	Release();
}

HRESULT CCubeTex::CreateBuffer(CUBE_CLIPPING_TYPE eType)
{
	m_eCubeType = eType;

	switch (m_eCubeType)
	{
	case Engine::CUBE_NORMAL:
		CreateCubeNormal();
		break;
	case Engine::CUBE_FRONT:
		CreateCubeFront();
		break;
	case Engine::CUBE_CullB:
		CreateCubeCullB();
		break;
	case Engine::CUBE_CullL:
		CreateCubeCullL();
		break;
	case Engine::CUBE_CullR:
		CreateCubeCullR();
		break;
	case Engine::CUBE_CullLR:
		CreateCubeCullLR();
		break;
	case Engine::CUBE_CullBackAndB:
		CreateCubeCullBackAndB();
		break;
	case Engine::CUBE_Player:
		CreateCubePlayer();
		break;
	case Engine::CUBE_SkyBox:
		CreateCubeSkyBox();
		break;
	}

	return S_OK;
}

void CCubeTex::Render(void)
{
	CVIBuffer::Render_NoIndices();
}

void CCubeTex::Release(void)
{
}

HRESULT CCubeTex::CreateCubeNormal()
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 36;
	m_dwTriCnt = 12;
	m_dwVtxFVF = VTXFVF_TEX;

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTXTEX*      pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	// 앞
	pVtxTex[0].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[0].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[1].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[1].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[2].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[2].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[3].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[3].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[4].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[4].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[5].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[5].vTex = D3DXVECTOR2(1.f, 1.f);

	// 우
	pVtxTex[6].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[6].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[7].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[7].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[8].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[8].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[9].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[9].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[10].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[10].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[11].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[11].vTex = D3DXVECTOR2(1.f, 1.f);

	// 뒤 (반시계방향)
	pVtxTex[12].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[12].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[13].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[13].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[14].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[14].vTex = D3DXVECTOR2(1.f, 1.f);
	pVtxTex[15].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[15].vTex = D3DXVECTOR2(1.f, 1.f);
	pVtxTex[16].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[16].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[17].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[17].vTex = D3DXVECTOR2(0.f, 1.f);

	// 좌
	pVtxTex[18].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[18].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[19].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[19].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[20].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[20].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[21].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[21].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[22].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[22].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[23].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[23].vTex = D3DXVECTOR2(1.f, 1.f);

	// 상
	pVtxTex[24].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[24].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[25].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[25].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[26].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[26].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[27].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[27].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[28].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[28].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[29].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[29].vTex = D3DXVECTOR2(1.f, 1.f);

	// 하
	pVtxTex[30].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[30].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[31].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[31].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[32].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[32].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[33].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[33].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[34].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[34].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[35].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[35].vTex = D3DXVECTOR2(1.f, 1.f);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CCubeTex::CreateCubeFront()
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 6;
	m_dwTriCnt = 2;
	m_dwVtxFVF = VTXFVF_TEX;

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTXTEX*      pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	// 앞
	pVtxTex[0].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[0].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[1].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[1].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[2].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[2].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[3].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[3].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[4].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[4].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[5].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[5].vTex = D3DXVECTOR2(1.f, 1.f);

	m_pVB->Unlock();

	return S_OK;
}

/************************ (SongSong) ************************/
/*  Front 예전꺼 ( 앞면만 텍스쳐보이고 다른면들 검은색 버전)  */
//HRESULT CCubeTex::CreateCubeFront()
//{
//   m_dwVtxSize = sizeof(VTXTEX);
//   m_dwVtxCnt = 36;
//   m_dwTriCnt = 12;
//   m_dwVtxFVF = VTXFVF_TEX;
//
//   m_IdxFmt = D3DFMT_INDEX16;
//   m_dwIdxSize = sizeof(INDEX16);
//
//   FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);
//
//   VTXTEX*      pVtxTex = nullptr;
//
//   m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);
//
//   //// 앞
//   //pVtxTex[0].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
//   //pVtxTex[0].vTex = D3DXVECTOR2(0.f, 1.f);
//   //pVtxTex[1].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
//   //pVtxTex[1].vTex = D3DXVECTOR2(0.f, 0.f);
//   //pVtxTex[2].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
//   //pVtxTex[2].vTex = D3DXVECTOR2(1.f, 0.f);
//   //pVtxTex[3].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
//   //pVtxTex[3].vTex = D3DXVECTOR2(0.f, 1.f);
//   //pVtxTex[4].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
//   //pVtxTex[4].vTex = D3DXVECTOR2(1.f, 0.f);
//   //pVtxTex[5].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
//   //pVtxTex[5].vTex = D3DXVECTOR2(1.f, 1.f);
//
//   //// 우 (반시계방향)
//   //pVtxTex[6].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
//   //pVtxTex[6].vTex = D3DXVECTOR2(1.f, 0.f);
//   //pVtxTex[7].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
//   //pVtxTex[7].vTex = D3DXVECTOR2(0.f, 0.f);
//   //pVtxTex[8].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
//   //pVtxTex[8].vTex = D3DXVECTOR2(0.f, 1.f);
//   //pVtxTex[9].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
//   //pVtxTex[9].vTex = D3DXVECTOR2(1.f, 0.f);
//   //pVtxTex[10].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
//   //pVtxTex[10].vTex = D3DXVECTOR2(0.f, 1.f);
//   //pVtxTex[11].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
//   //pVtxTex[11].vTex = D3DXVECTOR2(1.f, 1.f);
//
//   //// 뒤 (반시계방향)
//   //pVtxTex[12].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
//   //pVtxTex[12].vTex = D3DXVECTOR2(0.f, 0.f);
//   //pVtxTex[13].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
//   //pVtxTex[13].vTex = D3DXVECTOR2(1.f, 0.f);
//   //pVtxTex[14].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
//   //pVtxTex[14].vTex = D3DXVECTOR2(0.f, 1.f);
//   //pVtxTex[15].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
//   //pVtxTex[15].vTex = D3DXVECTOR2(0.f, 1.f);
//   //pVtxTex[16].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
//   //pVtxTex[16].vTex = D3DXVECTOR2(1.f, 0.f);
//   //pVtxTex[17].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
//   //pVtxTex[17].vTex = D3DXVECTOR2(1.f, 1.f);
//
//   //// 좌 (반시계방향)
//   //pVtxTex[18].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
//   //pVtxTex[18].vTex = D3DXVECTOR2(0.f, 1.f);
//   //pVtxTex[19].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
//   //pVtxTex[19].vTex = D3DXVECTOR2(1.f, 0.f);
//   //pVtxTex[20].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
//   //pVtxTex[20].vTex = D3DXVECTOR2(0.f, 0.f);
//   //pVtxTex[21].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
//   //pVtxTex[21].vTex = D3DXVECTOR2(0.f, 1.f);
//   //pVtxTex[22].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
//   //pVtxTex[22].vTex = D3DXVECTOR2(1.f, 1.f);
//   //pVtxTex[23].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
//   //pVtxTex[23].vTex = D3DXVECTOR2(1.f, 0.f);
//
//   //// 상 (반시계방향)
//   //pVtxTex[24].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
//   //pVtxTex[24].vTex = D3DXVECTOR2(0.f, 1.f);
//   //pVtxTex[25].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
//   //pVtxTex[25].vTex = D3DXVECTOR2(1.f, 0.f);
//   //pVtxTex[26].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
//   //pVtxTex[26].vTex = D3DXVECTOR2(0.f, 0.f);
//   //pVtxTex[27].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
//   //pVtxTex[27].vTex = D3DXVECTOR2(0.f, 1.f);
//   //pVtxTex[28].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
//   //pVtxTex[28].vTex = D3DXVECTOR2(1.f, 1.f);
//   //pVtxTex[29].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
//   //pVtxTex[29].vTex = D3DXVECTOR2(1.f, 0.f);
//
//   //// 하 (반시계방향)
//   //pVtxTex[30].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
//   //pVtxTex[30].vTex = D3DXVECTOR2(0.f, 0.f);
//   //pVtxTex[31].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
//   //pVtxTex[31].vTex = D3DXVECTOR2(1.f, 0.f);
//   //pVtxTex[32].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
//   //pVtxTex[32].vTex = D3DXVECTOR2(0.f, 1.f);
//   //pVtxTex[33].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
//   //pVtxTex[33].vTex = D3DXVECTOR2(1.f, 0.f);
//   //pVtxTex[34].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
//   //pVtxTex[34].vTex = D3DXVECTOR2(1.f, 1.f);
//   //pVtxTex[35].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
//   //pVtxTex[35].vTex = D3DXVECTOR2(0.f, 1.f);
//
//   // 앞
//   pVtxTex[0].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
//   pVtxTex[0].vTex = D3DXVECTOR2(0.f, 1.f);
//   pVtxTex[1].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
//   pVtxTex[1].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[2].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
//   pVtxTex[2].vTex = D3DXVECTOR2(1.f, 0.f);
//   pVtxTex[3].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
//   pVtxTex[3].vTex = D3DXVECTOR2(0.f, 1.f);
//   pVtxTex[4].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
//   pVtxTex[4].vTex = D3DXVECTOR2(1.f, 0.f);
//   pVtxTex[5].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
//   pVtxTex[5].vTex = D3DXVECTOR2(1.f, 1.f);
//
//   // 우
//   pVtxTex[6].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
//   pVtxTex[6].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[7].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
//   pVtxTex[7].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[8].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
//   pVtxTex[8].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[9].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
//   pVtxTex[9].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[10].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
//   pVtxTex[10].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[11].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
//   pVtxTex[11].vTex = D3DXVECTOR2(1.f, 0.f);
//
//   // 뒤 (반시계방향)
//   pVtxTex[12].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
//   pVtxTex[12].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[13].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
//   pVtxTex[13].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[14].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
//   pVtxTex[14].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[15].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
//   pVtxTex[15].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[16].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
//   pVtxTex[16].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[17].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
//   pVtxTex[17].vTex = D3DXVECTOR2(0.f, 0.f);
//
//   // 좌
//   pVtxTex[18].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
//   pVtxTex[18].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[19].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
//   pVtxTex[19].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[20].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
//   pVtxTex[20].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[21].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
//   pVtxTex[21].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[22].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
//   pVtxTex[22].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[23].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
//   pVtxTex[23].vTex = D3DXVECTOR2(0.f, 0.f);
//
//   // 상
//   pVtxTex[24].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
//   pVtxTex[24].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[25].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
//   pVtxTex[25].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[26].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
//   pVtxTex[26].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[27].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
//   pVtxTex[27].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[28].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
//   pVtxTex[28].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[29].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
//   pVtxTex[29].vTex = D3DXVECTOR2(0.f, 0.f);
//
//   // 하
//   pVtxTex[30].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
//   pVtxTex[30].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[31].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
//   pVtxTex[31].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[32].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
//   pVtxTex[32].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[33].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
//   pVtxTex[33].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[34].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
//   pVtxTex[34].vTex = D3DXVECTOR2(0.f, 0.f);
//   pVtxTex[35].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
//   pVtxTex[35].vTex = D3DXVECTOR2(0.f, 0.f);
//
//   m_pVB->Unlock();
//
//   return S_OK;
//}

HRESULT CCubeTex::CreateCubeCullB()
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 30;
	m_dwTriCnt = 10;
	m_dwVtxFVF = VTXFVF_TEX;

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTXTEX*      pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	// 앞
	pVtxTex[0].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[0].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[1].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[1].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[2].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[2].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[3].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[3].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[4].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[4].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[5].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[5].vTex = D3DXVECTOR2(1.f, 1.f);

	// 우
	pVtxTex[6].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[6].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[7].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[7].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[8].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[8].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[9].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[9].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[10].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[10].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[11].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[11].vTex = D3DXVECTOR2(1.f, 1.f);

	// 뒤 (반시계방향)
	pVtxTex[12].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[12].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[13].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[13].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[14].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[14].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[15].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[15].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[16].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[16].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[17].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[17].vTex = D3DXVECTOR2(1.f, 1.f);

	// 왼
	pVtxTex[18].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[18].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[19].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[19].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[20].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[20].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[21].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[21].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[22].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[22].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[23].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[23].vTex = D3DXVECTOR2(1.f, 1.f);

	// 상
	pVtxTex[24].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[24].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[25].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[25].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[26].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[26].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[27].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[27].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[28].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[28].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[29].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[29].vTex = D3DXVECTOR2(1.f, 1.f);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT Engine::CCubeTex::CreateCubeCullL()
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 30;
	m_dwTriCnt = 10;
	m_dwVtxFVF = VTXFVF_TEX;

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTXTEX*      pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	// 앞
	pVtxTex[0].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[0].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[1].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[1].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[2].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[2].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[3].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[3].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[4].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[4].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[5].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[5].vTex = D3DXVECTOR2(1.f, 1.f);

	// 우
	pVtxTex[6].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[6].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[7].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[7].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[8].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[8].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[9].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[9].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[10].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[10].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[11].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[11].vTex = D3DXVECTOR2(0.f, 0.f);

	// 뒤 (반시계방향)
	pVtxTex[12].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[12].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[13].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[13].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[14].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[14].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[15].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[15].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[16].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[16].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[17].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[17].vTex = D3DXVECTOR2(0.f, 0.f);

	//// 좌 (반시계방향)
	//pVtxTex[18].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	//pVtxTex[18].vTex = D3DXVECTOR2(0.f, 1.f);
	//pVtxTex[19].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	//pVtxTex[19].vTex = D3DXVECTOR2(1.f, 0.f);
	//pVtxTex[20].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	//pVtxTex[20].vTex = D3DXVECTOR2(0.f, 0.f);
	//pVtxTex[21].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	//pVtxTex[21].vTex = D3DXVECTOR2(0.f, 1.f);
	//pVtxTex[22].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	//pVtxTex[22].vTex = D3DXVECTOR2(1.f, 1.f);
	//pVtxTex[23].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	//pVtxTex[23].vTex = D3DXVECTOR2(1.f, 0.f);

	// 상
	pVtxTex[18].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[18].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[19].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[19].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[20].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[20].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[21].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[21].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[22].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[22].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[23].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[23].vTex = D3DXVECTOR2(0.f, 0.f);
	/*pVtxTex[24].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[24].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[25].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[25].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[26].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[26].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[27].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[27].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[28].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[28].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[29].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[29].vTex = D3DXVECTOR2(1.f, 1.f);*/

	// 하
	pVtxTex[24].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[24].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[25].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[25].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[26].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[26].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[27].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[27].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[28].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[28].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[29].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[29].vTex = D3DXVECTOR2(0.f, 0.f);
	//pVtxTex[30].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	//pVtxTex[30].vTex = D3DXVECTOR2(0.f, 1.f);
	//pVtxTex[31].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	//pVtxTex[31].vTex = D3DXVECTOR2(1.f, 0.f);
	//pVtxTex[32].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	//pVtxTex[32].vTex = D3DXVECTOR2(0.f, 0.f);
	//pVtxTex[33].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	//pVtxTex[33].vTex = D3DXVECTOR2(1.f, 0.f);
	//pVtxTex[34].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	//pVtxTex[34].vTex = D3DXVECTOR2(0.f, 1.f);
	//pVtxTex[35].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	//pVtxTex[35].vTex = D3DXVECTOR2(1.f, 1.f);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT Engine::CCubeTex::CreateCubeCullR()
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 30;
	m_dwTriCnt = 10;
	m_dwVtxFVF = VTXFVF_TEX;

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTXTEX*      pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	// 앞
	pVtxTex[0].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[0].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[1].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[1].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[2].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[2].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[3].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[3].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[4].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[4].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[5].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[5].vTex = D3DXVECTOR2(1.f, 1.f);

	//// 우 (반시계방향)
	//pVtxTex[6].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	//pVtxTex[6].vTex = D3DXVECTOR2(1.f, 0.f);
	//pVtxTex[7].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	//pVtxTex[7].vTex = D3DXVECTOR2(0.f, 0.f);
	//pVtxTex[8].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	//pVtxTex[8].vTex = D3DXVECTOR2(0.f, 1.f);
	//pVtxTex[9].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	//pVtxTex[9].vTex = D3DXVECTOR2(1.f, 0.f);
	//pVtxTex[10].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	//pVtxTex[10].vTex = D3DXVECTOR2(0.f, 1.f);
	//pVtxTex[11].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	//pVtxTex[11].vTex = D3DXVECTOR2(1.f, 1.f);

	// 뒤 (반시계방향)
	pVtxTex[6].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[6].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[7].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[7].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[8].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[8].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[9].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[9].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[10].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[10].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[11].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[11].vTex = D3DXVECTOR2(0.f, 0.f);
	/*pVtxTex[12].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[12].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[13].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[13].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[14].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[14].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[15].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[15].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[16].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[16].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[17].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[17].vTex = D3DXVECTOR2(1.f, 1.f);*/

	// 좌
	pVtxTex[12].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[12].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[13].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[13].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[14].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[14].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[15].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[15].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[16].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[16].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[17].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[17].vTex = D3DXVECTOR2(0.f, 0.f);
	/*pVtxTex[18].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[18].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[19].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[19].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[20].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[20].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[21].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[21].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[22].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[22].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[23].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[23].vTex = D3DXVECTOR2(1.f, 1.f);*/

	// 상
	pVtxTex[18].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[18].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[19].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[19].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[20].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[20].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[21].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[21].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[22].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[22].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[23].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[23].vTex = D3DXVECTOR2(0.f, 0.f);
	//pVtxTex[24].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	//pVtxTex[24].vTex = D3DXVECTOR2(0.f, 0.f);
	//pVtxTex[25].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	//pVtxTex[25].vTex = D3DXVECTOR2(1.f, 0.f);
	//pVtxTex[26].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	//pVtxTex[26].vTex = D3DXVECTOR2(0.f, 1.f);
	//pVtxTex[27].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	//pVtxTex[27].vTex = D3DXVECTOR2(0.f, 1.f);
	//pVtxTex[28].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	//pVtxTex[28].vTex = D3DXVECTOR2(1.f, 0.f);
	//pVtxTex[29].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	//pVtxTex[29].vTex = D3DXVECTOR2(1.f, 1.f);

	// 하
	pVtxTex[24].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[24].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[25].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[25].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[26].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[26].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[27].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[27].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[28].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[28].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[29].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[29].vTex = D3DXVECTOR2(0.f, 0.f);
	/*pVtxTex[30].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[30].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[31].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[31].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[32].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[32].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[33].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[33].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[34].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[34].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[35].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[35].vTex = D3DXVECTOR2(1.f, 1.f);*/

	m_pVB->Unlock();

	return S_OK;
}

HRESULT Engine::CCubeTex::CreateCubeCullLR()
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 24;
	m_dwTriCnt = 8;
	m_dwVtxFVF = VTXFVF_TEX;

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTXTEX*      pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	// 앞
	pVtxTex[0].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[0].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[1].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[1].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[2].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[2].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[3].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[3].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[4].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[4].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[5].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[5].vTex = D3DXVECTOR2(1.f, 1.f);

	// 뒤 (반시계방향)
	pVtxTex[6].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[6].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[7].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[7].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[8].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[8].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[9].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[9].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[10].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[10].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[11].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[11].vTex = D3DXVECTOR2(0.f, 0.f);

	// 상
	pVtxTex[12].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[12].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[13].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[13].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[14].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[14].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[15].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[15].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[16].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[16].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[17].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[17].vTex = D3DXVECTOR2(0.f, 0.f);

	// 하
	pVtxTex[18].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[18].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[19].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[19].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[20].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[20].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[21].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[21].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[22].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[22].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[23].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[23].vTex = D3DXVECTOR2(0.f, 0.f);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CCubeTex::CreateCubeCullBackAndB()
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 24;
	m_dwTriCnt = 8;
	m_dwVtxFVF = VTXFVF_TEX;

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTXTEX*      pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	// 앞
	pVtxTex[0].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[0].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[1].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[1].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[2].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[2].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[3].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[3].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[4].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[4].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[5].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[5].vTex = D3DXVECTOR2(1.f, 1.f);

	// 우
	pVtxTex[6].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[6].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[7].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[7].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[8].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[8].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[9].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[9].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[10].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[10].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[11].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[11].vTex = D3DXVECTOR2(1.f, 1.f);

	// 왼
	pVtxTex[12].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[12].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[13].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[13].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[14].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[14].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[15].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[15].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[16].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[16].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[17].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[17].vTex = D3DXVECTOR2(1.f, 1.f);

	// 상
	pVtxTex[18].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[18].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[19].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[19].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[20].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[20].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[21].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[21].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[22].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[22].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[23].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[23].vTex = D3DXVECTOR2(1.f, 1.f);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CCubeTex::CreateCubePlayer()
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 24;
	m_dwTriCnt = 8;
	m_dwVtxFVF = VTXFVF_TEX;

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTXTEX*      pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	// 앞
	pVtxTex[0].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[0].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[1].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[1].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[2].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[2].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[3].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[3].vTex = D3DXVECTOR2(0.f, 1.f);
	pVtxTex[4].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[4].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[5].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[5].vTex = D3DXVECTOR2(1.f, 1.f);

	// 우
	pVtxTex[6].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[6].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[7].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[7].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[8].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[8].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[9].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[9].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[10].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[10].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[11].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[11].vTex = D3DXVECTOR2(0.f, 0.f);

	// 뒤 (반시계방향)
	pVtxTex[12].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[12].vTex = D3DXVECTOR2(1.f, 0.f);
	pVtxTex[13].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[13].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[14].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[14].vTex = D3DXVECTOR2(1.f, 1.f);
	pVtxTex[15].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[15].vTex = D3DXVECTOR2(1.f, 1.f);
	pVtxTex[16].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[16].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[17].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[17].vTex = D3DXVECTOR2(0.f, 1.f);

	// 좌
	pVtxTex[18].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[18].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[19].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[19].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[20].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[20].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[21].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[21].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[22].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[22].vTex = D3DXVECTOR2(0.f, 0.f);
	pVtxTex[23].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[23].vTex = D3DXVECTOR2(0.f, 0.f);

	m_pVB->Unlock();

	return S_OK;
}

HRESULT Engine::CCubeTex::CreateCubeSkyBox()
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 36;
	m_dwTriCnt = 12;
	m_dwVtxFVF = VTXFVF_TEX;

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwIdxSize = sizeof(INDEX16);

	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTXTEX*      pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	float CubeSize = 1024.f;
	float TexXSize = CubeSize * 4.f;
	float TexYSize = CubeSize * 3.f;

	// 앞
	pVtxTex[0].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[0].vTex = D3DXVECTOR2(CubeSize / TexXSize, CubeSize * 2 / TexYSize);
	pVtxTex[1].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[1].vTex = D3DXVECTOR2(CubeSize / TexXSize, CubeSize / TexYSize);
	pVtxTex[2].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[2].vTex = D3DXVECTOR2(CubeSize * 2 / TexXSize, CubeSize / TexYSize);
	pVtxTex[3].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[3].vTex = D3DXVECTOR2(CubeSize / TexXSize, CubeSize * 2 / TexYSize);
	pVtxTex[4].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[4].vTex = D3DXVECTOR2(CubeSize * 2 / TexXSize, CubeSize / TexYSize);
	pVtxTex[5].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[5].vTex = D3DXVECTOR2(CubeSize * 2 / TexXSize, CubeSize * 2 / TexYSize);

	// 우
	pVtxTex[6].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[6].vTex = D3DXVECTOR2(CubeSize * 2 / TexXSize, CubeSize * 2 / TexYSize);
	pVtxTex[7].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[7].vTex = D3DXVECTOR2(CubeSize * 2 / TexXSize, CubeSize / TexYSize);
	pVtxTex[8].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[8].vTex = D3DXVECTOR2(CubeSize * 3 / TexXSize, CubeSize / TexYSize);
	pVtxTex[9].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[9].vTex = D3DXVECTOR2(CubeSize * 2 / TexXSize, CubeSize * 2 / TexYSize);
	pVtxTex[10].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[10].vTex = D3DXVECTOR2(CubeSize * 3 / TexXSize, CubeSize / TexYSize);
	pVtxTex[11].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[11].vTex = D3DXVECTOR2(CubeSize * 3 / TexXSize, CubeSize * 2 / TexYSize);

	// 뒤 (반시계방향)
	pVtxTex[12].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[12].vTex = D3DXVECTOR2(CubeSize * 3 / TexXSize, CubeSize / TexYSize);
	pVtxTex[13].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[13].vTex = D3DXVECTOR2(CubeSize * 4 / TexXSize, CubeSize / TexYSize);
	pVtxTex[14].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[14].vTex = D3DXVECTOR2(CubeSize * 3 / TexXSize, CubeSize * 2 / TexYSize);
	pVtxTex[15].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[15].vTex = D3DXVECTOR2(CubeSize * 3 / TexXSize, CubeSize * 2 / TexYSize);
	pVtxTex[16].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[16].vTex = D3DXVECTOR2(CubeSize * 4 / TexXSize, CubeSize / TexYSize);
	pVtxTex[17].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[17].vTex = D3DXVECTOR2(CubeSize * 4 / TexXSize, CubeSize * 2 / TexYSize);

	// 좌
	pVtxTex[18].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[18].vTex = D3DXVECTOR2(0.f, CubeSize / TexYSize);
	pVtxTex[19].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[19].vTex = D3DXVECTOR2(CubeSize / TexXSize, CubeSize / TexYSize);
	pVtxTex[20].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[20].vTex = D3DXVECTOR2(0.f, CubeSize * 2 / TexYSize);
	pVtxTex[21].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[21].vTex = D3DXVECTOR2(0.f, CubeSize * 2 / TexYSize);
	pVtxTex[22].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[22].vTex = D3DXVECTOR2(CubeSize / TexXSize, CubeSize / TexYSize);
	pVtxTex[23].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[23].vTex = D3DXVECTOR2(CubeSize / TexXSize, CubeSize * 2 / TexYSize);

	// 상
	pVtxTex[24].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxTex[24].vTex = D3DXVECTOR2(CubeSize / TexXSize, 0.f);
	pVtxTex[25].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[25].vTex = D3DXVECTOR2(CubeSize * 2 / TexXSize, 0.f);
	pVtxTex[26].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[26].vTex = D3DXVECTOR2(CubeSize / TexXSize, CubeSize / TexYSize);
	pVtxTex[27].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxTex[27].vTex = D3DXVECTOR2(CubeSize / TexXSize, CubeSize / TexYSize);
	pVtxTex[28].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxTex[28].vTex = D3DXVECTOR2(CubeSize * 2 / TexXSize, 0.f);
	pVtxTex[29].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxTex[29].vTex = D3DXVECTOR2(CubeSize * 2 / TexXSize, CubeSize / TexYSize);

	// 하
	pVtxTex[30].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[30].vTex = D3DXVECTOR2(CubeSize * 1 / TexXSize, CubeSize * 2 / TexYSize);
	pVtxTex[31].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[31].vTex = D3DXVECTOR2(CubeSize * 2 / TexXSize, CubeSize * 3 / TexYSize);
	pVtxTex[32].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxTex[32].vTex = D3DXVECTOR2(CubeSize * 1 / TexXSize, CubeSize * 3 / TexYSize);
	pVtxTex[33].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxTex[33].vTex = D3DXVECTOR2(CubeSize * 2 / TexXSize, CubeSize * 3 / TexYSize);
	pVtxTex[34].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxTex[34].vTex = D3DXVECTOR2(CubeSize * 1 / TexXSize, CubeSize * 2 / TexYSize);
	pVtxTex[35].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxTex[35].vTex = D3DXVECTOR2(CubeSize * 2 / TexXSize, CubeSize * 2 / TexYSize);

	m_pVB->Unlock();

	return S_OK;
}


CCubeTex* CCubeTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, CUBE_CLIPPING_TYPE eType)
{
	CCubeTex*      pInstance = new CCubeTex(pGraphicDev);

	if (FAILED(pInstance->CreateBuffer(eType)))
		Safe_Delete(pInstance);

	return pInstance;
}