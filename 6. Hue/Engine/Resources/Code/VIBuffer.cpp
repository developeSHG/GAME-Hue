#include "VIBuffer.h"
#include "UvTex.h"
USING(Engine)

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwVtxSize(0)
	, m_dwVtxCnt(0)
	, m_dwVtxFVF(0)
	, m_dwTriCnt(0)
	, m_dwIdxSize(0)
{

}

Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CResources(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwVtxFVF(rhs.m_dwVtxFVF)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwIdxSize(rhs.m_dwIdxSize)
	, m_vecPlane(rhs.m_vecPlane)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

Engine::CVIBuffer::~CVIBuffer(void)
{
	Release();
}

HRESULT Engine::CVIBuffer::CreateBuffer(void)
{
	m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, // ���� ������ ������
		0,					  // ������ �����(0�� ��� ���� ���۸� ����ϰڴٴ� �ǹ�)
		m_dwVtxFVF,		  // ������ ��Ÿ�� ���� ���ڰ�	
		D3DPOOL_MANAGED,	  // �޸� Ǯ ��Ÿ��	
		&m_pVB,			  // �ƿ�ǲ ��ǥ��ü�� �ּ� 	
		NULL);			      // ���� ���� ���� ���� ���� ���� NULL�� ����


	m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,
		0,
		m_IdxFmt,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL);

	return S_OK;
}

void Engine::CVIBuffer::Render(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	// ����̽� ��ġ���� ���� �׸����� ���۸� ��ũ�����ִ� �Լ�

	// 1. �� �� ���Կ� ������ ���ΰ�
	// 2. ������ �Ѱ��� ���ΰ�
	// 3. ��𼭺��� �׸� ���ΰ�, 0�� ��� ���۰� �����ϴ� ó������ �׸���� �ǹ�
	// 4. � ������ �׸��⸦ ǥ���� ���ΰ�

	m_pGraphicDev->SetFVF(m_dwVtxFVF);

	m_pGraphicDev->SetIndices(m_pIB);
	// ����̽� ��ġ���� ���� �׸����� ������ �ε����� ��ũ�����ִ� �Լ�

	//m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
	// �̰��� �ε����� �̿��� �׸��� �Լ��̴�.
}

void CVIBuffer::Render_NoIndices(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	m_pGraphicDev->SetFVF(m_dwVtxFVF);

	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);
}

void CVIBuffer::Render_TriFans(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	m_pGraphicDev->SetFVF(m_dwVtxFVF);

	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, m_dwTriCnt);
}

void CVIBuffer::Update(void)
{

}

void Engine::CVIBuffer::Release(void)
{
	if (0 == (*m_pwRefCnt))
	{
		Safe_Release(m_pVB);
		Safe_Release(m_pIB);

		CResources::Release();
	}
	else
		(*m_pwRefCnt)--;

	m_vecPlane.clear();
	m_vecPlane.shrink_to_fit();
}

CResources * CVIBuffer::Clone(void)
{
	(*m_pwRefCnt)++;

	return new CVIBuffer(*this);
}



void CVIBuffer::CopyToClient(void * pVertex)
{
	void*		pOriginVtx = nullptr;

	m_pVB->Lock(0, 0, (void**)&pOriginVtx, 0);

	memcpy(pVertex, pOriginVtx, m_dwVtxCnt * m_dwVtxSize);

	m_pVB->Unlock();
}

void CVIBuffer::SendToEngine(void * pVertex)
{
	void*		pConvertVtx = nullptr;

	m_pVB->Lock(0, 0, (void**)&pConvertVtx, 0);

	memcpy(pConvertVtx, pVertex, m_dwVtxCnt * m_dwVtxSize);

	m_pVB->Unlock();
}

HRESULT CVIBuffer::GetVertexInfo(D3DXVECTOR3 & vPos, float * fA, float * fB, float * fC, float * fD)
{
	if (vPos.x<0 || vPos.x>128.f || vPos.z<0.f || vPos.z>128.f)
		return E_FAIL;

	int iPosX = (int)vPos.x;
	int iPosZ = (int)vPos.z;
	
	float fPosX = vPos.x - iPosX;
	float fPosZ = vPos.z - iPosZ;

	int iIndex = iPosZ * 128 * 2 + iPosX * 2;
	if (fPosX + fPosZ < 1)
		++iIndex;

	*fA = m_vecPlane[iIndex].a;
	*fB = m_vecPlane[iIndex].b;
	*fC = m_vecPlane[iIndex].c;
	*fD = m_vecPlane[iIndex].d;

	return S_OK;
}


