// ObjectTool.cpp : 구현 파일입니다.
#include "stdafx.h"
#include "Tool.h"
#include "ObjectTool.h"
#include "afxdialogex.h"

#include "Export_Function.h"
#include "ToolMgr.h"

#include "MapTool.h"

#include "Obj.h"
#include "ToolView.h"
// CObjectTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjectTool, CDialog)

CObjectTool::CObjectTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_OBJECTTOOL, pParent)
	, m_pResourceMgr(Engine::Get_ResourceMgr())
	, m_fPosX(0)
	, m_fPosY(0)
	, m_fPosZ(0)
	, m_fRotX(0)
	, m_fRotY(0)
	, m_fRotZ(0)
	, m_fScaleX(0)
	, m_fScaleY(0)
	, m_fScaleZ(0)
{

}

CObjectTool::~CObjectTool()
{
}

void CObjectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO1, m_ComboBoxObjectType);
	DDX_Control(pDX, IDC_LIST1, m_ListBoxObjectNum);
	DDX_Control(pDX, IDC_PICTURE, m_PictureCtrlObjectPreview);
	DDX_Text(pDX, IDC_EDIT1, m_fPosX);
	DDX_Text(pDX, IDC_EDIT2, m_fPosY);
	DDX_Text(pDX, IDC_EDIT3, m_fPosZ);
	DDX_Text(pDX, IDC_EDIT4, m_fRotX);
	DDX_Text(pDX, IDC_EDIT5, m_fRotY);
	DDX_Text(pDX, IDC_EDIT6, m_fRotZ);
	DDX_Text(pDX, IDC_EDIT7, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT8, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT9, m_fScaleZ);
	DDX_Control(pDX, IDC_LIST2, m_ObjectListBox);
}


BEGIN_MESSAGE_MAP(CObjectTool, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CObjectTool::OnCbnSelchangeCombo1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CObjectTool::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDOK2, &CObjectTool::OnBnClickedSave)
	ON_BN_CLICKED(IDCANCEL2, &CObjectTool::OnBnClickedLoad)
	ON_LBN_SELCHANGE(IDC_LIST2, &CObjectTool::OnLbnSelchangeObjectList)
	ON_BN_CLICKED(IDC_BUTTON2, &CObjectTool::OnBnClickedButtonApply)
END_MESSAGE_MAP()


// CObjectTool 메시지 처리기입니다.


