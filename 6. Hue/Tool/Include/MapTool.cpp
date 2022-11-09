// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "ToolMgr.h"
#include "afxdialogex.h"

#include "ToolView.h"
#include "Cube.h"
#include "Export_Function.h"

// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_iTerrainX(0)
	, m_iTerrainY(0)
	, m_fVertexITV(0)
	, m_fScaleCX(0)
	, m_fScaleCY(false)
	, m_fScaleCZ(false)
	, m_fMovingDist(0)
	, m_fMovingSpeed(0)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_TERRAIN, m_btnTerrainApply);
	DDX_Control(pDX, IDC_TERRAIN_X, m_editTerrainX);
	DDX_Control(pDX, IDC_TERRAIN_Z, m_editTerrainZ);
	DDX_Control(pDX, IDC_TERRAIN_ITV, m_editTerrainITV);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBoxCubeType);
	DDX_Control(pDX, IDC_COMBO2, m_ComboBoxCubeList);
	DDX_Control(pDX, IDC_LIST1, m_ListBoxCubeNum);
	DDX_Control(pDX, IDC_PICTURE, m_PictureCtrlCubePreview);
	DDX_Control(pDX, IDC_CHECK1, m_CheckBtnReset);
	DDX_Control(pDX, IDC_RADIO1, m_RadioBtnClippingOption[Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL]);
	DDX_Control(pDX, IDC_RADIO2, m_RadioBtnClippingOption[Engine::CUBE_CLIPPING_TYPE::CUBE_FRONT]);
	DDX_Control(pDX, IDC_RADIO3, m_RadioBtnClippingOption[Engine::CUBE_CLIPPING_TYPE::CUBE_CullB]);
	DDX_Control(pDX, IDC_RADIO4, m_RadioBtnClippingOption[Engine::CUBE_CLIPPING_TYPE::CUBE_CullL]);
	DDX_Control(pDX, IDC_RADIO5, m_RadioBtnClippingOption[Engine::CUBE_CLIPPING_TYPE::CUBE_CullR]);
	DDX_Control(pDX, IDC_RADIO18, m_RadioBtnClippingOption[Engine::CUBE_CLIPPING_TYPE::CUBE_CullLR]);
	DDX_Control(pDX, IDC_RADIO6, m_RadioBtnCubeColor[Engine::CUBE_COLOR::CUBE_RED]);
	DDX_Control(pDX, IDC_RADIO7, m_RadioBtnCubeColor[Engine::CUBE_COLOR::CUBE_ORANGE]);
	DDX_Control(pDX, IDC_RADIO8, m_RadioBtnCubeColor[Engine::CUBE_COLOR::CUBE_GREEN]);
	DDX_Control(pDX, IDC_RADIO9, m_RadioBtnCubeColor[Engine::CUBE_COLOR::CUBE_YELLOW]);
	DDX_Control(pDX, IDC_RADIO10, m_RadioBtnCubeColor[Engine::CUBE_COLOR::CUBE_BLUE]);
	DDX_Control(pDX, IDC_RADIO11, m_RadioBtnCubeColor[Engine::CUBE_COLOR::CUBE_SKYBLUE]);
	DDX_Control(pDX, IDC_RADIO12, m_RadioBtnCubeColor[Engine::CUBE_COLOR::CUBE_PINK]);
	DDX_Control(pDX, IDC_RADIO13, m_RadioBtnCubeColor[Engine::CUBE_COLOR::CUBE_PURPLE]);
	DDX_Control(pDX, IDC_EDIT11, m_editScaleCX);
	DDX_Control(pDX, IDC_EDIT12, m_editScaleCY);
	DDX_Control(pDX, IDC_EDIT13, m_editScaleCZ);
	DDX_Control(pDX, IDC_RADIO14, m_RadioBtnMovingDir[Engine::CUBE_MOVING_DIR::CUBE_UP]);
	DDX_Control(pDX, IDC_RADIO15, m_RadioBtnMovingDir[Engine::CUBE_MOVING_DIR::CUBE_DOWN]);
	DDX_Control(pDX, IDC_RADIO16, m_RadioBtnMovingDir[Engine::CUBE_MOVING_DIR::CUBE_LEFT]);
	DDX_Control(pDX, IDC_RADIO17, m_RadioBtnMovingDir[Engine::CUBE_MOVING_DIR::CUBE_RIGHT]);
	DDX_Control(pDX, IDC_EDIT10, m_editMovingDist);
	DDX_Control(pDX, IDC_EDIT14, m_editMovingSpeed);
	// User Value
	DDX_Text(pDX, IDC_TERRAIN_X, m_iTerrainX);
	DDX_Text(pDX, IDC_TERRAIN_Z, m_iTerrainY);
	DDX_Text(pDX, IDC_TERRAIN_ITV, m_fVertexITV);
	DDX_Text(pDX, IDC_EDIT11, m_fScaleCX);
	DDX_Text(pDX, IDC_EDIT12, m_fScaleCY);
	DDX_Text(pDX, IDC_EDIT13, m_fScaleCZ);
	DDX_Text(pDX, IDC_EDIT10, m_fMovingDist);
	DDX_Text(pDX, IDC_EDIT14, m_fMovingSpeed);
	DDX_Control(pDX, IDC_CHECK2, m_CheckBtnPicking);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TERRAIN, &CMapTool::OnBnClickedBtnTerrainApply)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMapTool::OnCbnSelchangeCubeType)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangeListCubeNum)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CMapTool::OnCbnSelchangeComboCubeList)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapTool::OnBnClickedButtonModify)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.


BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CToolMgr::GetInstance()->m_pMapTool = this;

	m_ComboBoxCubeType.ResetContent();

	m_ComboBoxCubeType.AddString(L"00. Deco");
	m_ComboBoxCubeType.AddString(L"01. Color");
	m_ComboBoxCubeType.AddString(L"02. Obstacle");

	/************************ (SongSong) ************************/
	/*  컬러 라디오버튼 초기화  */
	for (size_t i = 0; i < Engine::CUBE_COLOR::CUBE_COLOR_END; ++i)
		m_RadioBtnCubeColor[i].EnableWindow(FALSE);

	/************************ (SongSong) ************************/
	/*  스케일 초기화  */
	m_editScaleCX.EnableWindow(FALSE);
	m_editScaleCY.EnableWindow(FALSE);
	m_editScaleCZ.EnableWindow(FALSE);

	/************************ (SongSong) ************************/
	/*  무빙 라디오버튼 초기화  */
	for (size_t i = 0; i < Engine::CUBE_MOVING_DIR::CUBE_MOVING_DIR_END; ++i)
	{
		m_RadioBtnMovingDir[i].SetCheck(FALSE);
		m_RadioBtnMovingDir[i].EnableWindow(FALSE);
	}
	m_editMovingDist.EnableWindow(FALSE);
	m_editMovingSpeed.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

/************************ (SongSong) ************************/
/*  SetUp - Terrain Apply  */
void CMapTool::OnBnClickedBtnTerrainApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int iTerrainX = GetDlgItemInt(IDC_TERRAIN_X);
	int iTerrainZ = GetDlgItemInt(IDC_TERRAIN_Z);
	float iTerrainITV = (float)GetDlgItemInt(IDC_TERRAIN_ITV);
	iTerrainITV = 1.7f;

	if (1 >= iTerrainX || 1 >= iTerrainZ || 0 >= iTerrainITV)
		return;

	// Terrain 버퍼 삭제
	CToolMgr::GetInstance()->m_pResourceMgr->Delete_Buffer(Engine::RESOURCETYPE::RESOURCE_STATIC, L"Terrain");
	Engine::Safe_Delete(CToolMgr::GetInstance()->m_pTerrain);
	// Terrain 버퍼 재생성
	CToolMgr::GetInstance()->CreateTerrain(iTerrainX, iTerrainZ, (int)iTerrainITV);

	/************************ (SongSong) ************************/
	/*  Option 체크  */
	OptionCheck();

	/************************ (SongSong) ************************/
	/*   Reset버튼이 활성화되지 않았을때 실행  */
	if (FALSE == m_CheckBtnReset.GetCheck() && -1 != m_ListBoxCubeNum.GetCurSel())
	{
		/************************ (SongSong) ************************/
		/*  툴매니저 큐브버퍼삭제  */
		for (auto mapiter : CToolMgr::GetInstance()->m_mapCubeVec)
		{
			for (auto iter : mapiter.second)
			{
				Safe_Delete(iter);
			}
			mapiter.second.clear();
			//mapiter.second.shrink_to_fit();
		}
		CToolMgr::GetInstance()->m_mapCubeVec.clear();

		/************************ (SongSong) ************************/
		/*  툴매니저 큐브버퍼생성  */
		for (int z = 0; z < iTerrainZ - 1; ++z)
		{
			for (int x = 0; x < iTerrainX - 1; ++x)
			{
				float fTerrainITV = iTerrainITV / 2.f;

				Engine::CUBE_INFO tCubeInfo;
				tCubeInfo.vPos = { float(x) * iTerrainITV + fTerrainITV, float(z) * iTerrainITV + fTerrainITV, 0.f };
				tCubeInfo.vScale = { fTerrainITV, fTerrainITV, fTerrainITV / 2.f };
				tCubeInfo.eType = m_eCubeClippingType;
				tCubeInfo.tColor = m_eCubeColor;
				lstrcpy(tCubeInfo.tTex.szTex, m_wstrCubeTexKey.c_str());
				tCubeInfo.tTex.iIndex = m_ListBoxCubeNum.GetCurSel();

				//if(FALSE == m_CheckBtnReset.GetCheck()/* && true == m_RadioBtnClippingOption[CUBE_CLIPPING::CullR].GetCheck()*/)
				CToolMgr::GetInstance()->CreateCube(m_wstrCubeTypeKey, tCubeInfo);

				CToolMgr::GetInstance()->m_pToolView->m_pPickExistCube = nullptr;
			}
		}
	}
	/************************ (SongSong) ************************/
	/*   Reset버튼이 활성화 되었을때 실행  */
	else if (TRUE == m_CheckBtnReset.GetCheck())
	{
		/************************ (SongSong) ************************/
		/*  툴매니저 큐브버퍼삭제  */
		for (auto mapiter : CToolMgr::GetInstance()->m_mapCubeVec)
		{
			for (auto iter : mapiter.second)
			{
				Safe_Delete(iter);
			}
			mapiter.second.clear();
		}
		CToolMgr::GetInstance()->m_mapCubeVec.clear();
	}

	UpdateData(FALSE);
}

