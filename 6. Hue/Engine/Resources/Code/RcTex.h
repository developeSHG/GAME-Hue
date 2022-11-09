#ifndef RcTex_h__
#define RcTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex : public CVIBuffer
{
protected:
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CRcTex(void);

public:
	HRESULT		CreateBuffer(void);
	void		Render(void);
	void		Release(void);

public:
	static CRcTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

END


#endif // RcTex_h__
