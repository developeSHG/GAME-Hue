#include "KeyMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr(void)
{

}

CKeyMgr::~CKeyMgr(void)
{
	Release();
}

void CKeyMgr::Update(void)
{
	m_dwKey = 0;

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwKey |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwKey |= KEY_DOWN;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwKey |= KEY_RETURN;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwKey |= KEY_SPACE;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKey |= KEY_RBUTTON;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		m_dwKey |= KEY_SHIFT;
	if (GetAsyncKeyState(VK_TAB) & 0x8000)
		m_dwKey |= KEY_TAB;

	if (GetAsyncKeyState('A') & 0x8000)
		m_dwKey |= KEY_A;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwKey |= KEY_D;
	if (GetAsyncKeyState('W') & 0x8000)
		m_dwKey |= KEY_W;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwKey |= KEY_S;
	if (GetAsyncKeyState('Q') & 0x8000)
		m_dwKey |= KEY_Q;
	if (GetAsyncKeyState('E') & 0x8000)
		m_dwKey |= KEY_E;

	if (GetAsyncKeyState(0x31) & 0x8000)
		m_dwKey |= KEY_1;
	if (GetAsyncKeyState(0x32) & 0x8000)
		m_dwKey |= KEY_2;
	if (GetAsyncKeyState(0x33) & 0x8000)
		m_dwKey |= KEY_3;
	if (GetAsyncKeyState(0x34) & 0x8000)
		m_dwKey |= KEY_4;
	if (GetAsyncKeyState(0x35) & 0x8000)
		m_dwKey |= KEY_5;
	if (GetAsyncKeyState(0x36) & 0x8000)
		m_dwKey |= KEY_6;
}

bool CKeyMgr::KeyDown(DWORD dwKey)
{
	// 이 전에 누른적 없고 현재 눌렸으면 TRUE
	if (!(m_dwKeyDown & dwKey) && (m_dwKey & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}

	// 이전에 누른적 있고 현재 누르지 않았으면 FALSE
	if ((m_dwKeyDown & dwKey) && !(m_dwKey & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}

	return false;
}

bool CKeyMgr::KeyUp(DWORD dwKey)
{
	// 이전에 누른적 있고 현재 누르지 않았다면 TRUE
	if ((m_dwKeyPressed & dwKey) && !(m_dwKey & dwKey))
	{
		m_dwKeyPressed ^= dwKey;
		return true;
	}

	// 이전에 누른적 없고 현재 눌렀다면 FALSE
	if (!(m_dwKeyPressed & dwKey) && (m_dwKey & dwKey))
	{
		m_dwKeyPressed |= dwKey;
		return false;
	}

	return false;
}

bool CKeyMgr::KeyPressing(DWORD dwKey)
{
	if (m_dwKey & dwKey)
		return true;

	return false;
}

bool CKeyMgr::KeyCombined(DWORD dwFirst, DWORD dwSecond)
{
	if (KeyDown(dwSecond) && KeyPressing(dwFirst))
		return true;

	return false;
}

void CKeyMgr::Release(void)
{
}