BOOL CObjectTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	//CToolMgr::GetInstance()->m_pMapTool .m_ComboBoxCubeList = nullptr;

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CToolMgr::GetInstance()->m_pObjectTool = this;

	m_ComboBoxObjectType.ResetContent();

	m_ComboBoxObjectType.AddString(L"00. Axe");
	m_ComboBoxObjectType.AddString(L"01. Boat");
	m_ComboBoxObjectType.AddString(L"02. Boss");
	m_ComboBoxObjectType.AddString(L"03. Candle");
	m_ComboBoxObjectType.AddString(L"04. Chandlier");

	m_ComboBoxObjectType.AddString(L"05. Door");
	m_ComboBoxObjectType.AddString(L"06. DoorLock");
	m_ComboBoxObjectType.AddString(L"07. Etc");
	m_ComboBoxObjectType.AddString(L"08. FirePot");
	m_ComboBoxObjectType.AddString(L"09. Grass");

	m_ComboBoxObjectType.AddString(L"10. Key");
	m_ComboBoxObjectType.AddString(L"11. Lantern");
	m_ComboBoxObjectType.AddString(L"12. Mushroom");
	m_ComboBoxObjectType.AddString(L"13. Pipe");
	m_ComboBoxObjectType.AddString(L"14. PotPlant");

	m_ComboBoxObjectType.AddString(L"15. Rope");
	m_ComboBoxObjectType.AddString(L"16. Statue");
	m_ComboBoxObjectType.AddString(L"17. Topiary");
	m_ComboBoxObjectType.AddString(L"18. Tree");
	m_ComboBoxObjectType.AddString(L"19. Trinket");

	m_ComboBoxObjectType.AddString(L"20. Trophie");
	m_ComboBoxObjectType.AddString(L"21. Vase");

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CObjectTool::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 픽쳐컨트롤 초기화
	CToolMgr::GetInstance()->Render_Begin();
	CToolMgr::GetInstance()->Render_End(m_PictureCtrlObjectPreview.m_hWnd);

	// 콤보박스 선택한 번호 가져오기
	int iTypeIndex = m_ComboBoxObjectType.GetCurSel();

	// 리스트 초기화
	m_ListBoxObjectNum.ResetContent();

	// 가져온 번호로 리스트 출력 결정
	switch (iTypeIndex)
	{
	case 0:// Axe
		m_ListBoxObjectNum.AddString(L"Axe_00");
		break;
	case 1:// Boat
		m_ListBoxObjectNum.AddString(L"Boat_00");
		m_ListBoxObjectNum.AddString(L"Boat_01");
		break;
	case 2:// Boss
		m_ListBoxObjectNum.AddString(L"Boss_00");
		break;
	case 3:// Candle
		m_ListBoxObjectNum.AddString(L"Candle_00");
		m_ListBoxObjectNum.AddString(L"Candle_01");
		m_ListBoxObjectNum.AddString(L"Candle_02");
		m_ListBoxObjectNum.AddString(L"Candle_03");
		break;
	case 4:// Chandlier
		m_ListBoxObjectNum.AddString(L"Chandlier_00");
		break;
		break;
	case 5:// Door
		m_ListBoxObjectNum.AddString(L"Door_00");
		break;
	case 6:// DoorLock
		m_ListBoxObjectNum.AddString(L"DoorLock_00");
		break;
	case 7:// Etc
		m_ListBoxObjectNum.AddString(L"Etc_00");
		m_ListBoxObjectNum.AddString(L"Etc_01");
		m_ListBoxObjectNum.AddString(L"Etc_02");
		m_ListBoxObjectNum.AddString(L"Etc_03");
		m_ListBoxObjectNum.AddString(L"Etc_04");
		m_ListBoxObjectNum.AddString(L"Etc_05");
		break;
	case 8:// FirePot
		m_ListBoxObjectNum.AddString(L"FirePot_00");
		m_ListBoxObjectNum.AddString(L"FirePot_01");
		m_ListBoxObjectNum.AddString(L"FirePot_02");
		m_ListBoxObjectNum.AddString(L"FirePot_03");
		m_ListBoxObjectNum.AddString(L"FirePot_04");
		m_ListBoxObjectNum.AddString(L"FirePot_05");
		m_ListBoxObjectNum.AddString(L"FirePot_06");
		break;
	case 9:// Grass
		m_ListBoxObjectNum.AddString(L"Grass_00");
		m_ListBoxObjectNum.AddString(L"Grass_01");
		m_ListBoxObjectNum.AddString(L"Grass_02");
		m_ListBoxObjectNum.AddString(L"Grass_03");
		m_ListBoxObjectNum.AddString(L"Grass_04");
		m_ListBoxObjectNum.AddString(L"Grass_05");
		m_ListBoxObjectNum.AddString(L"Grass_06");
		m_ListBoxObjectNum.AddString(L"Grass_07");
		break;
	case 10:// Key
		m_ListBoxObjectNum.AddString(L"Key_00");
		break;
	case 11:// Lantern
		m_ListBoxObjectNum.AddString(L"Lantern_00");
		m_ListBoxObjectNum.AddString(L"Lantern_01");
		m_ListBoxObjectNum.AddString(L"Lantern_02");
		break;
	case 12:// Mushroom
		m_ListBoxObjectNum.AddString(L"Mushroom_00");
		m_ListBoxObjectNum.AddString(L"Mushroom_01");
		m_ListBoxObjectNum.AddString(L"Mushroom_02");
		m_ListBoxObjectNum.AddString(L"Mushroom_03");
		m_ListBoxObjectNum.AddString(L"Mushroom_04");
		m_ListBoxObjectNum.AddString(L"Mushroom_05");
		m_ListBoxObjectNum.AddString(L"Mushroom_06");
		m_ListBoxObjectNum.AddString(L"Mushroom_07");
		m_ListBoxObjectNum.AddString(L"Mushroom_08");
		m_ListBoxObjectNum.AddString(L"Mushroom_09");
		m_ListBoxObjectNum.AddString(L"Mushroom_10");
		m_ListBoxObjectNum.AddString(L"Mushroom_11");
		m_ListBoxObjectNum.AddString(L"Mushroom_12");
		m_ListBoxObjectNum.AddString(L"Mushroom_13");
		m_ListBoxObjectNum.AddString(L"Mushroom_14");
		m_ListBoxObjectNum.AddString(L"Mushroom_15");
		m_ListBoxObjectNum.AddString(L"Mushroom_16");
		m_ListBoxObjectNum.AddString(L"Mushroom_17");
		break;
	case 13:// Pipe
		m_ListBoxObjectNum.AddString(L"Pipe_00");
		m_ListBoxObjectNum.AddString(L"Pipe_01");
		m_ListBoxObjectNum.AddString(L"Pipe_02");
		m_ListBoxObjectNum.AddString(L"Pipe_03");
		m_ListBoxObjectNum.AddString(L"Pipe_04");
		m_ListBoxObjectNum.AddString(L"Pipe_05");
		m_ListBoxObjectNum.AddString(L"Pipe_06");
		m_ListBoxObjectNum.AddString(L"Pipe_07");
		m_ListBoxObjectNum.AddString(L"Pipe_08");
		break;
	case 14:// PotPlant
		m_ListBoxObjectNum.AddString(L"PotPlant_00");
		m_ListBoxObjectNum.AddString(L"PotPlant_01");
		break;
	case 15:// Rope
		m_ListBoxObjectNum.AddString(L"Rope_00");
		break;
	case 16:// Statue
		m_ListBoxObjectNum.AddString(L"Statue_00");
		m_ListBoxObjectNum.AddString(L"Statue_01");
		m_ListBoxObjectNum.AddString(L"Statue_02");
		m_ListBoxObjectNum.AddString(L"Statue_03");
		m_ListBoxObjectNum.AddString(L"Statue_04");
		m_ListBoxObjectNum.AddString(L"Statue_05");
		m_ListBoxObjectNum.AddString(L"Statue_06");
		m_ListBoxObjectNum.AddString(L"Statue_07");
		break;
	case 17:// Topiary
		m_ListBoxObjectNum.AddString(L"Topiary_00");
		m_ListBoxObjectNum.AddString(L"Topiary_01");
		m_ListBoxObjectNum.AddString(L"Topiary_02");
		m_ListBoxObjectNum.AddString(L"Topiary_03");
		m_ListBoxObjectNum.AddString(L"Topiary_04");
		m_ListBoxObjectNum.AddString(L"Topiary_05");
		break;
	case 18:// Tree
		m_ListBoxObjectNum.AddString(L"Tree_00");
		m_ListBoxObjectNum.AddString(L"Tree_01");
		m_ListBoxObjectNum.AddString(L"Tree_02");
		m_ListBoxObjectNum.AddString(L"Tree_03");
		m_ListBoxObjectNum.AddString(L"Tree_04");
		m_ListBoxObjectNum.AddString(L"Tree_05");
		break;
	case 19:// Trinket
		m_ListBoxObjectNum.AddString(L"Trinket_00");
		m_ListBoxObjectNum.AddString(L"Trinket_01");
		m_ListBoxObjectNum.AddString(L"Trinket_02");
		m_ListBoxObjectNum.AddString(L"Trinket_03");
		m_ListBoxObjectNum.AddString(L"Trinket_04");
		m_ListBoxObjectNum.AddString(L"Trinket_05");
		break;
	case 20:// Trophie
		m_ListBoxObjectNum.AddString(L"Trophie_00");
		m_ListBoxObjectNum.AddString(L"Trophie_01");
		m_ListBoxObjectNum.AddString(L"Trophie_02");
		m_ListBoxObjectNum.AddString(L"Trophie_03");
		m_ListBoxObjectNum.AddString(L"Trophie_04");
		m_ListBoxObjectNum.AddString(L"Trophie_05");
		m_ListBoxObjectNum.AddString(L"Trophie_06");
		m_ListBoxObjectNum.AddString(L"Trophie_07");
		m_ListBoxObjectNum.AddString(L"Trophie_08");
		m_ListBoxObjectNum.AddString(L"Trophie_09");
		m_ListBoxObjectNum.AddString(L"Trophie_10");
		m_ListBoxObjectNum.AddString(L"Trophie_11");
		m_ListBoxObjectNum.AddString(L"Trophie_12");
		break;
	case 21:// Vase
		m_ListBoxObjectNum.AddString(L"Vase_00");
		m_ListBoxObjectNum.AddString(L"Vase_01");
		m_ListBoxObjectNum.AddString(L"Vase_02");
		m_ListBoxObjectNum.AddString(L"Vase_03");
		m_ListBoxObjectNum.AddString(L"Vase_04");
		m_ListBoxObjectNum.AddString(L"Vase_05");
		m_ListBoxObjectNum.AddString(L"Vase_06");
		m_ListBoxObjectNum.AddString(L"Vase_07");
		m_ListBoxObjectNum.AddString(L"Vase_08");
		m_ListBoxObjectNum.AddString(L"Vase_09");
		m_ListBoxObjectNum.AddString(L"Vase_10");
		m_ListBoxObjectNum.AddString(L"Vase_11");
		m_ListBoxObjectNum.AddString(L"Vase_12");
		m_ListBoxObjectNum.AddString(L"Vase_13");
		break;
	default:
		break;
	}
}


