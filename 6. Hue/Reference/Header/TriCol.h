#ifndef TriCol_h__
#define TriCol_h__

#include "VIBuffer.h"

#include "Engine_Include.h"
BEGIN(Engine)

class ENGINE_DLL CTriCol : public CVIBuffer
{
protected:
	explicit CTriCol(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CTriCol(void);

public:
	HRESULT				CreateBuffer(void);
	virtual void		Render(void);
	void				Release(void);


public:
	static CTriCol*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

END


#endif // TriCol_h__
