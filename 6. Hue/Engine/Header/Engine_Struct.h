#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Enum.h"
#include "Engine_Macro.h"

namespace Engine
{
	typedef	struct tagVertexColor
	{
		D3DXVECTOR3		vPos;
		DWORD			dwColor;

	}VTXCOL;

	const DWORD VTXFVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTexture
	{
		D3DXVECTOR3		vPos;
		D3DXVECTOR2		vTex;

	}VTXTEX;

	const DWORD VTXFVF_TEX = D3DFVF_XYZ | D3DFVF_TEX1;


	typedef struct test {

	}TEST;

	typedef struct tagIndex16
	{
		WORD		_0, _1, _2;

	}INDEX16;

	typedef struct tagIndex32
	{
		DWORD		_0, _1, _2;

	}INDEX32;

	typedef struct tagInfo
	{
		D3DXVECTOR3		vPos;
		D3DXVECTOR3		vDir;
		D3DXVECTOR3		vLook;
		D3DXMATRIX		matWorld;

	}INFO;

	typedef struct tagCubeTexInfo
	{
		TCHAR					szTex[MID_STR];
		WORD					iIndex;

	}CUBE_TEX_INFO;

	typedef struct tagCubePlatformInfo
	{
		CUBE_MOVING_DIR			eDir;
		float					fDist;
		float					fSpeed;

	}CUBE_PLATFORM_INFO;

	typedef struct tagCubeInfo
	{
		CUBE_CLIPPING_TYPE		eType;
		D3DXVECTOR3				vPos;
		D3DXVECTOR3				vScale;
		CUBE_TEX_INFO			tTex;
		CUBE_COLOR				tColor;
		CUBE_PLATFORM_INFO		tPlatformInfo;

	}CUBE_INFO;

	typedef struct tagObjectInfo
	{
		CUBE_CLIPPING_TYPE		eType;
		D3DXVECTOR3				vPos;
		D3DXVECTOR3				vScale;
		CUBE_TEX_INFO			tTex;
		CUBE_COLOR				tColor;
		int						iKey;
	}OBJ_INFO;
}

#endif // Engine_Struct_h__
