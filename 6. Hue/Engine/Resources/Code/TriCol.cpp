#include "TriCol.h"
USING(Engine)

Engine::CTriCol::CTriCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CTriCol::~CTriCol(void)
{
	Release();
}

HRESULT Engine::CTriCol::CreateBuffer(void)
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 3;
	m_dwTriCnt = 1;
	m_dwVtxFVF = VTXFVF_COL;

	FAILED_CHECK_RETURN(CVIBuffer::CreateBuffer(), E_FAIL);

	VTXCOL*		pVtxCol = nullptr;

	// 1 의미 : 메모리 공간 잠그는 개념
	// 2 의미 : 이 객체를 통해 정점들에게 접근하겠다는 의미

	m_pVB->Lock(0, 0, (void**)&pVtxCol, 0);
	// 1 : 어디서부터 잠글 것인가
	// 2 : 어디까지 잠글 것인가(0인 경우 전체 영역을 의미하는 default 옵션)
	// 3 : 잠근 메모리 영역을 접근할 수 있는 주소 값을 저장하는 포인터
	// 4 : 잠그는 형태를 묻는 옵션, 정적버퍼인 경우 0을 넣어줌

	pVtxCol[0].vPos = D3DXVECTOR3(0.f, 1.f, 0.f);
	pVtxCol[0].dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);




	pVtxCol[1].vPos = D3DXVECTOR3(1.f, -1.f, 0.f);
	pVtxCol[1].dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	pVtxCol[2].vPos = D3DXVECTOR3(-1.f, -1.f, 0.f);
	pVtxCol[2].dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	m_pVB->Unlock();

	return S_OK;
}

void Engine::CTriCol::Render(void)
{
	CVIBuffer::Render();

}

void Engine::CTriCol::Release(void)
{
}

CTriCol* Engine::CTriCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTriCol*		pInstance = new CTriCol(pGraphicDev);

	if (FAILED(pInstance->CreateBuffer()))
		Safe_Delete(pInstance);

	return pInstance;
}
