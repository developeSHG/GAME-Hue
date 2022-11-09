#ifndef RcMouse_h__
#define RcMouse_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcMouse : public CVIBuffer
{
protected:
	explicit CRcMouse(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CRcMouse(void);

public:
	HRESULT		CreateBuffer(void);
	void		Render(void);
	void		Release(void);

public:
	static CRcMouse*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

END


#endif // RcMouse_h__
