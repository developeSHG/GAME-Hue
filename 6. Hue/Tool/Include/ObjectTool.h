#pragma once
#include "afxwin.h"
#include "Engine_Include.h"
#include "MFCEnum.h"

BEGIN(Engine)

class CResourceMgr;

END
// CObjectTool 대화 상자입니다.

class CObjectTool : public CDialog
{
	DECLARE_DYNAMIC(CObjectTool)

public:
	CObjectTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CObjectTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECTTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()


	// Event Message
public:
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnLbnSelchangeList1();

	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnLbnSelchangeObjectList();
	afx_msg void OnBnClickedButtonApply();
public:
	HRESULT		ResourceForStage(void);
	// Control Value
public:
	CComboBox m_ComboBoxObjectType;
	CListBox m_ListBoxObjectNum;
	CStatic m_PictureCtrlObjectPreview;
	CListBox m_ObjectListBox;

	// User Function
public:
	void UpdateInfo(D3DXVECTOR3& vPos, D3DXVECTOR3& vRot, D3DXVECTOR3& vScale, bool bisExport);

	// User Value
public:
	wstring							m_wstrCubeTypeKey = L"";
	wstring							m_wstrCubeTexKey = L"";
	Engine::CUBE_CLIPPING_TYPE		m_eCubeClippingType = Engine::CUBE_CLIPPING_TYPE::CUBE_TYPE_END;
	Engine::CUBE_COLOR				m_eCubeColor = Engine::CUBE_COLOR::CUBE_COLOR_END;
	bool							m_bOpen = false;
public:
	Engine::CResourceMgr*		m_pResourceMgr = nullptr;
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
	float m_fRotX;
	float m_fRotY;
	float m_fRotZ;
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;

	//afx_msg void OnEnChangeEditPosX();


};