void CObjectTool::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 픽쳐컨트롤 초기화
	CToolMgr::GetInstance()->Render_Begin();


	UpdateData(TRUE);

	int iTypeIndex = m_ComboBoxObjectType.GetCurSel();
	int iListIndex = m_ListBoxObjectNum.GetCurSel();

	if (-1 == iListIndex)
		return;

	//// 오브젝트 
	//switch (iListIndex)
	//{
	//case 0:
	//	m_wstrCubeTypeKey = L"Object_Door";
	//	break;
	//default:
	//	break;
	//}


	// 오브젝트 텍스쳐 키 && 클래스 선택
	//CToolMgr::GetInstance()->Render_Begin();
	switch (iTypeIndex)
	{
	case 0:
		m_wstrCubeTexKey = L"Texture_Object_Axe";
		m_wstrCubeTypeKey = L"Object_Axe";
		break;
	case 1:
		m_wstrCubeTexKey = L"Texture_Object_Boat";
		m_wstrCubeTypeKey = L"Object_Boat";
		break;
	case 2:
		m_wstrCubeTexKey = L"Texture_Object_Boss";
		m_wstrCubeTypeKey = L"Object_Boss";
		break;
	case 3:
		m_wstrCubeTexKey = L"Texture_Object_Candle";
		m_wstrCubeTypeKey = L"Object_Candle";
		break;
	case 4:
		m_wstrCubeTexKey = L"Texture_Object_Chandlier";
		m_wstrCubeTypeKey = L"Object_Chandlier";
		break;
	case 5:
		m_wstrCubeTexKey = L"Texture_Object_Door";
		m_wstrCubeTypeKey = L"Object_Door";
		break;
	case 6:
		m_wstrCubeTexKey = L"Texture_Object_DoorLock";
		m_wstrCubeTypeKey = L"Object_DoorLock";
		break;
	case 7:
		m_wstrCubeTexKey = L"Texture_Object_Etc";
		m_wstrCubeTypeKey = L"Object_Etc";
		break;
	case 8:
		m_wstrCubeTexKey = L"Texture_Object_FirePot";
		m_wstrCubeTypeKey = L"Object_FirePot";
		break;
	case 9:
		m_wstrCubeTexKey = L"Texture_Object_Grass";
		m_wstrCubeTypeKey = L"Object_Grass";
		break;
	case 10:
		m_wstrCubeTexKey = L"Texture_Object_Key";
		m_wstrCubeTypeKey = L"Object_Key";
		break;
	case 11:
		m_wstrCubeTexKey = L"Texture_Object_Lantern";
		m_wstrCubeTypeKey = L"Object_Lantern";
		break;
	case 12:
		m_wstrCubeTexKey = L"Texture_Object_Mushroom";
		m_wstrCubeTypeKey = L"Object_Mushroom";
		break;
	case 13:
		m_wstrCubeTexKey = L"Texture_Object_Pipe";
		m_wstrCubeTypeKey = L"Object_Pipe";
		break;
	case 14:
		m_wstrCubeTexKey = L"Texture_Object_PotPlant";
		m_wstrCubeTypeKey = L"Object_PotPlant";
		break;
	case 15:
		m_wstrCubeTexKey = L"Texture_Object_Rope";
		m_wstrCubeTypeKey = L"Object_Rope";
		break;
	case 16:
		m_wstrCubeTexKey = L"Texture_Object_Statue";
		m_wstrCubeTypeKey = L"Object_Statue";
		break;
	case 17:
		m_wstrCubeTexKey = L"Texture_Object_Topiary";
		m_wstrCubeTypeKey = L"Object_Topiary";
		break;
	case 18:
		m_wstrCubeTexKey = L"Texture_Object_Tree";
		m_wstrCubeTypeKey = L"Object_Tree";
		break;
	case 19:
		m_wstrCubeTexKey = L"Texture_Object_Trinket";
		m_wstrCubeTypeKey = L"Object_Trinket";
		break;
	case 20:
		m_wstrCubeTexKey = L"Texture_Object_Trophie";
		m_wstrCubeTypeKey = L"Object_Trophie";
		break;
	case 21:
		m_wstrCubeTexKey = L"Texture_Object_Vase";
		m_wstrCubeTypeKey = L"Object_Vase";
		break;
	default:
		break;
	}
	m_pResourceMgr->SetTexture(Engine::RESOURCE_STATIC, m_wstrCubeTexKey, iListIndex);
	m_pResourceMgr->Render(L"Buffer_RcTex");

	CToolMgr::GetInstance()->Render_End(m_PictureCtrlObjectPreview.m_hWnd);
	UpdateData(FALSE);
}