/************************ (SongSong) ************************/
/*  CubeImage - Type  */
void CMapTool::OnCbnSelchangeCubeType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iIndex = m_ComboBoxCubeType.GetCurSel();

	if (-1 == iIndex)
		return;

	/************************ (SongSong) ************************/
	/*  컨트롤변수 초기화  */
	OptionReset();

	switch (iIndex)
	{
	case 0:
		m_ComboBoxCubeList.AddString(L"00. Aztec");
		m_ComboBoxCubeList.AddString(L"01. Brick");
		m_ComboBoxCubeList.AddString(L"02. Bricks");
		m_ComboBoxCubeList.AddString(L"03. Cobbles");
		m_ComboBoxCubeList.AddString(L"04. RockPool");
		m_ComboBoxCubeList.AddString(L"05. Square");
		m_ComboBoxCubeList.AddString(L"06. Vine");
		m_ComboBoxCubeList.AddString(L"07. WireScenery");
		break;
	case 1:
		m_ComboBoxCubeList.AddString(L"00. BrickColor");
		m_ComboBoxCubeList.AddString(L"01. Crate");
		m_ComboBoxCubeList.AddString(L"02. CrateLarge");
		m_ComboBoxCubeList.AddString(L"03. CrateTall");
		m_ComboBoxCubeList.AddString(L"04. Platform");
		break;
	case 2:
		m_ComboBoxCubeList.AddString(L"00. Skull");
		m_ComboBoxCubeList.AddString(L"01. Rope");
		m_ComboBoxCubeList.AddString(L"02. Laser");
		m_ComboBoxCubeList.AddString(L"03. Spikes");
		m_ComboBoxCubeList.AddString(L"04. Stairs");
		m_ComboBoxCubeList.AddString(L"05. Rock");
		m_ComboBoxCubeList.AddString(L"06. Balloon");
		break;
	}
}

/************************ (SongSong) ************************/
/*  CubeImage - List  */
void CMapTool::OnCbnSelchangeComboCubeList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iTypeIndex = m_ComboBoxCubeType.GetCurSel();
	int iListIndex = m_ComboBoxCubeList.GetCurSel();

	if (-1 == iListIndex)
		return;

	/************************ (SongSong) ************************/
	/*  컨트롤변수 초기화  */
	OptionReset(FALSE);

	switch (iTypeIndex)
	{
		/************************ (SongSong) ************************/
		/*  00. Deco  */
	case 0:
		switch (iListIndex)
		{
		case 0:
		case 1:
		case 2:
			m_ListBoxCubeNum.AddString(L"0");
			m_ListBoxCubeNum.AddString(L"1");
			m_ListBoxCubeNum.AddString(L"2");
			m_ListBoxCubeNum.AddString(L"3");
			break;
		case 3:
			m_ListBoxCubeNum.AddString(L"0");
			m_ListBoxCubeNum.AddString(L"1");
			m_ListBoxCubeNum.AddString(L"2");
			m_ListBoxCubeNum.AddString(L"3");
			m_ListBoxCubeNum.AddString(L"4");
			m_ListBoxCubeNum.AddString(L"5");
			break;
		case 4:
		case 6:
		case 7:
			m_ListBoxCubeNum.AddString(L"0");
			m_ListBoxCubeNum.AddString(L"1");
			m_ListBoxCubeNum.AddString(L"2");
			m_ListBoxCubeNum.AddString(L"3");
			m_ListBoxCubeNum.AddString(L"4");
			break;
		case 5:
			m_ListBoxCubeNum.AddString(L"0");
			m_ListBoxCubeNum.AddString(L"1");
			m_ListBoxCubeNum.AddString(L"2");
			m_ListBoxCubeNum.AddString(L"3");
			m_ListBoxCubeNum.AddString(L"4");
			m_ListBoxCubeNum.AddString(L"5");
			m_ListBoxCubeNum.AddString(L"6");
			m_ListBoxCubeNum.AddString(L"7");
			m_ListBoxCubeNum.AddString(L"8");
			break;
		}
		break;
		/************************ (SongSong) ************************/
		/*  01. Color  */
	case 1:
		switch (iListIndex)
		{
		case 0:
			m_ListBoxCubeNum.AddString(L"0");
			m_ListBoxCubeNum.AddString(L"1");
			m_ListBoxCubeNum.AddString(L"2");
			m_ListBoxCubeNum.AddString(L"3");
			break;
		case 1:
		case 3:
			m_ListBoxCubeNum.AddString(L"0");
			m_ListBoxCubeNum.AddString(L"1");
			break;
		case 2:
		case 4:
			m_ListBoxCubeNum.AddString(L"0");
			break;
		}
		break;

		/************************ (SongSong) ************************/
		/*  2. Obstacle  */
	case 2:
		switch (iListIndex)
		{
			// Skull, Spikes, Balloon, Stairs
		case 0:
		case 3:
		case 6:
			m_ListBoxCubeNum.AddString(L"0");
			break;
		case 4:
			m_ListBoxCubeNum.AddString(L"0");
			m_ListBoxCubeNum.AddString(L"1");
			break;
			// Rope, Laser
		case 1:
		case 2:
			m_ListBoxCubeNum.AddString(L"0");
			m_ListBoxCubeNum.AddString(L"1");
			break;
			// Rock
		case 5:
			m_ListBoxCubeNum.AddString(L"0");
			m_ListBoxCubeNum.AddString(L"1");
			m_ListBoxCubeNum.AddString(L"2");
			break;
		}
		break;

	}
}

