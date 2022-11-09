#ifndef Stage3_h__
#define Stage3_h__

#include "Scene.h"

BEGIN(Engine)

class CManagement;
class CResourceMgr;

END

class CCubeFirecracker;
class CCubeFireparent;
class CPlayer;
class CTerrain;

class CStage3 : public Engine::CScene
{
private:
	explicit CStage3(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CStage3(void);

public:
	virtual HRESULT Initialize(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

	void CreateFirecracker(D3DXVECTOR3 m_parentPos, bool IsKing);
	void CreateKingcracker(D3DXVECTOR3 m_parentPos);
	void CreateFireparent();
	void		MakeKingFire();

private:
	HRESULT		Add_Environment(void);
	HRESULT		Add_GameObject(void);
	HRESULT		Add_UI(void);

	int			m_iExlodeTime;
	bool		m_bIsPlayerEndingSound;

	bool		m_bIsKingFire;
	float		m_fMakeFireTime;
private:
	Engine::CManagement*		m_pManagement = nullptr;
	Engine::CResourceMgr*		m_pResourceMgr = nullptr;

	int							m_iMakeFireTime;

	list<CCubeFireparent*>			m_listFireparent;
	list<CCubeFirecracker*>			m_listFirecracker;
private:
	//юс╫ц
	CPlayer*					m_pPlayer = nullptr;
	bool						m_bLateInit;
	bool						m_bMoreKingCracker;
	int							m_iMoreKingCracker;
	float						m_fMakeKingTime;
public:
	static CStage3* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
};

#endif // CStage3
