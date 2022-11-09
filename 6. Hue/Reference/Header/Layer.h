#ifndef Layer_h__
#define Layer_h__

#include "Engine_Include.h"

#include "GameObject.h"
BEGIN(Engine)

class ENGINE_DLL CLayer
{
protected:
	explicit CLayer(LPDIRECT3DDEVICE9 pGraphicDev);
public:
    ~CLayer(void);

public:
	HRESULT AddObject(const wstring& wstrObjKey, CGameObject* pGameObject);
	void	Update(void);
	void    LateUpdate(void);
	void	Render(void);


	CGameObject* FindObject(const wstring& wstrObjKey);

private:
	void	Release(void);

private:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	map<wstring, list<CGameObject*>>    m_mapObjList;

public:
	static CLayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

};

END
#endif // Layer_h__
