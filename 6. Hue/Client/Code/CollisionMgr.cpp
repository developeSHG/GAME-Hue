#include "stdafx.h"
#include "CollisionMgr.h"
#include <iostream>
#include "CubeMgr.h"
#include "Cube.h"
#include "CubeDeco.h"
#include "CubeBrick.h"
#include "CubeCrate.h"
#include "CubeSkull.h"
#include "CubePlatform.h"
#include "CubeRope.h"
#include "CubeLaser.h"
#include "CubeStairs.h"
#include "CubeRock.h"
#include "CubeBalloon.h"
#include "TimeMgr.h"
#include "Player.h"
#include "PlayerTuto.h"
#include "ButtonX.h"
#include "Laser.h"
#include "Transform.h"
#include "Player.h"
#define PLAYER_WIDTH 0.8f
#define PLAYER_HEIGHT 0.45f
#define CUBE_WIDTH 1.0f
#define CUBE_HEIGHT 1.0f
#define SKELETON_WIDTH 1.0f
#define SKELETON_HEIGHT 1.0f
#define PLATFORM_WIDTH 1.0f
#define PLATFORM_HEIGHT 1.0f
#define CRATE_WIDTH 1.0f
#define CRATE_HEIGHT 1.0f
#define ROPE_WIDTH 0.6f
BEGIN(Engine)

END

IMPLEMENT_SINGLETON(CCollisionMgr)
CCollisionMgr::CCollisionMgr(void)
{

}

CCollisionMgr::~CCollisionMgr(void)
{
	Release();
}

void CCollisionMgr::Release(void)
{
	//Engine::Safe_Single_Destroy(m_pCubeMgr);
}

bool CCollisionMgr::IsCollisionCrateToCube(CCubeCrate * pCrate)
{
	bool bCrateFall = false;
	bool bReturn = false;
	D3DXVECTOR3 vMinCrateCube;
	D3DXVECTOR3 vMaxCrateCube;
	D3DXVECTOR3 vMinCube;
	D3DXVECTOR3 vMaxCube;
	// Crate
	vMinCrateCube.x = (pCrate)->Get_InfoCom()->m_vPos.x - (pCrate)->Get_InfoCom()->m_vScale.x + 0.1f;
	vMinCrateCube.y = (pCrate)->Get_InfoCom()->m_vPos.y - (pCrate)->Get_InfoCom()->m_vScale.y;
	vMinCrateCube.z = (pCrate)->Get_InfoCom()->m_vPos.z - (pCrate)->Get_InfoCom()->m_vScale.z;

	vMaxCrateCube.x = (pCrate)->Get_InfoCom()->m_vPos.x + (pCrate)->Get_InfoCom()->m_vScale.x - 0.1f;
	vMaxCrateCube.y = (pCrate)->Get_InfoCom()->m_vPos.y + (pCrate)->Get_InfoCom()->m_vScale.y;
	vMaxCrateCube.z = (pCrate)->Get_InfoCom()->m_vPos.z + (pCrate)->Get_InfoCom()->m_vScale.z;

	CCubeMgr::GetInstance()->SetPlayerColColor(Engine::CUBE_COLOR::CUBE_COLOR_END);

	for (auto mapiter : *CCubeMgr::GetInstance()->Get_MapCubeList())
	{
		for (auto iter : mapiter.second)
		{
			if (iter == pCrate)
				continue;

			wstring Convert(iter->GetTexName());
			if (Convert == L"Texture_Object_Rope" || Convert == L"Texture_Object_Laser" || Convert == L"Texture_CubeObstacle_Rock" || iter == pCrate)
				continue;

			vMinCube = (iter)->Get_InfoCom()->m_vPos - (iter)->Get_InfoCom()->m_vScale;
			vMaxCube = (iter)->Get_InfoCom()->m_vPos + (iter)->Get_InfoCom()->m_vScale;

			//  색 체크
			if ( (iter->Get_CubeColor() != CCubeMgr::GetInstance()->Get_CurCubeColor()) || iter->Get_CubeColor() == Engine::CUBE_COLOR::CUBE_COLOR_END)
			{
				// AABB 충돌체크
				if (ABC(vMinCrateCube, vMaxCrateCube, vMinCube, vMaxCube))
				{
					bReturn = true;
					float fWidth1 = abs(vMaxCrateCube.x - vMinCube.x);
					float fHeight1 = abs(vMaxCube.y - vMinCrateCube.y);
					float fWidth2 = abs(vMaxCube.x - vMinCrateCube.x);
					float fHeight2 = abs(vMaxCrateCube.y - vMinCube.y);

					if (pCrate->GetCrateFall())
						bCrateFall = true;

					if ( pCrate->Get_CubeColor() == CCubeMgr::GetInstance()->Get_CurCubeColor() )
						continue;
					// Crate가 큐브 보다 중점이 높다
					if ((pCrate)->Get_InfoCom()->m_vPos.y >= vMaxCube.y - 0.1f)
					{
						//Crate 
						if ((pCrate)->Get_InfoCom()->m_vPos.x >= vMinCube.x + 0.01f && (pCrate)->Get_InfoCom()->m_vPos.x <= vMaxCube.x - 0.01f)
						{
							(pCrate)->Get_InfoCom()->m_vPos.y += fHeight1;
						}
					}
					//Crate가 큐브 보다 중점이 낮다
					else if ((pCrate)->Get_InfoCom()->m_vPos.y <= vMinCube.y + 0.1f && Convert != L"Texture_CubeObstacle_Skull")
					{
						// Crate는 큐브 왼쪽에 있다
						if ((pCrate)->Get_InfoCom()->m_vPos.x <= vMinCube.x+0.15f)
						{
							// 왼쪽으로 밀기
							(pCrate)->SetCollision(CHECK::SIDE_RIGHT, true);
							(pCrate)->SetCollision(CHECK::SIDE_LEFT, false);
							(pCrate)->Get_InfoCom()->m_vPos.x -= (fWidth1 + 0.05f);
						}
						// Crate는 큐브 오른쪽에 있다
						else if((pCrate)->Get_InfoCom()->m_vPos.x >= vMaxCube.x - 0.15f)
						{
							// 오른쪽으로 밀기
							(pCrate)->SetCollision(CHECK::SIDE_LEFT, true);
							(pCrate)->SetCollision(CHECK::SIDE_RIGHT, false);
							(pCrate)->Get_InfoCom()->m_vPos.x += (fWidth2 + 0.05f);
						}
						//
						else
						{
						}
					}
					//Crate가 큐브와 같은Y선상에 잇다
					else if ( (pCrate)->Get_InfoCom()->m_vPos.y < vMaxCube.y && (pCrate)->Get_InfoCom()->m_vPos.y > vMinCube.y)
					{
						// Crate는 큐브 왼쪽에 있다
						if ((pCrate)->Get_InfoCom()->m_vPos.x <= (iter)->Get_InfoCom()->m_vPos.x)
						{
							// 왼쪽으로 밀기
							(pCrate)->Get_InfoCom()->m_vPos.x -= (fWidth1 + 0.05f);
						}
						// Crate는 큐브 오른쪽에 있다
						else if((pCrate)->Get_InfoCom()->m_vPos.x > (iter)->Get_InfoCom()->m_vPos.x)
						{
							// 오른쪽으로 밀기
							(pCrate)->Get_InfoCom()->m_vPos.x += (fWidth2 + 0.05f);
						}
					}
				}
				else
				{
				}
				float fDistX = (iter)->Get_InfoCom()->m_vPos.x - (pCrate)->Get_InfoCom()->m_vPos.x;
				if (fDistX < 0.6f)
					Engine::CUBE_COLOR eColor = iter->Get_CubeColor();
			}
			else
			{
				if (ABC(vMinCrateCube, vMaxCrateCube, vMinCube, vMaxCube))
				{
					Engine::CUBE_COLOR eColor = iter->Get_CubeColor();
					CCubeMgr::GetInstance()->SetPlayerColColor(eColor);
				}
			}
		}
	}

	if (bCrateFall)
	{
		Engine::Get_SoundMgr()->MyPlaySound(L"CrateFall.wav", Engine::CHANNELID::EFFECT);
		bCrateFall = false;
	}
	return bReturn;
}

