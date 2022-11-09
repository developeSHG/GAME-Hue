#ifndef GameObject_h__
#define GameObject_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CGameObject(void);

public:
	virtual HRESULT	Initialize(void);
	virtual void	Update(void);
	virtual void	Render(void);			//»Æ¿Œ
	virtual void	RenderInsideWorld(void);
	virtual void	LateUpdate(void);

	CComponent* FindComponent(const wstring& wstrCompKey);

public:
	TCHAR*			GetBufferName() const { return m_pBufferName; }
	TCHAR*			GetTexName() const { return m_pTexName; }

private:
	virtual void	Release(void);

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	map<wstring, CComponent*>   m_mapComponent;

	TCHAR*						m_pBufferName;
	TCHAR*						m_pTexName;
};

END
#endif // GameObject_h__
