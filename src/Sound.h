#pragma once

#include <stddef.h>

#include "WindowsWrapper.h"

#include "Backends/Audio.h"
#include "PixTone.h"

#include "nds/ndstypes.h"

#define SE_MAX 160	// According to the Organya source code release, this is the real name for this constant

class SOUNDBUFFER
{
	public:
		SOUNDBUFFER(size_t bufSize, const void* data_ptr);
		~SOUNDBUFFER();
		
		void Release();
		
		void Lock(s8 **buffer, size_t *size);
		void Unlock();
		
		void SetCurrentPosition(uint32_t dwNewPosition);
		void SetFrequency(uint32_t dwFrequency);
		void SetVolume(int32_t lVolume);
		void SetPan(int32_t lPan);
		void Play(bool bLooping);
		void Stop();
		
		void Mix(long *stream, uint32_t samples);
		
		SOUNDBUFFER *next;
	
	public:
		s8 *data;
		size_t size;
		
		bool playing;
		bool looping;
		bool looped;
		
		int timer;
		int endTimer;

		int frequency;
		int volume;
		int pan;
		
		int samplePosition;

		signed char channelId;
};

enum SoundEffectNames
{
	SND_YES_NO_CHANGE_CHOICE = 1,
	SND_MESSAGE_TYPING = 2,
	SND_QUOTE_BUMP_HEAD = 3,
	SND_SWITCH_WEAPON = 4,
	SND_YES_NO_PROMPT = 5,
	// To be continued
	SND_SILLY_EXPLOSION = 25,
	SND_LARGE_OBJECT_HIT_GROUND = 26,
	// To be continued
	SND_ENEMY_SHOOT_PROJETILE = 39,
	// To be continued
	SND_BEHEMOTH_LARGE_HURT = 52,
	// To be continued
	SND_EXPLOSION = 72
	// To be continued
};

enum SoundMode
{
	SOUND_MODE_PLAY_LOOP = -1,
	SOUND_MODE_STOP = 0,
	SOUND_MODE_PLAY = 1
};

extern SOUNDBUFFER* lpSECONDARYBUFFER[SE_MAX];

extern BOOL audio_backend_initialised;

BOOL InitDirectSound(void);
void EndDirectSound(void);
void PlaySoundObject(int no, int mode);
void ChangeSoundFrequency(int no, unsigned long rate);
void ChangeSoundVolume(int no, long volume);
void ChangeSoundPan(int no, long pan);
size_t MakePixToneObject(const PIXTONEPARAMETER *ptp, int ptp_num, int no);
void DoOrganya(void);
void killAllSounds(void);
BOOL ReadSound(int no);