// 플레이어 상호작용
bool CCollisionMgr::IsCollisionPlayerToCube(CPlayer* pPlayer, bool *pPlayerProperties, bool* bCollSide, bool& bInteraction, bool& bIsLeftOn, bool& bIsRightOn, bool& bIsHang, bool& bRopeEnd, CHECK::SIDE& eSide, PLAYERSTATE& eState, bool IsOnGrass)
{
	static bool bLandSound = false;
	static bool bLandCrate = false;
	//	static bool bGetBattery = false;
	static bool bDragCrate = false;
	static bool bDragSound = false;		// 드래그 사운드 토글 On/Off
	static bool bWalkCrate = false;

	static float fSoundDelay;
	// Init
	D3DXVECTOR3 vMinCube;
	D3DXVECTOR3 vMaxCube;
	D3DXVECTOR3 vMinPlayer;
	D3DXVECTOR3 vMaxPlayer;
	D3DXVECTOR3 vMinCollBox;
	D3DXVECTOR3 vMaxCollBox;
	bCollSide[1] = false;
	bCollSide[2] = false;
	bCollSide[3] = false;
	bCollSide[4] = false;
	bool bReturn = false;
	bool bHangCheck = false;
	//pPlayer->SetOnStair(false);
	CCubeMgr::GetInstance()->SetPlayerColColor(Engine::CUBE_COLOR::CUBE_COLOR_END);

	for (auto mapiter : *CCubeMgr::GetInstance()->Get_MapCubeList())
	{
		for (auto iter : mapiter.second)
		{


			// 모든 큐브 동일 적용
			vMinCube = (iter)->Get_InfoCom()->m_vPos - (iter)->Get_InfoCom()->m_vScale;
			vMaxCube = (iter)->Get_InfoCom()->m_vPos + (iter)->Get_InfoCom()->m_vScale;

			// 플레이어 상호작용 박스
			vMinCollBox.x = (pPlayer)->Get_Transform()->m_vPos.x - ((pPlayer)->Get_Transform()->m_vScale.x - 0.65f);
			vMinCollBox.y = (pPlayer)->Get_Transform()->m_vPos.y - (pPlayer)->Get_Transform()->m_vScale.y;
			vMinCollBox.z = (pPlayer)->Get_Transform()->m_vPos.z - (pPlayer)->Get_Transform()->m_vScale.z;

			vMaxCollBox.x = (pPlayer)->Get_Transform()->m_vPos.x + ((pPlayer)->Get_Transform()->m_vScale.x - 0.65f);
			vMaxCollBox.y = (pPlayer)->Get_Transform()->m_vPos.y + (pPlayer)->Get_Transform()->m_vScale.y;
			vMaxCollBox.z = (pPlayer)->Get_Transform()->m_vPos.z + (pPlayer)->Get_Transform()->m_vScale.z;

			// 플레이어 박스
			vMinPlayer.x = (pPlayer)->Get_Transform()->m_vPos.x - ((pPlayer)->Get_Transform()->m_vScale.x - PLAYER_WIDTH);
			vMinPlayer.y = (pPlayer)->Get_Transform()->m_vPos.y - ((pPlayer)->Get_Transform()->m_vScale.y - PLAYER_HEIGHT);
			vMinPlayer.z = (pPlayer)->Get_Transform()->m_vPos.z - (pPlayer)->Get_Transform()->m_vScale.z;

			vMaxPlayer.x = (pPlayer)->Get_Transform()->m_vPos.x + ((pPlayer)->Get_Transform()->m_vScale.x - PLAYER_WIDTH);
			vMaxPlayer.y = (pPlayer)->Get_Transform()->m_vPos.y + ((pPlayer)->Get_Transform()->m_vScale.y - PLAYER_HEIGHT);
			vMaxPlayer.z = (pPlayer)->Get_Transform()->m_vPos.z + (pPlayer)->Get_Transform()->m_vScale.z;

			//  색 체크
			if ((iter->Get_CubeColor() != CCubeMgr::GetInstance()->Get_CurCubeColor()) || iter->Get_CubeColor() == Engine::CUBE_COLOR::CUBE_COLOR_END)
			{
				// 큐브 종류 체크
				wstring Convert(iter->GetTexName());
				// 0. 해골
				if (Convert == L"Texture_CubeObstacle_Skull")
				{
					// 0-0. 충돌 체크
					if (ABC(vMinPlayer, vMaxPlayer, vMinCube, vMaxCube) && !(pPlayer)->GetDead())
					{
						bReturn = true;
						float fWidth1 = abs(vMaxPlayer.x - vMinCube.x);
						float fHeight1 = abs(vMaxCube.y - vMinPlayer.y);
						float fWidth2 = abs(vMaxCube.x - vMinPlayer.x);
						float fHeight2 = abs(vMaxPlayer.y - vMinCube.y);

						// 0-0-1. 플레이어가 해골보다 높이 있을 때
						if ((pPlayer)->Get_Transform()->m_vPos.y >= vMaxCube.y)
						{
							if (pPlayer->Get_CurState() == PLAYERSTATE::STATE_FALL)
								bLandSound = true;

							// 0-0-1-1.  플레이어가 해골과 같은 X 범위내에 있다면 (오차값)
							if ((pPlayer)->Get_Transform()->m_vPos.x >= vMinCube.x + 0.01f || (pPlayer)->Get_Transform()->m_vPos.x >= vMinCube.x - 0.01f)
							{
								// 0-0. 에스컬레이터 방지
								if (eState != STATE_JUMP)
								{
									// 0-0-. 해골 올라갈 때
									if (dynamic_cast<CCubeSkull*>(iter)->Get_SkullNextState() == SKULLSTATE::SKULL_UP)
									{
										(pPlayer)->Get_Transform()->m_vPos.y += fHeight1;
									}
									// 0-0-. 해골 내려갈 때 (오차값)
									else if (dynamic_cast<CCubeSkull*>(iter)->Get_SkullNextState() == SKULLSTATE::SKULL_FALL)
									{
										if (eState == STATE_WALK || eState == STATE_IDLE1)
										{
											(pPlayer)->Get_Transform()->m_vPos.y = vMaxCube.y + PLAYER_HEIGHT - 0.1195f;
										}
									}
									else if (dynamic_cast<CCubeSkull*>(iter)->Get_SkullNextState() == SKULLSTATE::SKULL_IDLE)
									{
										if (eState == STATE_WALK || eState == STATE_IDLE1)
										{
											//(pPlayer)->Get_Transform()->m_vPos.y = vMaxCube.y + PLAYER_HEIGHT - 0.1195f;
											(pPlayer)->Get_Transform()->m_vPos.y += fHeight1 * (GET_TIME * 25.0f);
										}
									}

									pPlayerProperties[STATE_FALL] = false;
									bCollSide[1] = true;
								}//0-0-1... END.
							}//0-0-1-1 END.
						}//0-0-1 END.
						 // 0-0-2. 플레이어가 해골보다 낮게 있을 때
						else if ((pPlayer)->Get_Transform()->m_vPos.y + PLAYER_HEIGHT <= vMinCube.y)
						{
							// 0-0-3. 
							if ((pPlayer)->Get_Transform()->m_vPos.x >= vMinCube.x + 0.01f || (pPlayer)->Get_Transform()->m_vPos.x >= vMinCube.x - 0.01f)
							{
								// 0-0-4. 해골이 올라가는 상태가 아니라면
								if (dynamic_cast<CCubeSkull*>(iter)->Get_SkullNextState() != SKULLSTATE::SKULL_UP && eState != STATE_FALL)
								{
									(pPlayer)->Setplayerproperties(PLAYERSTATE::STATE_DIE, true);
									Engine::Get_SoundMgr()->StopAll();
									Engine::Get_SoundMgr()->MyPlaySound(L"DeadBySkull.wav", Engine::CHANNELID::EFFECT);
									bCollSide[2] = true;
								}
							}
						}
						// 0-0-3. 플레이어가 해골과 같은Y선상에 있을 때
						else if ((pPlayer)->Get_Transform()->m_vPos.y < vMaxCube.y && (pPlayer)->Get_Transform()->m_vPos.y >vMinCube.y)
						{
							// 0-0-3-1. 플레이어가 해골 왼쪽에 있다
							if ((pPlayer)->Get_Transform()->m_vPos.x <= vMinCube.x)
							{
								// 왼쪽으로 밀기
								(pPlayer)->Get_Transform()->m_vPos.x -= (fWidth1) * (GET_TIME * 15.0f);
								bCollSide[3] = true;
							}
							// 플레이어가 큐브 오른쪽에 있다
							else
							{
								// 오른쪽으로 밀기
								(pPlayer)->Get_Transform()->m_vPos.x += (fWidth2) * (GET_TIME * 15.0f);
								bCollSide[4] = true;
							}
						}
					}//0-0 END.
					float fDistX = (iter)->Get_InfoCom()->m_vPos.x - (pPlayer)->Get_Transform()->m_vPos.x;
					if (fDistX < 0.6f)
						Engine::CUBE_COLOR eColor = iter->Get_CubeColor();
					continue;
				}//0 해골 END.
				 // 1. 플랫폼
				else if (Convert == L"Texture_CubeColor_Platform")
				{
					// 1-0. 충돌 체크
					if (ABC(vMinPlayer, vMaxPlayer, vMinCube, vMaxCube) && !(pPlayer)->GetDead())
					{

						bReturn = true;
						float fWidth1 = abs(vMaxPlayer.x - vMinCube.x);
						float fHeight1 = abs(vMaxCube.y - vMinPlayer.y);
						float fWidth2 = abs(vMaxCube.x - vMinPlayer.x);
						float fHeight2 = abs(vMaxPlayer.y - vMinCube.y);

						// 1-0-1. 플레이어가 플랫폼 위에 있을때 
						if ((pPlayer)->Get_Transform()->m_vPos.y >= (iter)->Get_InfoCom()->m_vPos.y)
						{
							if (pPlayer->Get_CurState() == PLAYERSTATE::STATE_FALL)
								bLandSound = true;

							// 1-0-1-1. 큐브 x범위 내에 있을 때 
							if (vMaxPlayer.x >= vMinCube.x + 0.01f || vMinPlayer.x <= vMaxCube.x - 0.01f)
							{
								// 1-0-1-2. X방향 가져오기
								if (dynamic_cast<CCubePlatform*>(iter)->Get_State() == Engine::CUBE_RIGHT && dynamic_cast<CCubePlatform*>(iter)->Get_IsStop() == false)
									(pPlayer)->Get_Transform()->m_vPos.x += dynamic_cast<CCubePlatform*>(iter)->Get_CubeInfo().tPlatformInfo.fSpeed*GET_TIME;
								else if (dynamic_cast<CCubePlatform*>(iter)->Get_State() == Engine::CUBE_LEFT&& dynamic_cast<CCubePlatform*>(iter)->Get_IsStop() == false)
									(pPlayer)->Get_Transform()->m_vPos.x -= dynamic_cast<CCubePlatform*>(iter)->Get_CubeInfo().tPlatformInfo.fSpeed*GET_TIME;

								// 1-0-1-3. 에스컬레이터 방지
								if (eState != STATE_JUMP)
								{
									//  1-0-1-4. 올라가는 상태
									if (dynamic_cast<CCubePlatform*>(iter)->Get_State() == Engine::CUBE_MOVING_DIR::CUBE_UP && dynamic_cast<CCubePlatform*>(iter)->Get_IsStop() == false)
									{
										if (eState == STATE_WALK || eState == STATE_IDLE1 || eState == STATE_RUN)
										{
											(pPlayer)->Get_Transform()->m_vPos.y = vMaxCube.y + PLAYER_HEIGHT;
										}
										if (eState == STATE_JUMP)
										{
											(pPlayer)->Get_Transform()->m_vPos.y += vMaxCube.y *GET_TIME;
										}
									}
									//  1-0-1-5. 내려가는 상태 
									else if ((dynamic_cast<CCubePlatform*>(iter)->Get_State() == Engine::CUBE_MOVING_DIR::CUBE_DOWN) && dynamic_cast<CCubePlatform*>(iter)->Get_IsStop() == false)
									{
										if (eState == STATE_WALK || eState == STATE_IDLE1 || eState == STATE_RUN)
										{
											(pPlayer)->Get_Transform()->m_vPos.y = vMaxCube.y + PLAYER_HEIGHT - 0.0895f;
										}
									}
									// 1-0-1-6. 멈췃을때 
									else if (dynamic_cast<CCubePlatform*>(iter)->Get_IsStop())
									{
										if (eState == STATE_WALK || eState == STATE_IDLE1 || eState == STATE_RUN)
										{
											(pPlayer)->Get_Transform()->m_vPos.y += fHeight1 * (GET_TIME * 25.0f);
										}
									}
									pPlayerProperties[STATE_FALL] = false;
									bCollSide[1] = true;
								}//1-0-1-2 END.
							}//1-0-1-1 END.
						}//1-0-1 END.
						 // 1-0-2. 플랫폼 아래라면
						else if (vMaxPlayer.y <= (iter)->Get_InfoCom()->m_vPos.y)
						{
							// 아무것도하지마
						}//1-0-2 END.
						 // 1-0-3. 같은 Y선상에 존재한다면
						else if (vMinPlayer.y < (iter)->Get_InfoCom()->m_vPos.y && vMaxPlayer.y >(iter)->Get_InfoCom()->m_vPos.y)
						{
							// 1-0-3-1. 플레이어가 큐브 왼쪽에 있다
							if ((pPlayer)->Get_Transform()->m_vPos.x < vMinCube.x)
							{
								// 왼쪽으로 밀기
								(pPlayer)->Get_Transform()->m_vPos.x -= (fWidth1);
								bCollSide[3] = true;
							}
							// 1-0-3-2. 플레이어가 큐브 오른쪽에 있다
							else if ((pPlayer)->Get_Transform()->m_vPos.x > vMaxCube.x)
							{
								// 오른쪽으로 밀기
								(pPlayer)->Get_Transform()->m_vPos.x += (fWidth2);
								bCollSide[4] = true;
							}
							// 1-0-3-3. 플레이어가 큐브 사이에 있다
							else
							{
								//걍 낑겨
							}
						}// 1-0-3 END.

					}//1-0 END.
					float fDistX = (iter)->Get_CubeInfo().vPos.x - dynamic_cast<CPlayer*>(pPlayer)->Get_Transform()->m_vPos.x;
					if (fDistX < 0.6f)
						Engine::CUBE_COLOR eColor = iter->Get_CubeColor();
					continue;
				}//1. 플랫폼 END.
				 // 2. 나무상자
				else if (Convert == L"Texture_CubeColor_Crate" || Convert == L"Texture_CubeColor_CrateLarge" || Convert == L"Texture_CubeColor_CrateTall")
				{
					// 2-0. 상호작용키를 눌렷을경우
					if (bInteraction)
					{
						// 2-0-1. 상호작용 충돌범위 체크 
						if (ABC(vMinCollBox, vMaxCollBox, vMinCube, vMaxCube) && !(pPlayer)->GetDead())
						{
							// 2-0-2. 플레이어가 상호작용할 나무상자와 같은 Y 범위내에 있다면
							if ((pPlayer)->Get_Transform()->m_vPos.y < vMaxCube.y && (pPlayer)->Get_Transform()->m_vPos.y > vMinCube.y)
							{
								if (!pPlayer->GetIsRegisterInteraction())
								{
									// 판자 상호작용 등록
									pPlayer->SetRegisterInteraction(true);
									dynamic_cast<CCubeCrate*>(iter)->SetOwned(true);
								}
							
								// 소유된 판자라면
								if (dynamic_cast<CCubeCrate*>(iter)->GetIsOwned())
								{
									// 판자 당기기 사운드
									if (bDragSound == false)
										bDragCrate = true;

									// 2-0-3-1. 플레이어가 큐브 왼쪽에 있다
									if ((pPlayer)->Get_Transform()->m_vPos.x <= (iter)->Get_InfoCom()->m_vPos.x)
									{
										// 0. 왼쪽에서 왼쪽으로 당기기
										if (bIsLeftOn)
										{
											eSide = CHECK::SIDE_RIGHT;
											(iter)->Get_InfoCom()->m_vPos.x = vMaxPlayer.x + (iter)->Get_InfoCom()->m_vScale.x /*+ 0.1f*/;
										}
										// 1. 왼쪽에서 오른쪽으로 밀기
										else if (bIsRightOn)
										{
											if ((dynamic_cast<CCubeCrate*>(iter)->GetCollision()[CHECK::SIDE_RIGHT] == false))
											{
												eSide = CHECK::SIDE_RIGHT;
												(iter)->Get_InfoCom()->m_vPos.x = vMaxPlayer.x + (iter)->Get_InfoCom()->m_vScale.x + 0.05f;
											}
											else
											{
												//(pPlayer)->Get_Transform()->m_vPos.x = vMinCube.x;
											}
										}
									}// 2-0-3-1. END
									 //  2-0-3-2. 플레이어가 큐브 오른쪽에 있다
									else
									{

										// 3. 오른쪽에서 왼쪽으로 밀기
										if (bIsLeftOn)
										{
											if ((dynamic_cast<CCubeCrate*>(iter)->GetCollision()[CHECK::SIDE_LEFT] == false))
											{
												eSide = CHECK::SIDE_LEFT;
												(iter)->Get_InfoCom()->m_vPos.x = vMinPlayer.x - (iter)->Get_InfoCom()->m_vScale.x - 0.05f;
											}
											else
											{
												//(pPlayer)->Get_Transform()->m_vPos.x += (fWidth1) * (GET_TIME * 15.0f);
												//(pPlayer)->Get_Transform()->m_vPos.x = vMaxCube.x;

											}

										}
										// 4. 오른쪽에서 오른쪽으로 당기기
										else if (bIsRightOn)
										{
											eSide = CHECK::SIDE_LEFT;
											(iter)->Get_InfoCom()->m_vPos.x = vMinPlayer.x - (iter)->Get_InfoCom()->m_vScale.x/* - 0.1f*/;
										}
									}// 2-0-3-2. END
								}// 소유판자 체크
								
							}// 2-0-1. END

						}// 2-0. END
						else
						{

						}
					}// 2. END
					 // 2-1. 상호작용 키를 안눌럿을경우
					else
					{
						pPlayer->SetRegisterInteraction(false);
						dynamic_cast<CCubeCrate*>(iter)->SetOwned(false);
						bDragSound = false;
						// 2-1-1. AABB 충돌체크
						if (ABC(vMinPlayer, vMaxPlayer, vMinCube, vMaxCube) && !dynamic_cast<CPlayer*>(pPlayer)->GetDead())
						{
							bReturn = true;
							float fWidth1 = abs(vMaxPlayer.x - vMinCube.x);
							float fHeight1 = abs(vMaxCube.y - vMinPlayer.y);
							float fWidth2 = abs(vMaxCube.x - vMinPlayer.x);
							float fHeight2 = abs(vMaxPlayer.y - vMinCube.y);

							// 2-1-1-1. 플레이어가 나무상자보다 높이 있을 때
							if ((pPlayer)->Get_Transform()->m_vPos.y >= vMaxCube.y)
							{
								// 크레이트 떨어지는 상태
								if (dynamic_cast<CCubeCrate*>(iter)->GetCrateFall())
								{
									// 큐브 x범위 내에 있을 때 
									if ((pPlayer)->Get_Transform()->m_vPos.x >= vMinCube.x + 0.01f || (pPlayer)->Get_Transform()->m_vPos.x <= vMinCube.x - 0.01f)
									{
										// 에스컬레이터 방지
										if (eState != STATE_JUMP)
										{
											(pPlayer)->Get_Transform()->m_vPos.y += fHeight1 -0.1f;
											pPlayerProperties[STATE_FALL] = false;
											bCollSide[1] = true;

											if ((pPlayer->Get_CurState() == PLAYERSTATE::STATE_WALK || pPlayer->Get_CurState() == PLAYERSTATE::STATE_RUN))
											{
												fSoundDelay += GET_TIME;
												bWalkCrate = true;
											}

											if (pPlayer->Get_CurState() == PLAYERSTATE::STATE_FALL)
											{
												bLandSound = true;
												bLandCrate = true;
											}

										}
									}
								}
								// 크레이트 떨어지는 상태 아닐 때
								else
								{
									// 큐브 x범위 내에 있을 때 
									if ((pPlayer)->Get_Transform()->m_vPos.x >= vMinCube.x + 0.01f || (pPlayer)->Get_Transform()->m_vPos.x <= vMinCube.x - 0.01f)
									{
										// 에스컬레이터 방지
										if (eState != STATE_JUMP)
										{
											(pPlayer)->Get_Transform()->m_vPos.y += fHeight1;
											pPlayerProperties[STATE_FALL] = false;
											bCollSide[1] = true;

											if ((pPlayer->Get_CurState() == PLAYERSTATE::STATE_WALK || pPlayer->Get_CurState() == PLAYERSTATE::STATE_RUN))
											{
												fSoundDelay += GET_TIME;
												bWalkCrate = true;
											}

											if (pPlayer->Get_CurState() == PLAYERSTATE::STATE_FALL)
											{
												bLandSound = true;
												bLandCrate = true;
											}

										}
									}
								}
							
							}// 2-1-1-1 END.
							 // 2-1-1-2. 플레이어가 나무상자보다 낮다
							 //else if ((pPlayer)->Get_Transform()->m_vPos.y <= (iter)->Get_InfoCom()->m_vPos.y)
							else if ((pPlayer)->Get_Transform()->m_vPos.y <= vMinCube.y)
							{
								if ((pPlayer)->Get_Transform()->m_vPos.x >= vMinCube.x + 0.01f || (pPlayer)->Get_Transform()->m_vPos.x >= vMinCube.x - 0.01f)
								{
									// 아래밀기
									(pPlayer)->Get_Transform()->m_vPos.y -= fHeight2 * (GET_TIME*25.0f);
									bCollSide[2] = true;
								}
							}// 2-1-1-2 END.
							 // 2-1-1-3. 플레이어가 같은Y선상에 존재한다.
							else if ((pPlayer)->Get_Transform()->m_vPos.y < vMaxCube.y && (pPlayer)->Get_Transform()->m_vPos.y > vMinCube.y)
							{
								// 플레이어가 큐브 왼쪽에 있다
								if ((pPlayer)->Get_Transform()->m_vPos.x <= (iter)->Get_InfoCom()->m_vPos.x)
								{
									if (eSide != CHECK::SIDE_RIGHT)
										(pPlayer)->Get_Transform()->m_vPos.x -= (fWidth1) * (GET_TIME * 15.0f);
									//dynamic_cast<CPlayer*>(pPlayer)->Get_Transform()->m_vPos.x = vMinCube.x-0.2f;
									bCollSide[3] = true;
								}
								// 플레이어가 큐브 오른쪽에 있다
								else
								{
									if (eSide != CHECK::SIDE_LEFT)
										dynamic_cast<CPlayer*>(pPlayer)->Get_Transform()->m_vPos.x += (fWidth2) * (GET_TIME * 15.0f);
									//dynamic_cast<CPlayer*>(pPlayer)->Get_Transform()->m_vPos.x = vMaxCube.x+0.2f;
									bCollSide[4] = true;
								}
							}// 2-1-1-3 END.
						}// 2-1-1 END.
						float fDistX = (iter)->Get_CubeInfo().vPos.x - dynamic_cast<CPlayer*>(pPlayer)->Get_Transform()->m_vPos.x;
						if (fDistX < 0.6f)
							Engine::CUBE_COLOR eColor = iter->Get_CubeColor();
						continue;
					}// 2-1 END.
				}
				// 3. 밧줄(사다리)
				else if (Convert == L"Texture_Object_Rope")
				{
					vMinCube.y = (iter)->Get_InfoCom()->m_vPos.y - ((iter)->Get_InfoCom()->m_vScale.y - 0.1f);
					vMaxCube.y = (iter)->Get_InfoCom()->m_vPos.y + ((iter)->Get_InfoCom()->m_vScale.y - 0.1f);

					// Hang키 안눌렷따
					if (bHangCheck)
					{
						bRopeEnd = false;
						continue;
					}
					if (pPlayerProperties[STATE_HANG] == FALSE)
						continue;
					// Hang키 눌렷다
					else if (pPlayerProperties[STATE_HANG] == TRUE && bIsHang == FALSE) //키를 누르고 아직 안매달려잇을때
					{
						// AABB 충돌체크
						if (ABC(vMinPlayer, vMaxPlayer, vMinCube, vMaxCube) && !dynamic_cast<CPlayer*>(pPlayer)->GetDead())
						{
							//(pPlayer)->Get_PlayerInfoCom()->m_vPos.x = (iter)->Get_InfoCom()->m_vPos.x;
							bReturn = true;
							bIsHang = true;
						}
						//float fDistX = (iter)->Get_InfoCom()->m_vPos.x - (pPlayer)->Get_Transform()->m_vPos.x;
					}
					else if (eState == STATE_HANG && bIsHang == true) //매달려 있고
					{
						// AABB 충돌체크
						if (ABC(vMinPlayer, vMaxPlayer, vMinCube, vMaxCube) && !(pPlayer)->GetDead())
						{
							bReturn = true;
							bHangCheck = true;
							float fDistX = (iter)->Get_InfoCom()->m_vPos.x - (pPlayer)->Get_Transform()->m_vPos.x;
							(pPlayer)->Get_PlayerInfoCom()->m_vPos.x += fDistX*GET_TIME*2.0f;
							// 플레이어가 사다리보다 높다면
							if ((pPlayer)->Get_Transform()->m_vPos.y - PLAYER_HEIGHT*2.0f >= vMaxCube.y)
							{
								bRopeEnd = true;
							}
						}
						else
						{
						}
					}
				}
				// 4. 레이저
				else if (Convert == L"Texture_Object_Laser")
					continue;
				// 5. 가시(파이크)
				else if (Convert == L"Texture_CubeObstacle_Spikes")
				{
					// AABB 충돌체크
					if (ABC(vMinPlayer, vMaxPlayer, vMinCube, vMaxCube) && !dynamic_cast<CPlayer*>(pPlayer)->GetDead())
					{
						pPlayer->Setplayerproperties(PLAYERSTATE::STATE_DIE, true);
						Engine::Get_SoundMgr()->StopAll();
						Engine::Get_SoundMgr()->MyPlaySound(L"DeadBySpike.wav", Engine::CHANNELID::EFFECT);
					}
				}
				// 6. 떨어지는 돌덩이
				else if (Convert == L"Texture_CubeObstacle_Rock")
				{
					float fDist;
					if (CollisionCheckSphere(fDist, (pPlayer)->Get_Transform()->m_vPos, (pPlayer)->Get_Transform()->m_vScale.x * 0.5f, (iter)->Get_InfoCom()->m_vPos, (iter)->Get_InfoCom()->m_vScale.x))
					{
						if (pPlayer->GetPlayerProper()[PLAYERSTATE::STATE_DIE] == false)
						{
							pPlayer->Setplayerproperties(PLAYERSTATE::STATE_DIE, true);
							Engine::Get_SoundMgr()->MyPlaySound(L"DeadByRock.wav", Engine::CHANNELID::EFFECT);
							dynamic_cast<CCubeRock*>(iter)->ReSetPosition();
						}
					}
				}
				// 7. 계단
				else if (Convert == L"Texture_CubeObstacle_Stairs")
				{
					if (ABC(vMinPlayer, vMaxPlayer, vMinCube, vMaxCube) && !(pPlayer)->GetDead())
					{
						float fB = static_cast<CCubeStairs*>(iter)->Get_fB();
						float fA = static_cast<CCubeStairs*>(iter)->Get_fA();
						float fLineY = fA * pPlayer->Get_Transform()->m_vPos.x + fB + 0.6f;

						if (fLineY >= pPlayer->Get_Transform()->m_vPos.y)
						{
							pPlayer->SetJumpPower(-7.0f);
							pPlayerProperties[STATE_JUMP] = false;
							pPlayerProperties[STATE_HANG] = false;
							pPlayerProperties[STATE_FALL] = false;
							pPlayer->Get_Transform()->m_vPos.y = fLineY;
							pPlayer->SetOnStair(true);
						}
						else
						{
							//pPlayer->SetOnStair(false);
						}
					}
					else
					{


					}

					pPlayer->SetPlayerInput(false);
					pPlayer->m_fCircleActivePower = 0.f;
				}
				//8. 풍선이면 그냥 skip
				else if (Convert == L"Texture_CubeObstacle_Balloon")
				{
					continue;
				}
				// 99. 나머지 모든 큐브
				else
				{
					// 99-0. 계단상태면 걸러라.(임시)
					/* if (pPlayer->GetIsOnStair())
					continue;*/
					// 99-0. 충돌체크
					if (ABC(vMinPlayer, vMaxPlayer, vMinCube, vMaxCube) && !(pPlayer)->GetDead())
					{
						pPlayer->SetOnStair(false);
						bReturn = true;
						float fWidth1 = abs(vMaxPlayer.x - vMinCube.x);
						float fHeight1 = abs(vMaxCube.y - vMinPlayer.y);
						float fWidth2 = abs(vMaxCube.x - vMinPlayer.x);
						float fHeight2 = abs(vMaxPlayer.y - vMinCube.y);

						// 99-0-1. 플레이어가 큐브 보다 높이 있다.
						if (vMinPlayer.y >= (iter)->Get_InfoCom()->m_vPos.y + 0.5f)
						{
							if (pPlayer->Get_CurState() == PLAYERSTATE::STATE_FALL)
								bLandSound = true;
							// 99-0-1-1. 플레이어가
							if ((pPlayer)->Get_Transform()->m_vPos.x > vMinCube.x && (pPlayer)->Get_Transform()->m_vPos.x < vMaxCube.x)
							{
								// 99-0-1-2. 에스컬레이터 방지
								if (eState != STATE_JUMP)
								{
									(pPlayer)->Get_Transform()->m_vPos.y += fHeight1 * (GET_TIME * 25.0f);
									pPlayerProperties[STATE_FALL] = false;
									bCollSide[1] = true;
								}//99-0-1-2 END.
							}//99-0-1-1 END.
						}//99-0-1 END.
						 // 99-0-2. 플레이어가 큐브 보다 낮다.
						else if (vMaxPlayer.y <= (iter)->Get_InfoCom()->m_vPos.y - 0.5f)
						{
							// 99-0-2-1. 플레이어가 -> 이프문 수정필요*********************************
							if ((pPlayer)->Get_Transform()->m_vPos.x >= vMinCube.x + 0.01f && (pPlayer)->Get_Transform()->m_vPos.x <= vMaxCube.x - 0.01f)
							{
								// 99-0-2-2. 에스컬레이터 방지
								if (eState != STATE_FALL)
								{
									(pPlayer)->Get_Transform()->m_vPos.y -= fHeight2 * (GET_TIME*25.0f);
									bCollSide[2] = true;
								}//99-0-2-2 END.
							}//99-0-2-1 END.
						}//99-0-2 END.
						 // 99-0-3. 플레이어가 같은 Y선상에 존재한다.
						else if (vMinPlayer.y < (iter)->Get_InfoCom()->m_vPos.y + 0.5f && vMaxPlayer.y >(iter)->Get_InfoCom()->m_vPos.y - 0.5f)
						{
							// 99-0-3-1. 플레이어가 큐브 왼쪽에 있다
							if ((pPlayer)->Get_Transform()->m_vPos.x <= (iter)->Get_InfoCom()->m_vPos.x)
							{
								(pPlayer)->Get_Transform()->m_vPos.x -= (fWidth1) * (GET_TIME * 15.0f);
								bCollSide[3] = true;
							}
							// 99-0-3-2. 플레이어가 큐브 오른쪽에 있다
							else
							{
								(pPlayer)->Get_Transform()->m_vPos.x += (fWidth2) * (GET_TIME * 15.0f);
								bCollSide[4] = true;
							}
						}//99-0-3 END.

						 // 99-0-4. 메달린 상태라면
						if (bIsHang)
						{
							bIsHang = false;
							eState = PLAYERSTATE::STATE_IDLE1;
							return true;
						}//99-0-4 END.

					}//99-0 END.
					 // 큐브 색 체크
					float fDistX = (iter)->Get_CubeInfo().vPos.x - dynamic_cast<CPlayer*>(pPlayer)->Get_Transform()->m_vPos.x;
					if (fDistX < 0.6f)
						Engine::CUBE_COLOR eColor = iter->Get_CubeColor();
					continue;
				}
			}
			else
			{
				if (ABC(vMinPlayer, vMaxPlayer, vMinCube, vMaxCube))
				{
					if (dynamic_cast<CCubeBalloon*>(iter) == nullptr
						&& dynamic_cast<CCubeRope*>(iter) == nullptr)
					{
						Engine::CUBE_COLOR eColor = iter->Get_CubeColor();
						CCubeMgr::GetInstance()->SetPlayerColColor(eColor);
					}
				}
			}
		}
	}

	if (bLandSound && bCollSide[1] && !CCubeMgr::GetInstance()->Get_IsRender())
	{
		if (IsOnGrass)
		{
			if (!bLandCrate && !bInteraction)
				Engine::Get_SoundMgr()->MyPlaySound(L"PlayerLandGrass.wav", Engine::CHANNELID::EFFECT);
			else if (bLandCrate && !bInteraction)
				Engine::Get_SoundMgr()->MyPlaySound(L"PlayerCrateLand.wav", Engine::CHANNELID::EFFECT);
		}
		else
		{
			if (!bLandCrate && !bInteraction)
				Engine::Get_SoundMgr()->MyPlaySound(L"PlayerLand.wav", Engine::CHANNELID::EFFECT);
			else if (bLandCrate && !bInteraction)
				Engine::Get_SoundMgr()->MyPlaySound(L"PlayerCrateLand.wav", Engine::CHANNELID::EFFECT);
		}

		bLandSound = false;
		bLandCrate = false;
	}
	if (bDragCrate)
	{
		Engine::Get_SoundMgr()->MyPlaySound(L"PlayerCrateDrag.wav", Engine::CHANNELID::INTERACT);
		Engine::Get_SoundMgr()->MyPlaySound(L"PlayerCrateDrag.wav", Engine::CHANNELID::INTERACT2);
		bDragCrate = false;
		bDragSound = true;
	}
	if (!bDragSound)
	{
		Engine::Get_SoundMgr()->StopSound(Engine::CHANNELID::INTERACT);
		Engine::Get_SoundMgr()->StopSound(Engine::CHANNELID::INTERACT2);
	}
	if (bWalkCrate && fSoundDelay >= 0.5f)
	{
		Engine::Get_SoundMgr()->MyPlaySound(L"PlayerCrateWalk.wav", Engine::CHANNELID::EFFECT);
		bWalkCrate = false;
		fSoundDelay = 0.0f;
	}


	return bReturn;
}

