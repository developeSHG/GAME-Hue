#ifndef CubeCol_h__
#define CubeCol_h__

#include "VIBuffer.h"

#include "Engine_Include.h"
BEGIN(Engine)

class ENGINE_DLL CCubeCol : public CVIBuffer
{
protected:
	explicit CCubeCol(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CCubeCol(void);

public:
	HRESULT				CreateBuffer();
	virtual void		Render(void);
	void				Release(void);

public:
	static CCubeCol*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

END


#endif // CubeCol_h__
