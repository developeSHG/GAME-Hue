#ifndef CircleCol_h__
#define CircleCol_h__

#include "VIBuffer.h"

#include "Engine_Include.h"
BEGIN(Engine)

class ENGINE_DLL CCircleCol : public CVIBuffer
{
protected:
	explicit CCircleCol(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CCircleCol(void);

public:
	HRESULT				CreateBuffer();
	virtual void		Render(void);
	void				Release(void);

public:
	static CCircleCol*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

END


#endif // CircleCol_h__
