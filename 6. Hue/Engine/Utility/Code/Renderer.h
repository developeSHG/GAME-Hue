#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Include.h"
#include "Scene.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer
{
private:
	CRenderer(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	~CRenderer(void);

public:
	void		SetScene(CScene* pScene);

public:
	HRESULT		Initialize(void);
	void		Render(void);

public:
	void SetBackColor(D3DCOLOR tColor) { m_tBackColor = tColor; }

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	CScene*					m_pScene;
	D3DCOLOR				m_tBackColor = D3DCOLOR_XRGB(128, 128, 128);
public:
	static CRenderer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

};

END
#endif // Renderer_h__