/************************ (SongSong) ************************/
/*  CubeImage - Num  */
void CMapTool::OnLbnSelchangeListCubeNum()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int iTypeIndex = m_ComboBoxCubeType.GetCurSel();
	int iListIndex = m_ComboBoxCubeList.GetCurSel();
	int iNumIndex = m_ListBoxCubeNum.GetCurSel();

	if (-1 == iNumIndex)
		return;

	/************************ (SongSong) ************************/
	/*  컨트롤변수 초기화  */
	for (size_t i = 0; i < Engine::CUBE_CLIPPING_TYPE::CUBE_TYPE_END - 3; ++i)
		m_RadioBtnClippingOption[i].SetCheck(FALSE);
	for (size_t i = 0; i < Engine::CUBE_COLOR::CUBE_COLOR_END; ++i)
	{
		m_RadioBtnCubeColor[i].SetCheck(FALSE);
		m_RadioBtnCubeColor[i].EnableWindow(FALSE);
	}

	CToolMgr::GetInstance()->Render_Begin();

	switch (iTypeIndex)
	{
		/************************ (SongSong) ************************/
		/*  00. Deco  */
	case 0:
		switch (iListIndex)
		{
		case 0:
			m_wstrCubeTexKey = L"Texture_CubeDeco_Aztec";
			break;
		case 1:
			m_wstrCubeTexKey = L"Texture_CubeDeco_Brick";
			break;
		case 2:
			m_wstrCubeTexKey = L"Texture_CubeDeco_Bricks";
			break;
		case 3:
			m_wstrCubeTexKey = L"Texture_CubeDeco_Cobbles";
			break;
		case 4:
			m_wstrCubeTexKey = L"Texture_CubeDeco_RockPool";
			break;
		case 5:
			m_wstrCubeTexKey = L"Texture_CubeDeco_Square";
			break;
		case 6:
			m_wstrCubeTexKey = L"Texture_CubeDeco_Vine";
			break;
		case 7:
			m_wstrCubeTexKey = L"Texture_CubeDeco_WireScenery";
			break;
		}

		if (0 == iNumIndex)
			m_RadioBtnClippingOption[Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL].SetCheck(TRUE);
		else
			m_RadioBtnClippingOption[Engine::CUBE_CLIPPING_TYPE::CUBE_CullLR].SetCheck(TRUE);

		m_wstrCubeTypeKey = L"CubeDeco";

		break;

		/************************ (SongSong) ************************/
		/*  01. Color  */
	case 1:
		switch (iListIndex)
		{
		case 0:
			m_wstrCubeTypeKey = L"CubeBrick";
			m_wstrCubeTexKey = L"Texture_CubeColor_BrickColor";
			break;
		case 1:
			m_wstrCubeTypeKey = L"CubeCrate";
			m_wstrCubeTexKey = L"Texture_CubeColor_Crate";
			break;
		case 2:
			m_wstrCubeTypeKey = L"CubeCrate";
			m_wstrCubeTexKey = L"Texture_CubeColor_CrateLarge";
			break;
		case 3:
			m_wstrCubeTypeKey = L"CubeCrate";
			m_wstrCubeTexKey = L"Texture_CubeColor_CrateTall";
			break;
		case 4:
			m_wstrCubeTypeKey = L"CubePlatform";
			m_wstrCubeTexKey = L"Texture_CubeColor_Platform";
		}

		m_RadioBtnClippingOption[Engine::CUBE_CLIPPING_TYPE::CUBE_NORMAL].SetCheck(TRUE);

		for (size_t i = 0; i < Engine::CUBE_COLOR::CUBE_COLOR_END; ++i)
			m_RadioBtnCubeColor[i].EnableWindow(TRUE);
		m_RadioBtnCubeColor[Engine::CUBE_COLOR::CUBE_RED].SetCheck(TRUE);

		// Brick 클래스가 아닌것들
		if (0 != iListIndex)
		{
			m_editScaleCX.EnableWindow(TRUE);
			m_editScaleCY.EnableWindow(TRUE);
			m_fScaleCX = 1.7f;
			m_fScaleCY = 1.7f;
			//m_editScaleCZ.EnableWindow(TRUE);
		}
		// Platform 클래스
		if (4 == iListIndex)
		{
			for (size_t i = 0; i < Engine::CUBE_MOVING_DIR::CUBE_MOVING_DIR_END; ++i)
				m_RadioBtnMovingDir[i].EnableWindow(TRUE);
			m_RadioBtnMovingDir[Engine::CUBE_MOVING_DIR::CUBE_UP].SetCheck(TRUE);

			m_editMovingDist.EnableWindow(TRUE);
			m_editMovingSpeed.EnableWindow(TRUE);

			m_fMovingDist = 1.f;
			m_fMovingSpeed = 1.f;
		}

		break;

		/************************ (SongSong) ************************/
		/*  02. Obstacle  */
	case 2:
		switch (iListIndex)
		{
		case 0:
			m_wstrCubeTexKey = L"Texture_CubeObstacle_Skull";
			m_wstrCubeTypeKey = L"CubeSkull";
			m_RadioBtnClippingOption[Engine::CUBE_CLIPPING_TYPE::CUBE_CullB].SetCheck(TRUE);
			break;
		case 1:
			m_wstrCubeTexKey = L"Texture_CubeObstacle_Rope";
			m_wstrCubeTypeKey = L"CubeRope";
			m_RadioBtnClippingOption[Engine::CUBE_CLIPPING_TYPE::CUBE_FRONT].SetCheck(TRUE);
			break;
		case 2:
			m_wstrCubeTexKey = L"Texture_CubeObstacle_Laser";
			m_wstrCubeTypeKey = L"CubeLaser";
			m_RadioBtnClippingOption[Engine::CUBE_CLIPPING_TYPE::CUBE_FRONT].SetCheck(TRUE);
			break;
		case 3:
			m_wstrCubeTexKey = L"Texture_CubeObstacle_Spikes";
			m_wstrCubeTypeKey = L"CubeSpikes";
			m_RadioBtnClippingOption[Engine::CUBE_CLIPPING_TYPE::CUBE_FRONT].SetCheck(TRUE);
			break;
		case 4:
			m_wstrCubeTexKey = L"Texture_CubeObstacle_Stairs";
			m_wstrCubeTypeKey = L"CubeStairs";
			m_RadioBtnClippingOption[Engine::CUBE_CLIPPING_TYPE::CUBE_FRONT].SetCheck(TRUE);
			break;
		case 5:
			m_wstrCubeTexKey = L"Texture_CubeObstacle_Rock";
			m_wstrCubeTypeKey = L"CubeRock";
			m_RadioBtnClippingOption[Engine::CUBE_CLIPPING_TYPE::CUBE_FRONT].SetCheck(TRUE);
			break;
		case 6:
			m_wstrCubeTexKey = L"Texture_CubeObstacle_Balloon";
			m_wstrCubeTypeKey = L"CubeBalloon";
			m_RadioBtnClippingOption[Engine::CUBE_CLIPPING_TYPE::CUBE_FRONT].SetCheck(TRUE);
			break;
		}

		for (size_t i = 0; i < Engine::CUBE_COLOR::CUBE_COLOR_END; ++i)
			m_RadioBtnCubeColor[i].EnableWindow(TRUE);

		// Laser 클래스
		if (2 == iListIndex)
		{
			for (size_t i = 0; i < Engine::CUBE_COLOR::CUBE_COLOR_END; ++i)
				m_RadioBtnCubeColor[i].EnableWindow(TRUE);
			m_RadioBtnCubeColor[Engine::CUBE_COLOR::CUBE_RED].SetCheck(TRUE);

			m_RadioBtnMovingDir[Engine::CUBE_MOVING_DIR::CUBE_UP].EnableWindow(TRUE);
			m_RadioBtnMovingDir[Engine::CUBE_MOVING_DIR::CUBE_LEFT].EnableWindow(TRUE);
			m_RadioBtnMovingDir[Engine::CUBE_MOVING_DIR::CUBE_LEFT].SetCheck(TRUE);
			m_editMovingDist.EnableWindow(TRUE);
			m_editMovingSpeed.EnableWindow(TRUE);
			m_fMovingDist = 1.f;
			m_fMovingSpeed = 1.f;
		}

		// Stairs 클래스
		if (4 == iListIndex)
		{
			m_RadioBtnMovingDir[Engine::CUBE_MOVING_DIR::CUBE_LEFT].EnableWindow(TRUE);
			m_RadioBtnMovingDir[Engine::CUBE_MOVING_DIR::CUBE_RIGHT].EnableWindow(TRUE);
			m_RadioBtnMovingDir[Engine::CUBE_MOVING_DIR::CUBE_RIGHT].SetCheck(TRUE);
			m_editMovingDist.EnableWindow(TRUE);
			m_editMovingSpeed.EnableWindow(TRUE);
			m_fMovingDist = 1.f;
			m_fMovingSpeed = 1.f;
		}

		break;

	}

	/************************ (SongSong) ************************/
	/*   Control Value Option Check  */
	OptionCheck();

	m_pResourceMgr->SetTexture(Engine::RESOURCE_STATIC, m_wstrCubeTexKey, iNumIndex);
	m_pResourceMgr->Render(L"Buffer_RcTex");
	CToolMgr::GetInstance()->Render_End(m_PictureCtrlCubePreview.m_hWnd);

	UpdateData(FALSE);
}