bool CCollisionMgr::IsCollisionSkullToCube(CCubeSkull* pObject)
{
	bool bReturn = false;

	for (auto mapiter : *CCubeMgr::GetInstance()->Get_MapCubeList())
	{
		for (auto iter : mapiter.second)
		{

			wstring Convert(iter->GetTexName());
			if (Convert == L"Texture_Object_Rope" || Convert == L"Texture_Object_Laser")
				continue;

			// Cube
			D3DXVECTOR3 vMinCube;
			vMinCube = (iter)->Get_InfoCom()->m_vPos - (iter)->Get_InfoCom()->m_vScale;
			D3DXVECTOR3 vMaxCube;
			vMaxCube = (iter)->Get_InfoCom()->m_vPos + (iter)->Get_InfoCom()->m_vScale;
			// Skull
			D3DXVECTOR3 vMinSkull;
			vMinSkull.x = (pObject)->Get_InfoCom()->m_vPos.x - 0.98f *(pObject)->Get_InfoCom()->m_vScale.x;
			vMinSkull.y = (pObject)->Get_InfoCom()->m_vPos.y - (pObject)->Get_InfoCom()->m_vScale.y;
			vMinSkull.z = (pObject)->Get_InfoCom()->m_vPos.z - (pObject)->Get_InfoCom()->m_vScale.z;

			D3DXVECTOR3 vMaxSkull;
			vMaxSkull.x = (pObject)->Get_InfoCom()->m_vPos.x + 0.98f *(pObject)->Get_InfoCom()->m_vScale.x;
			vMaxSkull.y = (pObject)->Get_InfoCom()->m_vPos.y + (pObject)->Get_InfoCom()->m_vScale.y;
			vMaxSkull.z = (pObject)->Get_InfoCom()->m_vPos.z + (pObject)->Get_InfoCom()->m_vScale.z;

			if (iter == pObject)
				continue;

			if ((iter->Get_CubeColor() != CCubeMgr::GetInstance()->Get_CurCubeColor()) || iter->Get_CubeColor() == Engine::CUBE_COLOR::CUBE_COLOR_END)
			{
				// AABB 충돌체크
				if (ABC(vMinSkull, vMaxSkull, vMinCube, vMaxCube))
				{
					bReturn = true;
					float fWidth1 = abs(vMaxSkull.x - vMinCube.x);
					float fHeight1 = abs(vMaxCube.y - vMinSkull.y);
					float fWidth2 = abs(vMaxCube.x - vMinSkull.x);
					float fHeight2 = abs(vMaxSkull.y - vMinCube.y);
					if (vMinSkull.y >= (iter)->Get_InfoCom()->m_vPos.y)
					{
						// 위로밀기
						(pObject)->Get_InfoCom()->m_vPos.y += fHeight1 * (GET_TIME * 25.0f);
					}
				}
			}
		}
	}
	return bReturn;
}

