#ifndef CubeTex_h__
#define CubeTex_h__

#include "VIBuffer.h"

#include "Engine_Include.h"
BEGIN(Engine)

class ENGINE_DLL CCubeTex : public CVIBuffer
{
protected:
	explicit CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CCubeTex(void);

public:
	HRESULT				CreateBuffer(CUBE_CLIPPING_TYPE eType);
	virtual void		Render(void);
	void				Release(void);

private:
	HRESULT				CreateCubeNormal();
	HRESULT				CreateCubeFront();
	HRESULT				CreateCubeCullB();
	HRESULT				CreateCubeCullL();
	HRESULT				CreateCubeCullR();
	HRESULT				CreateCubeCullLR();
	HRESULT				CreateCubeCullBackAndB();
	HRESULT				CreateCubePlayer();
	HRESULT				CreateCubeSkyBox();

private:
	CUBE_CLIPPING_TYPE	m_eCubeType;

public:
	static CCubeTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev, CUBE_CLIPPING_TYPE eType);
};

END


#endif // CubeTex_h__
