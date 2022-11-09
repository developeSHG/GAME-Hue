#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev
{
	DECLARE_SINGLETON(CGraphicDev)

private:
	CGraphicDev(void);
public:
	~CGraphicDev(void);

public:
	enum WINMODE { MODE_FULL, MODE_WIN };

public: // getter
	LPDIRECT3DDEVICE9	  GetDevice(void) { return m_pDevice; }
	LPD3DXFONT			  GetFont(void)	  { return m_pFont; }
	LPD3DXSPRITE		  GetSprite(void) { return m_pSprite; }
	
public:
	HRESULT InitGraphicDev(WINMODE Mode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY);

private:
	LPDIRECT3D9					m_pSDK;
	LPDIRECT3DDEVICE9			m_pDevice; // 장치를 대표하는 객체다.
	LPD3DXFONT					m_pFont;
	LPD3DXSPRITE				m_pSprite;

private:
	void SetParameters(D3DPRESENT_PARAMETERS& d3dpp,
						WINMODE Mode,
						HWND hWnd,
						const WORD& wSizeX,
						const WORD& wSizeY);

	HRESULT Create_Font();
	HRESULT Create_Sprite();

	void Release(void);

};

END


#endif // GraphicDev_h__


