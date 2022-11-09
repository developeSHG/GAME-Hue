#ifndef Scene_h__
#define Scene_h__

#include "Engine_Include.h"
#include "Layer.h"
BEGIN(Engine)

class ENGINE_DLL CScene
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CScene(void);

public:
	virtual HRESULT	Initialize(void);
	virtual void	Update(void);
	virtual void	LateUpdate(void);
	virtual void	Render(void);
	virtual void	Release(void);

protected:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	map<WORD, CLayer*>		m_mapLayer;

private:

};

END
#endif // Scene_h__