bool CCollisionMgr::IsCollisionRockToCube(CCubeRock * pObject)
{
	static float fImpactSoundDelay = 0.0f;
	bool bReturn = false;
	bool bRockImpact = false;
	bool bRockReSet = false;
	float fDist = 0.0f;
	D3DXVECTOR3 vMinCube;
	D3DXVECTOR3 vMaxCube;
	D3DXVECTOR3 vMinRockCube;
	D3DXVECTOR3 vMaxRockCube;
	CCubeMgr::GetInstance()->SetPlayerColColor(Engine::CUBE_COLOR::CUBE_COLOR_END);
	for (auto mapiter : *CCubeMgr::GetInstance()->Get_MapCubeList())
	{
		for (auto iter : mapiter.second)
		{
			// 모든 큐브 동일 적용
			vMinCube = (iter)->Get_InfoCom()->m_vPos - (iter)->Get_InfoCom()->m_vScale;
			vMaxCube = (iter)->Get_InfoCom()->m_vPos + (iter)->Get_InfoCom()->m_vScale;
			// 돌 큐브 동일 적용
			vMinRockCube = (pObject)->Get_InfoCom()->m_vPos - (pObject)->Get_InfoCom()->m_vScale;
			vMaxRockCube = (pObject)->Get_InfoCom()->m_vPos + (pObject)->Get_InfoCom()->m_vScale;

			// 0. 큐브 종류 체크
			wstring Convert(iter->GetTexName());
			// 1. 제외큐브들
			if (Convert == L"Texture_Object_Laser" || Convert == L"Texture_Object_Rope" || pObject == iter)
				continue;
			// 2. 계단
			else if (Convert == L"Texture_CubeObstacle_Stairs")
			{
				// 0-1. 색 검사 (일치)
				if ((iter->Get_CubeColor() != CCubeMgr::GetInstance()->Get_CurCubeColor()) || iter->Get_CubeColor() == Engine::CUBE_COLOR::CUBE_COLOR_END)
				{
					// 0-1-1. 충돌검사 (충돌)
					if (CollisionCheckSphere(fDist, (pObject)->Get_InfoCom()->m_vPos, (pObject)->Get_InfoCom()->m_vScale.x, (iter)->Get_InfoCom()->m_vPos/*-D3DXVECTOR3()*/, (iter)->Get_InfoCom()->m_vScale.x - 0.5f))
					{
						// 0-1-1-1. 돌덩이가 큐브 보다 높이 있다.
						if ((pObject)->Get_InfoCom()->m_vPos.y >= (iter)->Get_InfoCom()->m_vPos.y)
						{
							bRockImpact = true;
							(pObject)->SetAcc(0.0f);
							(pObject)->SetSpeed(-1.5f, 0.0f);
						}
					}
					//0-1-2 END.
					float fDistX = (iter)->Get_InfoCom()->m_vPos.x - (pObject)->Get_InfoCom()->m_vPos.x;
					if (fDistX < 0.6f)
						Engine::CUBE_COLOR eColor = iter->Get_CubeColor();
					continue;
				}//0-1 END.
				 // 0-2. 색검사 (불일치)
				else
				{
					if (CollisionCheckSphere(fDist, (pObject)->Get_InfoCom()->m_vPos, (pObject)->Get_InfoCom()->m_vScale.x*0.5f, (iter)->Get_InfoCom()->m_vPos, (iter)->Get_InfoCom()->m_vScale.x))
					{
						Engine::CUBE_COLOR eColor = iter->Get_CubeColor();
						CCubeMgr::GetInstance()->SetPlayerColColor(eColor);
					}
				}//0-2 END.
			}
			// 3. 닿으면 무시할 큐브들 
			else if (Convert == L"Texture_CubeDeco_Brick")
			{

			}
			//// 4. 닿으면 상호작용 할 큐브들 
			//else if (Convert == L"Texture_CubeDeco_Brick")
			//{

			//}
			// 5. 닿으면 삭제시킬 큐브들
			else
			{
				// 0-1. 색 검사 (일치)
				if ((iter->Get_CubeColor() != CCubeMgr::GetInstance()->Get_CurCubeColor()) || iter->Get_CubeColor() == Engine::CUBE_COLOR::CUBE_COLOR_END)
				{
					// 0-1-1. 충돌검사 (충돌)
					if (CollisionCheckSphere(fDist, (pObject)->Get_InfoCom()->m_vPos, (pObject)->Get_InfoCom()->m_vScale.x, (iter)->Get_InfoCom()->m_vPos, (iter)->Get_InfoCom()->m_vScale.x - 0.1f))
					{
						bRockReSet = true;
						(pObject)->ReSetPosition();

					}
					// 0-1-2. 충돌검사 (비충돌)
					else
					{

					}
					//0-1-2 END.
					float fDistX = (iter)->Get_InfoCom()->m_vPos.x - (pObject)->Get_InfoCom()->m_vPos.x;
					if (fDistX < 0.6f)
						Engine::CUBE_COLOR eColor = iter->Get_CubeColor();
					continue;
				}//0-1 END.
				 // 0-2. 색검사 (불일치)
				else
				{
					if (CollisionCheckSphere(fDist, (pObject)->Get_InfoCom()->m_vPos, (pObject)->Get_InfoCom()->m_vScale.x*0.5f, (iter)->Get_InfoCom()->m_vPos, (iter)->Get_InfoCom()->m_vScale.x))
					{
						Engine::CUBE_COLOR eColor = iter->Get_CubeColor();
						CCubeMgr::GetInstance()->SetPlayerColColor(eColor);
					}
				}//0-2 END.
			}
		}
	}
	float fPlayerX = CCubeMgr::GetInstance()->GetPlayer()->Get_Transform()->m_vPos.x;
	float fPlayerY = CCubeMgr::GetInstance()->GetPlayer()->Get_Transform()->m_vPos.y;
	float fDistX = fPlayerX - (pObject)->Get_InfoCom()->m_vPos.x;
	float fDistY = fPlayerY - (pObject)->Get_InfoCom()->m_vPos.y;
	float fDistance = sqrtf(fDistX * fDistX + fDistY * fDistY);
	(pObject)->SetDist(fDistance);

	fImpactSoundDelay += GET_TIME;	
	if (fDistance < 10.f)
	{
		if (bRockImpact)
		{
			(pObject)->SetImpactSoundPlay(bRockImpact);
			bRockImpact = false;
		}
		if (bRockReSet)
		{
			(pObject)->SetFallSoundPlay(bRockReSet);
		}
	}
	else
	{
		if (fImpactSoundDelay > 1.5f)
		{
			if (bRockImpact)
			{
				(pObject)->SetImpactSoundPlay(bRockImpact);
				bRockImpact = false;
				fImpactSoundDelay = 0.0f;
			}
		}
	}


	return bReturn;
}