void CMapTool::OnBnClickedButtonModify()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	Engine::CCube* pExitstCube = CToolMgr::GetInstance()->m_pToolView->m_pPickExistCube;
	if (nullptr == pExitstCube)
	{
		AfxMessageBox(L"선택된 Cube가 없습니다!");
		return;
	}

	/************************ (SongSong) ************************/
	/*  옵션체크  */
	OptionCheck();

	const_cast<Engine::CUBE_CLIPPING_TYPE>(pExitstCube->Get_CubeInfo().eType) = m_eCubeClippingType;
	const_cast<Engine::CUBE_COLOR>(pExitstCube->Get_CubeColor()) = m_eCubeColor;

	/************************ (SongSong) ************************/
	/*  Scale 체크(Crate, Platform 등등)  */
	if (TRUE == m_editScaleCX.IsWindowEnabled() && 0.f != m_fScaleCX
		&& TRUE == m_editScaleCY.IsWindowEnabled() && 0.f != m_fScaleCY)
	{
		const_cast<Engine::CUBE_INFO&>(pExitstCube->Get_CubeInfo()).vPos.y += (float(m_fScaleCY) / 2.f) - pExitstCube->Get_CubeInfo().vScale.y;
		const_cast<Engine::CUBE_INFO&>(pExitstCube->Get_CubeInfo()).vScale = { float(m_fScaleCX) / 2.f, float(m_fScaleCY) / 2.f, pExitstCube->Get_CubeInfo().vScale.z };
	}

	/************************ (SongSong) ************************/
	/*  MovingDetail 체크(Platform 등등)  */
	if (TRUE == m_editMovingDist.IsWindowEnabled() && 0.f != m_fMovingDist
		&& TRUE == m_editMovingSpeed.IsWindowEnabled() && 0.f != m_fMovingSpeed)
	{
		MovingDetailDirCheck(const_cast<Engine::CUBE_INFO&>(pExitstCube->Get_CubeInfo()));
	}

	pExitstCube->SetMaterial(m_eCubeColor);

	AfxMessageBox(L"수정되었습니다!");

	CToolMgr::GetInstance()->m_pToolView->Invalidate(FALSE);
	UpdateData(FALSE);
}

