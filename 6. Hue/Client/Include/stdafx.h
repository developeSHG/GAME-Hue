// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"
#include <iostream>
using namespace std;

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <functional>
#include <ctime>
#include "value.h"
#include "Struct.h"
#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)

#endif

#include <crtdbg.h>

//// �ܼ�â ����
//#ifdef UNICODE
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
//#else
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
//#endif

using namespace std;