int CCollisionMgr::IsCollisionRightLeftLaserToCube(CLaser * pLaser)
{
	float fPosMinX = pLaser->Get_InfoCom()->m_vPos.x - pLaser->Get_InfoCom()->m_vScale.x;
	float fPosMaxX = pLaser->Get_InfoCom()->m_vPos.x + pLaser->Get_InfoCom()->m_vScale.x;
	float fPosY = pLaser->Get_InfoCom()->m_vPos.y;
	float fPosZ = pLaser->Get_InfoCom()->m_vPos.z;

	float fResultX = 100.f;
	float fResultY = 0.f;
	float fResultCubeX = 0.f;
	bool  bIsCollide = false;

	for (auto mapiter : *CCubeMgr::GetInstance()->Get_MapCubeList())
	{
		if (mapiter.first == CUBEID::CUBE_LASER)
			continue;
		for (auto iter : mapiter.second)
		{ 
			if (CCubeMgr::GetInstance()->Get_CurCubeColor() == iter->Get_CubeColor()
				&& iter->Get_CubeColor() != Engine::CUBE_COLOR::CUBE_COLOR_END)
				continue;
			//float fParentX = pLaser->GetParentLaser()->Get_LaserInfoCom()->m_vPos.x + 0.15f;
			float fCubeX = iter->Get_InfoCom()->m_vPos.x - iter->Get_InfoCom()->m_vScale.x;
			float fCubeY = iter->Get_InfoCom()->m_vPos.y;
			float fCubeZ = iter->Get_InfoCom()->m_vPos.z;

			if ((fCubeX > fPosMinX && fCubeX < fPosMaxX) && fabsf(fPosY - fCubeY) < 0.5f && fabsf(fPosZ - fCubeZ) < 0.8f)
			{
				if (fResultX > pLaser->GetParentLaser()->Get_LaserInfoCom()->m_vPos.x
					+ ((fCubeX - pLaser->GetParentLaser()->Get_LaserInfoCom()->m_vPos.x) / 2.f) + 0.3f)
				{
					fResultX = pLaser->GetParentLaser()->Get_LaserInfoCom()->m_vPos.x
						+ ((fCubeX - pLaser->GetParentLaser()->Get_LaserInfoCom()->m_vPos.x) / 2.f) + 0.3f;
					fResultY = fPosY;
					fResultCubeX = fCubeX;
					bIsCollide = true;
				}
			}
		}
	}

	if (bIsCollide)
	{
		pLaser->Get_InfoCom()->m_vPos.x = fResultX - 0.3f;
		pLaser->Get_InfoCom()->m_vPos.y = fResultY;
		pLaser->Get_InfoCom()->m_vScale.x = fResultX - pLaser->GetParentLaser()->Get_LaserInfoCom()->m_vPos.x - 0.3f;
		pLaser->SetMoving(false);
	}
	else
		pLaser->SetMoving(true);

	return 0;
}

