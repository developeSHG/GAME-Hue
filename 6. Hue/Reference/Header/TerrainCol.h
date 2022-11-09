#ifndef TerrainCol_h__
#define TerrainCol_h__

#include "VIBuffer.h"

#include "Engine_Include.h"
BEGIN(Engine)

class ENGINE_DLL CTerrainCol : public CVIBuffer
{
protected:
	explicit CTerrainCol(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CTerrainCol(void);

public:
	HRESULT				CreateBuffer(const WORD& wCntX,
									const WORD& wCntZ,
									const WORD& wItv);
	virtual void		Render(void);
	void				Release(void);

public:
	static CTerrainCol*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
								const WORD& wCntX,
								const WORD& wCntZ,
								const WORD& wItv);
};

END


#endif // TerrainCol_h__