HRESULT CObjectTool::ResourceForStage(void)
{
	// *** Object *** 
	// Axe 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Axe",
		L"../Bin/HueResources/Object/Axe/Axe_0%d.png",
		1);
	// Boat 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Boat",
		L"../Bin/HueResources/Object/Boat/Boat_0%d.png",
		2);
	// Boss 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Boss",
		L"../Bin/HueResources/Object/Boss/Boss_0%d.png",
		1);
	// Candle 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Candle",
		L"../Bin/HueResources/Object/Candle/Candle_0%d.png",
		4);
	// Chandlier 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Chandlier",
		L"../Bin/HueResources/Object/Chandlier/Chandlier_0%d.png",
		1);
	// Door 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Door",
		L"../Bin/HueResources/Object/Door/Door_0%d.png",
		1);
	// DoorLock 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_DoorLock",
		L"../Bin/HueResources/Object/DoorLock/DoorLock_0%d.png",
		1);
	// Etc 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Etc",
		L"../Bin/HueResources/Object/Etc/Etc_0%d.png",
		6);
	// FirePot 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_FirePot",
		L"../Bin/HueResources/Object/FirePot/FirePot_0%d.png",
		7);
	// Grass 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Grass",
		L"../Bin/HueResources/Object/Grass/Grass_0%d.png",
		8);
	// Key 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Key",
		L"../Bin/HueResources/Object/Key/Key_0%d.png",
		1);
	// Lantern 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Lantern",
		L"../Bin/HueResources/Object/Lantern/Lantern_0%d.png",
		3);
	// Mushroom 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Mushroom",
		L"../Bin/HueResources/Object/Mushroom/Mushroom_0%d.png",
		18);
	// Pipe 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Pipe",
		L"../Bin/HueResources/Object/Pipe/Pipe_0%d.png",
		9);
	// PotPlant 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_PotPlant",
		L"../Bin/HueResources/Object/PotPlant/PotPlant_0%d.png",
		2);
	// Rope 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Rope",
		L"../Bin/HueResources/Object/Rope/Rope_0%d.png",
		1);
	// Statue 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Statue",
		L"../Bin/HueResources/Object/Statue/Statue_0%d.png",
		8);
	// Topiary 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Topiary",
		L"../Bin/HueResources/Object/Topiary/Topiary_0%d.png",
		6);
	// Tree 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Tree",
		L"../Bin/HueResources/Object/Tree/Tree_0%d.png",
		6);
	// Trinket 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Trinket",
		L"../Bin/HueResources/Object/Trinket/Trinket_0%d.png",
		6);
	// Trophie 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Trophie",
		L"../Bin/HueResources/Object/Trophie/Trophie_0%d.png",
		13);
	// Vase 텍스쳐
	m_pResourceMgr->Add_Texture(CToolMgr::GetInstance()->m_pDevice,
		Engine::RESOURCE_STATIC,
		Engine::TEX_NORMAL,
		L"Texture_Object_Vase",
		L"../Bin/HueResources/Object/Vase/Vase_0%d.png",
		14);

	return S_OK;
}