int CCollisionMgr::IsCollisionUpDownLaserToCube(CLaser * pLaser)
{
	float fPosMinY = pLaser->Get_InfoCom()->m_vPos.y - pLaser->Get_InfoCom()->m_vScale.x;
	float fPosMaxY = pLaser->Get_InfoCom()->m_vPos.y + pLaser->Get_InfoCom()->m_vScale.x;
	float fPosX = pLaser->Get_InfoCom()->m_vPos.x;

	float fResultX = 0.f;
	float fResultY = -100.f;
	float fResultCubeY = 0.f;
	bool  bIsCollide = false;

	for (auto mapiter : *CCubeMgr::GetInstance()->Get_MapCubeList())
	{
		if (mapiter.first == CUBEID::CUBE_LASER)
			continue;
		for (auto iter : mapiter.second)
		{
			if (CCubeMgr::GetInstance()->Get_CurCubeColor() == iter->Get_CubeColor()
				&& iter->Get_CubeColor() != Engine::CUBE_COLOR::CUBE_COLOR_END)
				continue;
			//float fParentX = pLaser->GetParentLaser()->Get_LaserInfoCom()->m_vPos.x + 0.15f;
			float fCubeX = iter->Get_InfoCom()->m_vPos.x;
			float fCubeY = iter->Get_InfoCom()->m_vPos.y + iter->Get_InfoCom()->m_vScale.y;

			if ((fCubeY > fPosMinY && fCubeY < fPosMaxY) && fabsf(fPosX - fCubeX) < 0.5f)
			{
				if (fResultY < fCubeY
					+ ((pLaser->GetParentLaser()->Get_LaserInfoCom()->m_vPos.y - fCubeY) / 2.f) + 0.3f)
				{
					fResultY = fCubeY
						+ ((pLaser->GetParentLaser()->Get_LaserInfoCom()->m_vPos.y - fCubeY) / 2.f) + 0.3f;
					fResultX = fPosX;
					fResultCubeY = fCubeY;
					bIsCollide = true;
				}
			}
		}
	}

	if (bIsCollide)
	{
		pLaser->Get_InfoCom()->m_vPos.x = fResultX;
		pLaser->Get_InfoCom()->m_vPos.y = fResultY - 0.3f;
		pLaser->Get_InfoCom()->m_vScale.x = pLaser->GetParentLaser()->Get_LaserInfoCom()->m_vPos.y - fResultY + 0.3f;
		pLaser->SetMoving(false);
	}
	else
		pLaser->SetMoving(true);

	return 0;
}

