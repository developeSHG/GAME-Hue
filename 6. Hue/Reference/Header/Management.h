#ifndef Management_h__
#define Management_h__

#include "Engine_Include.h"
#include "Scene.h"
#include "Renderer.h"

BEGIN(Engine)

class ENGINE_DLL CManagement
{
	DECLARE_SINGLETON(CManagement)

private:
	CManagement(void);
	~CManagement(void);

public:
	template<typename T>
	HRESULT	SceneChange(T& Functor);

public:
	HRESULT		Initialize(LPDIRECT3DDEVICE9 pGraphicDev);
	void		Update(void);
	void		LateUpdate(void);
	void		Render(void);

public:
	void		SetBackColor(D3DCOLOR color)
	{
		if (m_pRenderer)
			m_pRenderer->SetBackColor(color);
	}
private:
	void		Release(void);

private:
	CRenderer*				m_pRenderer = nullptr;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	CScene*					m_pScene = nullptr;

};


template<typename T>
HRESULT Engine::CManagement::SceneChange(T& Functor)
{
	if (nullptr != m_pScene)
		Safe_Delete(m_pScene);

	FAILED_CHECK_RETURN(Functor(&m_pScene, m_pGraphicDev), E_FAIL);

	m_pRenderer->SetScene(m_pScene);

	return S_OK;
}

END
#endif // Management_h__