void CMapTool::OnBnClickedButtonSave()
{
	CFileDialog Dlg(
		FALSE,   /* 열기 TRUE, 저장 FALSE */
		L"dat", /* 디폴트 확장자 */
		L"제목없음.dat", /* 디폴트 파일이름 */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", /* 파일 형식 */
		this /* 부모 윈도우 */);

	TCHAR szCurDir[MAX_STR] = L"";

	//현재 작업 경로를 얻어오는 함수
	GetCurrentDirectory(MAX_STR, szCurDir);
	//경로상에 파일명을 제거하는 함수. 제거할 파일명이 없다면 말단 폴더명을 제거함.
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szCurDir; // 절대경로

	if (IDOK == Dlg.DoModal())
	{
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Save Failed");
			return;
		}

		DWORD dwByte = 0;
		TCHAR szCubeType[MIN_STR];
		Engine::CUBE_INFO tCubeInfo;

		for (auto mapiter : CToolMgr::GetInstance()->m_mapCubeVec)
		{
			for (auto iter : mapiter.second)
			{
				lstrcpy(szCubeType, mapiter.first.c_str());
				tCubeInfo = iter->Get_CubeInfo();

				WriteFile(hFile, szCubeType, sizeof(szCubeType), &dwByte, nullptr);
				WriteFile(hFile, &tCubeInfo, sizeof(Engine::CUBE_INFO), &dwByte, nullptr);
			}
		}

		hFile = CreateFile(L"../../Data/Terrain(Tool).dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Save Failed");
			return;
		}


		WriteFile(hFile, &m_iTerrainX, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &m_iTerrainY, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &m_fVertexITV, sizeof(float), &dwByte, nullptr);

		CloseHandle(hFile);
		AfxMessageBox(L"Save Complete");
	}

	Invalidate(FALSE);
}