int CCollisionMgr::IsCollisionLaserToPlayer(CLaser * pLaser, CPlayer * pPlayer, bool bIsDown)
{
	if (CCubeMgr::GetInstance()->Get_CurCubeColor() == pLaser->Get_CubeColor())
		return 0;
	if (pPlayer->Get_CurState() == PLAYERSTATE::STATE_DIE)
		return 0;

	if (!bIsDown)
	{
		float fPosMinX = pLaser->Get_InfoCom()->m_vPos.x - pLaser->Get_InfoCom()->m_vScale.x;
		float fPosMaxX = pLaser->Get_InfoCom()->m_vPos.x + pLaser->Get_InfoCom()->m_vScale.x;
		float fMinPosY = pLaser->Get_InfoCom()->m_vPos.y - 0.2f;
		float fMaxPosY = pLaser->Get_InfoCom()->m_vPos.y + 0.2f;
		float fPlayerX = pPlayer->Get_Transform()->m_vPos.x;
		float fPlayerY = pPlayer->Get_Transform()->m_vPos.y;
		float fPlayerZ = pPlayer->Get_Transform()->m_vPos.z;

		if ((fPlayerX > fPosMinX && fPlayerX < fPosMaxX) && (fPlayerY > fMinPosY && fPlayerY < fMaxPosY))
		{
			pPlayer->Setplayerproperties(PLAYERSTATE::STATE_DIE, true);

			Engine::Get_SoundMgr()->StopAll();
			Engine::Get_SoundMgr()->MyPlaySound(L"DeadByLaser.wav", Engine::CHANNELID::EFFECT);
		}
	}
	else
	{
		float fPosMinX = pLaser->Get_InfoCom()->m_vPos.x - 0.2f;
		float fPosMaxX = pLaser->Get_InfoCom()->m_vPos.x + 0.2f;
		float fMinPosY = pLaser->Get_InfoCom()->m_vPos.y - pLaser->Get_InfoCom()->m_vScale.x;
		float fMaxPosY = pLaser->Get_InfoCom()->m_vPos.y + pLaser->Get_InfoCom()->m_vScale.x;
		float fPlayerX = pPlayer->Get_Transform()->m_vPos.x;
		float fPlayerY = pPlayer->Get_Transform()->m_vPos.y;
		float fPlayerZ = pPlayer->Get_Transform()->m_vPos.z;

		if ((fPlayerX > fPosMinX && fPlayerX < fPosMaxX) && (fPlayerY > fMinPosY && fPlayerY < fMaxPosY))
		{
			pPlayer->Setplayerproperties(PLAYERSTATE::STATE_DIE, true);
			Engine::Get_SoundMgr()->StopAll();
			Engine::Get_SoundMgr()->MyPlaySound(L"DeadByLaser.wav", Engine::CHANNELID::EFFECT);
		}
	}
	return 0;
}

int CCollisionMgr::IsCollisionLaserToPlayerStage3(CLaser * pLaser, CPlayer * pPlayer, bool bIsDown)
{
	if (CCubeMgr::GetInstance()->Get_CurCubeColor() == pLaser->Get_CubeColor())
		return 0;
	if (pPlayer->Get_CurState() == PLAYERSTATE::STATE_DIE)
		return 0;

	if (!bIsDown)
	{
		float fPosMinX = pLaser->Get_InfoCom()->m_vPos.x - pLaser->Get_InfoCom()->m_vScale.x;
		float fPosMaxX = pLaser->Get_InfoCom()->m_vPos.x + pLaser->Get_InfoCom()->m_vScale.x;
		float fMinPosY = pLaser->Get_InfoCom()->m_vPos.y - 0.2f;
		float fMaxPosY = pLaser->Get_InfoCom()->m_vPos.y + 0.2f;
		float fPlayerX = pPlayer->Get_Transform()->m_vPos.x;
		float fPlayerY = pPlayer->Get_Transform()->m_vPos.y;
		float fPlayerZ = pPlayer->Get_Transform()->m_vPos.z;

		if ((fPlayerX > fPosMinX && fPlayerX < fPosMaxX) && (fPlayerY > fMinPosY && fPlayerY < fMaxPosY)
			&& (fPlayerZ > pLaser->Get_InfoCom()->m_vPos.z - 0.3f && fPlayerZ < pLaser->Get_InfoCom()->m_vPos.z + 0.3f))
		{
			pPlayer->Setplayerproperties(PLAYERSTATE::STATE_DIE, true);

			Engine::Get_SoundMgr()->StopAll();
			Engine::Get_SoundMgr()->MyPlaySound(L"DeadByLaser.wav", Engine::CHANNELID::EFFECT);
		}
	}
	else
	{
		float fPosMinX = pLaser->Get_InfoCom()->m_vPos.x - 0.2f;
		float fPosMaxX = pLaser->Get_InfoCom()->m_vPos.x + 0.2f;
		float fMinPosY = pLaser->Get_InfoCom()->m_vPos.y - pLaser->Get_InfoCom()->m_vScale.x;
		float fMaxPosY = pLaser->Get_InfoCom()->m_vPos.y + pLaser->Get_InfoCom()->m_vScale.x;
		float fPlayerX = pPlayer->Get_Transform()->m_vPos.x;
		float fPlayerY = pPlayer->Get_Transform()->m_vPos.y;
		float fPlayerZ = pPlayer->Get_Transform()->m_vPos.z;

		if ((fPlayerX > fPosMinX && fPlayerX < fPosMaxX) && (fPlayerY > fMinPosY && fPlayerY < fMaxPosY)
			&& (fPlayerZ > pLaser->Get_InfoCom()->m_vPos.z - 0.3f && fPlayerZ < pLaser->Get_InfoCom()->m_vPos.z + 0.3f))
		{
			pPlayer->Setplayerproperties(PLAYERSTATE::STATE_DIE, true);
			Engine::Get_SoundMgr()->StopAll();
			Engine::Get_SoundMgr()->MyPlaySound(L"DeadByLaser.wav", Engine::CHANNELID::EFFECT);
		}
	}
	return 0;
}

