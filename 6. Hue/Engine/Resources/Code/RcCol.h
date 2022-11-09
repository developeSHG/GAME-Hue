#ifndef RcCol_h__
#define RcCol_h__

#include "VIBuffer.h"

#include "Engine_Include.h"
BEGIN(Engine)

class ENGINE_DLL CRcCol : public CVIBuffer
{
protected:
	explicit CRcCol(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CRcCol(void);

public:
	HRESULT				CreateBuffer(void);
	virtual void		Render(void);
	void				Release(void);

public:
	static CRcCol*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

END


#endif // RcCol_h__
