#pragma once
#include "afxwin.h"

#include "Engine_Include.h"
#include "MFCEnum.h"

BEGIN(Engine)

class CResourceMgr;

END

// CMapTool 대화 상자입니다.


class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// Event
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnTerrainApply();

	afx_msg void OnCbnSelchangeCubeType();
	afx_msg void OnCbnSelchangeComboCubeList();
	afx_msg void OnLbnSelchangeListCubeNum();

	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonModify();

public:
	void		OptionCheck(void);
	void		OptionReset(bool _bType = true);

public:
	HRESULT		MovingDetailDirCheck(Engine::CUBE_INFO& _tCubeInfo);

public:
	HRESULT		ResourceForStage(void);


public:
	// Control
	CButton			m_btnTerrainApply;
	CEdit			m_editTerrainX;
	CEdit			m_editTerrainZ;
	CEdit			m_editTerrainITV;
	CComboBox		m_ComboBoxCubeType;
	CComboBox		m_ComboBoxCubeList;
	CListBox		m_ListBoxCubeNum;
	CStatic			m_PictureCtrlCubePreview;
	CButton			m_CheckBtnReset;
	CButton			m_RadioBtnClippingOption[Engine::CUBE_CLIPPING_TYPE::CUBE_TYPE_END - 3];
	CButton			m_RadioBtnCubeColor[Engine::CUBE_COLOR::CUBE_COLOR_END];
	CEdit			m_editScaleCX;
	CEdit			m_editScaleCY;
	CEdit			m_editScaleCZ;
	CButton			m_RadioBtnMovingDir[Engine::CUBE_MOVING_DIR::CUBE_MOVING_DIR_END];
	CEdit			m_editMovingDist;
	CEdit			m_editMovingSpeed;
	CButton			m_CheckBtnPicking;

public:
	Engine::CResourceMgr*		m_pResourceMgr = nullptr;

public:
	wstring							m_wstrCubeTypeKey = L"";
	wstring							m_wstrCubeTexKey = L"";
	Engine::CUBE_CLIPPING_TYPE		m_eCubeClippingType = Engine::CUBE_CLIPPING_TYPE::CUBE_TYPE_END;
	Engine::CUBE_COLOR				m_eCubeColor = Engine::CUBE_COLOR::CUBE_COLOR_END;

public:
	// User Value
	int m_iTerrainX;
	int m_iTerrainY;
	float m_fVertexITV;
	float m_fScaleCX;
	float m_fScaleCY;
	float m_fScaleCZ;
	int m_fMovingDist;
	float m_fMovingSpeed;
};