void CObjectTool::UpdateInfo(D3DXVECTOR3& vPos, D3DXVECTOR3& vRot, D3DXVECTOR3& vScale, bool bIsUpdateData)
{
	// 정보 가져오기
	if (bIsUpdateData)
	{
		/*UpdateData(true);
		m_fPosX = vPos.x;
		m_fPosY = vPos.y;
		m_fPosZ = vPos.z;
		m_fRotX = vRot.x;
		m_fRotY = vRot.y;
		m_fRotZ = vRot.z;
		m_fScaleX = vScale.x;
		m_fScaleY = vScale.y;
		m_fScaleZ = vScale.z;*/
	}
	// 정보 입력
	else
	{
		m_fPosX = vPos.x;
		m_fPosY = vPos.y;
		m_fPosZ = vPos.z;
		m_fRotX = vRot.x;
		m_fRotY = vRot.y;
		m_fRotZ = vRot.z;
		m_fScaleX = vScale.x;
		m_fScaleY = vScale.y;
		m_fScaleZ = vScale.z;
		UpdateData(false);
	}

}

// 저장
void CObjectTool::OnBnClickedSave()
{
	CFileDialog Dlg(
		FALSE,	/* 열기 TRUE, 저장 FALSE */
		L"objdat", /* 디폴트 확장자 */
		L"ObjData00.objdat", /* 디폴트 파일이름 */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.objdat)|*.objdat||", /* 파일 형식 */
		this /* 부모 윈도우 */);

	TCHAR szCurDir[MAX_STR] = L"";
	GetCurrentDirectory(MAX_STR, szCurDir);
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
		Engine::OBJ_INFO tObjInfo;
		for (auto mapiter : CToolMgr::GetInstance()->m_mapObjList)
		{
			for (auto iter : mapiter.second)
			{
				lstrcpy(szCubeType, mapiter.first.c_str());
				tObjInfo = iter->Get_CubeInfo();

				WriteFile(hFile, szCubeType, sizeof(szCubeType), &dwByte, nullptr);
				WriteFile(hFile, &tObjInfo, sizeof(Engine::OBJ_INFO), &dwByte, nullptr);
			}
		}

		hFile = CreateFile(L"../../Data/Terrain(Tool).dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			AfxMessageBox(L"Save Failed");
			return;
		}
	/*	WriteFile(hFile, &m_iTerrainX, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &m_iTerrainY, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &m_fVertexITV, sizeof(float), &dwByte, nullptr);*/

		CloseHandle(hFile);
		AfxMessageBox(L"Save Complete");
	}

	Invalidate(FALSE);
}

