#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Resources.h"

#include "Engine_Include.h"
BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CResources
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
public:
	virtual ~CVIBuffer(void);

public:
	HRESULT					CreateBuffer(void);
	virtual void			Render(void);
	virtual void			Render_NoIndices(void);
	virtual void			Render_TriFans(void);
	virtual void			Update(void);

	virtual void			Release(void);
	virtual CResources*		Clone(void);

public:
	void CopyToClient(void* pVertex);
	void SendToEngine(void* pVertex);

	HRESULT GetVertexInfo(D3DXVECTOR3& vPos, float* fA, float* fB, float* fC, float* fD);
protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	vector<D3DXPLANE> m_vecPlane;

	DWORD						m_dwVtxSize;
	DWORD						m_dwVtxCnt;
	DWORD						m_dwVtxFVF;
	DWORD						m_dwTriCnt;

	DWORD						m_dwIdxSize;
	D3DFORMAT					m_IdxFmt;
};

END
#endif // VIBuffer_h__
