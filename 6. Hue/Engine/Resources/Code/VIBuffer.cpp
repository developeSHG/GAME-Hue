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
	m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, // 정점 버퍼의 사이즈
		0,					  // 정점의 사용방식(0인 경우 정적 버퍼를 사용하겠다는 의미)
		m_dwVtxFVF,		  // 정점의 스타일 지정 인자값	
		D3DPOOL_MANAGED,	  // 메모리 풀 스타일	
		&m_pVB,			  // 아웃풋 대표객체의 주소 	
		NULL);			      // 버퍼 생성 시의 예약 상태 거의 NULL로 설정


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
	// 디바이스 장치에게 현재 그리려는 버퍼를 링크시켜주는 함수

	// 1. 몇 번 슬롯에 보관할 것인가
	// 2. 누구를 넘겨줄 것인가
	// 3. 어디서부터 그릴 것인가, 0인 경우 버퍼가 보관하는 처음부터 그리라는 의미
	// 4. 어떤 단위로 그리기를 표현할 것인가

	m_pGraphicDev->SetFVF(m_dwVtxFVF);

	m_pGraphicDev->SetIndices(m_pIB);
	// 디바이스 장치에게 현재 그리려는 도형의 인덱스를 링크시켜주는 함수

	//m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
	// 이것은 인덱스를 이용한 그리기 함수이다.
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