// 불러오기
void CObjectTool::OnBnClickedLoad()
{
	UpdateData(TRUE);

	// 충돌방지 - 현재 ToolView 비우기



	CFileDialog Dlg(
		TRUE,	/* 열기 TRUE, 저장 FALSE */
		L"objdat", /* 디폴트 확장자 */
		L"ObjData00.objdat", /* 디폴트 파일이름 */
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.objdat)|*.objdat||", /* 파일 형식 */
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


		CToolMgr::GetInstance()->Render_Begin();
		CToolMgr::GetInstance()->Render_End(m_PictureCtrlObjectPreview.m_hWnd);

		CToolMgr::GetInstance()->m_pToolView->PickCubeSendToEngine();

		// 비우기
		for (auto mapiter : CToolMgr::GetInstance()->m_mapObjList)
		{
			for (auto iter : mapiter.second)
			{
				Safe_Delete(iter);
			}
			mapiter.second.clear();
		}
		CToolMgr::GetInstance()->m_mapObjList.clear();

		m_ObjectListBox.ResetContent();

		DWORD dwByte = 0;
		TCHAR szCubeType[MIN_STR];
		Engine::OBJ_INFO tObjInfo;
		CToolMgr::GetInstance()->m_pToolView->iCountKey = 0;

		while (TRUE)
		{
			ReadFile(hFile, szCubeType, sizeof(szCubeType), &dwByte, nullptr);
			ReadFile(hFile, &tObjInfo, sizeof(Engine::OBJ_INFO), &dwByte, nullptr);
			if (0 == dwByte)
				break;
			CToolMgr::GetInstance()->m_pToolView->iCountKey++;
			CToolMgr::GetInstance()->CreateObject(szCubeType, tObjInfo);
			CToolMgr::GetInstance()->m_pToolView->m_pPickExistCube = nullptr;
		}


		//************************ (SongSong) ************************/
		//*  터레인 로드  */
		//hFile = CreateFile(L"../../Data/Terrain(Tool).dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		//if (INVALID_HANDLE_VALUE == hFile)
		//{
		//	AfxMessageBox(L"Load Failed");
		//	return;
		//}

		//// Terrain 버퍼 삭제
		//CToolMgr::GetInstance()->m_pResourceMgr->Delete_Buffer(Engine::RESOURCETYPE::RESOURCE_STATIC, L"Terrain");
		//Engine::Safe_Delete(CToolMgr::GetInstance()->m_pTerrain);

		CloseHandle(hFile);
		AfxMessageBox(L"Load Complete");
	}


	for (auto& mapiter : CToolMgr::GetInstance()->m_mapObjList)
	{
		for (list<Engine::CObj*>::iterator& listIter = mapiter.second.begin(); listIter != mapiter.second.end(); )
		{
			// 정보 보내기
			wstring Convert(&(*listIter)->Get_CubeInfo().tTex.szTex[0]);
			wstring SumKey = Convert + L"_" + to_wstring((*listIter)->Get_CubeInfo().iKey);
			m_ObjectListBox.AddString(SumKey.c_str());
			++listIter;
		}
	}


	CToolMgr::GetInstance()->Update();
	CToolMgr::GetInstance()->Render();
	Invalidate(FALSE);
	UpdateData(FALSE);
}

