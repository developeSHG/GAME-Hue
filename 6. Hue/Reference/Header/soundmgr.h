#ifndef SoundMgr_h__
#define SoundMgr_h__

#include "Engine_Include.h"

#include <io.h>
#include "fmod.h"

BEGIN(Engine)
class ENGINE_DLL CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)

private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize();
	void Update();
	void Release();

public:
	void MyPlaySound(TCHAR* pSoundKey, Engine::CHANNELID eID);
	void PlayBGM(TCHAR* pSoundKey, bool bReduceVolume = false);
	void StopSound(Engine::CHANNELID eID);
	void StopAll();

public:
	void LoadSoundFile();
	bool m_bIsInitialize;
private:
	// FMOD_SOUND : ���� ���ҽ� ������ ���� ����[ 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL -  ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[Engine::CHANNELID::MAXCHANNEL];
	// ���� , ä�� ��ü �� ��ġ�� ������ ��ü. 
	FMOD_SYSTEM* m_pSystem;

};

END
#endif	//SoundMgr_h__
