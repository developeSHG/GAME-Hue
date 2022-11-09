#include "GraphicDev.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGraphicDev)

CGraphicDev::CGraphicDev(void)
	: m_pSDK(NULL)
	, m_pDevice(NULL)
{

}

CGraphicDev::~CGraphicDev(void)
{
	Release();
}

HRESULT CGraphicDev::InitGraphicDev(WINMODE Mode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY)
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9		devicecaps;
	ZeroMemory(&devicecaps, sizeof(D3DCAPS9));

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &devicecaps)))
	{
		MSG_BOX("GetDeviceCaps Failed");
		return E_FAIL;
	}

	DWORD		vp;

	if (devicecaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	vp |= D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp, Mode, hWnd, wSizeX, wSizeY);

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL
		, hWnd, vp, &d3dpp, &m_pDevice)))
	{
		MSG_BOX("CreateDevice Failed");
		return E_FAIL;
	}

	Create_Sprite();
	Create_Font();
	//////////////////////////////////////////////////////////////////////////////////////////////


	return S_OK;
}

HRESULT CGraphicDev::Create_Font()
{
	D3DXFONT_DESC desc;                     // 폰트를 설정한다.
	desc.CharSet = HANGUL_CHARSET;
	lstrcpy(desc.FaceName, L"");                   // FaceName을 비워놓으면 Windows Default Font를 사용한다.
	desc.Height = 15;    // 주의
	desc.Width = 8;
	desc.Weight = FW_NORMAL;
	desc.Quality = DEFAULT_QUALITY;
	desc.MipLevels = 1;
	desc.Italic = 0;
	desc.OutputPrecision = OUT_DEFAULT_PRECIS;
	desc.PitchAndFamily = FF_DONTCARE;

	HRESULT hr = D3DXCreateFontIndirect(m_pDevice, &desc, &m_pFont);        // Font 생성

	//MSG_BOX("CreateFont Failed");

	return hr;
}

HRESULT CGraphicDev::Create_Sprite()
{
	HRESULT hr = D3DXCreateSprite(m_pDevice, &m_pSprite);

	//MSG_BOX(hr, L"D3DXCreateSprite Failed", E_FAIL);

	return hr;
}

void CGraphicDev::SetParameters(D3DPRESENT_PARAMETERS& d3dpp, WINMODE Mode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY)
{
	d3dpp.BackBufferWidth = wSizeX;
	d3dpp.BackBufferHeight = wSizeY;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dpp.MultiSampleQuality = 0;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.EnableAutoDepthStencil = TRUE;

	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = Mode;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

void CGraphicDev::Release(void)
{
	Engine::Safe_Release(m_pFont);
	Engine::Safe_Release(m_pSprite);
	Engine::Safe_Release(m_pDevice);
	Engine::Safe_Release(m_pSDK);
}
