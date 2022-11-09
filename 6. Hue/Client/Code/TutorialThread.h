#ifndef TutorialThread_h__
#define TutorialThread_h__

#include "Scene.h"

BEGIN(Engine)

class CManagement;
class CResourceMgr;

END

class CPlayerTuto;

class CTutorialThread : public Engine::CScene
{
private:
	explicit CTutorialThread(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CTutorialThread(void);

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

public:
	static HRESULT		BufferForStage(void);
	static HRESULT		TextureForTutorial(void);
	static HRESULT		TextureForStage(void);

private:
	static unsigned __stdcall	LoadingTextureByThread(void* pParam);

private:
	Engine::CManagement*		m_pManagement = nullptr;
	Engine::CResourceMgr*		m_pResourceMgr = nullptr;

private:
	HANDLE						m_hLoadingThread;
	CRITICAL_SECTION			m_CriticalSection;
	wstring						m_wstrLoadingMessage;

private:
	static bool m_bLoadingComplete;

private:
	//юс╫ц
	CPlayerTuto*				m_pPlayerTuto = nullptr;
	
public:
	static CTutorialThread* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

#endif // TutorialThread_h__