// 리스트 클릭시
void CObjectTool::OnLbnSelchangeObjectList()
{
	int iObj = m_ObjectListBox.GetCurSel();
	//CString strTexInfo;
	//m_ObjectListBox.GetText(iObj, strTexInfo);

	for (auto& mapiter : CToolMgr::GetInstance()->m_mapObjList)
	{
		for ( list<Engine::CObj*>::iterator& listIter = mapiter.second.begin(); listIter != mapiter.second.end(); )
		{
			if (iObj == (*listIter)->Get_CubeInfo().iKey)
			{
				UpdateData(true);
				m_fPosX = (*listIter)->Get_CubeInfo().vPos.x;
				m_fPosY = (*listIter)->Get_CubeInfo().vPos.y;
				m_fPosZ = (*listIter)->Get_CubeInfo().vPos.z;

				m_fScaleX = (*listIter)->Get_CubeInfo().vScale.x;
				m_fScaleY = (*listIter)->Get_CubeInfo().vScale.y;
				m_fScaleZ = (*listIter)->Get_CubeInfo().vScale.z;
				UpdateData(false);
				return;
			}
			++listIter;
		}
	}
}

// 적용버튼 클릭
void CObjectTool::OnBnClickedButtonApply()
{
	int iObj = m_ObjectListBox.GetCurSel();
	for (auto& mapiter : CToolMgr::GetInstance()->m_mapObjList)
	{
		for (list<Engine::CObj*>::iterator& listIter = mapiter.second.begin(); listIter != mapiter.second.end(); )
		{
			if (iObj == (*listIter)->Get_CubeInfo().iKey)
			{
				UpdateData(TRUE);
				(*listIter)->Set_ObjPos(D3DXVECTOR3(m_fPosX, m_fPosY, m_fPosZ));
				(*listIter)->Set_ObjScale(D3DXVECTOR3(m_fScaleX, m_fScaleY, m_fScaleZ));
				UpdateData(FALSE);
				return;
			}
			++listIter;
		}
	}
}
