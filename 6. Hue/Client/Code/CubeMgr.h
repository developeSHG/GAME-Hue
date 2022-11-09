#ifndef CubeMgr_h__
#define CubeMgr_h__

#include "Engine_Include.h"
#include "Include.h"

BEGIN(Engine)

class CCube;
class CObj;
class CResourceMgr;
class CGameObject;
END

class CCameraObserver;
class CObjBasic;
typedef map<CUBEID, list<Engine::CCube*>>		CurStageCube;
typedef map<OBJID, list<CObjBasic*>>			CurStageObj;
typedef map<OBJECTNAME, list<Engine::CGameObject*>>	AllStageParticle;

class CButtonX;
class CPlayer;
class CDoor;
class CCubeMgr
{
	DECLARE_SINGLETON(CCubeMgr)

private:
	explicit CCubeMgr(void);
	~CCubeMgr(void);

private:
	void		Release(void);

public:
	HRESULT		Load_Tutorial(LPDIRECT3DDEVICE9 _pGraphicDev);
	HRESULT		Load_Stage(LPDIRECT3DDEVICE9 _pGraphicDev);
	HRESULT		Load_Stage2(LPDIRECT3DDEVICE9 _pGraphicDev);
	HRESULT		Load_Stage3(LPDIRECT3DDEVICE9 _pGraphicDev);
	HRESULT		Load_Cube(LPDIRECT3DDEVICE9 _pGraphicDev, const TCHAR* _szPathFile, SCENEID eID);
	HRESULT		Load_Obj(LPDIRECT3DDEVICE9 _pGraphicDev, const TCHAR* _szPathFile, SCENEID eID);


public:
	D3DXMATRIX	m_View;
	D3DXMATRIX	m_Proj;
public:
	void		RenderInsideWorld();
	void		RenderOutsideWorld();
	void		RenderViewportCube();

	void		Update(void);
	void		LateUpdate(void);

	D3DXVECTOR3		GetDoorPos();
	D3DXVECTOR3		GetLockDoorPos();

	void		CreateParticle(void);

	void		SetBackColor(D3DCOLOR tColor) { m_tBackColor = tColor; }
	D3DCOLOR    GetBackColor() { return m_tBackColor; }
private:
	D3DCOLOR				m_tBackColor = D3DCOLOR_XRGB(128, 128, 128);

public:
	bool		Get_IsResize() { return m_bIsResize; }
	bool		Get_IsRender() { return m_bIsRender; }
	bool		Get_IsOriginPos() { return m_bInitOriginPos; }
	bool		Get_IsStageChange() { return m_bIsStageChange; }

	void		Set_IsResize(bool b) { m_bIsResize = b; }
	void		Set_IsStageChange(bool b) { m_bIsStageChange = b; }
	void		Set_IsRender(bool b) { m_bIsRender = b; }
	void		Set_IsOriginPos(bool b) { m_bInitOriginPos = b; }
private:
	bool		m_bIsResize;
	bool		m_bIsRender;
	bool		m_bInitOriginPos;
	bool		m_bIsStageChange;
public:
	// Get
	const CurStageCube* 						Get_MapCubeList() { return m_mapCurStageCube; }
	const CurStageObj* 							Get_MapObjList() { return m_mapCurStageObj; }
	//const CurStageObject* 						Get_MapObjList() { return m_mapStage1Object; }

	const list<Engine::CCube*>&					Get_ListCubeType(CUBEID _eCubeID);
	const list<CObjBasic*>&					Get_ListObjType(OBJID _eObjID);
	CPlayer* GetPlayer() { return m_pPlayer; }
	bool					m_bGetBattery;
	void					ResetSaveCount() {m_iSaveCount = 0; }
public:
	Engine::CGameObject*	Get_ButtonX() { return m_mapAllStageParticle[OBJECTNAME::BUTTON_X].front(); }

public:
	// Set
	void	Set_CurStageCube(SCENEID _eSceneID);
	void	Set_CurCubeColor(Engine::CUBE_COLOR eCubeColor) { m_eCurCubeColor = eCubeColor; }
	Engine::CUBE_COLOR Get_CurCubeColor() { return m_eCurCubeColor; }
	void	SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	bool	GetPlayerDie() { return m_bIsPlayerDie; }
	void	SetPlayerDie(bool b) { m_bIsPlayerDie = b; }
	void	ResetCubePos();
	void	SetIsSave(bool b) { m_bGetSavePos = b; }
	void	SetSavePos(D3DXVECTOR3 vecSavePos) { m_vecSavePos = vecSavePos; m_vecSavePos.y += 0.5f; }
private:
	Engine::CResourceMgr*						m_pResourceMgr = nullptr;
	CCameraObserver*							m_pCameraObserver = nullptr;
	bool										m_bGetSavePos;
	D3DXVECTOR3									m_vecSavePos;

	bool										m_bIsPlayerDie;
private:
	//bool										m_bIs
	void										CreateStage3Cube(void);
	void										CreateStage3Object(void);
private:
	CurStageCube								m_mapTutorialCubeList;
	CurStageCube								m_mapStage1CubeList;
	CurStageCube								m_mapStage2CubeList;
	CurStageCube								m_mapStage3CubeList;

	CurStageObj									m_mapTutorialObjList;
	CurStageObj									m_mapStage1ObjList;
	CurStageObj									m_mapStage2ObjList;
	CurStageObj									m_mapStage3ObjList;
private:
	CurStageCube*								m_mapCurStageCube;
	CurStageObj*								m_mapCurStageObj;

	AllStageParticle							m_mapAllStageParticle;

	Engine::CUBE_COLOR							m_eCurCubeColor = Engine::CUBE_COLOR::CUBE_COLOR_END;

	//±èÀç¿ì
private:
	Engine::CUBE_COLOR							m_ePlayerColColor;

public:

	void SetPlayerColColor(Engine::CUBE_COLOR eColor) { m_ePlayerColColor = eColor; }
	Engine::CUBE_COLOR GetPlayerColColor() { return m_ePlayerColColor; }
public:

	void UnlockDoor();

	bool				GetResetBattery() { return m_bResetBattery; }
	void				SetResetBattery(bool b) { m_bResetBattery = b; }
	void				PlusSaveCount() { m_iSaveCount++; }
	int					GetSaveCount() { return m_iSaveCount; }
private:
	LPDIRECT3DDEVICE9							m_pGraphicDev;

	bool										m_bResetBattery;
	CPlayer*									m_pPlayer;
	int											m_iSaveCount = 0;
};


#endif // CubeMgr_h__
