#ifndef RcyTex_h__
#define RcyTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcyTex : public CVIBuffer
{
protected:
	explicit CRcyTex(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CRcyTex(void);

public:
	HRESULT		CreateBuffer(void);
	void		Render(void);
	void		Release(void);

public:
	static CRcyTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

END


#endif // RcyTex_h__