void CMapTool::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog Dlg(
		TRUE,   /* 열기 TRUE, 저장 FALSE */
		L"dat", /* 디폴트 확장자 */
		L"제목없음.dat", /* 디폴트 파일이름 */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", /* 파일 형식 */
		this /* 부모 윈도우 */);

	TCHAR szCurDir[MAX_STR] = L"";

	//현재 작업 경로를 얻어오는 함수
	GetCurrentDirectory(MAX_STR, szCurDir);
	//경로상에 파일명을 제거하는 함수. 제거할 파일명이 없다면 말단 폴더명을 제거함.
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szCurDir; // 절대경로

	if (IDOK == Dlg.DoModal())
	{
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Load Failed");
			return;
		}

		m_ComboBoxCubeList.ResetContent();
		m_ListBoxCubeNum.ResetContent();
		for (size_t i = 0; i < Engine::CUBE_CLIPPING_TYPE::CUBE_TYPE_END - 3; ++i)
			m_RadioBtnClippingOption[i].SetCheck(FALSE);
		for (size_t i = 0; i < Engine::CUBE_COLOR::CUBE_COLOR_END; ++i)
		{
			m_RadioBtnCubeColor[i].SetCheck(FALSE);
			m_RadioBtnCubeColor[i].EnableWindow(FALSE);
		}

		/************************ (SongSong) ************************/
		/*  픽쳐컨트롤 초기화  */
		CToolMgr::GetInstance()->Render_Begin();
		CToolMgr::GetInstance()->Render_End(m_PictureCtrlCubePreview.m_hWnd);

		/************************ (SongSong) ************************/
		/*  툴매니저 큐브버퍼삭제  */
		for (auto mapiter : CToolMgr::GetInstance()->m_mapCubeVec)
		{
			for (auto iter : mapiter.second)
			{
				Safe_Delete(iter);
			}
			mapiter.second.clear();
		}
		CToolMgr::GetInstance()->m_mapCubeVec.clear();

		DWORD dwByte = 0;
		TCHAR szCubeType[MIN_STR];
		Engine::CUBE_INFO tCubeInfo;

		/************************ (SongSong) ************************/
		/*  툴매니저 큐브버퍼생성  */
		while (TRUE)
		{
			ReadFile(hFile, szCubeType, sizeof(szCubeType), &dwByte, nullptr);
			ReadFile(hFile, &tCubeInfo, sizeof(Engine::CUBE_INFO), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CToolMgr::GetInstance()->CreateCube(szCubeType, tCubeInfo);

			CToolMgr::GetInstance()->m_pToolView->m_pPickExistCube = nullptr;
		}

		/************************ (SongSong) ************************/
		/*  터레인 로드  */
		hFile = CreateFile(L"../../Data/Terrain(Tool).dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Load Failed");
			return;
		}

		ReadFile(hFile, &m_iTerrainX, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &m_iTerrainY, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &m_fVertexITV, sizeof(float), &dwByte, nullptr);

		m_iTerrainX = 100;
		m_iTerrainY = 100;
		m_fVertexITV = 0.f;

		// Terrain 버퍼 삭제
		CToolMgr::GetInstance()->m_pResourceMgr->Delete_Buffer(Engine::RESOURCETYPE::RESOURCE_STATIC, L"Terrain");
		Engine::Safe_Delete(CToolMgr::GetInstance()->m_pTerrain);
		// Terrain 버퍼 재생성
		CToolMgr::GetInstance()->CreateTerrain(m_iTerrainX, m_iTerrainY, (int)m_fVertexITV);

		CloseHandle(hFile);
		AfxMessageBox(L"Load Complete");
	}

	CToolMgr::GetInstance()->Update();
	CToolMgr::GetInstance()->Render();
	//CToolMgr::GetInstance()->m_pToolView->Invalidate(FALSE);
	Invalidate(FALSE);
	UpdateData(FALSE);
}

void CMapTool::OptionCheck(void)
{
	/************************ (SongSong) ************************/
	/*  Clipping Option 체크  */
	for (size_t i = 0; i < Engine::CUBE_CLIPPING_TYPE::CUBE_TYPE_END - 3; ++i)
	{
		if (TRUE == m_RadioBtnClippingOption[i].GetCheck())
		{
			m_eCubeClippingType = Engine::CUBE_CLIPPING_TYPE(i);

			if (Engine::CUBE_CLIPPING_TYPE::CUBE_CullB == i)
				m_eCubeClippingType = Engine::CUBE_CLIPPING_TYPE::CUBE_CullBackAndB;

			break;
		}
	}

	/************************ (SongSong) ************************/
	/*  Color 체크  */
	m_eCubeColor = Engine::CUBE_COLOR::CUBE_COLOR_END;      // 컬러큐브가 아닌것은 End로 초기화하기 떄문에!
	for (size_t i = 0; i < Engine::CUBE_COLOR::CUBE_COLOR_END; ++i)
	{
		if (TRUE == m_RadioBtnCubeColor[i].GetCheck())
		{
			m_eCubeColor = Engine::CUBE_COLOR(i);
			break;
		}
	}
}

