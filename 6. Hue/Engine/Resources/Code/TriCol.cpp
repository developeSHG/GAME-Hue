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

	// 1 �ǹ� : �޸� ���� ��״� ����
	// 2 �ǹ� : �� ��ü�� ���� �����鿡�� �����ϰڴٴ� �ǹ�

	m_pVB->Lock(0, 0, (void**)&pVtxCol, 0);
	// 1 : ��𼭺��� ��� ���ΰ�
	// 2 : ������ ��� ���ΰ�(0�� ��� ��ü ������ �ǹ��ϴ� default �ɼ�)
	// 3 : ��� �޸� ������ ������ �� �ִ� �ּ� ���� �����ϴ� ������
	// 4 : ��״� ���¸� ���� �ɼ�, ���������� ��� 0�� �־���

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
