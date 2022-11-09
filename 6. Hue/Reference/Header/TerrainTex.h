#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CTerrainTex(void);

public:
	HRESULT				CreateBuffer(const WORD& wCntX,
									const WORD& wCntZ,
									const WORD& wItv);

	virtual void		Render(void);
	virtual void		Release(void);

	

public:
	static CTerrainTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
								const WORD& wCntX, 
								const WORD& wCntZ, 
								const WORD& wItv);
};

END
#endif // TerrainTex_h__