void CMapTool::OptionReset(bool _bType/* = true*/)
{
	UpdateData(TRUE);

	/************************ (SongSong) ************************/
	/*  리스트박스 초기화  */
	if (TRUE == _bType)
		m_ComboBoxCubeList.ResetContent();
	m_ListBoxCubeNum.ResetContent();

	/************************ (SongSong) ************************/
	/*  클리핑 라디오버튼 초기화  */
	for (size_t i = 0; i < Engine::CUBE_CLIPPING_TYPE::CUBE_TYPE_END - 3; ++i)
		m_RadioBtnClippingOption[i].SetCheck(FALSE);

	/************************ (SongSong) ************************/
	/*  컬러 라디오버튼 초기화  */
	for (size_t i = 0; i < Engine::CUBE_COLOR::CUBE_COLOR_END; ++i)
	{
		m_RadioBtnCubeColor[i].SetCheck(FALSE);
		m_RadioBtnCubeColor[i].EnableWindow(FALSE);
	}

	/************************ (SongSong) ************************/
	/*  스케일 초기화  */
	m_editScaleCX.EnableWindow(FALSE);
	m_editScaleCY.EnableWindow(FALSE);
	m_editScaleCZ.EnableWindow(FALSE);
	m_fScaleCX = 0.f;
	m_fScaleCY = 0.f;
	m_fScaleCZ = 0.f;

	/************************ (SongSong) ************************/
	/*  무빙 라디오버튼 초기화  */
	for (size_t i = 0; i < Engine::CUBE_MOVING_DIR::CUBE_MOVING_DIR_END; ++i)
	{
		m_RadioBtnMovingDir[i].SetCheck(FALSE);
		m_RadioBtnMovingDir[i].EnableWindow(FALSE);
	}
	m_editMovingDist.EnableWindow(FALSE);
	m_fMovingDist = 0;
	m_editMovingSpeed.EnableWindow(FALSE);
	m_fMovingSpeed = 0.f;

	/************************ (SongSong) ************************/
	/*  픽쳐컨트롤 초기화  */
	CToolMgr::GetInstance()->Render_Begin();
	CToolMgr::GetInstance()->Render_End(m_PictureCtrlCubePreview.m_hWnd);

	UpdateData(FALSE);
}

HRESULT CMapTool::MovingDetailDirCheck(Engine::CUBE_INFO& _tCubeInfo)
{
	/************************ (SongSong) ************************/
	/*  무빙 라디오버튼 체크  */
	for (size_t i = 0; i < Engine::CUBE_MOVING_DIR::CUBE_MOVING_DIR_END; ++i)
	{
		if (TRUE == m_RadioBtnMovingDir[i].GetCheck())
		{
			_tCubeInfo.tPlatformInfo.eDir = Engine::CUBE_MOVING_DIR(i);
			_tCubeInfo.tPlatformInfo.fDist = (float)m_fMovingDist;
			_tCubeInfo.tPlatformInfo.fSpeed = m_fMovingSpeed;

			return S_OK;
		}
		else if (Engine::CUBE_MOVING_DIR::CUBE_MOVING_DIR_END - 1 == i)
			return E_FAIL;
	}
}

HRESULT CMapTool::ResourceForStage(void)
{
	// Basic, Deco 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeDeco_Aztec",
		L"../Bin/HueResources/Box/Aztec/Aztec_0%d.png",
		4);

	// Basic, Deco 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeDeco_Brick",
		L"../Bin/HueResources/Box/Brick/Brick_0%d.png",
		4);

	// Basic, Deco 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeDeco_Bricks",
		L"../Bin/HueResources/Box/Bricks/Bricks_0%d.png",
		4);

	// Basic, Deco 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeDeco_Cobbles",
		L"../Bin/HueResources/Box/Cobbles/Cobbles_0%d.png",
		6);

	// Basic, Deco 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeDeco_RockPool",
		L"../Bin/HueResources/Box/RockPool/RockPool_0%d.png",
		5);

	// Basic, Deco 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeDeco_Square",
		L"../Bin/HueResources/Box/Square/Square_0%d.png",
		9);

	// Basic, Deco 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeDeco_Vine",
		L"../Bin/HueResources/Box/Vine/Vine_0%d.png",
		5);

	// Basic, Deco 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeDeco_WireScenery",
		L"../Bin/HueResources/Box/WireScenery/WireScenery_0%d.png",
		5);

	// Color 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeColor_BrickColor",
		L"../Bin/HueResources/Box/BrickColor/BrickColor_0%d.png",
		4);

	// Color 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeColor_Crate",
		L"../Bin/HueResources/Box/Crate/Crate_0%d.png",
		2);

	// Color 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeColor_CrateLarge",
		L"../Bin/HueResources/Box/CrateLarge/CrateLarge_0%d.png",
		1);

	// Color 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeColor_CrateTall",
		L"../Bin/HueResources/Box/CrateTall/CrateTall_0%d.png",
		2);

	// Platform
	// Color 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeColor_Platform",
		L"../Bin/HueResources/Obstacle/MovingPlatform/MovingPlatform_0%d.png",
		1);

	// SKULL
	// Color 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeObstacle_Skull",
		L"../Bin/HueResources/Obstacle/Skull/Skull_0%d.png",
		1);

	// Rope
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeObstacle_Rope",
		L"../Bin/HueResources/Box/Rope/Rope_0%d.png",
		1);

	// Laser
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeObstacle_Laser",
		L"../Bin/HueResources/Box/Laser/Laser_0%d.png",
		3);

	// Spikes
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeObstacle_Spikes",
		L"../Bin/HueResources/Obstacle/Spikes/Spikes_0%d.png",
		2);

	// Stairs
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeObstacle_Stairs",
		L"../Bin/HueResources/Background/Stairs/Stairs_0%d.png",
		2);

	// Rock
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeObstacle_Rock",
		L"../Bin/HueResources/Obstacle/ColorRock/ColorRock_0%d.png",
		3);

	// Balloon
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_CubeObstacle_Balloon",
		L"../Bin/HueResources/Obstacle/Balloon/Balloon_0%d.png",
		1);


	return S_OK;
}
