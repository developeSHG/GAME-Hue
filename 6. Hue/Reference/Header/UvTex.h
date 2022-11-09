#ifndef UvTex_h__
#define UvTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CUvTex : public CVIBuffer
{
protected:
	explicit CUvTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUvTex(const CUvTex& rhs);
public:
	virtual ~CUvTex(void);

public:
	HRESULT		CreateBuffer(void);
	void		Render(void);
	void		Release(void);
	virtual void Update(void);

	CResources*		CloneUV(void);
private:
	float		m_fUvTex;
public:
	static CUvTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

END


#endif // UvTex_h__