int CCollisionMgr::IsCollisionBalloonCubeToPlayer(CCubeBrick * pBrick, CPlayer * pPlayer, BALLOON_STATE& eState)
{
	/*if (pBrick->Get_CubeColor() == CCubeMgr::GetInstance()->Get_CurCubeColor())
	return 0;*/
	D3DXVECTOR3 vMinPlayer, vMaxPlayer;
	D3DXVECTOR3 vMinBalloonCube, vMaxBalloonCube;

	vMinBalloonCube = (pBrick)->Get_InfoCom()->m_vPos - (pBrick)->Get_InfoCom()->m_vScale;
	vMaxBalloonCube = (pBrick)->Get_InfoCom()->m_vPos + (pBrick)->Get_InfoCom()->m_vScale;

	vMinPlayer.x = (pPlayer)->Get_Transform()->m_vPos.x - ((pPlayer)->Get_Transform()->m_vScale.x - PLAYER_WIDTH);
	vMinPlayer.y = (pPlayer)->Get_Transform()->m_vPos.y - ((pPlayer)->Get_Transform()->m_vScale.y - PLAYER_HEIGHT);
	vMinPlayer.z = (pPlayer)->Get_Transform()->m_vPos.z - (pPlayer)->Get_Transform()->m_vScale.z;

	vMaxPlayer.x = (pPlayer)->Get_Transform()->m_vPos.x + ((pPlayer)->Get_Transform()->m_vScale.x - PLAYER_WIDTH);
	vMaxPlayer.y = (pPlayer)->Get_Transform()->m_vPos.y + ((pPlayer)->Get_Transform()->m_vScale.y - PLAYER_HEIGHT);
	vMaxPlayer.z = (pPlayer)->Get_Transform()->m_vPos.z + (pPlayer)->Get_Transform()->m_vScale.z;

	//  색 체크
	if ((pBrick->Get_CubeColor() != CCubeMgr::GetInstance()->Get_CurCubeColor()) || pBrick->Get_CubeColor() == Engine::CUBE_COLOR::CUBE_COLOR_END)
	{
		// 1-0. 충돌비교
		if (ABC(vMinPlayer, vMaxPlayer, vMinBalloonCube, vMaxBalloonCube) && !(pPlayer)->GetDead())
		{
			//bReturn = true;
			float fWidth1 = abs(vMaxPlayer.x - vMinBalloonCube.x);
			float fHeight1 = abs(vMaxBalloonCube.y - vMinPlayer.y);
			float fWidth2 = abs(vMaxBalloonCube.x - vMinPlayer.x);
			float fHeight2 = abs(vMaxPlayer.y - vMinBalloonCube.y);

			////
			//// 99-0-1. 플레이어가 큐브 보다 높이 있다.
			//if (vMinPlayer.y >= (iter)->Get_InfoCom()->m_vPos.y + 0.5f)
			//{
			//	if (pPlayer->Get_CurState() == PLAYERSTATE::STATE_FALL)
			//		bLandSound = true;
			//	// 99-0-1-1. 플레이어가
			//	if ((pPlayer)->Get_Transform()->m_vPos.x > vMinCube.x && (pPlayer)->Get_Transform()->m_vPos.x < vMaxCube.x)
			//	{
			//	}

			////


			// 1-0-1. 플레이어가 플랫폼 위에 있을때 
			if ((pPlayer)->Get_Transform()->m_vPos.y >= (pBrick)->Get_InfoCom()->m_vPos.y)
			{
				// 1-0-1-1. 큐브 x범위 내에 있을 때 
				if (vMaxPlayer.x >= vMinBalloonCube.x + 0.01f || vMinPlayer.x <= vMaxBalloonCube.x - 0.01f)
				{
					// 1-0-1-3. 에스컬레이터 방지
					if (pPlayer->Get_CurState() != STATE_JUMP)
					{
						//  1-0-1-4. 올라가는 상태
						if (eState == BALLOON_STATE::BALLOON_UP)
						{
							if (pPlayer->Get_CurState() == STATE_WALK || pPlayer->Get_CurState() == STATE_IDLE1 || pPlayer->Get_CurState() == STATE_RUN)
							{
								(pPlayer)->Get_Transform()->m_vPos.y = vMaxBalloonCube.y + PLAYER_HEIGHT;
							}
							if (pPlayer->Get_CurState() == STATE_JUMP)
							{
								(pPlayer)->Get_Transform()->m_vPos.y += vMaxBalloonCube.y *GET_TIME;
							}
						}
						//  1-0-1-5. 내려가는 상태 
						else if ((eState == BALLOON_STATE::BALLOON_DOWN))
						{
							if (pPlayer->Get_CurState() == STATE_WALK || pPlayer->Get_CurState() == STATE_IDLE1 || pPlayer->Get_CurState() == STATE_RUN)
							{
								(pPlayer)->Get_Transform()->m_vPos.y = vMaxBalloonCube.y + PLAYER_HEIGHT - 0.1095f;
							}
						}
						// 1-0-1-6. 멈췃을때 
						else if (eState == BALLOON_STATE::BALLOON_STOP)
						{
							if (pPlayer->Get_CurState() == STATE_WALK || pPlayer->Get_CurState() == STATE_IDLE1 || pPlayer->Get_CurState() == STATE_RUN)
							{
								(pPlayer)->Get_Transform()->m_vPos.y += fHeight1 * (GET_TIME * 25.0f);
							}
						}
						pPlayer->GetPlayerProper()[STATE_FALL] = false;
						pPlayer->GetPlayerCollside()[1] = true;
					}//1-0-1-2 END.
				}//1-0-1-1 END.
			}//1-0-1 END.
			 // 1-0-2. 플랫폼 아래라면
			else if (vMaxPlayer.y <= (pBrick)->Get_InfoCom()->m_vPos.y)
			{
				// 아무것도하지마
			}//1-0-2 END.
			 // 1-0-3. 같은 Y선상에 존재한다면
			else if (vMinPlayer.y < (pBrick)->Get_InfoCom()->m_vPos.y && vMaxPlayer.y >(pBrick)->Get_InfoCom()->m_vPos.y)
			{
				// 1-0-3-1. 플레이어가 큐브 왼쪽에 있다
				if ((pPlayer)->Get_Transform()->m_vPos.x < vMinBalloonCube.x)
				{
					//// 왼쪽으로 밀기
					(pPlayer)->Get_Transform()->m_vPos.x -= (fWidth1);
					pPlayer->GetPlayerCollside()[3] = true;
				}
				// 1-0-3-2. 플레이어가 큐브 오른쪽에 있다
				else if ((pPlayer)->Get_Transform()->m_vPos.x > vMaxBalloonCube.x)
				{
					//// 오른쪽으로 밀기
					(pPlayer)->Get_Transform()->m_vPos.x += (fWidth2);
					pPlayer->GetPlayerCollside()[4] = true;
				}
				// 1-0-3-3. 플레이어가 큐브 사이에 있다
				else
				{
					//걍 낑겨
				}
			}// 1-0-3 END.

			pPlayer->SetPlayerInput(false);
			pPlayer->m_fCircleActivePower = 0.f;
		}
	}






	//// 99-0-1. 플레이어가 큐브 보다 높이 있다.
	//if (vMinPlayer.y >= pBrick->Get_InfoCom()->m_vPos.y + 0.5f)
	//{
	// // 99-0-1-1. 플레이어가  -> 이프문 수정필요*********************************
	// if ((pPlayer)->Get_Transform()->m_vPos.x > vMinBalloonCube.x + 0.1f || (pPlayer)->Get_Transform()->m_vPos.x < vMaxBalloonCube.x - 0.1f)
	// {
	// // 99-0-1-2. 에스컬레이터 방지
	// if (pPlayer->Get_CurState() != STATE_JUMP)
	// {
	// (pPlayer)->Get_Transform()->m_vPos.y += fHeight1 * (GET_TIME * 25.0f);
	// pPlayer->GetPlayerProper()[STATE_FALL] = false;
	// pPlayer->GetPlayerCollside()[1] = true;
	// }//99-0-1-2 END.
	// }//99-0-1-1 END.
	//}//99-0-1 END.
	// // 99-0-2. 플레이어가 큐브 보다 낮다.
	//else if (vMaxPlayer.y <= pBrick->Get_InfoCom()->m_vPos.y - 0.5f)
	//{
	// // 99-0-2-1. 플레이어가 -> 이프문 수정필요*********************************
	// if ((pPlayer)->Get_Transform()->m_vPos.x >= vMinBalloonCube.x + 0.01f || (pPlayer)->Get_Transform()->m_vPos.x >= vMinBalloonCube.x - 0.01f)
	// {
	// // 99-0-2-2. 에스컬레이터 방지
	// if (pPlayer->Get_CurState() != STATE_FALL)
	// {
	// (pPlayer)->Get_Transform()->m_vPos.y -= fHeight2 * (GET_TIME*25.0f);
	// pPlayer->GetPlayerCollside()[2] = true;
	// }//99-0-2-2 END.
	// }//99-0-2-1 END.
	//}//99-0-2 END.
	// // 99-0-3. 플레이어가 같은 Y선상에 존재한다.
	//else if (vMinPlayer.y < pBrick->Get_InfoCom()->m_vPos.y + 0.5f && vMaxPlayer.y > pBrick->Get_InfoCom()->m_vPos.y - 0.5f)
	//{
	// // 99-0-3-1. 플레이어가 큐브 왼쪽에 있다
	// if ((pPlayer)->Get_Transform()->m_vPos.x <= pBrick->Get_InfoCom()->m_vPos.x)
	// {
	// (pPlayer)->Get_Transform()->m_vPos.x -= (fWidth1) * (GET_TIME * 15.0f);
	// pPlayer->GetPlayerCollside()[3] = true;
	// }
	// // 99-0-3-2. 플레이어가 큐브 오른쪽에 있다
	// else
	// {
	// (pPlayer)->Get_Transform()->m_vPos.x += (fWidth2) * (GET_TIME * 15.0f);
	// pPlayer->GetPlayerCollside()[4] = true;
	// }
	//}//99-0-3 END.


	return 0;
}

bool CCollisionMgr::ABC(D3DXVECTOR3 & _vMinPlayer, D3DXVECTOR3 & _vMaxPlayer, D3DXVECTOR3 & _vMinCube, D3DXVECTOR3 & _MaxCube)
{
	// 충돌시
	if (_vMinCube.x <= _vMaxPlayer.x && _MaxCube.x >= _vMinPlayer.x &&
		_vMinCube.y <= _vMaxPlayer.y && _MaxCube.y >= _vMinPlayer.y &&
		_vMinCube.z <= _vMaxPlayer.z && _MaxCube.z >= _vMinPlayer.z)
	{
		return true;
	}
	return false;
}

bool CCollisionMgr::CollisionCheckSphere(float& fOut, D3DXVECTOR3 vCenter1, float fRadius1, D3DXVECTOR3 vCenter2, float fRadius2)
{
	float fDistance;
	D3DXVECTOR3 vDiff;
	vDiff = vCenter2 - vCenter1;
	fDistance = D3DXVec3Length(&vDiff);
	fOut = (fRadius1 + fRadius2) - fDistance;

	if (fDistance <= (fRadius1 + fRadius2))
		return true;
	return false;
}

BALLOON_COLCHECK CCollisionMgr::IsCollisionBalloonToCube(CCubeBalloon * pBalloon, CCubeBrick * pCubeBrick)
{
	D3DXVECTOR3 vMinBalloon, vMaxBalloon;
	D3DXVECTOR3 vMinBalloonCube, vMaxBalloonCube;

	vMinBalloon = (pBalloon)->Get_InfoCom()->m_vPos - (pBalloon)->Get_InfoCom()->m_vScale;
	vMaxBalloon = (pBalloon)->Get_InfoCom()->m_vPos + (pBalloon)->Get_InfoCom()->m_vScale;

	vMinBalloonCube = (pCubeBrick)->Get_InfoCom()->m_vPos - (pCubeBrick)->Get_InfoCom()->m_vScale;
	vMaxBalloonCube = (pCubeBrick)->Get_InfoCom()->m_vPos + (pCubeBrick)->Get_InfoCom()->m_vScale;

	for (auto mapiter : *CCubeMgr::GetInstance()->Get_MapCubeList())
	{
		for (auto iter : mapiter.second)
		{
			if (dynamic_cast<CCubeBrick*>(iter) == nullptr
				&& dynamic_cast<CCubeDeco*>(iter) == nullptr)
				continue;

			D3DXVECTOR3 vMinCube;
			vMinCube = (iter)->Get_InfoCom()->m_vPos - (iter)->Get_InfoCom()->m_vScale;
			D3DXVECTOR3 vMaxCube;
			vMaxCube = (iter)->Get_InfoCom()->m_vPos + (iter)->Get_InfoCom()->m_vScale;

			if (ABC(vMinBalloon, vMaxBalloon, vMinCube, vMaxCube))
			{
				if ((CCubeMgr::GetInstance()->Get_CurCubeColor() != iter->Get_CubeColor()) || CCubeMgr::GetInstance()->Get_CurCubeColor() == Engine::CUBE_COLOR::CUBE_COLOR_END)
					return BALLOON_COLCHECK::COL_BALLOON;
			}

			if (ABC(vMinBalloonCube, vMaxBalloonCube, vMinCube, vMaxCube))
			{
				if ((CCubeMgr::GetInstance()->Get_CurCubeColor() != iter->Get_CubeColor()) || CCubeMgr::GetInstance()->Get_CurCubeColor() == Engine::CUBE_COLOR::CUBE_COLOR_END)
					return BALLOON_COLCHECK::COL_BALLOONCUBE;
			}


		}
	}

	return BALLOON_COLCHECK::COL_END;
}
