#ifndef Tutorial_h__
#define Tutorial_h__

#include "Scene.h"

BEGIN(Engine)

class CManagement;
class CResourceMgr;
class CGameObject;

END

class CPlayerTuto;
class CTerrain;
class CColorCircle;
class CLoading;

class CTutorial : public Engine::CScene
{
private:
	explicit CTutorial(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CTutorial(void);

public:
	virtual HRESULT Initialize(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	HRESULT		Add_Environment(void);
	HRESULT		Add_GameObject(void);
	HRESULT		Add_UI(void);

private:
	Engine::CManagement*		m_pManagement = nullptr;
	Engine::CResourceMgr*		m_pResourceMgr = nullptr;

private:
	//юс╫ц
	CPlayerTuto*				m_pPlayerTuto = nullptr;
	CColorCircle*				m_pColorCircle = nullptr;
	CLoading*					m_pLoading = nullptr;

private:
	list<Engine::CGameObject*>			m_listTutoObject;

public:
	static CTutorial* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

#endif // Tutorial_h__
