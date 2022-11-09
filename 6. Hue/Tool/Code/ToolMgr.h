#ifndef ToolMgr_h__
#define ToolMgr_h__


#include "Engine_Include.h"

BEGIN(Engine)

class  CGraphicDev;
class  CResourceMgr;
class  CKeyMgr;
class  CTimeMgr;
class  CCube;
class  CObj;
class  CGameObject;
END

class COrthoCamera;
class CTerrain;
class CMapTool;
class CToolView;
class CObjectTool;
class CToolMgr
{
	DECLARE_SINGLETON(CToolMgr)
private:
	CToolMgr();
public:
	~CToolMgr();

public:
	HRESULT		Initialize(HWND _hWnd);
	void	    Render();
	void		Update();
	void		Release(void);

public:
	void		Render_Begin();
	void		Render_End(HWND hWnd = nullptr);

public:
	HRESULT		ResourceForStage(void);

public:
	void Set_Handle(HWND _hWnd);

public:
	//기본 매니저, 핸들......
	HWND m_hWnd;
	Engine::CTimeMgr*			m_pTimeMgr = nullptr;
	Engine::CGraphicDev*		m_pGraphicDev = nullptr;
	Engine::CKeyMgr*			m_pKeyMgr = nullptr;
	Engine::CResourceMgr*		m_pResourceMgr = nullptr;
	LPDIRECT3DDEVICE9			m_pDevice = nullptr;

public:
	//모든 Toolview.... 멤버로
	TOOL_ID			m_eToolID;
	CToolView*		m_pToolView = nullptr;
	CMapTool*		m_pMapTool = nullptr;
	CObjectTool*	m_pObjectTool = nullptr;
public:
	void		SetUpLight();

public:
	//카메라, 투영..
	COrthoCamera* m_pOrthoCamera;
	void		Setup_Proj();
	void		Setup_ClieProj();

public:
	//Etc
	CTerrain* m_pTerrain = nullptr;
	void CreateTerrain(int vtxX, int vtxZ, int vtxItv);

public:
	map<wstring, list<Engine::CCube*>> m_mapCubeVec;
	map<wstring, list<Engine::CObj*>> m_mapObjList;
	void CreateCube(const wstring& wstrCubeKey, Engine::CUBE_INFO& _tCubeInfo);
	void CreateObject(const wstring& wstrObjKey, Engine::OBJ_INFO& _tObjInfo);
};


#endif // ToolMgr_h__
